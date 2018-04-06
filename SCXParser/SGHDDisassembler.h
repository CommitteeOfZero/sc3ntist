#pragma once
#include "SC3BaseDisassembler.h"
class SGHDDisassembler : public SC3BaseDisassembler {
public:
	SGHDDisassembler(SCXFile& file) : SC3BaseDisassembler(file) {}

	SC3Instruction* DisassembleAt(SCXOffset address,
		SCXOffset maxLength) override;
};
