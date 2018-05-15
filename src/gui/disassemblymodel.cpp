#include "disassemblymodel.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"
#include <sstream>
#include "textdump.h"
#include <algorithm>
#include "debuggerapplication.h"
#include "project.h"
#include "analysis.h"
#include "viewhelper.h"

DisassemblyModel::DisassemblyModel(const SCXFile *script, QObject *parent)
    : QAbstractItemModel(parent), _script(script) {
  reload();

  connect(dApp->project(), &Project::commentChanged, this,
          &DisassemblyModel::onCommentChanged);
  connect(dApp->project(), &Project::labelNameChanged, this,
          &DisassemblyModel::onLabelNameChanged);
  connect(dApp->project(), &Project::varNameChanged, this,
          &DisassemblyModel::onVarNameChanged);
  connect(dApp->project(), &Project::allVarsChanged, this,
          &DisassemblyModel::onAllVarsChanged);
}

void DisassemblyModel::reload() {
  _labelRows.clear();

  int labelCount = _script->disassembly().size();
  _labelRows.reserve(labelCount);
  for (int i = 0; i < labelCount; i++) {
    const SC3CodeBlock *label = _script->disassembly()[i].get();
    DisassemblyRow labelRow;
    labelRow.type = RowType::Label;
    labelRow.id = i;
    labelRow.address = label->address();
    labelRow.parent = nullptr;
    int instCount = label->instructions().size();
    labelRow.children.reserve(instCount + 1);
    _labelRows.push_back(std::move(labelRow));
    DisassemblyRow *movedLabelRow = &_labelRows.data()[i];
    for (int j = 0; j < instCount; j++) {
      DisassemblyRow instRow;
      instRow.type = RowType::Instruction;
      instRow.id = j;
      instRow.address = label->instructions()[j]->position();
      instRow.parent = movedLabelRow;
      movedLabelRow->children.push_back(std::move(instRow));
      DisassemblyRow *movedInstRow = &movedLabelRow->children.data()[j];

      QString comment =
          dApp->project()->getComment(_script->getId(), movedInstRow->address);
      if (!comment.isEmpty()) {
        DisassemblyRow commentRow;
        commentRow.type = RowType::Comment;
        commentRow.id = 0;
        commentRow.address = movedInstRow->address;
        commentRow.parent = movedInstRow;
        movedInstRow->children.push_back(std::move(commentRow));
      }
    }
  }
}

int DisassemblyModel::rowCount(const QModelIndex &parent) const {
  if (!parent.isValid()) {
    return _labelRows.size();
  }
  const DisassemblyRow *parentRow =
      static_cast<DisassemblyRow *>(parent.internalPointer());
  if (parentRow == nullptr) return 0;
  return parentRow->children.size();
}

int DisassemblyModel::columnCount(const QModelIndex &parent) const {
  return (int)ColumnType::NumColumns;
}

bool DisassemblyModel::indexIsLabel(const QModelIndex &index) const {
  if (!index.isValid()) return false;
  const DisassemblyRow *row =
      static_cast<DisassemblyRow *>(index.internalPointer());
  if (row == nullptr) return false;
  return row->type == RowType::Label;
}

const SC3CodeBlock *DisassemblyModel::labelForIndex(
    const QModelIndex &index) const {
  if (!index.isValid()) return nullptr;
  const DisassemblyRow *row =
      static_cast<DisassemblyRow *>(index.internalPointer());
  while (row != nullptr) {
    if (row->type == RowType::Label)
      return _script->disassembly()[row->id].get();
    row = row->parent;
  }
  return nullptr;
}

QModelIndex DisassemblyModel::indexForLabel(SCXOffset labelId) const {
  if (labelId > _labelRows.size()) return QModelIndex();
  return createIndex(labelId, 0, (void *)&_labelRows.data()[labelId]);
}

QModelIndex DisassemblyModel::firstIndexForAddress(SCXOffset address) const {
  int labelId = firstLabelForAddress(_script, address);
  if (labelId < 0) return QModelIndex();
  if (_labelRows[labelId].address == address) return indexForLabel(labelId);
  int instId = instIdAtAddress(_script, labelId, address);
  if (instId < 0) return indexForLabel(labelId);
  return createIndex(instId, 0,
                     (void *)&_labelRows[labelId].children.data()[instId]);
}

SCXOffset DisassemblyModel::addressForIndex(const QModelIndex &index) const {
  if (!index.isValid()) return -1;
  const DisassemblyRow *row =
      static_cast<DisassemblyRow *>(index.internalPointer());
  if (row == nullptr) return -1;

  return row->address;
}

QModelIndex DisassemblyModel::index(int row, int column,
                                    const QModelIndex &parent) const {
  if (!parent.isValid())
    return createIndex(row, column, (void *)&_labelRows.data()[row]);
  const DisassemblyRow *parentRow =
      static_cast<DisassemblyRow *>(parent.internalPointer());
  if (parentRow == nullptr) return QModelIndex();
  if (row >= parentRow->children.size()) return QModelIndex();
  return createIndex(row, column, (void *)&parentRow->children.data()[row]);
}

QModelIndex DisassemblyModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) return QModelIndex();

  const DisassemblyRow *row =
      static_cast<DisassemblyRow *>(index.internalPointer());

  if (row == nullptr) return QModelIndex();
  if (row->parent == nullptr) return QModelIndex();
  return createIndex(row->parent->id, 0, row->parent);
}

QVariant DisassemblyModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (role != Qt::DisplayRole) return QVariant();
  switch ((ColumnType)section) {
    case ColumnType::Address:
      return "Address";
    case ColumnType::Code:
      return "Code";
    case ColumnType::Text:
      return "Text";
  }
  return QVariant();
}

QVariant DisassemblyModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();
  if (role != Qt::DisplayRole) return QVariant();

  const DisassemblyRow *row =
      static_cast<DisassemblyRow *>(index.internalPointer());
  if (row == nullptr) return QVariant();

  switch ((ColumnType)index.column()) {
    case ColumnType::Address:
      return QVariant(row->address);
    case ColumnType::Code: {
      const SC3CodeBlock *label = labelForIndex(index);
      switch (row->type) {
        case RowType::Label: {
          return QVariant(
              dApp->project()->getLabelName(_script->getId(), label->id()));
        }
        case RowType::Instruction: {
          const SC3Instruction *inst = label->instructions()[row->id].get();
          return QVariant(QString::fromStdString(
              DumpSC3InstructionToText(true, dApp->project()->contextProvider(),
                                       _script->getId(), inst)));
        }
        case RowType::Comment: {
          return QVariant("; " + dApp->project()->getComment(_script->getId(),
                                                             row->address));
        }
        default: { return QVariant(); }
      }
    }
    case ColumnType::Text: {
      if (row->type != RowType::Instruction) return QVariant();
      const SC3CodeBlock *label = labelForIndex(index);
      const SC3Instruction *inst = label->instructions()[row->id].get();
      return QVariant(firstStringInInstruction(inst));
    }
    default: { return QVariant(); }
  }
}

Qt::ItemFlags DisassemblyModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return 0;
  Qt::ItemFlags result = QAbstractItemModel::flags(index);
  const DisassemblyRow *row =
      static_cast<DisassemblyRow *>(index.internalPointer());
  if (row == nullptr) return 0;
  if (row->children.size() == 0) result |= Qt::ItemFlag::ItemNeverHasChildren;
  return result;
}

QString DisassemblyModel::firstStringInInstruction(
    const SC3Instruction *inst) const {
  for (const auto &arg : inst->args()) {
    if (arg.type == StringRef) {
      if (arg.uint16_value < _script->getStringCount())
        return "; " +
               dApp->project()->getString(_script->getId(), arg.uint16_value);
      return "";
    }
  }
  return "";
}

void DisassemblyModel::onCommentChanged(int fileId, SCXOffset address,
                                        const QString &comment) {
  if (fileId != _script->getId()) return;

  int labelId, instId;
  std::tie(labelId, instId) = instIdAtAddress(_script, address);
  if (labelId < 0 || instId < 0) return;

  DisassemblyRow *instructionRow = &_labelRows[labelId].children.data()[instId];

  // TODO multiline

  if (comment.isEmpty()) {
    for (auto it = instructionRow->children.begin();
         it != instructionRow->children.end(); it++) {
      if (it->type == RowType::Comment) {
        beginRemoveRows(createIndex(instId, 0, instructionRow),
                        it - instructionRow->children.begin(),
                        it - instructionRow->children.begin());
        instructionRow->children.erase(it);
        endRemoveRows();
        return;
      }
    }
  } else {
    for (int i = 0; i < instructionRow->children.size(); i++) {
      if (instructionRow->children[i].type == RowType::Comment) {
        auto index =
            createIndex(i, (int)ColumnType::Code, (void *)instructionRow);
        emit dataChanged(index, index);
        return;
      }
    }

    // no existing comment
    beginInsertRows(createIndex(instId, 0, instructionRow), 0, 0);
    DisassemblyRow commentRow;
    commentRow.id = instructionRow->children.size();
    commentRow.address = instructionRow->address;
    commentRow.type = RowType::Comment;
    commentRow.parent = instructionRow;
    instructionRow->children.insert(instructionRow->children.begin(),
                                    std::move(commentRow));
    endInsertRows();
  }
}

void DisassemblyModel::onLabelNameChanged(int fileId, int labelId,
                                          const QString &name) {
  if (fileId != _script->getId()) return;
  if (labelId < 0 || labelId >= _labelRows.size()) return;

  // slow but easy way to invalidate all code rows, for xrefs
  beginResetModel();
  endResetModel();

  /*
  QModelIndex origIndex = indexForLabel(labelId);
  QModelIndex codeIndex = createIndex(origIndex.row(), (int)ColumnType::Code,
                                      origIndex.internalPointer());
  emit dataChanged(codeIndex, codeIndex);
  */
}

void DisassemblyModel::onVarNameChanged(VariableRefType type, int var,
                                        const QString &name) {
  beginResetModel();
  endResetModel();
}
void DisassemblyModel::onAllVarsChanged() {
  beginResetModel();
  endResetModel();
}