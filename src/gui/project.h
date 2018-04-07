#pragma once

#include <vector>
#include <QObject>
#include "parser/SCXFile.h"

class Project : public QObject {
  Q_OBJECT

 public:
  explicit Project(const QString& filePath, QObject* parent);

  std::vector<std::unique_ptr<SCXFile>>& files() { return _files; }

 private:
  std::vector<std::unique_ptr<SCXFile>> _files;
};