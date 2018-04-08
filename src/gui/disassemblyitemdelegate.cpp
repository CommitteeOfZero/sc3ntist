#include "disassemblyitemdelegate.h"
#include "disassemblymodel.h"
#include <QPainter>
#include <QTextDocument>
#include <QApplication>
#include <QAbstractTextDocumentLayout>
#include "viewhelper.h"

void DisassemblyItemDelegate::paint(QPainter* painter,
                                    const QStyleOptionViewItem& option,
                                    const QModelIndex& index) const {
  QString richText = richTextFor(index);
  if (!richText.isEmpty()) {
    return paintRichText(painter, option, index, richText);
  }

  switch ((DisassemblyModel::ColumnType)index.column()) {
    case DisassemblyModel::ColumnType::Breakpoint: {
      // TODO parameterize
      painter->fillRect(option.rect, QColor("#d3b892"));
      break;
    }
    default: {
      QStyledItemDelegate::paint(painter, option, index);
      break;
    }
  }
}

QString DisassemblyItemDelegate::richTextFor(const QModelIndex& index) const {
  switch ((DisassemblyModel::ColumnType)index.column()) {
    case DisassemblyModel::ColumnType::Address: {
      return "<b>" + displayTextForAddress(index.data().toInt()) + "</b>  ";
    }
    case DisassemblyModel::ColumnType::Code: {
      const DisassemblyRow* row =
          static_cast<DisassemblyRow*>(index.internalPointer());
      switch (row->type) {
        case DisassemblyModel::RowType::Comment: {
          return "<span style='color: #606060'>&nbsp;&nbsp;&nbsp;&nbsp;" +
                 index.data().toString().toHtmlEscaped() + "</span>";
        }
        case DisassemblyModel::RowType::Label: {
          return "<span style='font-weight: bold; color: #008000'>#" +
                 index.data().toString().toHtmlEscaped() + ":</span>";
        }
      }
    }
    default: { return QString(); }
  }
}

// https://stackoverflow.com/a/1956781
void DisassemblyItemDelegate::paintRichText(QPainter* painter,
                                            const QStyleOptionViewItem& option,
                                            const QModelIndex& index,
                                            const QString& richText) const {
  QStyleOptionViewItemV4 options = option;
  initStyleOption(&options, index);

  painter->save();

  QTextDocument doc;
  // TODO global unified styling
  doc.setDefaultStyleSheet(
      "body { font-family: 'Lucida Console'; font-size: 9pt; }");
  doc.setHtml("<body>" + richText + "</body>");

  const QWidget* widget = options.widget;
  QStyle* style = widget ? widget->style() : QApplication::style();
  options.text = "";
  style->drawControl(QStyle::CE_ItemViewItem, &options, painter, widget);

  painter->translate(options.rect.left(), options.rect.top());
  QRect clip(0, 0, options.rect.width(), options.rect.height());
  doc.drawContents(painter, clip);

  painter->restore();
}

QSize DisassemblyItemDelegate::sizeHint(const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const {
  QString richText = richTextFor(index);
  if (richText.isEmpty()) return QStyledItemDelegate::sizeHint(option, index);

  QTextDocument doc;
  doc.setDefaultStyleSheet(
      "body { font-family: 'Lucida Console'; font-size: 9pt; }");
  doc.setHtml("<body>" + richText + "</body>");
  return QSize(doc.idealWidth(), doc.size().height());
}