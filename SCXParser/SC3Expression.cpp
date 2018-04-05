#include "SC3Expression.h"
#include <unordered_map>
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
  bool constAllowed;
};

const static std::unordered_map<SC3ExpressionTokenType, OpInfo> OperatorInfos = {
    {Multiply, {"*", 9, false, true}},
    {Divide, {"/", 9, false, true}},
    {Add, {"+", 8, false, true}},
    {Subtract, {"-", 8, false, true}},
    {Modulo, {"%", 9, false, true}},
    {LeftShift, {"<<", 7, false, true}},
    {RightShift, {">>", 7, false, true}},
    {BitwiseAnd, {"&", 4, false, true}},
    {BitwiseXor, {"^", 3, false, true}},
    {BitwiseOr, {"|", 2, false, true}},
    {Equal, {"==", 5, false, true}},
    {NotEqual, {"!=", 5, false, true}},
    {LessThanEqual, {"<=", 6, false, true}},
    {GreaterThanEqual, {">=", 6, false, true}},
    {LessThan, {"<", 6, false, true}},
    {GreaterThan, {">", 6, false, true}},
    {Assign, {"=", 1, true, false}},
    {MultiplyAssign, {"*=", 1, true, false}},
    {DivideAssign, {"/=", 1, true, false}},
    {AddAssign, {"+=", 1, true, false}},
    {SubtractAssign, {"-=", 1, true, false}},
    {ModuloAssign, {"%=", 1, true, false}},
    {LeftShiftAssign, {"<<=", 1, true, false}},
    {RightShiftAssign, {">>=", 1, true, false}},
    {BitwiseAndAssign, {"&=", 1, true, false}},
    {BitwiseOrAssign, {"|=", 1, true, false}},
    {BitwiseXorAssign, {"^=", 1, true, false}},
    {Increment, {"++", 11, false, false}},
    {Decrement, {"--", 11, false, false}},
    {Negation, {"~", 10, true, true}},
    {FuncGlobalVars, {"GlobalVars", 11, false, false}},
    {FuncFlags, {"Flags", 11, false, false}},
    {FuncDataAccess, {"DataAccess", 11, false, false}},
    {FuncLabelTable, {"LabelTable", 11, false, false}},
    {FuncFarLabelTable, {"FarLabelTable", 11, false, false}},
    {FuncThreadVars, {"ThreadVars", 11, false, false}},
    {FuncDMA, {"DMA", 11, false, false}},
    {FuncUnk2F, {"GetUnk2F", 11, false, false}},
    {FuncUnk30, {"GetUnk30", 11, false, false}},
    {FuncNop31, {"Nop31", 11, false, false}},
    {FuncNop32, {"Nop32", 11, false, false}},
    {FuncRandom, {"Random", 11, false, false}}};

std::string SC3Expression::toString(bool evalConst) const {
  std::stack<std::pair<SubExpr, int>> stack;
  for (const auto &token : _tokens) {
    switch (token.type()) {
      case EndOfExpression:
        goto afterLoop;
        break;
      case ImmediateValue: {
        SubExpr se;
        se.isConst = true;
        se.str = std::to_string(token.value());
        se.constValue = token.value();
        stack.emplace(se, 99);
        break;
      }
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
        auto op = OperatorInfos.find(token.type())->second;
        SubExpr se = evalSubExpr(lhs, token.type(), rhs, evalConst);
        stack.emplace(se, op.precedence);
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
        auto op = OperatorInfos.find(token.type())->second;
        if (op.precedence < lhs.second) {
          _s << "(" << lhs.first.str << ")" << op.str;
        } else {
          _s << lhs.first.str << op.str;
        }
        SubExpr se;
        se.isConst = false;
        se.str = _s.str();
        stack.emplace(se, op.precedence);
        break;
      }
      case FuncUnk2F:
      case FuncUnk30:
      case FuncNop31:
      case FuncNop32: {
        auto op = OperatorInfos.find(token.type())->second;
        SubExpr se;
        se.isConst = false;
        se.str = op.str;
        stack.emplace(se, op.precedence);
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
        auto op = OperatorInfos.find(token.type())->second;
        SubExpr se = evalSubExpr(token.type(), rhs, evalConst);
        stack.emplace(se, op.precedence);
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
        auto op = OperatorInfos.find(token.type())->second;
        _s << op.str << "(" << arg1.first.str << ", " << arg2.first.str << ")";
        SubExpr se;
        se.isConst = false;
        se.str = _s.str();
        stack.emplace(se, op.precedence);
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
  return stack.top().first.str;
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
  if (_containsAssignment) return false;
  for (const auto &it : _tokens) {
    if (!OperatorInfos.find(it.type())->second.constAllowed) return false;
  }
  return true;
}

SC3Expression::SubExpr SC3Expression::evalSubExpr(
    const std::pair<SubExpr, int> &lhs, SC3ExpressionTokenType type,
    const std::pair<SubExpr, int> &rhs, bool shouldEval) const {
  bool evaluated = false;
  SubExpr result;
  if (shouldEval && lhs.first.isConst && rhs.first.isConst) {
    evaluated = true;
    result.isConst = true;
    switch (type) {
      case Multiply: {
        result.constValue = lhs.first.constValue * rhs.first.constValue;
        break;
      }
      case Divide: {
        result.constValue = lhs.first.constValue / rhs.first.constValue;
        break;
      }
      case Add: {
        result.constValue = lhs.first.constValue + rhs.first.constValue;
        break;
      }
      case Subtract: {
        result.constValue = lhs.first.constValue - rhs.first.constValue;
        break;
      }
      case Modulo: {
        result.constValue = lhs.first.constValue % rhs.first.constValue;
        break;
      }
      case LeftShift: {
        result.constValue = lhs.first.constValue << rhs.first.constValue;
        break;
      }
      case RightShift: {
        result.constValue = lhs.first.constValue >> rhs.first.constValue;
        break;
      }
      case BitwiseAnd: {
        result.constValue = lhs.first.constValue & rhs.first.constValue;
        break;
      }
      case BitwiseXor: {
        result.constValue = lhs.first.constValue ^ rhs.first.constValue;
        break;
      }
      case BitwiseOr: {
        result.constValue = lhs.first.constValue | rhs.first.constValue;
        break;
      }
      case Equal: {
        result.constValue = lhs.first.constValue == rhs.first.constValue;
        break;
      }
      case NotEqual: {
        result.constValue = lhs.first.constValue != rhs.first.constValue;
        break;
      }
      case LessThanEqual: {
        result.constValue = lhs.first.constValue <= rhs.first.constValue;
        break;
      }
      case GreaterThanEqual: {
        result.constValue = lhs.first.constValue >= rhs.first.constValue;
        break;
      }
      case LessThan: {
        result.constValue = lhs.first.constValue < rhs.first.constValue;
        break;
      }
      case GreaterThan: {
        result.constValue = lhs.first.constValue > rhs.first.constValue;
        break;
      }
      default: {
        evaluated = false;
        break;
      }
    }
    if (evaluated) {
      result.str = std::to_string(result.constValue);
    }
  }
  if (!evaluated) {
    auto op = OperatorInfos.find(type)->second;
    std::stringstream _s;
    if (lhs.second < op.precedence ||
        (lhs.second == op.precedence && op.rightAssociative)) {
      _s << "(" << lhs.first.str << ")";
    } else {
      _s << lhs.first.str;
    }
    _s << " " + op.str + " ";
    if (rhs.second < op.precedence ||
        (rhs.second == op.precedence && !op.rightAssociative)) {
      _s << "(" << rhs.first.str << ")";
    } else {
      _s << rhs.first.str;
    }
    result.isConst = false;
    result.str = _s.str();
  }
  return result;
}

SC3Expression::SubExpr SC3Expression::evalSubExpr(
    SC3ExpressionTokenType type, const std::pair<SubExpr, int> &rhs,
    bool shouldEval) const {
  bool evaluated = false;
  SubExpr result;
  if (shouldEval && rhs.first.isConst) {
    evaluated = true;
    result.isConst = true;
    switch (type) {
      case Negation: {
        result.constValue = ~rhs.first.constValue;
        break;
      }
      default: {
        evaluated = false;
        break;
      }
    }
    if (evaluated) {
      result.str = std::to_string(result.constValue);
    }
  }
  if (!evaluated) {
    auto op = OperatorInfos.find(type)->second;
    std::stringstream _s;
    if (type != Negation && type != FuncRandom) {
      _s << op.str << "[" << rhs.first.str << "]";
    } else {
      _s << op.str << "(" << rhs.first.str << ")";
    }
    result.isConst = false;
    result.str = _s.str();
  }
  return result;
}
