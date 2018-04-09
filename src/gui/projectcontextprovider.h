#pragma once
#include <parser/IContextProvider.h>

class Project;

class ProjectContextProvider : public IContextProvider {
 public:
  ProjectContextProvider(Project *project);
  ~ProjectContextProvider() {}
  std::string labelName(int fileId, int labelId) override;
  std::string globalVarName(int varId) override;
  std::string flagName(int flagId) override;

 private:
  Project *_project;
};