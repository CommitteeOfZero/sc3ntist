#include "disassemblyitemdelegate.h"
#include "disassemblymodel.h"
#include <QPainter>

void DisassemblyItemDelegate::paint(QPainter* painter,
                                    const QStyleOptionViewItem& option,
                                    const QModelIndex& index) const {
  if (index.column() == (int)DisassemblyModel::ColumnType::Breakpoint) {
    // TODO parameterize
    painter->fillRect(option.rect, QColor("#d3b892"));
  } else {
    QStyledItemDelegate::paint(painter, option, index);
    return;
  }
}