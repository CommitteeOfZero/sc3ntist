#pragma once
#include <string>
#include <QString>

class SCXFile;
class SC3Instruction;
class IContextProvider;

QString DumpSCXFileToText(bool richText, IContextProvider *ctx,
                              const SCXFile *file);
std::string DumpSC3InstructionToText(bool richText, IContextProvider *ctx,
                                     int fileId, const SC3Instruction *inst);

QString FirstStringInInstruction(const SC3Instruction* inst, const SCXFile* file);
