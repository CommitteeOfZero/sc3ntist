#pragma once
#include <vector>
#include <memory>
#include "SCXFile.h"
#include "SC3CodeBlock.h"

class SC3BaseDisassembler {
 public:
  SC3BaseDisassembler() = delete;

  void DisassembleFile();
  virtual SC3Instruction* DisassembleAt(SCXOffset address,
                                        SCXOffset maxLength) = 0;

 protected:
  SC3BaseDisassembler(SCXFile& file) : _file(file) {}
  SCXFile& _file;
};
