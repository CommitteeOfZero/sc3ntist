#pragma once
#include <QAbstractTableModel>
#include <QString>
#include "enums.h"
#include <utility>

class MemoryModel : public QAbstractTableModel {
  Q_OBJECT

 public:
  enum class ColumnType { VarType, Name, Comment, NumColumns };

  explicit MemoryModel(QObject *parent = 0);

  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  QModelIndex indexForVar(VariableRefType type, int var) const;
  std::pair<VariableRefType, int> varForIndex(const QModelIndex &index) const;

 private slots:
  void onVarTextChanged(VariableRefType type, int var, const QString &text);
  void onAllVarsChanged();
};