#include "textdump.h"
#include <iomanip>
#include <sstream>
#include <string>

#include "parser/SC3Argument.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"
#include "parser/SC3Expression.h"

#include "parser/IContextProvider.h"

#include "guiutil.h"

// TODO this could go into a per-file textifier class, held by DisassemblyModel,
// really

std::string SC3ExpressionNodeToString(bool richText, IContextProvider *ctx,
                                      int fileId,
                                      const SC3ExpressionNode *node) {
  std::stringstream _s;
  const OpInfo &thisOp = OperatorInfos.at(node->type);
  switch (node->type) {
    case ImmediateValue: {
      if (richText) _s << "<span class='number'>";
      _s << std::to_string(node->value);
      if (richText) _s << "</span>";
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
      const OpInfo &lhsOp = OperatorInfos.at(node->lhs->type);
      const OpInfo &rhsOp = OperatorInfos.at(node->rhs->type);
      if (lhsOp.precedence < thisOp.precedence ||
          (lhsOp.precedence == thisOp.precedence && thisOp.rightAssociative)) {
        _s << "("
           << SC3ExpressionNodeToString(richText, ctx, fileId, node->lhs.get())
           << ")";
      } else {
        _s << SC3ExpressionNodeToString(richText, ctx, fileId, node->lhs.get());
      }
      _s << " ";
      if (richText) _s << "<span class='operator'>";
      if (richText)
        _s << htmlEscape(thisOp.str);
      else
        _s << thisOp.str;
      if (richText) _s << "</span>";
      _s << " ";
      if (rhsOp.precedence < thisOp.precedence ||
          (rhsOp.precedence == thisOp.precedence && !thisOp.rightAssociative)) {
        _s << "("
           << SC3ExpressionNodeToString(richText, ctx, fileId, node->rhs.get())
           << ")";
      } else {
        _s << SC3ExpressionNodeToString(richText, ctx, fileId, node->rhs.get());
      }
      break;
    }
    case Increment:
    case Decrement: {
      const OpInfo &lhsOp = OperatorInfos.at(node->lhs->type);
      if (thisOp.precedence < lhsOp.precedence) {
        _s << "("
           << SC3ExpressionNodeToString(richText, ctx, fileId, node->lhs.get())
           << ")";
      } else {
        _s << SC3ExpressionNodeToString(richText, ctx, fileId, node->lhs.get());
      }
      if (richText) _s << "<span class='operator'>";
      _s << thisOp.str;
      if (richText) _s << "</span>";
      break;
    }
    case FuncUnk2F:
    case FuncUnk30:
    case FuncNop31:
    case FuncNop32: {
      if (richText) _s << "<span class='func'>";
      _s << thisOp.str;
      if (richText) _s << "</span>";
      break;
    }
    case FuncGlobalVars:
    case FuncFlags:
    case FuncLabelTable:
      if (richText) _s << "<span class='" << thisOp.str << "'>";
      _s << thisOp.str;
      if (richText) _s << "</span>";
      _s << "[";
      if (ctx == nullptr || node->rhs->type != ImmediateValue) {
        _s << SC3ExpressionNodeToString(richText, ctx, fileId, node->rhs.get());
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
            name = "#" + ctx->labelName(fileId, node->rhs->value);
            break;
          default:
            break;
        }
        if (richText) {
          if (name == std::to_string(node->rhs->value))
            _s << "<span class='number'>";
          else
            _s << "<span class='" << thisOp.str << "_name'>";
        }
        _s << name;
        if (richText) _s << "</span>";
      }
      _s << "]";
      break;
    case FuncThreadVars:
      if (richText) _s << "<span class='" << thisOp.str << "'>";
      _s << thisOp.str;
      if (richText) _s << "</span>";
      _s << "[" +
                SC3ExpressionNodeToString(richText, ctx, fileId,
                                          node->rhs.get()) +
                "]";
      break;
    case Negation:
    case FuncRandom: {
      if (richText) {
        if (node->type == Negation)
          _s << "<span class='operator'>";
        else
          _s << "<span class='func'>";
      }
      _s << thisOp.str;
      if (richText) _s << "</span>";
      _s << "(" +
                SC3ExpressionNodeToString(richText, ctx, fileId,
                                          node->rhs.get()) +
                ")";
      break;
    }
    case FuncDataAccess:
    case FuncFarLabelTable:
    case FuncDMA: {
      if (richText) _s << "<span class='func'>";
      _s << thisOp.str;
      if (richText) _s << "</span>";
      _s << "(" +
                SC3ExpressionNodeToString(richText, ctx, fileId,
                                          node->lhs.get()) +
                ", " +
                SC3ExpressionNodeToString(richText, ctx, fileId,
                                          node->rhs.get()) +
                ")";
      break;
    }
    default: { return ""; }
  }
  return _s.str();
}

std::string SC3ExpressionToString(bool richText, IContextProvider *ctx,
                                  int fileId, const SC3Expression &expr) {
  auto root = expr.simplified();
  return SC3ExpressionNodeToString(richText, ctx, fileId, root);
}

std::string SC3ArgumentToString(bool richText, IContextProvider *ctx,
                                int fileId, const SC3Argument &arg) {
  std::stringstream out;
  switch (arg.type) {
    case ByteArray: {
      if (richText) out << "<span class='byteArrayArg'>";
      out << uint8_vector_to_hex_string(arg.byteArrayValue);
      if (richText) out << "</span>";
      break;
    }
    case Byte: {
      if (richText) out << "<span class='byteArg number'>";
      out << std::to_string((unsigned)arg.byteValue);
      if (richText) out << "</span>";
      break;
    }
    case UInt16: {
      if (richText) out << "<span class='uint16Arg number'>";
      out << std::to_string((unsigned)arg.uint16_value);
      if (richText) out << "</span>";
      break;
    }
    case Expression: {
      const auto &expr = arg.exprValue;
      if (richText) out << "<span class='exprArg expr'>";
      out << SC3ExpressionToString(richText, ctx, fileId, expr);
      if (richText) out << "</span>";
      break;
    }
    case LocalLabel: {
      auto id = arg.uint16_value;
      if (ctx != nullptr) {
        if (richText)
          out << "<span class='localLabelArg namedLabelArg LabelTable_name'>";
        out << "#" + ctx->labelName(fileId, id);
        if (richText) out << "</span>";
      } else {
        if (richText) out << "<span class='localLabelArg unnamedLabelArg'>";
        out << "LocalLabelRef(";
        if (richText) out << "<span class='number'>";
        out << std::to_string(id);
        if (richText) out << "</span>";
        out << ")";
        if (richText) out << "</span>";
      }
      break;
    }
    case FarLabel: {
      const auto &expr = arg.exprValue;
      auto id = arg.uint16_value;
      if (richText) out << "<span class='farLabelArg unnamedLabelArg'>";
      out << "FarLabelRef(";
      if (richText) out << "<span class='expr'>";
      out << SC3ExpressionToString(richText, ctx, fileId, expr);
      if (richText) out << "</span>";
      out << ", ";
      if (richText) out << "<span class='number'>";
      out << std::to_string(id);
      if (richText) out << "</span>";
      out << ")";
      if (richText) out << "</span>";
      break;
    }
    case ReturnAddress: {
      auto id = arg.uint16_value;
      if (richText) out << "<span class='returnAddressArg'>";
      out << "ReturnAddressRef(";
      if (richText) out << "<span class='number'>";
      out << std::to_string(id);
      if (richText) out << "</span>";
      out << ")";
      if (richText) out << "</span>";
      break;
    }
    case StringRef: {
      auto id = arg.uint16_value;
      if (richText) out << "<span class='stringRefArg'>";
      out << "StringRef(";
      if (richText) out << "<span class='number'>";
      out << std::to_string(id);
      if (richText) out << "</span>";
      out << ")";
      if (richText) out << "</span>";
      break;
    }
    case ExprFlagRef: {
      const auto &expr = arg.exprValue;
      if (richText) out << "<span class='exprFlagRefArg'>";
      if (ctx != nullptr && expr.simplified()->type == ImmediateValue) {
        if (richText) out << "<span class='flagRefFunc'>";
        out << "FlagRef";
        if (richText) out << "</span>";
        out << "(";
        auto name = ctx->flagName(expr.simplified()->value);
        if (richText) {
          if (name == std::to_string(expr.simplified()->value))
            out << "<span class='number'>";
          else
            out << "<span class='Flags_name'>";
        }
        out << name;
        if (richText) out << "</span>";
        out << ")";
      } else {
        if (richText) out << "<span class='flagRefFunc'>";
        out << "FlagRef";
        if (richText) out << "</span>";
        out << "(";
        if (richText) out << "<span class='expr'>";
        out << SC3ExpressionToString(richText, ctx, fileId, expr);
        if (richText) out << "</span>";
        out << ")";
      }
      if (richText) out << "</span>";
      break;
    }
    case ExprGlobalVarRef: {
      const auto &expr = arg.exprValue;
      if (richText) out << "<span class='exprGlobalVarRefArg'>";
      if (ctx != nullptr && expr.simplified()->type == ImmediateValue) {
        if (richText) out << "<span class='globalVarRefFunc'>";
        out << "GlobalVarRef";
        if (richText) out << "</span>";
        out << "(";
        auto name = ctx->globalVarName(expr.simplified()->value);
        if (richText) {
          if (name == std::to_string(expr.simplified()->value))
            out << "<span class='number'>";
          else
            out << "<span class='GlobalVars_name'>";
        }
        out << name;
        if (richText) out << "</span>";
        out << ")";
      } else {
        if (richText) out << "<span class='globalVarRefFunc'>";
        out << "GlobalVarRef";
        if (richText) out << "</span>";
        out << "(";
        if (richText) out << "<span class='expr'>";
        out << SC3ExpressionToString(richText, ctx, fileId, expr);
        if (richText) out << "</span>";
        out << ")";
      }
      if (richText) out << "</span>";
      break;
    }
    case ExprThreadVarRef: {
      const auto &expr = arg.exprValue;
      if (richText) out << "<span class='exprThreadVarRefArg'>";
      if (richText) out << "<span class='threadVarRefFunc'>";
      out << "ThreadVarRef";
      if (richText) out << "</span>";
      out << "(";
      if (richText) out << "<span class='expr'>";
      out << SC3ExpressionToString(richText, ctx, fileId, expr);
      if (richText) out << "</span>";
      out << ")";
      if (richText) out << "</span>";
      break;
    }
    default: { return "unrecognized"; }
  }
  return out.str();
}

std::string DumpSC3InstructionToText(bool richText, IContextProvider *ctx,
                                     int fileId, const SC3Instruction *inst) {
  std::stringstream out;
  if (inst->name() == "Assign") {
    if (richText) out << "<span class='InstAssign'>";
    out << SC3ArgumentToString(richText, ctx, fileId, inst->args().at(0));
    out << "</span>";
  } else {
    if (richText)
      out << "<span class='Inst" << inst->name()
          << "'><span class='instructionName'>";
    out << inst->name();
    if (richText) out << "</span>";
    int i = 0;
    int argCount = inst->args().size();
    if (argCount > 0) {
      out << "(";
      for (const auto &arg : inst->args()) {
        i++;
        if (richText) out << "<span class='argName'>";
        out << arg.name << ": ";
        if (richText) out << "</span>";
        out << SC3ArgumentToString(richText, ctx, fileId, arg);
        if (i < argCount) out << ", ";
      }
      out << ")";
      if (richText) out << "</span>";
    }
  }
  return out.str();
}

std::string DumpSCXFileToText(bool richText, IContextProvider *ctx,
                              const SCXFile *file) {
  std::stringstream out;

  if (richText)
    out << "<html><head><title>" << file->getName()
        << "</title><link rel='stylesheet' type='text/css' "
           "href='sc3syntaxhighlight.css'></head><body>";

  for (const auto &label : file->disassembly()) {
    if (richText)
      out << "<br><br><div class='label'>";
    else
      out << "\n\n";

    if (ctx != nullptr)
      out << "#" << ctx->labelName(file->getId(), label->id()) << ":";
    else
      out << "#label" << label->id() << "_" << label->address() << ":";

    if (richText)
      out << "</div><br>";
    else
      out << "\n";

    for (const auto &inst : label->instructions()) {
      if (richText)
        out << "<div class='instruction'>";
      else
        out << "\t";

      out << DumpSC3InstructionToText(richText, ctx, file->getId(), inst.get());

      if (richText)
        out << "</div><br>";
      else
        out << "\n";
    }
  }

  if (richText) out << "</body></html>";

  return out.str();
}