#pragma once
#include <unordered_map>
#include <vector>

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

class SC3ExpressionToken {
 public:
  SC3ExpressionToken(SC3ExpressionTokenType type, int value)
      : _type(type), _value(value){};
  const SC3ExpressionTokenType type() const { return _type; }
  const int value() const { return _value; }

 private:
  const SC3ExpressionTokenType _type;
  const int _value;
};

class SC3ExpressionNode {
 public:
  ~SC3ExpressionNode();
  SC3ExpressionTokenType type;
  int value;
  SC3ExpressionNode *lhs;
  SC3ExpressionNode *rhs;

  SC3ExpressionNode *simplify() const;

  std::string toString() const;

 private:
  // binary
  static int evaluateOp(SC3ExpressionTokenType op, int lhs, int rhs);
  // unary
  static int evaluateOp(SC3ExpressionTokenType op, int rhs);
};

class SC3Expression {
 public:
  SC3Expression(uint8_t *rawExpression);
  ~SC3Expression();
  int rawLength() const { return _rawLength; }
  std::string toString(bool evalConst) const;

 private:
  struct Term {
    SC3ExpressionNode *node;
    uint8_t *end;
  };

  SC3ExpressionNode *_root;
  SC3ExpressionNode *_simplified;
  int _rawLength;
  Term parseTerm(uint8_t *start, uint8_t *end);
  std::vector<uint8_t *> _eaten;
};