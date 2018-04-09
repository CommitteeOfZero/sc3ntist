#pragma once

#include <vector>
#include <QObject>
#include "parser/SCXFile.h"
#include <QtSql>
#include "enums.h"

class Project : public QObject {
  Q_OBJECT

 public:
  // create new project
  explicit Project(const QString& dbPath, const QString& scriptFolder,
                   QObject* parent);
  // open old project
  explicit Project(const QString& dbPath, QObject* parent);
  ~Project();

  std::vector<std::unique_ptr<SCXFile>>& files() { return _files; }
  int currentFileId() const { return _currentFileId; }
  const SCXFile* currentFile() const;

  void switchFile(int id);

  QString getComment(int fileId, SCXOffset address);
  void setComment(int fileId, SCXOffset address, const QString& comment);

  QString getLabelName(int fileId, int labelId);
  void setLabelName(int fileId, int labelId, const QString& name);

  std::vector<std::pair<int, SCXOffset>> getVariableRefs(VariableRefType type,
                                                         int var);
  std::vector<std::pair<int, SCXOffset>> getLabelRefs(int fileId, int labelId);

 signals:
  void fileSwitched(int previousId);
  void commentChanged(int fileId, SCXOffset address, const QString& comment);
  void labelNameChanged(int fileId, int labelId, const QString& name);

 private:
  QSqlDatabase _db;
  std::vector<std::unique_ptr<SCXFile>> _files;
  int _currentFileId = -1;
  bool _inInitialLoad = true;

  void createDatabase(const QString& path);
  void openDatabase(const QString& path);
  void prepareStmts();
  void analyzeFile(const SCXFile* file);
  void loadFilesFromDb();
  void insertFile(const QString& name, uint8_t* data, int size);
  void insertVariableRef(int fileId, SCXOffset address, VariableRefType type,
                         int var);
  void insertLocalLabelRef(int fileId, SCXOffset address, int labelId);

  QSqlQuery _getCommentQuery;
  QSqlQuery _setCommentQuery;
  QSqlQuery _getLabelNameQuery;
  QSqlQuery _setLabelNameQuery;
  QSqlQuery _getFilesQuery;
  QSqlQuery _insertFileQuery;
  QSqlQuery _getVariableRefsQuery;
  QSqlQuery _insertVariableRefQuery;
  QSqlQuery _getLabelRefsQuery;
  QSqlQuery _insertLocalLabelRefQuery;
};