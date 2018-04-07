#include "disassemblymodel.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"
#include <sstream>
#include "textdump.h"
#include <algorithm>

DisassemblyModel::DisassemblyModel(const SCXFile *script, QObject *parent)
    : QAbstractItemModel(parent), _script(script) {
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
  if (row == nullptr) return nullptr;
  if (indexIsLabel(index)) {
    return _script->disassembly()[row->id].get();
  }
  return _script->disassembly()[row->parent->id].get();
}

QModelIndex DisassemblyModel::indexForLabel(SCXOffset labelId) const {
  if (labelId > _labelRows.size()) return QModelIndex();
  return createIndex(labelId, 0, (void *)&_labelRows.data()[labelId]);
}

// TODO use DisassemblyRows for this?
QModelIndex DisassemblyModel::firstIndexForAddress(SCXOffset address) const {
  const auto &disasm = _script->disassembly();
  const auto labelCount = disasm.size();
  if (labelCount == 0) return QModelIndex();
  int labelId = 0;
  for (int i = 0; i < labelCount; i++) {
    if (disasm[i]->address() == address) {
      return indexForLabel(i);
    }
    if (disasm[i]->address() < address) {
      labelId = i;
    }
    if (disasm[i]->address() > address) {
      break;
    }
  }
  const auto &insts = disasm[labelId]->instructions();
  const auto instCount = insts.size();
  if (instCount == 0) return indexForLabel(labelId);
  int instId = 0;
  for (int i = 0; i < instCount; i++) {
    if (insts[i]->position() == address) {
      instId = i;
      break;
    }
    if (insts[i]->position() < address) {
      instId = i;
    }
    if (insts[i]->position() > address) {
      break;
    }
  }
  return createIndex(instId, 0,
                     (void *)&_labelRows[labelId].children.data()[instId]);
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