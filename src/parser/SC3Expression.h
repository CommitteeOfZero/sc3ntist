#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

enum SC3ExpressionTokenType {
  EndOfExpression = 0x00,
  ImmediateValue = 0xFF,

  // binary operators
  Multiply = 0x01,
  Divide = 0x02,
  Add = 0x03,
  Subtract = 0x04,
  Modulo = 0x05,
  LeftShift = 0x06,
  RightShift = 0x07,
  BitwiseAnd = 0x08,
  BitwiseXor = 0x09,
  BitwiseOr = 0x0A,
  Equal = 0x0C,
  NotEqual = 0x0D,
  LessThanEqual = 0x0E,
  GreaterThanEqual = 0x0F,
  LessThan = 0x10,
  GreaterThan = 0x11,

  // binary assignment operators
  Assign = 0x14,
  MultiplyAssign = 0x15,
  DivideAssign = 0x16,
  AddAssign = 0x17,
  SubtractAssign = 0x18,
  ModuloAssign = 0x19,
  LeftShiftAssign = 0x1A,
  RightShiftAssign = 0x1B,
  BitwiseAndAssign = 0x1C,
  BitwiseOrAssign = 0x1D,
  BitwiseXorAssign = 0x1E,

  // postfix unary operators
  Increment = 0x20,
  Decrement = 0x21,

  // prefix unary operators
  Negation = 0x0B,

  // functions
  FuncGlobalVars = 0x28,
  FuncFlags = 0x29,
  FuncDataAccess = 0x2A,
  FuncLabelTable = 0x2B,
  FuncFarLabelTable = 0x2C,
  FuncThreadVars = 0x2D,
  FuncDMA = 0x2E,
  FuncUnk2F = 0x2F,
  FuncUnk30 = 0x30,
  FuncNop31 = 0x31,
  FuncNop32 = 0x32,
  FuncRandom = 0x33
};

enum SC3ExpressionTokenOperandsType {
  Binary,
  PostfixUnary,
  PrefixUnary,
  TwoArgFunc,
  None
};

struct OpInfo {
  const std::string str;
  int precedence;
  bool rightAssociative;
  bool constAllowed;
  SC3ExpressionTokenOperandsType operands;
};

struct tokenTypeHash {
  size_t operator()(const SC3ExpressionTokenType &type) const {
    return (size_t)type;
  }
};

extern const std::unordered_map<SC3ExpressionTokenType, OpInfo, tokenTypeHash>
    OperatorInfos;

struct SC3ExpressionToken {
  SC3ExpressionTokenType type;
  int precedence;
  int value;
  int length;
};

class SC3ExpressionNode {
 public:
  SC3ExpressionTokenType type;
  int value;
  std::unique_ptr<SC3ExpressionNode> lhs;
  std::unique_ptr<SC3ExpressionNode> rhs;

  SC3ExpressionNode *simplify() const;

  std::string toString() const;

  void traverse(std::function<void(const SC3ExpressionNode *)> visitor) const;

 private:
  // binary
  static int evaluateOp(SC3ExpressionTokenType op, int lhs, int rhs);
  // unary
  static int evaluateOp(SC3ExpressionTokenType op, int rhs);
};

class SC3Expression {
 public:
  SC3Expression();
  SC3Expression(uint8_t *rawExpression);
  int rawLength() const { return _rawLength; }
  std::string toString(bool evalConst) const;

  const SC3ExpressionNode *root() const { return _root.get(); }
  const SC3ExpressionNode *simplified() const { return _simplified.get(); }

 private:
  std::unique_ptr<SC3ExpressionNode> _root;
  std::unique_ptr<SC3ExpressionNode> _simplified;
  int _rawLength;

  uint8_t *_cursor;
  SC3ExpressionNode *parseSubExpression(int minPrec);
  SC3ExpressionNode *parseTerm();
  SC3ExpressionToken peekExpressionToken();
  void eatExpressionToken(const SC3ExpressionToken &token);
  SC3ExpressionToken nextExpressionToken();
};