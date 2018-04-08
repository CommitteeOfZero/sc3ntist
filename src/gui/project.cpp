#include "project.h"
#include <parser/CCDisassembler.h>
#include <parser/SC3CodeBlock.h>
#include <parser/SC3Instruction.h>
#include <QFile>
#include <QDirIterator>
#include <QStringList>
#include <stdexcept>
#include "analysis.h"

Project::Project(const QString& path, QObject* parent = 0) : QObject(parent) {
  initDatabase();

  // TODO: read from mpk, or folder + mlp index

  QDirIterator it(path, QStringList() << "*.scx", QDir::Files | QDir::Readable);

  while (it.hasNext()) {
    QFile file(it.next());
    if (!file.open(QIODevice::ReadOnly)) {
      throw std::runtime_error("Couldn't open file");
    }
    SCXOffset length = (SCXOffset)file.size();
    uint8_t* data = (uint8_t*)malloc(length);
    file.read((char*)data, length);
    file.close();
    insertFile(it.fileName(), data, length);
  }

  _inInitialLoad = false;
}

const SCXFile* Project::currentFile() const {
  if (_currentFileId < 0 || _currentFileId >= _files.size()) return nullptr;
  return _files[_currentFileId].get();
}

void Project::switchFile(int id) {
  if (id >= 0 && id < _files.size() && _currentFileId != id) {
    int previousId = _currentFileId;
    _currentFileId = id;
    emit fileSwitched(previousId);
  }
}

QString Project::getComment(int fileId, SCXOffset address) {
  _getCommentQuery.addBindValue(fileId);
  _getCommentQuery.addBindValue(address);
  _getCommentQuery.exec();
  _getCommentQuery.next();

  QString result;
  if (_getCommentQuery.isValid()) {
    result = QString::fromUtf8(_getCommentQuery.value(0).toByteArray());
  }

  return result;
}

void Project::setComment(int fileId, SCXOffset address,
                         const QString& comment) {
  _setCommentQuery.addBindValue(fileId);
  _setCommentQuery.addBindValue(address);
  _setCommentQuery.addBindValue(comment.toUtf8());
  _setCommentQuery.exec();

  emit commentChanged(fileId, address, comment);
}

QString Project::getLabelName(int fileId, int labelId) {
  if (fileId < 0 || fileId >= _files.size()) return "";
  const SCXFile* file = _files[fileId].get();
  if (labelId < 0 || labelId >= file->disassembly().size()) return "";
  _getLabelNameQuery.addBindValue(fileId);
  _getLabelNameQuery.addBindValue(labelId);
  _getLabelNameQuery.exec();
  _getLabelNameQuery.next();

  QString result;
  if (_getLabelNameQuery.isValid()) {
    result = QString::fromUtf8(_getLabelNameQuery.value(0).toByteArray());
  }

  // TODO: still not quite the right place
  if (result.isEmpty()) {
    return QString("label%1_%2")
        .arg(labelId)
        .arg(file->disassembly()[labelId]->address());
  }

  return result;
}

void Project::setLabelName(int fileId, int labelId, const QString& name) {
  _setLabelNameQuery.addBindValue(fileId);
  _setLabelNameQuery.addBindValue(labelId);
  _setLabelNameQuery.addBindValue(name.toUtf8());
  _setLabelNameQuery.exec();

  emit labelNameChanged(fileId, labelId, name);
}

void Project::insertFile(const QString& name, uint8_t* data, int size) {
  QVariant vdata(QByteArray::fromRawData((char*)data, size));
  int id = _files.size();

  std::unique_ptr<SCXFile> scxFile =
      std::make_unique<SCXFile>(data, size, name.toStdString(), id);
  CCDisassembler dis(*scxFile);
  dis.DisassembleFile();

  // find refs

  for (const auto& label : scxFile->disassembly()) {
    for (const auto& inst : label->instructions()) {
      std::vector<std::pair<VariableRefType, int>> refs =
          variableRefsInInstruction(inst.get());

      for (const auto& ref : refs) {
        insertVariableRef(id, inst->position(), ref.first, ref.second);
      }
    }
  }

  _files.push_back(std::move(scxFile));

  _insertFileQuery.addBindValue(id);
  _insertFileQuery.addBindValue(name.toUtf8());
  _insertFileQuery.addBindValue(vdata);
  _insertFileQuery.exec();
}

std::vector<std::pair<int, SCXOffset>> Project::getVariableRefs(
    VariableRefType type, int var) {
  QVariant vtype;
  vtype.setValue(type);
  _getVariableRefsQuery.addBindValue(vtype);
  _getVariableRefsQuery.addBindValue(var);
  _getVariableRefsQuery.exec();

  std::vector<std::pair<int, SCXOffset>> result;
  while (_getVariableRefsQuery.next()) {
    result.emplace_back(_getVariableRefsQuery.value(0).toInt(),
                        _getVariableRefsQuery.value(1).toInt());
  }

  return result;
}

// TODO: batch?
void Project::insertVariableRef(int fileId, SCXOffset address,
                                VariableRefType type, int var) {
  _insertVariableRefQuery.addBindValue(fileId);
  _insertVariableRefQuery.addBindValue(address);
  QVariant vtype;
  vtype.setValue(type);
  _insertVariableRefQuery.addBindValue(vtype);
  _insertVariableRefQuery.addBindValue(var);
  _insertVariableRefQuery.exec();
}

void Project::initDatabase() {
  // TODO: close database in destructor?
  // TODO: encoding
  _db = QSqlDatabase::addDatabase("QSQLITE");
  _db.setDatabaseName(":memory:");
  _db.open();

  QSqlQuery q;
  q.exec(
      "CREATE TABLE files("
      "id INTEGER PRIMARY KEY,"
      "name VARCHAR NOT NULL,"
      "data BLOB NOT NULL"
      ")");
  q.exec(
      "CREATE TABLE comments("
      "fileId INTEGER NOT NULL,"
      "address INTEGER NOT NULL,"
      "text TEXT NOT NULL,"
      "PRIMARY KEY (fileId, address)"
      ")");
  q.exec(
      "CREATE TABLE labels("
      "fileId INTEGER NOT NULL,"
      "labelId INTEGER NOT NULL,"
      "name TEXT NOT NULL,"
      "PRIMARY KEY (fileId, labelId)"
      ")");
  q.exec(
      "CREATE TABLE variableRefs("
      "refId INTEGER PRIMARY KEY,"
      "fileId INTEGER NOT NULL,"
      "address INTEGER NOT NULL,"
      "variableType INTEGER NOT NULL,"
      "variable INTEGER NOT NULL"
      ")");

  _getCommentQuery = QSqlQuery(_db);
  _getCommentQuery.prepare(
      "SELECT text FROM comments WHERE fileId = ? AND address = ?");
  _setCommentQuery = QSqlQuery(_db);
  _setCommentQuery.prepare(
      "REPLACE INTO comments (fileId, address, text) VALUES (?, ?, ?)");
  _getLabelNameQuery = QSqlQuery(_db);
  _getLabelNameQuery.prepare(
      "SELECT name FROM labels WHERE fileId = ? AND labelId = ?");
  _setLabelNameQuery = QSqlQuery(_db);
  _setLabelNameQuery.prepare(
      "REPLACE INTO labels (fileId, labelId, name) VALUES (?, ?, ?)");
  _getFileQuery = QSqlQuery(_db);
  _getFileQuery.prepare("SELECT id, name, data FROM files ORDER BY id ASC");
  _insertFileQuery = QSqlQuery(_db);
  _insertFileQuery.prepare(
      "INSERT INTO files (id, name, data) VALUES (?, ?, ?)");
  _getVariableRefsQuery = QSqlQuery(_db);
  _getVariableRefsQuery.prepare(
      "SELECT fileId, address FROM variableRefs WHERE variableType = ? AND "
      "variable = ?");
  _insertVariableRefQuery = QSqlQuery(_db);
  _insertVariableRefQuery.prepare(
      "INSERT INTO variableRefs (fileId, address, variableType, variable) "
      "VALUES (?, ?, ?, ?)");
}