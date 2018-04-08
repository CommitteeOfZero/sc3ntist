#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QTableWidget>
#include <parser/SCXTypes.h>

class XrefDialog : public QDialog {
  Q_OBJECT

 public:
  explicit XrefDialog(int fileId, SCXOffset address, QWidget *parent = 0);

 public slots:
  int exec() override;

 private:
  QTableWidget *_table;
  QDialogButtonBox *_buttons;
};