#include "worklistdialog.h"
#include "debuggerapplication.h"
#include "project.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <stdexcept>

WorklistDialog::WorklistDialog(QWidget *parent) : QDialog(parent) {
  _buttons =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(_buttons, &QDialogButtonBox::accepted, this,
          &WorklistDialog::okClicked);
  connect(_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QHBoxLayout *pathLayout = new QHBoxLayout(this);
  QLabel *pathLabel = new QLabel("Path:", this);
  _pathEdit = new QLineEdit(this);
  QPushButton *pathButton = new QPushButton("Browse...", this);
  connect(pathButton, &QPushButton::clicked, this,
          &WorklistDialog::browseClicked);

  pathLayout->addWidget(pathLabel);
  pathLayout->addWidget(_pathEdit);
  pathLayout->addWidget(pathButton);

  QHBoxLayout *encodingLayout = new QHBoxLayout(this);
  QLabel *encodingLabel = new QLabel("Encoding:", this);
  _sjisRadio = new QRadioButton("Shift-JIS", this);
  _utf8Radio = new QRadioButton("UTF-8", this);
  _sjisRadio->setChecked(true);

  encodingLayout->addWidget(encodingLabel);
  encodingLayout->addWidget(_sjisRadio);
  encodingLayout->addWidget(_utf8Radio);

  layout->addLayout(pathLayout);
  layout->addLayout(encodingLayout);
  layout->addWidget(_buttons);
  setLayout(layout);

  resize(500, height());
}

void WorklistDialog::okClicked() {
  try {
    if (_sjisRadio->isChecked()) {
      dApp->project()->importWorklist(_pathEdit->text(), "Shift-JIS");
    } else {
      dApp->project()->importWorklist(_pathEdit->text(), "UTF-8");
    }
    accept();
  } catch (const std::runtime_error &e) {
    QMessageBox::critical(this, "Error", "Could not read worklist");
  }
}

void WorklistDialog::browseClicked() {
  QFileDialog fileDlg(this, "Select worklist...");
  fileDlg.selectFile(_pathEdit->text());
  fileDlg.setFileMode(QFileDialog::ExistingFile);
  fileDlg.setOption(QFileDialog::ReadOnly, true);
  fileDlg.setNameFilter("worklist.txt (*.txt)");
  if (fileDlg.exec()) {
    _pathEdit->setText(fileDlg.selectedFiles().first());
  }
}