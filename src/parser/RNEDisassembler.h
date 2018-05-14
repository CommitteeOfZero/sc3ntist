#pragma once
#include "SC3BaseDisassembler.h"
class RNEDisassembler : public SC3BaseDisassembler {
 public:
  RNEDisassembler(SCXFile& file) : SC3BaseDisassembler(file) {}

  SC3Instruction* DisassembleAt(SCXOffset address,
                                SCXOffset maxLength) override;
};
