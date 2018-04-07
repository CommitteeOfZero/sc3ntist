#include "debuggerapplication.h"
#include "mainwindow.h"

DebuggerApplication::DebuggerApplication(int& argc, char** argv)
    : QApplication(argc, argv) {
  _w = new MainWindow(0);
}

DebuggerApplication::~DebuggerApplication() { delete _w; }

void DebuggerApplication::showWindow() { _w->show(); }