#pragma once
#include <string>
#include <vector>
#include "SCXTypes.h"
#include "SC3Expression.h"

enum SC3ArgumentType {
  ByteArray,
  Byte,
  UInt16,
  Expression,
  LocalLabel,
  FarLabel,
  ReturnAddress,
  StringRef,
  ExprFlagRef,
  ExprGlobalVarRef,
  ExprThreadVarRef
};

struct SC3Argument {
  SC3ArgumentType type;
  std::string name;
  std::vector<uint8_t> byteArrayValue;
  uint8_t byteValue;
  uint16_t uint16_value;
  SC3Expression exprValue;
};

SC3Argument SC3ArgumentFromByte(const std::string& name, uint8_t byteValue);