#include "viewhelper.h"

QString displayTextForAddress(SCXOffset address) {
  return QString("%1").arg(address, 8, 16, QChar('0'));
}