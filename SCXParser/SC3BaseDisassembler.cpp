#include "SC3BaseDisassembler.h"

void SC3BaseDisassembler::DisassembleFile() {
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

    _code.push_back(std::unique_ptr<SC3CodeBlock>(label));
  }
}