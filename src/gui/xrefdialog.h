#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QTableWidget>
#include <parser/SCXTypes.h>

class XrefDialog : public QDialog {
  Q_OBJECT

 public:
  explicit XrefDialog(int fileId, int labelIdOrAddress, bool isLabel,
                      QWidget *parent = 0);

 public slots:
  int exec() override;

 private:
  void addReferences(int &i, const QString &inRefText,
                     const std::vector<std::pair<int, SCXOffset>> &outRefs);

  QTableWidget *_table;
  QDialogButtonBox *_buttons;
};
