#include "SC3Expression.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>

static inline int readInt32Le(uint8_t *src) {
  return src[0] | (src[1] << 8) | (src[2] << 16) | (src[3] << 24);
}

SC3Expression::SC3Expression(uint8_t *rawExpression) {
  auto term = parseTerm(rawExpression, nullptr);

  if (term.node == nullptr) {
    _root = std::make_unique<SC3ExpressionNode>();
    _root->type = EndOfExpression;
    _rawLength = 1;
  } else {
    if (*term.end != SC3ExpressionTokenType::EndOfExpression) {
      throw std::runtime_error("End of expression expected");
    }
    _root = std::unique_ptr<SC3ExpressionNode>(term.node);
    _rawLength = (int)(term.end - rawExpression + 1);
  }
  _simplified = std::unique_ptr<SC3ExpressionNode>(_root->simplify());
  _eaten.clear();
}

std::string SC3Expression::toString(bool evalConst) const {
  if (_root == nullptr) return "";
  if (evalConst) {
    return _simplified->toString();
  }
  return _root->toString();
}

struct OpInfo {
  const std::string str;
  int precedence;
  bool rightAssociative;
  bool constAllowed;
};

const static std::unordered_map<SC3ExpressionTokenType, OpInfo> OperatorInfos =
    {{EndOfExpression, {"", 99, false, true}},
     {ImmediateValue, {"", 99, false, true}},
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

SC3Expression::Term SC3Expression::parseTerm(uint8_t *start, uint8_t *end) {
  uint8_t *cursor = start;
  uint8_t lowestPrecedence = 0xFF;
  uint8_t *lowestPrecedenceOp = nullptr;

  while (
      (cursor < end) ||
      (end == nullptr && *cursor != SC3ExpressionTokenType::EndOfExpression)) {
    if ((*cursor & 0x80) == 0x80) {  // immediate value
      uint8_t *imm = cursor;
      switch (*cursor & 0x60) {
        case 0:  // 1 byte
          cursor++;
          break;
        case 0x20:  // 2 bytes
          cursor += 2;
          break;
        case 0x40:  // 3 bytes
          cursor += 3;
          break;
        case 0x60:  // full little endian int32 after operator byte
          cursor += 5;
          break;
      }
      uint8_t prec = *cursor++;  // skip this, we don't actually need it
      if (lowestPrecedenceOp == nullptr &&
          std::find(std::begin(_eaten), std::end(_eaten), imm) ==
              std::end(_eaten)) {
        lowestPrecedenceOp = imm;
      }
    } else {
      if (lowestPrecedence > cursor[1] &&
          std::find(std::begin(_eaten), std::end(_eaten), cursor) ==
              std::end(_eaten)) {
        lowestPrecedence = cursor[1];
        lowestPrecedenceOp = cursor;
      }
      cursor += 2;
    }
  }

  if (lowestPrecedenceOp == nullptr) {
    Term t;
    t.node = nullptr;
    t.end = nullptr;
    return t;
  }

  SC3ExpressionNode *result = new SC3ExpressionNode();
  Term t;
  t.node = result;
  t.end = cursor;

  // TODO replace this with a more reasonable mask
  _eaten.push_back(lowestPrecedenceOp);
  cursor = lowestPrecedenceOp;

  if ((*cursor & 0x80) == 0x80)  // immediate value
  {
    int value;
    switch (cursor[0] & 0x60)  // length
    {
      case 0:  // 1 byte
        value = (cursor[0] & 0x1F);
        if (cursor[0] & 0x10)  // negative
          value |= 0xFFFFFFE0;
        cursor++;
        break;
      case 0x20:  // 2 bytes
        value = ((cursor[0] & 0x1F) << 8) | cursor[1];
        if (cursor[0] & 0x10)  // negative
          value |= 0xFFFFE000;
        cursor += 2;
        break;
      case 0x40:  // 3 bytes
        value = ((cursor[0] & 0x1F) << 16) | (cursor[2] << 8) | cursor[1];
        if (cursor[0] & 0x10)  // negative
          value |= 0xFFE00000;
        cursor += 3;
        break;
      case 0x60:  // full little endian int32
        cursor++;
        value = readInt32Le(cursor);
        cursor += 4;
    }
    cursor++;  // skip precedence byte

    result->type = SC3ExpressionTokenType::ImmediateValue;
    result->value = value;
  } else {
    SC3ExpressionTokenType type = (SC3ExpressionTokenType)(*cursor++);
    cursor++;
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
      case BitwiseXorAssign: {
        result->type = type;
        auto lhs = parseTerm(start, cursor);
        result->lhs = std::unique_ptr<SC3ExpressionNode>(lhs.node);
        auto rhs = parseTerm(cursor, end);
        result->rhs = std::unique_ptr<SC3ExpressionNode>(rhs.node);
        if (result->lhs == nullptr || result->rhs == nullptr)
          throw std::runtime_error("invalid expression");
        cursor = rhs.end;
        break;
      }
      // unary postfix
      case Increment:
      case Decrement: {
        result->type = type;
        result->lhs =
            std::unique_ptr<SC3ExpressionNode>(parseTerm(start, cursor).node);
        if (result->lhs == nullptr)
          throw std::runtime_error("invalid expression");
        break;
      }
      // 0 arg functions
      case FuncUnk2F:
      case FuncUnk30:
      case FuncNop31:
      case FuncNop32: {
        result->type = type;
        break;
      }
      // unary prefix, 1 arg functions
      case Negation:
      case FuncGlobalVars:
      case FuncFlags:
      case FuncLabelTable:
      case FuncThreadVars:
      case FuncRandom: {
        result->type = type;
        auto rhs = parseTerm(cursor, end);
        result->rhs = std::unique_ptr<SC3ExpressionNode>(rhs.node);
        if (result->rhs == nullptr)
          throw std::runtime_error("invalid expression");
        cursor = std::max(end, rhs.end);
        break;
      }
      // 2 arg functions
      case FuncDataAccess:
      case FuncFarLabelTable:
      case FuncDMA: {
        result->type = type;
        // not happy about this, these are technically two right-hand sides
        auto firstArg = parseTerm(cursor, end);
        result->lhs = std::unique_ptr<SC3ExpressionNode>(firstArg.node);
        auto secondArg = parseTerm(cursor, end);
        result->rhs = std::unique_ptr<SC3ExpressionNode>(secondArg.node);
        if (result->lhs == nullptr || result->rhs == nullptr)
          throw std::runtime_error("invalid expression");
        cursor = std::max(std::max(end, firstArg.end), secondArg.end);
        break;
      }
    }
  }

  return t;
}

SC3ExpressionNode *SC3ExpressionNode::simplify() const {
  SC3ExpressionNode *result = new SC3ExpressionNode();
  switch (type) {
    case EndOfExpression: {
      result->type = EndOfExpression;
      break;
    }
    case ImmediateValue: {
      result->type = ImmediateValue;
      result->value = value;
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
      SC3ExpressionNode *simpleLhs = lhs->simplify();
      SC3ExpressionNode *simpleRhs = rhs->simplify();
      if (simpleLhs->type == ImmediateValue &&
          simpleRhs->type == ImmediateValue &&
          OperatorInfos.at(type).constAllowed) {
        result->type = ImmediateValue;
        result->value = evaluateOp(type, simpleLhs->value, simpleRhs->value);
        delete simpleLhs;
        delete simpleRhs;
      } else {
        result->type = type;
        result->lhs = std::unique_ptr<SC3ExpressionNode>(simpleLhs);
        result->rhs = std::unique_ptr<SC3ExpressionNode>(simpleRhs);
      }
      break;
    }
    case Increment:
    case Decrement: {
      result->type = type;
      result->lhs = std::unique_ptr<SC3ExpressionNode>(lhs->simplify());
      break;
    }
    case FuncUnk2F:
    case FuncUnk30:
    case FuncNop31:
    case FuncNop32: {
      result->type = type;
      break;
    }
    case Negation:
    case FuncGlobalVars:
    case FuncFlags:
    case FuncLabelTable:
    case FuncThreadVars:
    case FuncRandom: {
      SC3ExpressionNode *simpleRhs = rhs->simplify();
      if (simpleRhs->type == ImmediateValue &&
          OperatorInfos.at(type).constAllowed) {
        result->type = ImmediateValue;
        result->value = evaluateOp(type, simpleRhs->value);
        delete simpleRhs;
      } else {
        result->type = type;
        result->rhs = std::unique_ptr<SC3ExpressionNode>(simpleRhs);
      }
      break;
    }
    case FuncDataAccess:
    case FuncFarLabelTable:
    case FuncDMA: {
      result->type = type;
      result->lhs = std::unique_ptr<SC3ExpressionNode>(lhs->simplify());
      result->rhs = std::unique_ptr<SC3ExpressionNode>(rhs->simplify());
      break;
    }
  }
  return result;
}

int SC3ExpressionNode::evaluateOp(SC3ExpressionTokenType op, int lhs, int rhs) {
  switch (op) {
    case Multiply:
      return lhs * rhs;
    case Divide:
      return lhs / rhs;
    case Add:
      return lhs + rhs;
    case Subtract:
      return lhs - rhs;
    case Modulo:
      return lhs % rhs;
    case LeftShift:
      return lhs << rhs;
    case RightShift:
      return lhs >> rhs;
    case BitwiseAnd:
      return lhs & rhs;
    case BitwiseXor:
      return lhs ^ rhs;
    case BitwiseOr:
      return lhs | rhs;
    case Equal:
      return lhs == rhs;
    case NotEqual:
      return lhs != rhs;
    case LessThanEqual:
      return lhs <= rhs;
    case GreaterThanEqual:
      return lhs >= rhs;
    case LessThan:
      return lhs < rhs;
    case GreaterThan:
      return lhs > rhs;
  }
  throw std::runtime_error("invalid operator");
}

int SC3ExpressionNode::evaluateOp(SC3ExpressionTokenType op, int rhs) {
  switch (op) {
    case Negation:
      return ~rhs;
  }
  throw std::runtime_error("invalid operator");
}

std::string SC3ExpressionNode::toString() const {
  const OpInfo &thisOp = OperatorInfos.at(type);
  switch (type) {
    case ImmediateValue: {
      return std::to_string(value);
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
      std::stringstream _s;
      const OpInfo &lhsOp = OperatorInfos.at(lhs->type);
      const OpInfo &rhsOp = OperatorInfos.at(rhs->type);
      bool shouldParenthesizeLhs = false;
      if (lhsOp.precedence < thisOp.precedence ||
          (lhsOp.precedence == thisOp.precedence && thisOp.rightAssociative)) {
        _s << "(" << lhs->toString() << ")";
      } else {
        _s << lhs->toString();
      }
      _s << " " << thisOp.str << " ";
      if (rhsOp.precedence < thisOp.precedence ||
          (rhsOp.precedence == thisOp.precedence && !thisOp.rightAssociative)) {
        _s << "(" << rhs->toString() << ")";
      } else {
        _s << rhs->toString();
      }
      return _s.str();
    }
    case Increment:
    case Decrement: {
      std::stringstream _s;
      const OpInfo &lhsOp = OperatorInfos.at(lhs->type);
      if (thisOp.precedence < lhsOp.precedence) {
        _s << "(" << lhs->toString() << ")";
      } else {
        _s << lhs->toString();
      }
      _s << thisOp.str;
      return _s.str();
    }
    case FuncUnk2F:
    case FuncUnk30:
    case FuncNop31:
    case FuncNop32: {
      return thisOp.str;
    }
    case Negation:
    case FuncGlobalVars:
    case FuncFlags:
    case FuncLabelTable:
    case FuncThreadVars:
    case FuncRandom: {
      if (type != Negation && type != FuncRandom) {
        return thisOp.str + "[" + rhs->toString() + "]";
      } else {
        return thisOp.str + "(" + rhs->toString() + ")";
      }
    }
    case FuncDataAccess:
    case FuncFarLabelTable:
    case FuncDMA: {
      return thisOp.str + "(" + lhs->toString() + ", " + rhs->toString() + ")";
    }
    default: { return ""; }
  }
}

void SC3ExpressionNode::traverse(
    std::function<void(const SC3ExpressionNode *)> visitor) const {
  visitor(this);
  if (lhs != nullptr) lhs->traverse(visitor);
  if (rhs != nullptr) rhs->traverse(visitor);
}