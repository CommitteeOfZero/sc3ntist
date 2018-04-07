#pragma once

#include <vector>
#include <QObject>
#include "parser/SCXFile.h"

class Project : public QObject {
  Q_OBJECT

 public:
  explicit Project(const QString& path, QObject* parent);

  std::vector<std::unique_ptr<SCXFile>>& files() { return _files; }
  int currentFileId() const { return _currentFileId; }
  const SCXFile* currentFile() const;

  void switchFile(int id);

 signals:
  void fileSwitched(int previousId);

 private:
  std::vector<std::unique_ptr<SCXFile>> _files;
  int _currentFileId = -1;
};