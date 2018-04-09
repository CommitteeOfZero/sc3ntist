#include "projectcontextprovider.h"
#include "project.h"

ProjectContextProvider::ProjectContextProvider(Project *project) {
  _project = project;
}

std::string ProjectContextProvider::labelName(int fileId, int labelId) {
  return _project->getLabelName(fileId, labelId).toStdString();
}

std::string ProjectContextProvider::globalVarName(int varId) {
  return _project->getVarName(VariableRefType::GlobalVar, varId).toStdString();
}

std::string ProjectContextProvider::flagName(int flagId) {
  return _project->getVarName(VariableRefType::Flag, flagId).toStdString();
}