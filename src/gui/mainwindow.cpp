#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "debuggerapplication.h"
#include "textdump.h"
#include "project.h"
#include "parser/SCXFile.h"
#include <vector>
#include "disassemblymodel.h"
#include "disassemblyview.h"
#include <QDockWidget>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setDockOptions(AnimatedDocks | AllowNestedDocks | AllowTabbedDocks |
                 GroupedDragging);

  // TODO: move children into custom subclasses

  QDockWidget *disassemblyDock = new QDockWidget("Disassembly", this);
  disassemblyDock->setFeatures(disassemblyDock->features() &
                               ~QDockWidget::DockWidgetClosable);
  disassemblyDock->setAllowedAreas(Qt::AllDockWidgetAreas);
  _disasmView = new DisassemblyView(disassemblyDock);
  disassemblyDock->setWidget(_disasmView);
  addDockWidget(Qt::RightDockWidgetArea, disassemblyDock);

  QDockWidget *fileListDock = new QDockWidget("File list", this);
  fileListDock->setFeatures(fileListDock->features() &
                            ~QDockWidget::DockWidgetClosable);
  fileListDock->setAllowedAreas(Qt::AllDockWidgetAreas);
  _fileList = new QListWidget(fileListDock);
  fileListDock->setWidget(_fileList);
  addDockWidget(Qt::LeftDockWidgetArea, fileListDock);
  connect(_fileList, &QListWidget::itemActivated,
          [=]() { dApp->project()->switchFile(_fileList->currentRow()); });

  // without a centralwidget we need to fill *at least* the whole width or the
  // resize is ignored
  resizeDocks({fileListDock, disassemblyDock}, {200, width()}, Qt::Horizontal);

  connect(dApp, &DebuggerApplication::projectOpened, this,
          &MainWindow::onProjectOpened);
  connect(dApp, &DebuggerApplication::projectClosed, this,
          &MainWindow::onProjectClosed);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onProjectOpened() {
  connect(dApp->project(), &Project::fileSwitched, this,
          &MainWindow::onFileSwitched);

  const auto &files = dApp->project()->files();
  auto fileCount = files.size();
  for (int i = 0; i < fileCount; i++) {
    _fileList->addItem(QString("[%1] %2").arg(i, 3).arg(
        QString::fromStdString(files[i]->getName())));
  }
  dApp->project()->switchFile(0);
}

void MainWindow::onProjectClosed() { _fileList->clear(); }

void MainWindow::onFileSwitched(int previousId) {
  {
    if (previousId >= 0) {
      _fileList->item(previousId)
          ->setText(
              QString("[%1] %2")
                  .arg(previousId, 3)
                  .arg(QString::fromStdString(
                      dApp->project()->files().at(previousId)->getName())));
    }
    int currentFileId = dApp->project()->currentFileId();
    auto currentFileName = dApp->project()->currentFile()->getName();
    _fileList->setCurrentRow(currentFileId);
    _fileList->currentItem()->setText(
        QString("==> [%1] %2")
            .arg(currentFileId, 3)
            .arg(QString::fromStdString(currentFileName)));
  }
}

void MainWindow::on_actionOpen_triggered() {
  dApp->tryCreateProject("G:/Games/SGTL/CCEnVitaPatch101/script_dis");
}

void MainWindow::on_actionClose_triggered() { dApp->closeProject(); }

void MainWindow::on_actionGo_to_address_triggered() {
  // TODO: disable item when no script loaded
  bool ok;
  QString input = QInputDialog::getText(
      this, "Go to address", "Address:", QLineEdit::Normal, QString(), &ok);
  if (!ok) return;
  int address = input.toInt(&ok, 0);
  if (!ok) return;
  _disasmView->goToAddress(address);
}
