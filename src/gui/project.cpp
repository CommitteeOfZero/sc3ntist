#include "project.h"
#include <parser/CCDisassembler.h>
#include <QFile>
#include <QDirIterator>
#include <QStringList>
#include <stdexcept>

Project::Project(const QString& path, QObject* parent = 0) : QObject(parent) {
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
    std::unique_ptr<SCXFile> scxFile =
        std::make_unique<SCXFile>(data, length, it.fileName().toStdString());
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