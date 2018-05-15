#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>

class NewProjectDialog : public QDialog {
  Q_OBJECT

 public:
  explicit NewProjectDialog(QWidget *parent = 0);

 private slots:
  void okClicked();
  void scriptBrowseClicked();
  void projectBrowseClicked();

 private:
  QDialogButtonBox *_buttons;
  QLineEdit *_scriptPathEdit;
  QLineEdit *_projectPathEdit;
  QComboBox *_gameComboBox;
};
