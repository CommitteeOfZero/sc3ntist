#pragma once
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

class SC3Expression {
 public:
  SC3Expression(uint8_t* rawExpression);
  bool containsAssignment() const { return _containsAssignment; }
  bool isConstexpr() const { return _isConstexpr; }
  int rawLength() const { return _rawLength; }
  std::string toString(bool evalConst) const;

 private:
  struct SubExpr {
    bool isConst;
    std::string str;
    int constValue;
  };

  // in postfix
  std::vector<SC3ExpressionToken> _tokens;
  bool _containsAssignment;
  bool _isConstexpr;
  int _rawLength;
  uint8_t* _parseCur;
  void parseTerm(int minPrecedence);
  bool checkContainsAssignment() const;
  bool checkIsConstexpr() const;
  // binary
  SubExpr evalSubExpr(const std::pair<SubExpr, int>& lhs,
                      SC3ExpressionTokenType type,
                      const std::pair<SubExpr, int>& rhs,
                      bool shouldEval) const;
  // unary
  SubExpr evalSubExpr(SC3ExpressionTokenType type,
                      const std::pair<SubExpr, int>& rhs,
                      bool shouldEval) const;
};
