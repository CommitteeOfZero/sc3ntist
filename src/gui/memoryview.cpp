#include "memoryview.h"
#include "memorymodel.h"
#include "debuggerapplication.h"
#include "project.h"
#include "xrefdialog.h"
#include <QHeaderView>
#include <QShortcut>

MemoryView::MemoryView(QWidget* parent) : QTableView(parent) {
  connect(dApp, &DebuggerApplication::projectOpened, this,
          &MemoryView::onProjectOpened);
  connect(dApp, &DebuggerApplication::projectClosed, this,
          &MemoryView::onProjectClosed);

  connect(horizontalHeader(), &QHeaderView::sectionCountChanged, this,
          &MemoryView::adjustHeader);

  horizontalHeader()->setSectionsMovable(false);
  horizontalHeader()->setSectionsClickable(false);

  verticalHeader()->setVisible(false);

  // TODO: resolve shortcut conflict
  QShortcut* xrefShortcut = new QShortcut(Qt::Key_X, this);
  connect(xrefShortcut, &QShortcut::activated, this,
          &MemoryView::onXrefKeyPress);
  xrefShortcut->setContext(Qt::WidgetWithChildrenShortcut);
  QShortcut* goShortcut = new QShortcut(Qt::Key_G, this);
  connect(goShortcut, &QShortcut::activated, this, &MemoryView::onGoKeyPress);
  goShortcut->setContext(Qt::WidgetWithChildrenShortcut);
}

void MemoryView::adjustHeader(int oldCount, int newCount) {
  if (newCount != (int)MemoryModel::ColumnType::NumColumns) return;
  horizontalHeader()->setSectionResizeMode(
      (int)MemoryModel::ColumnType::VarType, QHeaderView::ResizeToContents);
}

void MemoryView::goToVar(VariableRefType type, int var) {
  const MemoryModel* memModel = qobject_cast<MemoryModel*>(model());
  if (memModel == nullptr) return;

  QModelIndex index = memModel->indexForVar(type, var);
  setCurrentIndex(index);
  // in case the line was already selected, still scroll there
  scrollTo(index);
}

void MemoryView::onProjectOpened() {
  connect(dApp->project(), &Project::focusMemorySwitched, this,
          &MemoryView::goToVar);

  QAbstractItemModel* oldModel = model();
  setModel(new MemoryModel());
  if (oldModel != nullptr) delete oldModel;
}

void MemoryView::onProjectClosed() {
  QAbstractItemModel* oldModel = model();
  setModel(nullptr);
  if (oldModel != nullptr) delete oldModel;
}

void MemoryView::onXrefKeyPress() {
  const MemoryModel* memModel = qobject_cast<MemoryModel*>(model());
  if (memModel == nullptr) return;

  VariableRefType type;
  int var;
  std::tie(type, var) = memModel->varForIndex(currentIndex());
  if (var < 0) return;
  XrefDialog(type, var, this).exec();
}

void MemoryView::onGoKeyPress() {}