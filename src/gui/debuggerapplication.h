#pragma once

#include <QApplication>

class MainWindow;

#define dApp static_cast<::DebuggerApplication*>(QCoreApplication::instance())

class DebuggerApplication : public QApplication {
  Q_OBJECT

 public:
  explicit DebuggerApplication(int& argc, char** argv);
  ~DebuggerApplication();

  void showWindow();

  MainWindow* window() { return _w; }

 private:
  MainWindow* _w;
};