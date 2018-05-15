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
      painter->fillRect(option.rect, QColor("#101923"));
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
      return "<div class='address'>" +
             displayTextForAddress(index.data().toInt()) + "</div>";
    }
    case DisassemblyModel::ColumnType::Code: {
      const DisassemblyRow* row =
          static_cast<DisassemblyRow*>(index.internalPointer());
      switch (row->type) {
        case DisassemblyModel::RowType::Comment: {
          return "<div class='comment'>" +
                 index.data().toString().toHtmlEscaped() + "</div>";
        }
        case DisassemblyModel::RowType::Label: {
          return "<div class='label'>#" +
                 index.data().toString().toHtmlEscaped() + ":</div>";
        }
        case DisassemblyModel::RowType::Instruction: {
          return "<div class='instruction'>" + index.data().toString() +
                 "</div>";
        }
        default: { break; }
      }
    }
    case DisassemblyModel::ColumnType::Text: {
      return "<div class='string'>" + index.data().toString().toHtmlEscaped() +
             "</div>";
    }
    default: { break; }
  }
  return QString();
}

// https://stackoverflow.com/a/1956781
void DisassemblyItemDelegate::paintRichText(QPainter* painter,
                                            const QStyleOptionViewItem& option,
                                            const QModelIndex& index,
                                            const QString& richText) const {
  QStyleOptionViewItem options = option;
  initStyleOption(&options, index);

  painter->save();

  QTextDocument doc;
  doc.setDefaultStyleSheet(qApp->styleSheet());
  doc.setHtml("<div class='disassemblyRow'>" + richText + "</div>");

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
  doc.setDefaultStyleSheet(qApp->styleSheet());
  doc.setHtml("<div class='disassemblyRow'>" + richText + "</div>");
  return QSize(doc.idealWidth(), doc.size().height());
}