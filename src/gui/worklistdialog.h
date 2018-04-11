#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QRadioButton>

class WorklistDialog : public QDialog {
  Q_OBJECT

 public:
  explicit WorklistDialog(QWidget *parent = 0);

 private slots:
  void okClicked();
  void browseClicked();

 private:
  QDialogButtonBox *_buttons;
  QLineEdit *_pathEdit;
  QRadioButton *_sjisRadio;
  QRadioButton *_utf8Radio;
};
