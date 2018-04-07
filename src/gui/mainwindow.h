#pragma once

#include <QMainWindow>
#include <QTreeView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  QTreeView *_treeView;

 private slots:
  void onProjectOpened();
  void onProjectClosed();
  void on_actionOpen_triggered();
  void on_actionClose_triggered();
};
