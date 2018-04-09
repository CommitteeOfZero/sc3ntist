#pragma once

#include <QApplication>

class MainWindow;
class Project;

#define dApp static_cast<::DebuggerApplication*>(QCoreApplication::instance())

class DebuggerApplication : public QApplication {
  Q_OBJECT

 public:
  explicit DebuggerApplication(int& argc, char** argv);
  ~DebuggerApplication();

  void showWindow();

  MainWindow* window() { return _w; }
  Project* project() { return _project; }

  bool tryOpenProject(const QString& dbPath);
  bool tryCreateProject(const QString& dbPath, const QString& scriptFolder);
  void closeProject();

 signals:
  void projectClosed();
  void projectOpened();

 private:
  MainWindow* _w;
  Project* _project = nullptr;
};