#pragma once
#include "SCXFile.h"
#include <vector>
#include <memory>
#include "SC3Instruction.h"

class SC3CodeBlock
{
public:
	SC3CodeBlock(SCXTableIndex labelId, SCXOffset address);
	void Append(SC3Instruction* instr);
	const std::vector<std::unique_ptr<SC3Instruction>>& instructions() const { return _instructions; }
	const SCXTableIndex id() const { return _labelId; }
	const SCXOffset address() const { return _address; }
private:
	SCXTableIndex _labelId;
	SCXOffset _address;
	std::vector<std::unique_ptr<SC3Instruction>> _instructions;
};

