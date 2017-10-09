#pragma once
#include "SCXFile.h"
#include <vector>
#include "SC3Instruction.h"

class SC3CodeBlock
{
public:
	SC3CodeBlock(SCXTableIndex labelId, SCXOffset address);
	~SC3CodeBlock();
	void Append(SC3Instruction* instr);
	const std::vector<SC3Instruction*>& instructions() const { return _instructions; }
	const SCXTableIndex id() const { return _labelId; }
	const SCXOffset address() const { return _address; }
private:
	SCXTableIndex _labelId;
	SCXOffset _address;
	std::vector<SC3Instruction*> _instructions;
};

