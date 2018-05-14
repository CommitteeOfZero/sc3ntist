#include "project.h"
#include <parser/CCDisassembler.h>
#include <parser/SC3CodeBlock.h>
#include <parser/SC3Instruction.h>
#include <QFile>
#include <QDirIterator>
#include <QStringList>
#include <QTextStream>
#include <stdexcept>
#include "analysis.h"

// create new database
Project::Project(const QString& dbPath, const QString& loadPath,
                 QObject* parent = 0)
    : QObject(parent), _contextProvider(this) {
  createDatabase(dbPath);

  std::vector<TmpFileData> files;
  if (!importMpk(loadPath, files)) {
    for (const auto& file : files) {
      free(file.data);
    }
    files.clear();
    if (!importMlp(loadPath, files)) {
      for (const auto& file : files) {
        free(file.data);
      }
      throw std::runtime_error("Couldn't read input");
    }
  }

  _db.transaction();
  for (const auto& file : files) {
    insertFile(file.name, file.data, file.size, file.id);
  }

  for (int globalVar = 0; globalVar < 8000; globalVar++) {
    insertVariable(VariableRefType::GlobalVar, globalVar,
                   QString("%1").arg(globalVar));
  }
  // TODO: recheck if 8000 is the max
  for (int flag = 0; flag < 8000; flag++) {
    insertVariable(VariableRefType::Flag, flag, QString("%1").arg(flag));
  }
  _db.commit();

  _inInitialLoad = false;
}

// open old database
Project::Project(const QString& dbPath, QObject* parent = 0)
    : QObject(parent), _contextProvider(this) {
  openDatabase(dbPath);
  prepareStmts();
  loadFilesFromDb();

  _inInitialLoad = false;
}

Project::~Project() {
  if (_db.isOpen()) _db.close();
}

bool Project::importMpk(const QString& mpkPath,
                        std::vector<TmpFileData>& files) {
  QFile mpk(mpkPath);
  if (!mpk.open(QIODevice::ReadOnly)) {
    return false;
  }
  QByteArray magic = mpk.read(4);
  if (magic != QByteArrayLiteral("MPK\0")) return false;
  uint16_t versionMinor;
  uint16_t versionMajor;
  mpk.read((char*)&versionMinor, 2);
  mpk.read((char*)&versionMajor, 2);
  if (qFromLittleEndian(versionMinor) != 0 ||
      qFromLittleEndian(versionMajor) != 2)
    return false;
  int fileCount;
  mpk.read((char*)&fileCount, 4);
  fileCount = qFromLittleEndian(fileCount);
  for (int i = 0; i < fileCount; i++) {
    TmpFileData file;
    mpk.seek(0x40 + 0x100 * i);
    int compression;
    mpk.read((char*)&compression, 4);
    if (compression != 0) return false;
    mpk.read((char*)&file.id, 4);
    file.id = qFromLittleEndian(file.id);
    qint64 dataOffset;
    mpk.read((char*)&dataOffset, 8);
    dataOffset = qFromLittleEndian(dataOffset);
    qint64 dataSize;
    mpk.read((char*)&dataSize, 8);
    dataSize = qFromLittleEndian(dataSize);
    mpk.seek(mpk.pos() + 8);  // compressed == uncompressed
    if (dataSize > INT_MAX) return false;
    file.size = (int)dataSize;
    QByteArray name = mpk.read(0xE0);
    file.name = QString(name);
    mpk.seek(dataOffset);
    file.data = (uint8_t*)malloc(file.size);
    if (mpk.read((char*)file.data, file.size) < file.size) {
      free(file.data);
      return false;
    }
    files.push_back(file);
  }
  return true;
}

bool Project::importMlp(const QString& mlpPath,
                        std::vector<TmpFileData>& files) {
  QFile mlpFile(mlpPath);
  if (!mlpFile.open(QIODevice::ReadOnly)) {
    return false;
  }
  QTextStream in(&mlpFile);
  in.setCodec("UTF-8");
  while (!in.atEnd()) {
    TmpFileData file;
    QString line = in.readLine();
    int splitter = line.lastIndexOf(QChar(','));
    if (splitter < 0) {
      for (auto& script : files) {
        if (script.data != nullptr) free(script.data);
      }
      return false;
    }
    QString path =
        QFileInfo(mlpPath).absolutePath() + "/" + line.left(splitter);
    file.id = line.right(line.length() - (splitter + 1)).toInt();
    QFileInfo fi(path);
    file.name = fi.fileName();
    file.size = fi.size();
    QFile scriptFile(path);
    if (!scriptFile.open(QIODevice::ReadOnly)) {
      for (auto& script : files) {
        if (script.data != nullptr) free(script.data);
      }
      return false;
    }
    file.data = (uint8_t*)malloc(file.size);
    scriptFile.read((char*)file.data, file.size);
    files.push_back(file);
  }
  return true;
}

const SCXFile* Project::currentFile() const {
  if (_currentFileId < 0 || _files.count(_currentFileId) == 0) return nullptr;
  return _files.at(_currentFileId).get();
}

void Project::switchFile(int id) {
  if (id >= 0 && _currentFileId != id && _files.count(id) > 0) {
    int previousId = _currentFileId;
    _currentFileId = id;
    emit fileSwitched(previousId);
  }
}

void Project::goToAddress(int fileId, SCXOffset address) {
  switchFile(fileId);
  emit focusAddressSwitched(address);
}

void Project::focusMemory(VariableRefType type, int var) {
  emit focusMemorySwitched(type, var);
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
  if (fileId < 0 || _files.count(fileId) == 0) return "";
  const SCXFile* file = _files.at(fileId).get();
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

  // ugly, but we want to return the fallback if name was empty
  emit labelNameChanged(fileId, labelId, getLabelName(fileId, labelId));
}

QString Project::getVarName(VariableRefType type, int var) {
  QVariant vtype;
  vtype.setValue(type);
  _getVarNameQuery.addBindValue(vtype);
  _getVarNameQuery.addBindValue(var);
  _getVarNameQuery.exec();
  _getVarNameQuery.next();

  QString result;
  if (_getVarNameQuery.isValid()) {
    result = QString::fromUtf8(_getVarNameQuery.value(0).toByteArray());
  }
  if (result.isEmpty()) {
    return QString("%1").arg(var);
  }
  return result;
}
void Project::setVarName(const QString& name, VariableRefType type, int var) {
  _setVarNameQuery.addBindValue(name.toUtf8());
  QVariant vtype;
  vtype.setValue(type);
  _setVarNameQuery.addBindValue(vtype);
  _setVarNameQuery.addBindValue(var);
  _setVarNameQuery.exec();

  if (!_batchUpdatingVars) {
    emit varNameChanged(type, var, getVarName(type, var));
  }
}

QString Project::getVarComment(VariableRefType type, int var) {
  QVariant vtype;
  vtype.setValue(type);
  _getVarCommentQuery.addBindValue(vtype);
  _getVarCommentQuery.addBindValue(var);
  _getVarCommentQuery.exec();
  _getVarCommentQuery.next();

  QString result;
  if (_getVarCommentQuery.isValid()) {
    result = QString::fromUtf8(_getVarCommentQuery.value(0).toByteArray());
  }
  return result;
}
void Project::setVarComment(const QString& comment, VariableRefType type,
                            int var) {
  _setVarCommentQuery.addBindValue(comment.toUtf8());
  QVariant vtype;
  vtype.setValue(type);
  _setVarCommentQuery.addBindValue(vtype);
  _setVarCommentQuery.addBindValue(var);
  _setVarCommentQuery.exec();

  if (!_batchUpdatingVars) {
    emit varCommentChanged(type, var, comment);
  }
}

void Project::analyzeFile(const SCXFile* file) {
  int fileId = file->getId();

  // TODO: batch inserts?
  // probably not necessary because of the transaction
  for (const auto& label : file->disassembly()) {
    for (const auto& inst : label->instructions()) {
      const SC3Instruction* instruction = inst.get();
      std::vector<std::pair<VariableRefType, int>> varRefs =
          variableRefsInInstruction(instruction);
      std::vector<int> localLabelRefs =
          localLabelRefsInInstruction(instruction);

      for (const auto& ref : varRefs) {
        insertVariableRef(fileId, inst->position(), ref.first, ref.second);
      }
      for (const auto& ref : localLabelRefs) {
        insertLocalLabelRef(fileId, inst->position(), ref);
      }
    }
  }
}

void Project::loadFilesFromDb() {
  _getFilesQuery.exec();

  std::vector<std::pair<int, SCXOffset>> result;
  while (_getFilesQuery.next()) {
    int id = _getFilesQuery.value(0).toInt();
    std::string name = _getFilesQuery.value(1).toByteArray().toStdString();
    QByteArray bdata = _getFilesQuery.value(2).toByteArray();
    uint8_t* data = (uint8_t*)malloc(bdata.size());
    memcpy(data, bdata.constData(), bdata.size());

    std::unique_ptr<SCXFile> scxFile =
        std::unique_ptr<SCXFile>(new SCXFile(data, bdata.size(), name, id));
    CCDisassembler dis(*scxFile);
    dis.DisassembleFile();

    _files[id] = std::move(scxFile);
  }
}

void Project::insertFile(const QString& name, uint8_t* data, int size, int id) {
  QVariant vdata(QByteArray::fromRawData((char*)data, size));

  std::unique_ptr<SCXFile> scxFile =
      std::unique_ptr<SCXFile>(new SCXFile(data, size, name.toStdString(), id));
  CCDisassembler dis(*scxFile);
  dis.DisassembleFile();

  analyzeFile(scxFile.get());

  _files[id] = std::move(scxFile);

  _insertFileQuery.addBindValue(id);
  _insertFileQuery.addBindValue(name.toUtf8());
  _insertFileQuery.addBindValue(vdata);
  _insertFileQuery.exec();
}

void Project::insertVariable(VariableRefType type, int var,
                             const QString& name) {
  QVariant vtype;
  vtype.setValue(type);
  _insertVariableQuery.addBindValue(vtype);
  _insertVariableQuery.addBindValue(var);
  _insertVariableQuery.addBindValue(name);
  _insertVariableQuery.exec();
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

std::vector<std::pair<int, SCXOffset>> Project::getLabelRefs(int fileId,
                                                             int labelId) {
  _getLabelRefsQuery.addBindValue(fileId);
  _getLabelRefsQuery.addBindValue(labelId);
  _getLabelRefsQuery.exec();

  std::vector<std::pair<int, SCXOffset>> result;
  while (_getLabelRefsQuery.next()) {
    result.emplace_back(_getLabelRefsQuery.value(0).toInt(),
                        _getLabelRefsQuery.value(1).toInt());
  }

  return result;
}

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

void Project::insertLocalLabelRef(int fileId, SCXOffset address, int labelId) {
  _insertLocalLabelRefQuery.addBindValue(fileId);
  _insertLocalLabelRefQuery.addBindValue(address);
  _insertLocalLabelRefQuery.addBindValue(labelId);
  _insertLocalLabelRefQuery.exec();
}

void Project::importWorklist(const QString& path, const char* encoding) {
  QFile inFile(path);
  if (!inFile.open(QFile::ReadOnly)) {
    throw std::runtime_error("Couldn't open worklist at " + path.toStdString());
  }
  QTextStream in(&inFile);
  in.setCodec(encoding);

  VariableRefType currentType = VariableRefType::GlobalVar;

  _db.transaction();
  _batchUpdatingVars = true;
  QString name;
  int var;
  QChar c;
  while (!in.atEnd()) {
    in >> name;
    bool settingVar;
    if (name.startsWith(":flag")) {
      currentType = VariableRefType::Flag;
      settingVar = false;
    } else if (name.startsWith(":work")) {
      currentType = VariableRefType::GlobalVar;
      settingVar = false;
    } else {
      in >> var;
      setVarName(name, currentType, var);
      settingVar = true;
    }
    in.skipWhiteSpace();
    // read comment if present
    qint64 pos = in.pos();
    in >> c;
    if (c == QChar('/')) {
      if (settingVar) {
        in >> c;
        QString comment = in.readLine();
        setVarComment(comment, currentType, var);
      } else {
        in.readLine();
      }
    } else {
      in.seek(pos);
    }
  }
  _db.commit();
  _batchUpdatingVars = false;
  emit allVarsChanged();
}

void Project::openDatabase(const QString& path) {
  _db = QSqlDatabase::addDatabase("QSQLITE");
  _db.setDatabaseName(path);
  _db.open();
}

void Project::createDatabase(const QString& path) {
  if (QFile(path).exists()) QFile(path).remove();

  openDatabase(path);

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
      "CREATE TABLE variables("
      "variableType INTEGER NOT NULL,"
      "variable INTEGER NOT NULL,"
      "name TEXT NOT NULL,"
      "comment TEXT NULL,"
      "PRIMARY KEY (variableType, variable)"
      ")");
  q.exec(
      "CREATE TABLE variableRefs("
      "refId INTEGER PRIMARY KEY,"
      "fileId INTEGER NOT NULL,"
      "address INTEGER NOT NULL,"
      "variableType INTEGER NOT NULL,"
      "variable INTEGER NOT NULL"
      ")");
  q.exec(
      "CREATE TABLE localLabelRefs("
      "refId INTEGER PRIMARY KEY,"
      "fileId INTEGER NOT NULL,"
      "address INTEGER NOT NULL,"
      "labelId INTEGER NOT NULL"
      ")");
  prepareStmts();
}

// Unfortunately these tables need to exist before we can prepare the statements
void Project::prepareStmts() {
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
  _getFilesQuery = QSqlQuery(_db);
  _getFilesQuery.prepare("SELECT id, name, data FROM files ORDER BY id ASC");
  _insertFileQuery = QSqlQuery(_db);
  _insertFileQuery.prepare(
      "INSERT INTO files (id, name, data) VALUES (?, ?, ?)");
  _getAllVarNamesQuery = QSqlQuery(_db);
  _getAllVarNamesQuery.prepare(
      "SELECT name FROM variables WHERE variableType = ? ORDER BY variable "
      "ASC");
  _getVarNameQuery = QSqlQuery(_db);
  _getVarNameQuery.prepare(
      "SELECT name FROM variables WHERE variableType = ? AND variable = ?");
  _getVarCommentQuery = QSqlQuery(_db);
  _getVarCommentQuery.prepare(
      "SELECT comment FROM variables WHERE variableType = ? AND variable = ?");
  _insertVariableQuery = QSqlQuery(_db);
  _insertVariableQuery.prepare(
      "INSERT INTO variables (variableType, variable, name) VALUES (?, ?, ?)");
  _setVarNameQuery = QSqlQuery(_db);
  _setVarNameQuery.prepare(
      "UPDATE variables SET name = ? WHERE variableType = ? AND variable = ?");
  _setVarCommentQuery = QSqlQuery(_db);
  _setVarCommentQuery.prepare(
      "UPDATE variables SET comment = ? WHERE variableType = ? AND variable = "
      "?");
  _getVariableRefsQuery = QSqlQuery(_db);
  _getVariableRefsQuery.prepare(
      "SELECT fileId, address FROM variableRefs WHERE variableType = ? AND "
      "variable = ?");
  _insertVariableRefQuery = QSqlQuery(_db);
  _insertVariableRefQuery.prepare(
      "INSERT INTO variableRefs (fileId, address, variableType, variable) "
      "VALUES (?, ?, ?, ?)");
  // TODO union with far label refs
  _getLabelRefsQuery = QSqlQuery(_db);
  _getLabelRefsQuery.prepare(
      "SELECT fileId, address FROM localLabelRefs WHERE fileId = ? AND labelId "
      "= ?");
  _insertLocalLabelRefQuery = QSqlQuery(_db);
  _insertLocalLabelRefQuery.prepare(
      "INSERT INTO localLabelRefs (fileId, address, labelId) VALUES (?, ?, ?)");
}
