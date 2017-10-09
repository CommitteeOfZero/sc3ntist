#pragma once
#include <map>
#include <vector>
#include "SC3CodeBlock.h"
#include "SCXFile.h"

class SC3BaseDisassembler {
 public:
  SC3BaseDisassembler() = delete;
  virtual ~SC3BaseDisassembler();

  const std::vector<SC3CodeBlock*>& code() const { return _code; }

  void DisassembleFile();
  virtual SC3Instruction* DisassembleAt(SCXOffset address,
                                        SCXOffset maxLength) = 0;

 protected:
  SC3BaseDisassembler(SCXFile& file) : _file(file) {}

  SCXFile& _file;
  std::vector<SC3CodeBlock*> _code;
  void clearCode();
};
