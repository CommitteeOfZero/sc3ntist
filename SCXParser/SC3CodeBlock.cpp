#include "SC3CodeBlock.h"


SC3CodeBlock::SC3CodeBlock(SCXTableIndex labelId, SCXOffset address) : _labelId(labelId), _address(address)
{
}

void SC3CodeBlock::Append(SC3Instruction* instr)
{
	_instructions.push_back(std::unique_ptr<SC3Instruction>(instr));
}
