#include "disassemblymodel.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"
#include <sstream>
#include "textdump.h"
#include <algorithm>

// TODO: Per-instruction model (so instructions can have children for comments)

enum class ColumnType { Address, Code, NumColumns };

DisassemblyModel::DisassemblyModel(const SCXFile *script, QObject *parent)
    : QAbstractItemModel(parent), _script(script) {}

int DisassemblyModel::rowCount(const QModelIndex &parent) const {
  int labelCount = (int)_script->disassembly().size();
  if (!parent.isValid()) {
    return labelCount;
  }
  if (parent.row() < 0 || parent.row() >= labelCount ||
      parent.parent().isValid())
    return 0;

  int ct = _script->disassembly()[parent.row()]->instructions().size() + 1;
  return ct;
}

int DisassemblyModel::columnCount(const QModelIndex &parent) const {
  return (int)ColumnType::NumColumns;
}

bool DisassemblyModel::indexIsLabel(const QModelIndex &index) const {
  return !index.parent().isValid();
}

const SC3CodeBlock *DisassemblyModel::labelForIndex(
    const QModelIndex &index) const {
  if (indexIsLabel(index)) {
    return _script->disassembly()[index.row()].get();
  }
  return _script->disassembly()[index.parent().row()].get();
}

QModelIndex DisassemblyModel::indexForLabel(SCXOffset labelId) const {
  if (labelId > _script->disassembly().size()) return QModelIndex();
  return createIndex(labelId, 0, -1);
}

SCXOffset DisassemblyModel::addressForIndex(const QModelIndex &index) const {
  const SC3CodeBlock *label = labelForIndex(index);
  if (indexIsLabel(index)) {
    return label->address();
  } else {
    int instCount = (int)(label->instructions().size());
    if (instCount == 0) return label->address();
    return label->instructions()[std::min(index.row(), instCount - 1)]
        ->position();
  }
}

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
  return createIndex(instId, 0, labelId);
}

QModelIndex DisassemblyModel::index(int row, int column,
                                    const QModelIndex &parent) const {
  // We use aid=-1 to refer to labels, aid!=-1 to refer to data inside them
  if (!parent.isValid()) return createIndex(row, column, -1);
  return createIndex(row, column, parent.row());
}

QModelIndex DisassemblyModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) return QModelIndex();

  if (index.internalId() == -1) return QModelIndex();

  return createIndex(index.internalId(), 0, -1);
}

QVariant DisassemblyModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (role != Qt::DisplayRole) return QVariant();
  return QVariant();
}

QVariant DisassemblyModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();

  if (role != Qt::DisplayRole) return QVariant();

  switch ((ColumnType)index.column()) {
    case ColumnType::Address:
      return QVariant(
          QString("%1").arg(addressForIndex(index), 8, 16, QChar('0')));
    case ColumnType::Code: {
      const SC3CodeBlock *label = labelForIndex(index);

      if (indexIsLabel(index)) {
        std::stringstream _s;
        _s << "#label" << label->id() << "_" << label->address() << ":";
        return QVariant(QString::fromStdString(_s.str()));
      } else {
        if (index.row() >= (int)label->instructions().size()) {
          return QVariant("");
        } else {
          const SC3Instruction *inst = label->instructions()[index.row()].get();
          return QVariant(
              QString::fromStdString("    " + DumpSC3InstructionToText(inst)));
        }
      }
    }
  }
}

Qt::ItemFlags DisassemblyModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return 0;
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
  if (index.parent().isValid())
    return defaultFlags | Qt::ItemFlag::ItemNeverHasChildren;
  return defaultFlags;
}