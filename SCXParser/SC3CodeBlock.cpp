#include "SC3CodeBlock.h"


SC3CodeBlock::SC3CodeBlock(SCXTableIndex labelId, SCXOffset address) : _labelId(labelId), _address(address)
{
}

SC3CodeBlock::~SC3CodeBlock()
{
	for (auto& it = _instructions.begin(); it != _instructions.end(); ++it) {
		delete *it;
	}
}

void SC3CodeBlock::Append(SC3Instruction* instr)
{
	_instructions.push_back(instr);
}
