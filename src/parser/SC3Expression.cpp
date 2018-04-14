#include "SC3Expression.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>

static inline int readInt32Le(uint8_t *src) {
  return src[0] | (src[1] << 8) | (src[2] << 16) | (src[3] << 24);
}

SC3Expression::SC3Expression() {
  // allow blank expressions for arguments without expressions
}

SC3Expression::SC3Expression(uint8_t *rawExpression) {
#if 0
  auto term = parseTerm(rawExpression, nullptr);

  if (term.node == nullptr) {
    _root = std::unique_ptr<SC3ExpressionNode>(new SC3ExpressionNode());
    _root->type = EndOfExpression;
    _rawLength = 1;
  } else {
    if (*term.end != SC3ExpressionTokenType::EndOfExpression) {
      throw std::runtime_error("End of expression expected");
    }
    _root = std::unique_ptr<SC3ExpressionNode>(term.node);
    _rawLength = (int)(term.end - rawExpression + 1);
  }
#else
  _cursor = rawExpression;
  SC3ExpressionNode *expr = ParseSubExpression(0);
  if (expr == nullptr) {
    _root = std::unique_ptr<SC3ExpressionNode>(new SC3ExpressionNode());
    _root->type = EndOfExpression;
    _rawLength = 1;
  } else {
    _root = std::unique_ptr<SC3ExpressionNode>(expr);
    if (*_cursor != SC3ExpressionTokenType::EndOfExpression) {
      throw std::runtime_error("End of expression expected");
    }
    _rawLength = _cursor - rawExpression + 1;
  }
#endif
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

const std::unordered_map<SC3ExpressionTokenType, OpInfo, tokenTypeHash>
    OperatorInfos = {
        {EndOfExpression, {"", 99, false, true, None}},
        {ImmediateValue, {"", 99, false, true, None}},
        {Multiply, {"*", 9, false, true, Binary}},
        {Divide, {"/", 9, false, true, Binary}},
        {Add, {"+", 8, false, true, Binary}},
        {Subtract, {"-", 8, false, true, Binary}},
        {Modulo, {"%", 9, false, true, Binary}},
        {LeftShift, {"<<", 7, false, true, Binary}},
        {RightShift, {">>", 7, false, true, Binary}},
        {BitwiseAnd, {"&", 4, false, true, Binary}},
        {BitwiseXor, {"^", 3, false, true, Binary}},
        {BitwiseOr, {"|", 2, false, true, Binary}},
        {Equal, {"==", 5, false, true, Binary}},
        {NotEqual, {"!=", 5, false, true, Binary}},
        {LessThanEqual, {"<=", 6, false, true, Binary}},
        {GreaterThanEqual, {">=", 6, false, true, Binary}},
        {LessThan, {"<", 6, false, true, Binary}},
        {GreaterThan, {">", 6, false, true, Binary}},
        {Assign, {"=", 1, true, false, Binary}},
        {MultiplyAssign, {"*=", 1, true, false, Binary}},
        {DivideAssign, {"/=", 1, true, false, Binary}},
        {AddAssign, {"+=", 1, true, false, Binary}},
        {SubtractAssign, {"-=", 1, true, false, Binary}},
        {ModuloAssign, {"%=", 1, true, false, Binary}},
        {LeftShiftAssign, {"<<=", 1, true, false, Binary}},
        {RightShiftAssign, {">>=", 1, true, false, Binary}},
        {BitwiseAndAssign, {"&=", 1, true, false, Binary}},
        {BitwiseOrAssign, {"|=", 1, true, false, Binary}},
        {BitwiseXorAssign, {"^=", 1, true, false, Binary}},
        {Increment, {"++", 11, false, false, PostfixUnary}},
        {Decrement, {"--", 11, false, false, PostfixUnary}},
        {Negation, {"~", 10, true, true, PrefixUnary}},
        {FuncGlobalVars, {"GlobalVars", 11, false, false, PrefixUnary}},
        {FuncFlags, {"Flags", 11, false, false, PrefixUnary}},
        {FuncDataAccess, {"DataAccess", 11, false, false, TwoArgFunc}},
        {FuncLabelTable, {"LabelTable", 11, false, false, PrefixUnary}},
        {FuncFarLabelTable, {"FarLabelTable", 11, false, false, TwoArgFunc}},
        {FuncThreadVars, {"ThreadVars", 11, false, false, PrefixUnary}},
        {FuncDMA, {"DMA", 11, false, false, TwoArgFunc}},
        {FuncUnk2F, {"GetUnk2F", 11, false, false, None}},
        {FuncUnk30, {"GetUnk30", 11, false, false, None}},
        {FuncNop31, {"Nop31", 11, false, false, None}},
        {FuncNop32, {"Nop32", 11, false, false, None}},
        {FuncRandom, {"Random", 11, false, false, PrefixUnary}}};

SC3ExpressionNode *SC3Expression::ParseSubExpression(int minPrec) {
  SC3ExpressionNode *lhs = _ParseTerm();
  if (lhs == nullptr || lhs->type == EndOfExpression) return lhs;

  SC3ExpressionToken maybePostfix = PeekExpressionToken();
  OpInfo rhsInfo = OperatorInfos.at(maybePostfix.type);
  if (rhsInfo.operands == PostfixUnary && maybePostfix.precedence >= minPrec) {
    EatExpressionToken(maybePostfix);
    SC3ExpressionNode *result = new SC3ExpressionNode();
    result->type = maybePostfix.type;
    result->lhs = std::unique_ptr<SC3ExpressionNode>(lhs);
    lhs = result;
  }

  SC3ExpressionToken token;
  while ((token = PeekExpressionToken()).precedence >= minPrec) {
    OpInfo info = OperatorInfos.at(token.type);
    if (info.operands == Binary) {
      EatExpressionToken(token);
      SC3ExpressionNode *rhs = ParseSubExpression(token.precedence + 1);
      SC3ExpressionNode *result = new SC3ExpressionNode();
      result->lhs = std::unique_ptr<SC3ExpressionNode>(lhs);
      result->type = token.type;
      result->rhs = std::unique_ptr<SC3ExpressionNode>(rhs);
      lhs = result;
    } else {
      break;
    }
  }

  return lhs;
}

SC3ExpressionNode *SC3Expression::_ParseTerm() {
  SC3ExpressionToken token = NextExpressionToken();

  SC3ExpressionNode *term = nullptr;
  OpInfo info = OperatorInfos.at(token.type);
  if (token.type == EndOfExpression) {
    term = new SC3ExpressionNode();
    term->type = EndOfExpression;
    return term;
  } else if (token.type == ImmediateValue) {
    term = new SC3ExpressionNode();
    term->type = ImmediateValue;
    term->value = token.value;
  } else if (info.operands == PrefixUnary) {
    term = new SC3ExpressionNode();
    term->type = token.type;
    term->rhs = std::unique_ptr<SC3ExpressionNode>(
        ParseSubExpression(token.precedence + 1));
  } else if (info.operands == TwoArgFunc) {
    term = new SC3ExpressionNode();
    term->type = token.type;
    term->lhs = std::unique_ptr<SC3ExpressionNode>(
        ParseSubExpression(token.precedence + 1));
    term->rhs = std::unique_ptr<SC3ExpressionNode>(
        ParseSubExpression(token.precedence + 1));
  } else if (info.operands == Binary || info.operands == PostfixUnary) {
    return nullptr;
  }

  return term;
}

SC3ExpressionToken SC3Expression::PeekExpressionToken() {
  auto result = NextExpressionToken();
  _cursor -= result.length;
  return result;
}

void SC3Expression::EatExpressionToken(const SC3ExpressionToken &token) {
  _cursor += token.length;
}

SC3ExpressionToken SC3Expression::NextExpressionToken() {
  uint8_t *oldCursor = _cursor;
  SC3ExpressionToken result;

  if ((*_cursor & 0x80) == 0x80)  // immediate value
  {
    int value = 0;
    switch (_cursor[0] & 0x60)  // length
    {
      case 0:  // 1 byte
        value = (_cursor[0] & 0x1F);
        if (_cursor[0] & 0x10)  // negative
          value |= 0xFFFFFFE0;
        _cursor++;
        break;
      case 0x20:  // 2 bytes
        value = ((_cursor[0] & 0x1F) << 8) | _cursor[1];
        if (_cursor[0] & 0x10)  // negative
          value |= 0xFFFFE000;
        _cursor += 2;
        break;
      case 0x40:  // 3 bytes
        value = ((_cursor[0] & 0x1F) << 16) | (_cursor[2] << 8) | _cursor[1];
        if (_cursor[0] & 0x10)  // negative
          value |= 0xFFE00000;
        _cursor += 3;
        break;
      case 0x60:  // full little endian int32
        _cursor++;
        value = readInt32Le(_cursor);
        _cursor += 4;
    }
    result.precedence = *_cursor++;

    result.type = SC3ExpressionTokenType::ImmediateValue;
    result.value = value;
  } else {
    result.type = (SC3ExpressionTokenType)*_cursor++;
    if (result.type == SC3ExpressionTokenType::EndOfExpression) {
      result.precedence = -1;
    } else {
      result.precedence = *_cursor++;
    }
  }

  result.length = _cursor - oldCursor;
  return result;
}

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
      cursor++;  // skip precedence byte
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
    int value = 0;
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
      default: { throw std::runtime_error("invalid expression"); }
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
    default: { result->type = SC3ExpressionTokenType::EndOfExpression; }
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
    default:
      throw std::runtime_error("invalid operator");
  }
}

int SC3ExpressionNode::evaluateOp(SC3ExpressionTokenType op, int rhs) {
  switch (op) {
    case Negation:
      return ~rhs;
    default:
      throw std::runtime_error("invalid operator");
  }
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