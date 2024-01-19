#pragma once

#include <QMainWindow>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class DisassemblyView;
class MemoryView;

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  DisassemblyView *_disasmView;
  QListWidget *_fileList;
  QListWidget *_labelList;
  MemoryView *_memoryView;

 private slots:
  void onProjectOpened();
  void onProjectClosed();
  void onFileSwitched(int previousId);
  void onLabelNameChanged(int fileId, int labelId, const QString &name);
  void on_actionOpen_triggered();
  void on_actionClose_triggered();
  void on_actionGo_to_address_triggered();
  void on_actionEdit_stylesheet_triggered();
  void on_actionImport_worklist_triggered();
  void on_actionNew_project_triggered();
  void on_actionExport_Files_triggered();
};
