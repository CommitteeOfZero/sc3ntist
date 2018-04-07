#include "project.h"
#include <parser/CCDisassembler.h>
#include <QFile>
#include <stdexcept>

Project::Project(const QString& filePath, QObject* parent = 0)
    : QObject(parent) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("Couldn't open file");
  }
  SCXOffset length = (SCXOffset)file.size();
  uint8_t* data = (uint8_t*)malloc(length);
  file.read((char*)data, length);
  file.close();
  std::unique_ptr<SCXFile> scxFile = std::make_unique<SCXFile>(data, length);
  CCDisassembler dis(*scxFile);
  dis.DisassembleFile();
  _files.push_back(std::move(scxFile));
}