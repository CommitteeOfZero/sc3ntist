#pragma once
#include <string>

class IContextProvider {
 public:
  virtual ~IContextProvider() {}
  virtual std::string labelName(int fileId, int labelId) = 0;
  virtual std::string globalVarName(int varId) = 0;
  virtual std::string flagName(int flagId) = 0;
};