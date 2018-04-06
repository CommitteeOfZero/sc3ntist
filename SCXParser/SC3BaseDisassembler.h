#pragma once
#include <vector>
#include <memory>
#include "SC3CodeBlock.h"
#include "SCXFile.h"

class SC3BaseDisassembler {
 public:
  SC3BaseDisassembler() = delete;

  const std::vector<std::unique_ptr<SC3CodeBlock>>& code() const { return _code; }

  void DisassembleFile();
  virtual SC3Instruction* DisassembleAt(SCXOffset address,
                                        SCXOffset maxLength) = 0;

 protected:
  SC3BaseDisassembler(SCXFile& file) : _file(file) {}

  SCXFile& _file;
  std::vector<std::unique_ptr<SC3CodeBlock>> _code;
};
