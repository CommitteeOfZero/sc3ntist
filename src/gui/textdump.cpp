#include "textdump.h"
#include <iomanip>
#include <sstream>
#include <string>

#include "parser/SC3Argument.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"
#include "parser/SC3Expression.h"

#include "parser/IContextProvider.h"

static std::string uint8_vector_to_hex_string(const std::vector<uint8_t> &v) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  std::vector<uint8_t>::const_iterator it;

  for (it = v.begin(); it != v.end(); it++) {
    if (it != v.begin()) ss << " ";
    ss << std::setw(2) << static_cast<unsigned>(*it);
  }

  return ss.str();
}

// TODO this could go into a per-file textifier class, held by DisassemblyModel,
// really

std::string SC3ExpressionNodeToString(IContextProvider *ctx, int fileId,
                                      const SC3ExpressionNode *node) {
  const OpInfo &thisOp = OperatorInfos.at(node->type);
  switch (node->type) {
    case ImmediateValue: {
      return std::to_string(node->value);
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
      const OpInfo &lhsOp = OperatorInfos.at(node->lhs->type);
      const OpInfo &rhsOp = OperatorInfos.at(node->rhs->type);
      if (lhsOp.precedence < thisOp.precedence ||
          (lhsOp.precedence == thisOp.precedence && thisOp.rightAssociative)) {
        _s << "(" << SC3ExpressionNodeToString(ctx, fileId, node->lhs.get())
           << ")";
      } else {
        _s << SC3ExpressionNodeToString(ctx, fileId, node->lhs.get());
      }
      _s << " " << thisOp.str << " ";
      if (rhsOp.precedence < thisOp.precedence ||
          (rhsOp.precedence == thisOp.precedence && !thisOp.rightAssociative)) {
        _s << "(" << SC3ExpressionNodeToString(ctx, fileId, node->rhs.get())
           << ")";
      } else {
        _s << SC3ExpressionNodeToString(ctx, fileId, node->rhs.get());
      }
      return _s.str();
    }
    case Increment:
    case Decrement: {
      std::stringstream _s;
      const OpInfo &lhsOp = OperatorInfos.at(node->lhs->type);
      if (thisOp.precedence < lhsOp.precedence) {
        _s << "(" << SC3ExpressionNodeToString(ctx, fileId, node->lhs.get())
           << ")";
      } else {
        _s << SC3ExpressionNodeToString(ctx, fileId, node->lhs.get());
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
    case FuncGlobalVars:
    case FuncFlags:
    case FuncLabelTable:
      if (ctx == nullptr || node->rhs->type != ImmediateValue) {
        return thisOp.str + "[" +
               SC3ExpressionNodeToString(ctx, fileId, node->rhs.get()) + "]";
      } else {
        std::string name;
        switch (node->type) {
          case FuncGlobalVars:
            name = ctx->globalVarName(node->rhs->value);
            break;
          case FuncFlags:
            name = ctx->flagName(node->rhs->value);
            break;
          case FuncLabelTable:
            name = ctx->labelName(fileId, node->rhs->value);
            break;
          default:
            break;
        }
        return thisOp.str + "[" + name + "]";
      }
    case FuncThreadVars:
      return thisOp.str + "[" +
             SC3ExpressionNodeToString(ctx, fileId, node->rhs.get()) + "]";
    case Negation:
    case FuncRandom: {
      return thisOp.str + "(" +
             SC3ExpressionNodeToString(ctx, fileId, node->rhs.get()) + ")";
    }
    case FuncDataAccess:
    case FuncFarLabelTable:
    case FuncDMA: {
      return thisOp.str + "(" +
             SC3ExpressionNodeToString(ctx, fileId, node->lhs.get()) + ", " +
             SC3ExpressionNodeToString(ctx, fileId, node->rhs.get()) + ")";
    }
    default: { return ""; }
  }
}

std::string SC3ExpressionToString(IContextProvider *ctx, int fileId,
                                  const SC3Expression &expr) {
  auto root = expr.simplified();
  return SC3ExpressionNodeToString(ctx, fileId, root);
}

std::string SC3ArgumentToString(IContextProvider *ctx, int fileId,
                                const SC3Argument &arg) {
  switch (arg.type) {
    case ByteArray: {
      return uint8_vector_to_hex_string(arg.byteArrayValue);
      break;
    }
    case Byte: {
      return std::to_string((unsigned)arg.byteValue);
      break;
    }
    case UInt16: {
      return std::to_string((unsigned)arg.uint16_value);
      break;
    }
    case Expression: {
      const auto &expr = arg.exprValue;
      return SC3ExpressionToString(ctx, fileId, expr);
      break;
    }
    case LocalLabel: {
      auto id = arg.uint16_value;
      if (ctx != nullptr) {
        return "#" + ctx->labelName(fileId, id);
      } else {
        return "LocalLabelRef(" + std::to_string(id) + ")";
      }
      break;
    }
    case FarLabel: {
      const auto &expr = arg.exprValue;
      auto id = arg.uint16_value;
      return "FarLabelRef(" + SC3ExpressionToString(ctx, fileId, expr) + ", " +
             std::to_string(id) + ")";
    }
    case ReturnAddress: {
      auto id = arg.uint16_value;
      return "ReturnAddressRef(" + std::to_string(id) + ")";
      break;
    }
    case StringRef: {
      auto id = arg.uint16_value;
      return "StringRef(" + std::to_string(id) + ")";
      break;
    }
    case ExprFlagRef: {
      const auto &expr = arg.exprValue;
      if (ctx != nullptr && expr.simplified()->type == ImmediateValue) {
        return "FlagRef(" + ctx->flagName(expr.simplified()->value) + ")";
      }
      return "FlagRef(" + SC3ExpressionToString(ctx, fileId, expr) + ")";
      break;
    }
    case ExprGlobalVarRef: {
      const auto &expr = arg.exprValue;
      if (ctx != nullptr && expr.simplified()->type == ImmediateValue) {
        return "GlobalVarRef(" + ctx->globalVarName(expr.simplified()->value) +
               ")";
      }
      return "GlobalVarRef(" + SC3ExpressionToString(ctx, fileId, expr) + ")";
      break;
    }
    case ExprThreadVarRef: {
      const auto &expr = arg.exprValue;
      return "ThreadVarRef(" + SC3ExpressionToString(ctx, fileId, expr) + ")";
      break;
    }
  }
  return "unrecognized";
}

std::string DumpSC3InstructionToText(IContextProvider *ctx, int fileId,
                                     const SC3Instruction *inst) {
  std::stringstream out;
  if (inst->name() == "Assign") {
    out << SC3ArgumentToString(ctx, fileId, inst->args().at(0));
  } else {
    out << inst->name();
    int i = 0;
    int argCount = inst->args().size();
    if (argCount > 0) {
      out << "(";
      for (const auto &arg : inst->args()) {
        i++;
        out << arg.name << ": ";
        out << SC3ArgumentToString(ctx, fileId, arg);
        if (i < argCount) out << ", ";
      }
      out << ")";
    }
  }
  return out.str();
}

std::string DumpSCXFileToText(IContextProvider *ctx, const SCXFile *file) {
  std::stringstream out;
  for (const auto &label : file->disassembly()) {
    if (ctx != nullptr) {
      out << "\n#" << ctx->labelName(file->getId(), label->id()) << ":\n";
    } else {
      out << "\n#label" << label->id() << "_" << label->address() << ":\n";
    }
    for (const auto &inst : label->instructions()) {
      out << "\t" << DumpSC3InstructionToText(ctx, file->getId(), inst.get())
          << "\n";
    }
  }
  return out.str();
}