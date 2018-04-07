#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "debuggerapplication.h"
#include "textdump.h"
#include "project.h"
#include "parser/SCXFile.h"
#include <vector>
#include "disassemblymodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  _treeView = new QTreeView(this);
  _treeView->setUniformRowHeights(true);
  _treeView->setItemsExpandable(false);
  _treeView->setWordWrap(false);
  _treeView->setIndentation(0);
  _treeView->setRootIsDecorated(false);
  setCentralWidget(_treeView);

  connect(dApp, &DebuggerApplication::projectOpened, this,
          &MainWindow::onProjectOpened);
  connect(dApp, &DebuggerApplication::projectClosed, this,
          &MainWindow::onProjectClosed);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onProjectOpened() {
  QAbstractItemModel *oldModel = _treeView->model();
  _treeView->setModel(
      new DisassemblyModel(dApp->project()->files().at(0).get()));
  if (oldModel != nullptr) delete oldModel;
  _treeView->expandAll();
}

void MainWindow::onProjectClosed() {
  QAbstractItemModel *oldModel = _treeView->model();
  _treeView->setModel(nullptr);
  if (oldModel != nullptr) delete oldModel;
}

void MainWindow::on_actionOpen_triggered() {
  dApp->tryCreateProject(
      "G:/Games/SGTL/CCEnVitaPatch101/script_dis/_system.scx");
}

void MainWindow::on_actionClose_triggered() { dApp->closeProject(); }
