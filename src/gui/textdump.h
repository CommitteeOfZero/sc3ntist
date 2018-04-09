#pragma once
#include <string>

class SCXFile;
class SC3Instruction;
class IContextProvider;

std::string DumpSCXFileToText(IContextProvider *ctx, const SCXFile *file);
std::string DumpSC3InstructionToText(IContextProvider *ctx, int fileId,
                                     const SC3Instruction *inst);