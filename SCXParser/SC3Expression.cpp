#include "SC3Expression.h"
#include <map>
#include <sstream>
#include <stack>
#include <string>

static inline int readInt32Le(uint8_t *src) {
  return src[0] | (src[1] << 8) | (src[2] << 16) | (src[3] << 24);
}

SC3Expression::SC3Expression(uint8_t *rawExpression)
    : _parseCur(rawExpression) {
  parseTerm(-1);

  if (*_parseCur != SC3ExpressionTokenType::EndOfExpression) {
    throw std::runtime_error("End of expression expected");
  }

  _tokens.emplace_back(SC3ExpressionTokenType::EndOfExpression, 0);
  _parseCur++;
  _rawLength = _parseCur - rawExpression;

  _containsAssignment = checkContainsAssignment();
  _isConstexpr = checkIsConstexpr();
}

struct OpInfo {
  const std::string str;
  int precedence;
  bool rightAssociative;
};

const static std::map<SC3ExpressionTokenType, OpInfo> OperatorInfos = {
    {Multiply, {"*", 9, false}},
    {Divide, {"/", 9, false}},
    {Add, {"+", 8, false}},
    {Subtract, {"-", 8, false}},
    {Modulo, {"%", 9, false}},
    {LeftShift, {"<<", 7, false}},
    {RightShift, {">>", 7, false}},
    {BitwiseAnd, {"&", 4, false}},
    {BitwiseXor, {"^", 3, false}},
    {BitwiseOr, {"|", 2, false}},
    {Equal, {"==", 5, false}},
    {NotEqual, {"!=", 5, false}},
    {LessThanEqual, {"<=", 6, false}},
    {GreaterThanEqual, {">=", 6, false}},
    {LessThan, {"<", 6, false}},
    {GreaterThan, {">", 6, false}},
    {Assign, {"=", 1, true}},
    {MultiplyAssign, {"*=", 1, true}},
    {DivideAssign, {"/=", 1, true}},
    {AddAssign, {"+=", 1, true}},
    {SubtractAssign, {"-=", 1, true}},
    {ModuloAssign, {"%=", 1, true}},
    {LeftShiftAssign, {"<<=", 1, true}},
    {RightShiftAssign, {">>=", 1, true}},
    {BitwiseAndAssign, {"&=", 1, true}},
    {BitwiseOrAssign, {"|=", 1, true}},
    {BitwiseXorAssign, {"^=", 1, true}},
    {Increment, {"++", 11, false}},
    {Decrement, {"--", 11, false}},
    {Negation, {"~", 10, true}},
    {FuncGlobalVars, {"GlobalVars", 11, false}},
    {FuncFlags, {"Flags", 11, false}},
    {FuncDataAccess, {"DataAccess", 11, false}},
    {FuncLabelTable, {"LabelTable", 11, false}},
    {FuncFarLabelTable, {"FarLabelTable", 11, false}},
    {FuncThreadVars, {"ThreadVars", 11, false}},
    {FuncDMA, {"DMA", 11, false}},
    {FuncUnk2F, {"GetUnk2F", 11, false}},
    {FuncUnk30, {"GetUnk30", 11, false}},
    {FuncNop31, {"Nop31", 11, false}},
    {FuncNop32, {"Nop32", 11, false}},
    {FuncRandom, {"Random", 11, false}}};

std::string SC3Expression::toString() const {
  std::stack<std::pair<std::string, int>> stack;
  for (const auto &token : _tokens) {
    switch (token.type()) {
      case EndOfExpression:
        goto afterLoop;
        break;
      case ImmediateValue:
        stack.emplace(std::to_string(token.value()), 99);
        break;
      case Multiply:
      case Divide:
      case Add:
      case Subtract:
      case Modulo:
      case LeftShift:
      case RightShift:
      case BitwiseAnd:
      case BitwiseXor:
      case BitwiseOr:
      case Equal:
      case NotEqual:
      case LessThanEqual:
      case GreaterThanEqual:
      case LessThan:
      case GreaterThan:
      case Assign:
      case MultiplyAssign:
      case DivideAssign:
      case AddAssign:
      case SubtractAssign:
      case ModuloAssign:
      case LeftShiftAssign:
      case RightShiftAssign:
      case BitwiseAndAssign:
      case BitwiseOrAssign:
      case BitwiseXorAssign: {
        if (stack.size() < 2) {
          return "unrecognized_expression";
        }
        auto rhs = stack.top();
        stack.pop();
        auto lhs = stack.top();
        stack.pop();
        std::stringstream _s;
        auto op = OperatorInfos.at(token.type());
        if (lhs.second < op.precedence ||
            (lhs.second == op.precedence && op.rightAssociative)) {
          _s << "(" << lhs.first << ")";
        } else {
          _s << lhs.first;
        }
        _s << " " + op.str + " ";
        if (rhs.second < op.precedence ||
            (rhs.second == op.precedence && !op.rightAssociative)) {
          _s << "(" << rhs.first << ")";
        } else {
          _s << rhs.first;
        }
        stack.emplace(_s.str(), op.precedence);
        break;
      }
      case Increment:
      case Decrement: {
        if (stack.size() < 1) {
          return "unrecognized_expression";
        }
        auto lhs = stack.top();
        stack.pop();
        std::stringstream _s;
        auto op = OperatorInfos.at(token.type());
        if (op.precedence < lhs.second) {
          _s << "(" << lhs.first << ")" << op.str;
        } else {
          _s << lhs.first << op.str;
        }
        stack.emplace(_s.str(), op.precedence);
        break;
      }
      case FuncUnk2F:
      case FuncUnk30:
      case FuncNop31:
      case FuncNop32: {
        auto op = OperatorInfos.at(token.type());
        stack.emplace(op.str, op.precedence);
        break;
      }
      case Negation:
      case FuncGlobalVars:
      case FuncFlags:
      case FuncLabelTable:
      case FuncThreadVars:
      case FuncRandom: {
        if (stack.size() < 1) {
          return "unrecognized_expression";
        }
        auto rhs = stack.top();
        stack.pop();
        std::stringstream _s;
        auto op = OperatorInfos.at(token.type());
        if (token.type() != Negation && token.type() != FuncRandom) {
          _s << op.str << "[" << rhs.first << "]";
        } else {
          _s << op.str << "(" << rhs.first << ")";
        }
        stack.emplace(_s.str(), op.precedence);
        break;
      }
      case FuncDataAccess:
      case FuncFarLabelTable:
      case FuncDMA: {
        if (stack.size() < 2) {
          return "unrecognized_expression";
        }
        auto arg1 = stack.top();
        stack.pop();
        auto arg2 = stack.top();
        stack.pop();
        std::stringstream _s;
        auto op = OperatorInfos.at(token.type());
        _s << op.str << "(" << arg1.first << ", " << arg2.first << ")";
        stack.emplace(_s.str(), op.precedence);
        break;
      }
    }
  }
afterLoop:
  if (stack.size() == 0) {
    return "";
  }
  if (stack.size() != 1) {
    return "unrecognized_expression";
  }
  return stack.top().first;
}

void SC3Expression::parseTerm(int minPrecedence) {
  std::stack<std::pair<SC3ExpressionTokenType, uint8_t>> tmpStack;

  while (*_parseCur != SC3ExpressionTokenType::EndOfExpression) {
    if ((*_parseCur & 0x80) == 0x80)  // immediate value
    {
      int value;
      switch (_parseCur[0] & 0x60)  // length
      {
        case 0:  // 1 byte
          value = (_parseCur[0] & 0x1F);
          if (_parseCur[0] & 0x10)  // negative
            value |= 0xFFFFFFE0;
          _parseCur++;
          break;
        case 0x20:  // 2 bytes
          value = ((_parseCur[0] & 0x1F) << 8) | _parseCur[1];
          if (_parseCur[0] & 0x10)  // negative
            value |= 0xFFFFE000;
          _parseCur += 2;
          break;
        case 0x40:  // 3 bytes
          value = ((_parseCur[0] & 0x1F) << 16) | (_parseCur[2] << 8) |
                  _parseCur[1];
          if (_parseCur[0] & 0x10)  // negative
            value |= 0xFFE00000;
          _parseCur += 3;
          break;
        case 0x60:  // full little endian int32
          _parseCur++;
          value = readInt32Le(_parseCur);
          _parseCur += 4;
      }
      _parseCur++;  // skip precedence byte

      _tokens.emplace_back(SC3ExpressionTokenType::ImmediateValue, value);
    } else {
      SC3ExpressionTokenType type = (SC3ExpressionTokenType)(*_parseCur++);
      int precedence = *_parseCur++;
      if (minPrecedence >= 0 && precedence < minPrecedence) {
        _parseCur -= 2;
        goto afterLoop;
      }
      switch (type) {
        // binary infix
        case Multiply:
        case Divide:
        case Add:
        case Subtract:
        case Modulo:
        case LeftShift:
        case RightShift:
        case BitwiseAnd:
        case BitwiseXor:
        case BitwiseOr:
        case Equal:
        case NotEqual:
        case LessThanEqual:
        case GreaterThanEqual:
        case LessThan:
        case GreaterThan:
        case Assign:
        case MultiplyAssign:
        case DivideAssign:
        case AddAssign:
        case SubtractAssign:
        case ModuloAssign:
        case LeftShiftAssign:
        case RightShiftAssign:
        case BitwiseAndAssign:
        case BitwiseOrAssign:
        case BitwiseXorAssign:
          while (!tmpStack.empty() && precedence <= tmpStack.top().second) {
            _tokens.emplace_back(tmpStack.top().first, 0);
            tmpStack.pop();
          }
          tmpStack.push(std::make_pair(type, precedence));
          break;
        // unary postfix, 0 arg functions
        case Increment:
        case Decrement:
        case FuncUnk2F:
        case FuncUnk30:
        case FuncNop31:
        case FuncNop32:
          _tokens.emplace_back(type, 0);
          break;
        // unary prefix, 1 arg functions
        case Negation:
        case FuncGlobalVars:
        case FuncFlags:
        case FuncLabelTable:
        case FuncThreadVars:
        case FuncRandom:
          parseTerm(precedence);
          _tokens.emplace_back(type, 0);
          break;
        // 2 arg functions
        case FuncDataAccess:
        case FuncFarLabelTable:
        case FuncDMA:
          parseTerm(precedence);
          parseTerm(precedence);
          _tokens.emplace_back(type, 0);
          break;
        default:
          throw std::runtime_error("Unexpected token type");
          break;
      }
    }
  }
afterLoop:
  while (!tmpStack.empty()) {
    _tokens.emplace_back(tmpStack.top().first, 0);
    tmpStack.pop();
  }
}

bool SC3Expression::checkContainsAssignment() const {
  static std::vector<SC3ExpressionTokenType> assignmentTypes = {
      Assign,           MultiplyAssign,  DivideAssign,    AddAssign,
      SubtractAssign,   ModuloAssign,    LeftShiftAssign, RightShiftAssign,
      BitwiseAndAssign, BitwiseOrAssign, BitwiseXorAssign};

  for (const auto &it : _tokens) {
    if (std::find(assignmentTypes.begin(), assignmentTypes.end(), it.type()) !=
        assignmentTypes.end())
      return true;
  }
  return false;
}

bool SC3Expression::checkIsConstexpr() const {
  static std::vector<SC3ExpressionTokenType> variableTypes = {
      FuncGlobalVars,    FuncFlags,      FuncDataAccess,
      FuncFarLabelTable, FuncThreadVars, FuncDMA,
      FuncUnk2F,         FuncUnk30,      FuncRandom};

  if (_containsAssignment) return false;
  for (const auto &it : _tokens) {
    if (std::find(variableTypes.begin(), variableTypes.end(), it.type()) !=
        variableTypes.end())
      return false;
  }
  return true;
}
