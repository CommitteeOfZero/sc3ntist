#include "disassemblymodel.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"
#include <sstream>
#include "textdump.h"
#include <algorithm>

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