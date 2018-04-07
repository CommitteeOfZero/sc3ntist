#pragma once
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "parser/SCXTypes.h"
#include <vector>

class SCXFile;
class SC3CodeBlock;

class DisassemblyModel : public QAbstractItemModel {
  Q_OBJECT

 public:
  explicit DisassemblyModel(const SCXFile* script, QObject* parent = 0);
  ~DisassemblyModel() {}

  QVariant data(const QModelIndex& index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QModelIndex firstIndexForAddress(SCXOffset address) const;

 private:
  enum class ColumnType { Address, Code, NumColumns };
  enum class RowType { Label, Instruction, Comment, Blank };
  struct DisassemblyRow {
    RowType type;
    int id;
    SCXOffset address;
    std::vector<DisassemblyRow> children;
    DisassemblyRow* parent;
  };

  const SCXFile* _script;
  std::vector<DisassemblyRow> _labelRows;

  bool indexIsLabel(const QModelIndex& index) const;
  const SC3CodeBlock* labelForIndex(const QModelIndex& index) const;
  QModelIndex indexForLabel(SCXOffset labelId) const;
};