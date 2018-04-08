#include "disassemblymodel.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"
#include <sstream>
#include "textdump.h"
#include <algorithm>
#include "debuggerapplication.h"
#include "project.h"

DisassemblyModel::DisassemblyModel(const SCXFile *script, QObject *parent)
    : QAbstractItemModel(parent), _script(script) {
  reload();

  connect(dApp->project(), &Project::commentChanged, this,
          &DisassemblyModel::onCommentChanged);
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

// TODO use DisassemblyRows for this?
QModelIndex DisassemblyModel::firstIndexForAddress(SCXOffset address) const {
  const auto &disasm = _script->disassembly();
  int labelId = firstLabelForAddress(address);
  if (labelId < 0) return QModelIndex();
  if (_labelRows[labelId].address == address) return indexForLabel(labelId);
  int instId = firstInstructionForAddress(labelId, address);
  if (instId < 0) return indexForLabel(labelId);
  return createIndex(instId, 0,
                     (void *)&_labelRows[labelId].children.data()[instId]);
}

int DisassemblyModel::firstLabelForAddress(SCXOffset address) const {
  const auto &disasm = _script->disassembly();
  const auto labelCount = disasm.size();
  if (labelCount == 0) return -1;
  int labelId = 0;
  for (int i = 0; i < labelCount; i++) {
    if (disasm[i]->address() == address) {
      return i;
    }
    if (disasm[i]->address() < address) {
      labelId = i;
    }
    if (disasm[i]->address() > address) {
      break;
    }
  }
  return labelId;
}

int DisassemblyModel::firstInstructionForAddress(SCXOffset address) const {
  return firstInstructionForAddress(firstLabelForAddress(address), address);
}

int DisassemblyModel::firstInstructionForAddress(int labelId,
                                                 SCXOffset address) const {
  if (labelId < 0 || labelId > _script->disassembly().size()) return -1;
  const auto &insts = _script->disassembly()[labelId]->instructions();
  const auto instCount = insts.size();
  if (instCount == 0) return -1;
  int instId = 0;
  for (int i = 0; i < instCount; i++) {
    if (insts[i]->position() == address) {
      return i;
    }
    if (insts[i]->position() < address) {
      instId = i;
    }
    if (insts[i]->position() > address) {
      break;
    }
  }
  return instId;
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
      return QVariant(QString("%1").arg(row->address, 8, 16, QChar('0')));
    case ColumnType::Code: {
      const SC3CodeBlock *label = labelForIndex(index);
      switch (row->type) {
        case RowType::Label: {
          std::stringstream _s;
          _s << "#label" << label->id() << "_" << label->address() << ":";
          return QVariant(QString::fromStdString(_s.str()));
        }
        case RowType::Instruction: {
          const SC3Instruction *inst = label->instructions()[row->id].get();
          return QVariant(
              QString::fromStdString("    " + DumpSC3InstructionToText(inst)));
        }
        case RowType::Comment: {
          return QVariant("    ; " + dApp->project()->getComment(
                                         _script->getId(), row->address));
        }
        default: { return QVariant(); }
      }
    }
  }
  return QVariant();
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

void DisassemblyModel::onCommentChanged(int fileId, SCXOffset address,
                                        const QString &comment) {
  if (fileId != _script->getId()) return;

  int labelId = firstLabelForAddress(address);
  if (labelId < 0) return;
  int instId = firstInstructionForAddress(labelId, address);
  if (instId < 0) return;

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