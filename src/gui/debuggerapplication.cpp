#include "debuggerapplication.h"
#include "mainwindow.h"
#include "project.h"
#include <QFile>
#include <QTextStream>

DebuggerApplication::DebuggerApplication(int& argc, char** argv)
    : QApplication(argc, argv) {
  _w = new MainWindow(0);

  // TODO style with item delegates instead for flexibility
  QFile qssFile(":/stylesheet.qss");
  qssFile.open(QFile::ReadOnly | QFile::Text);
  QTextStream ts(&qssFile);
  setStyleSheet(ts.readAll());
}

DebuggerApplication::~DebuggerApplication() { delete _w; }

void DebuggerApplication::showWindow() { _w->show(); }

bool DebuggerApplication::tryOpenProject(const QString& dbPath) {
  if (_project != nullptr) closeProject();

  try {
    _project = new Project(dbPath, this);
  } catch (std::runtime_error e) {
    return false;
  }
  emit projectOpened();
  return true;
}

bool DebuggerApplication::tryCreateProject(const QString& dbPath,
                                           const QString& loadPath,
                                           const SupportedGame* game) {
  if (_project != nullptr) closeProject();

  try {
    _project = new Project(dbPath, loadPath, game, this);
  } catch (std::runtime_error e) {
    return false;
  }
  emit projectOpened();
  return true;
}

void DebuggerApplication::closeProject() {
  if (_project != nullptr) {
    emit projectClosed();
    delete _project;
    _project = nullptr;
  }
}