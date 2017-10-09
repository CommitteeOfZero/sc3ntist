#pragma once
#include "SC3BaseDisassembler.h"
class CCDisassembler : public SC3BaseDisassembler {
 public:
  CCDisassembler(SCXFile& file) : SC3BaseDisassembler(file) {}
  ~CCDisassembler();

  SC3Instruction* DisassembleAt(SCXOffset address,
                                SCXOffset maxLength) override;
};
