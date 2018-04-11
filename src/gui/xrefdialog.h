#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QTableWidget>
#include <parser/SCXTypes.h>
#include "enums.h"

class XrefDialog : public QDialog {
  Q_OBJECT

 public:
  explicit XrefDialog(int fileId, int labelIdOrAddress, bool isLabel,
                      QWidget *parent = 0);
  explicit XrefDialog(VariableRefType type, int var, QWidget *parent = 0);

 public slots:
  int exec() override;
  void accept() override;

 private slots:
  void goToItem();

 private:
  void setupViewBeforeData();
  void setupViewAfterData();
  void addReferences(int &i, const QString &inRefText,
                     const std::vector<std::pair<int, SCXOffset>> &outRefs);
  void addVarRefs(int &i,
                  const std::vector<std::pair<VariableRefType, int>> &varRefs);

  QTableWidget *_table;
  QDialogButtonBox *_buttons;

  std::vector<std::pair<int, SCXOffset>> _outAddresses;
};
