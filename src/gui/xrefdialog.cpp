#include "xrefdialog.h"
#include "analysis.h"
#include "debuggerapplication.h"
#include "project.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include "viewhelper.h"

XrefDialog::XrefDialog(int fileId, int labelIdOrAddress, bool isLabel,
                       QWidget *parent)
    : QDialog(parent) {
  _table = new QTableWidget(this);

  _table->setColumnCount(2);
  _table->setHorizontalHeaderLabels(QStringList() << "Item"
                                                  << "Referenced at");
  _table->horizontalHeader()->setSectionsMovable(false);
  _table->horizontalHeader()->setSectionsClickable(true);
  // TODO user resizable
  _table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  _table->setSelectionBehavior(QAbstractItemView::SelectRows);
  _table->setSelectionMode(QAbstractItemView::SingleSelection);
  _table->verticalHeader()->setVisible(false);

  if (isLabel) {
    // refs to label
    QString inRefText = QString("#%1").arg(
        dApp->project()->getLabelName(fileId, labelIdOrAddress));
    int i = 0;
    auto outLabelRefs = dApp->project()->getLabelRefs(fileId, labelIdOrAddress);
    // also show the label itself
    outLabelRefs.emplace_back(fileId, dApp->project()
                                          ->files()[fileId]
                                          ->disassembly()[labelIdOrAddress]
                                          ->address());
    addReferences(i, inRefText, outLabelRefs);
  } else {
    if (fileId < 0 || fileId >= dApp->project()->files().size()) return;
    const SCXFile *file = dApp->project()->files()[fileId].get();

    int i = 0;

    auto inVarRefs = variableRefsAtAddress(file, labelIdOrAddress);
    for (const auto &inRef : inVarRefs) {
      QString inRefText;
      if (inRef.first == VariableRefType ::GlobalVar) {
        inRefText = QString("GlobalVars[%1]").arg(inRef.second);
      } else {
        inRefText = QString("Flags[%1]").arg(inRef.second);
      }
      addReferences(
          i, inRefText,
          dApp->project()->getVariableRefs(inRef.first, inRef.second));
    }

    auto inLabelRefs = localLabelRefsAtAddress(file, labelIdOrAddress);
    for (int labelId : inLabelRefs) {
      QString inRefText =
          QString("#%1").arg(dApp->project()->getLabelName(fileId, labelId));
      auto outLabelRefs = dApp->project()->getLabelRefs(fileId, labelId);
      // also show the label itself
      outLabelRefs.emplace_back(
          fileId,
          dApp->project()->files()[fileId]->disassembly()[labelId]->address());
      addReferences(i, inRefText, outLabelRefs);
    }

    if (i == 0) return;
  }

  _table->setSortingEnabled(true);
  _table->sortByColumn(0, Qt::AscendingOrder);
  _table->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);

  _buttons =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(_table);
  layout->addWidget(_buttons);
  setLayout(layout);

  resize(500, 300);

  connect(_table, &QTableWidget::itemDoubleClicked, this, &XrefDialog::accept);
}

void XrefDialog::addReferences(
    int &i, const QString &inRefText,
    const std::vector<std::pair<int, SCXOffset>> &outRefs) {
  for (const auto &outRef : outRefs) {
    const auto &refFile = dApp->project()->files()[outRef.first];
    _table->insertRow(i);
    auto col0 = new QTableWidgetItem(inRefText);
    col0->setData(Qt::UserRole, QVariant(i));
    _table->setItem(i, 0, col0);
    auto col1 = new QTableWidgetItem(
        QString("%1@%2").arg(QString::fromStdString(refFile->getName()),
                             displayTextForAddress(outRef.second)));
    col1->setData(Qt::UserRole, QVariant(i));
    _table->setItem(i, 1, col1);
    i++;
  }
  std::copy(outRefs.begin(), outRefs.end(), std::back_inserter(_outAddresses));
}

int XrefDialog::exec() {
  if (_table->rowCount() < 1) return QDialog::Rejected;
  return QDialog::exec();
}

void XrefDialog::accept() {
  QDialog::accept();
  goToItem();
}

void XrefDialog::goToItem() {
  auto selectedItems = _table->selectedItems();
  if (selectedItems.size() == 0) return;
  auto tableItem = selectedItems.first();
  if (tableItem == nullptr) return;
  auto refId = tableItem->data(Qt::UserRole).toInt();
  if (refId < 0 || refId > _outAddresses.size()) return;
  dApp->project()->goToAddress(_outAddresses[refId].first,
                               _outAddresses[refId].second);
}