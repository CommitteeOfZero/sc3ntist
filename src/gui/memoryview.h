#pragma once
#include <QTableView>
#include "enums.h"

class MemoryView : public QTableView {
  Q_OBJECT
 public:
  explicit MemoryView(QWidget *parent = 0);
  ~MemoryView(){};

 public slots:
  void goToVar(VariableRefType type, int var);

 private slots:
  void onXrefKeyPress();
  void onGoKeyPress();

  void adjustHeader(int oldCount, int newCount);

  void onProjectOpened();
  void onProjectClosed();
};