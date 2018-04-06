#pragma once
#include "SC3BaseDisassembler.h"
class ZeroDisassembler : public SC3BaseDisassembler {
 public:
  ZeroDisassembler(SCXFile& file) : SC3BaseDisassembler(file) {}

  SC3Instruction* DisassembleAt(SCXOffset address,
                                SCXOffset maxLength) override;
};
