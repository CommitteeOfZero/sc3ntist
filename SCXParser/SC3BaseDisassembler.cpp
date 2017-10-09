#include "SC3BaseDisassembler.h"

SC3BaseDisassembler::~SC3BaseDisassembler() { clearCode(); }

void SC3BaseDisassembler::DisassembleFile() {
  clearCode();

  if (_file.getLabelCount() < 1) return;
  SCXOffset pos = _file.getLabelOffset(0);

  for (SCXTableIndex i = 0; i < _file.getLabelCount(); i++) {
    SC3CodeBlock* label = new SC3CodeBlock(i, _file.getLabelOffset(i));
    SCXOffset end;
    if (i == _file.getLabelCount() - 1)
      end = _file.getCodeEndOffset();
    else
      end = _file.getLabelOffset(i + 1);

    while (pos < end) {
      SC3Instruction* inst = DisassembleAt(pos, end - pos);
      label->Append(inst);
      pos += inst->length();
    }

    _code.push_back(label);
  }
}

void SC3BaseDisassembler::clearCode() {
  for (auto& it = _code.begin(); it != _code.end(); ++it) {
    delete *it;
  }
  _code.clear();
}
