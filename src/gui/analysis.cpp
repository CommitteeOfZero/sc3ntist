#include <algorithm>
#include "analysis.h"

int firstLabelForAddress(const SCXFile *file, SCXOffset address) {
  const auto &disasm = file->disassembly();
  const auto labelCount = disasm.size();
  if (labelCount == 0) return -1;
  int labelId = 0;
  for (int i = 0; i < labelCount; i++) {
    if (disasm[i]->address() == address) {
      return i;
    }
    if (disasm[i]->address() < address) {
      labelId = i;
    }
    if (disasm[i]->address() > address) {
      break;
    }
  }
  return labelId;
}

std::pair<int, int> instructionAtAddress(const SCXFile *file,
                                         SCXOffset address) {
  int labelId = firstLabelForAddress(file, address);
  int instId;
  while (labelId < file->disassembly().size()) {
    instId = instructionAtAddress(file, labelId, address);
    if (instId >= 0) return std::make_pair(labelId, instId);
    labelId++;
  }
  return std::make_pair(-1, -1);
}

int instructionAtAddress(const SCXFile *file, int labelId, SCXOffset address) {
  if (labelId < 0 || labelId > file->disassembly().size()) return -1;
  const auto &insts = file->disassembly()[labelId]->instructions();
  const auto instCount = insts.size();
  if (instCount == 0) return -1;
  int instId = 0;
  for (int i = 0; i < instCount; i++) {
    if (insts[i]->position() == address) {
      return i;
    }
    if (insts[i]->position() < address) {
      instId = i;
    }
    if (insts[i]->position() > address) {
      break;
    }
  }
  return instId;
}

std::vector<std::pair<VariableRefType, int>> variableRefsInInstruction(
    const SC3Instruction *inst) {
  std::vector<std::pair<VariableRefType, int>> refs;

  for (const auto &arg : inst->args()) {
    if (arg.type == SC3ArgumentType::Expression ||
        arg.type == SC3ArgumentType::ExprFlagRef ||
        arg.type == SC3ArgumentType::ExprGlobalVarRef ||
        arg.type == SC3ArgumentType::ExprThreadVarRef ||
        arg.type == SC3ArgumentType::FarLabel) {
      auto argRefs = variableRefsInExpression(arg.exprValue);
      std::copy(argRefs.begin(), argRefs.end(), std::back_inserter(refs));
    }
    if ((arg.type == SC3ArgumentType::ExprGlobalVarRef ||
         arg.type == SC3ArgumentType::ExprFlagRef) &&
        expressionIsConstant(arg.exprValue)) {
      VariableRefType type = arg.type == SC3ArgumentType::ExprGlobalVarRef
                                 ? VariableRefType::GlobalVar
                                 : VariableRefType::Flag;
      refs.emplace_back(type, constantValueForExpression(arg.exprValue));
    }
  }

  return refs;
}

std::vector<std::pair<VariableRefType, int>> variableRefsAtAddress(
    const SCXFile *file, SCXOffset address) {
  int labelId, instId;
  std::tie(labelId, instId) = instructionAtAddress(file, address);
  std::vector<std::pair<VariableRefType, int>> refs;
  if (labelId < 0 || labelId >= file->disassembly().size()) return refs;
  const auto &label = file->disassembly()[labelId];
  if (instId < 0 || instId >= label->instructions().size()) return refs;
  const auto &inst = label->instructions()[instId];

  refs = variableRefsInInstruction(inst.get());

  return refs;
}

std::vector<std::pair<VariableRefType, int>> variableRefsInExpression(
    const SC3Expression &expr) {
  std::vector<std::pair<VariableRefType, int>> result;
  const SC3ExpressionNode *root = expr.simplified();
  if (root == nullptr) return result;
  root->traverse([&](const SC3ExpressionNode *node) {
    if (node->rhs != nullptr &&
        node->rhs->type == SC3ExpressionTokenType::ImmediateValue) {
      if (node->type == SC3ExpressionTokenType::FuncGlobalVars) {
        result.emplace_back(VariableRefType ::GlobalVar, node->rhs->value);
      } else if (node->type == SC3ExpressionTokenType::FuncFlags) {
        result.emplace_back(VariableRefType ::Flag, node->rhs->value);
      }
    }
  });
  return result;
}

bool expressionIsConstant(const SC3Expression &expr) {
  return expr.simplified() == nullptr ||
         expr.simplified()->type == SC3ExpressionTokenType::ImmediateValue;
}

int constantValueForExpression(const SC3Expression &expr) {
  if (expr.simplified() == nullptr) return 0;
  return expr.simplified()->value;
}