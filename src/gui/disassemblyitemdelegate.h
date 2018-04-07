#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QStyleOptionViewItem>

class DisassemblyItemDelegate : public QStyledItemDelegate {
  Q_OBJECT

 public:
  DisassemblyItemDelegate(QWidget* parent = 0) : QStyledItemDelegate(parent) {}

  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
};