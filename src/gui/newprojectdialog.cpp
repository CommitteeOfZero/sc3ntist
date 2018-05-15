#include "newprojectdialog.h"
#include "debuggerapplication.h"
#include "parser/SupportedGame.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <stdexcept>

NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent) {
  _buttons =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(_buttons, &QDialogButtonBox::accepted, this,
          &NewProjectDialog::okClicked);
  connect(_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QHBoxLayout *scriptPathLayout = new QHBoxLayout(this);
  QLabel *scriptPathLabel = new QLabel("Script path:", this);
  _scriptPathEdit = new QLineEdit(this);
  QPushButton *scriptPathButton = new QPushButton("Browse...", this);
  connect(scriptPathButton, &QPushButton::clicked, this,
          &NewProjectDialog::scriptBrowseClicked);

  scriptPathLayout->addWidget(scriptPathLabel);
  scriptPathLayout->addWidget(_scriptPathEdit);
  scriptPathLayout->addWidget(scriptPathButton);

  QHBoxLayout *projectPathLayout = new QHBoxLayout(this);
  QLabel *projectPathLabel = new QLabel("Project path:", this);
  _projectPathEdit = new QLineEdit(this);
  QPushButton *projectPathButton = new QPushButton("Browse...", this);
  connect(projectPathButton, &QPushButton::clicked, this,
          &NewProjectDialog::projectBrowseClicked);

  projectPathLayout->addWidget(projectPathLabel);
  projectPathLayout->addWidget(_projectPathEdit);
  projectPathLayout->addWidget(projectPathButton);

  QHBoxLayout *gameLayout = new QHBoxLayout(this);
  QLabel *gameLabel = new QLabel("Game:", this);
  _gameComboBox = new QComboBox(this);
  for (const auto &game : SupportedGames) {
    _gameComboBox->addItem(QString::fromStdString(game->name()),
                           QVariant(game->id()));
  }

  gameLayout->addWidget(gameLabel);
  gameLayout->addWidget(_gameComboBox);

  layout->addLayout(scriptPathLayout);
  layout->addLayout(projectPathLayout);
  layout->addLayout(gameLayout);
  layout->addWidget(_buttons);
  setLayout(layout);

  resize(500, height());
}

void NewProjectDialog::okClicked() {
  int id = _gameComboBox->currentData().toInt();
  if (id < 0 || id >= SupportedGames.size()) {
    QMessageBox::critical(this, "Error", "Could not create project");
    return;
  }

  if (!dApp->tryCreateProject(_projectPathEdit->text(), _scriptPathEdit->text(),
                              SupportedGames[id])) {
    QMessageBox::critical(this, "Error", "Could not create project");
    return;
  }
  accept();
}

void NewProjectDialog::scriptBrowseClicked() {
  QFileDialog fileDlg(this, "Select scripts...");
  fileDlg.selectFile(_scriptPathEdit->text());
  fileDlg.setFileMode(QFileDialog::ExistingFile);
  fileDlg.setOption(QFileDialog::ReadOnly, true);
  fileDlg.setNameFilter("script.mpk (*.mpk);;MLP index (*.mlp)");
  if (fileDlg.exec()) {
    _scriptPathEdit->setText(fileDlg.selectedFiles().first());
  }
}

void NewProjectDialog::projectBrowseClicked() {
  QFileDialog fileDlg(this, "Select project save file...");
  fileDlg.selectFile(_scriptPathEdit->text());
  fileDlg.setFileMode(QFileDialog::AnyFile);
  fileDlg.setAcceptMode(QFileDialog::AcceptSave);
  fileDlg.setNameFilter("Project files (*.sqlite)");
  if (fileDlg.exec()) {
    _projectPathEdit->setText(fileDlg.selectedFiles().first());
  }
}