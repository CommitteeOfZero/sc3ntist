#include "project.h"
#include <parser/CCDisassembler.h>
#include <QFile>
#include <QDirIterator>
#include <QStringList>
#include <stdexcept>

Project::Project(const QString& path, QObject* parent = 0) : QObject(parent) {
  initDatabase();

  // TODO: read from mpk, or folder + mlp index

  QDirIterator it(path, QStringList() << "*.scx", QDir::Files | QDir::Readable);

  int fileId = 0;

  while (it.hasNext()) {
    QFile file(it.next());
    if (!file.open(QIODevice::ReadOnly)) {
      throw std::runtime_error("Couldn't open file");
    }
    SCXOffset length = (SCXOffset)file.size();
    uint8_t* data = (uint8_t*)malloc(length);
    file.read((char*)data, length);
    file.close();
    std::unique_ptr<SCXFile> scxFile = std::make_unique<SCXFile>(
        data, length, it.fileName().toStdString(), fileId++);
    CCDisassembler dis(*scxFile);
    dis.DisassembleFile();
    _files.push_back(std::move(scxFile));
  }
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

  _getCommentQuery = QSqlQuery(_db);
  _getCommentQuery.prepare(
      "SELECT text FROM comments WHERE fileId = ? AND address = ?");
  _setCommentQuery = QSqlQuery(_db);
  _setCommentQuery.prepare(
      "REPLACE INTO comments (fileId, address, text) VALUES (?, ?, ?)");
}