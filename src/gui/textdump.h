#pragma once
#include <string>

class SCXFile;
class SC3Instruction;
class IContextProvider;

std::string DumpSCXFileToText(bool richText, IContextProvider *ctx,
                              const SCXFile *file);
std::string DumpSC3InstructionToText(bool richText, IContextProvider *ctx,
                                     int fileId, const SC3Instruction *inst);