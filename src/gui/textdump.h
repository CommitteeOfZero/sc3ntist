#pragma once
#include <string>

class SCXFile;
class SC3Instruction;

std::string DumpSCXFileToText(const SCXFile *file);
std::string DumpSC3InstructionToText(const SC3Instruction *inst);