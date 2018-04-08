#pragma once

#include <QMainWindow>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class DisassemblyView;

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

 private slots:
  void onProjectOpened();
  void onProjectClosed();
  void onFileSwitched(int previousId);
  void onLabelNameChanged(int fileId, int labelId, const QString &name);
  void on_actionOpen_triggered();
  void on_actionClose_triggered();
  void on_actionGo_to_address_triggered();
};
