#include "mainwindow.h"
#include "debuggerapplication.h"

int main(int argc, char *argv[]) {
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  DebuggerApplication a(argc, argv);
  a.showWindow();

  return a.exec();
}
