#include "textdump.h"
#include <iomanip>
#include <sstream>
#include <string>

#include "parser/SC3Argument.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"

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

static std::string SC3ArgumentToString(const SC3Argument *arg) {
  switch (arg->type()) {
    case ByteArray: {
      auto data = static_cast<const SC3ArgByteArray *>(arg)->data();
      return uint8_vector_to_hex_string(data);
      break;
    }
    case Byte: {
      auto value = static_cast<const SC3ArgByte *>(arg)->value();
      return std::to_string((unsigned)value);
      break;
    }
    case UInt16: {
      auto value = static_cast<const SC3ArgUInt16 *>(arg)->value();
      return std::to_string((unsigned)value);
      break;
    }
    case Expression: {
      const auto &expr = static_cast<const SC3ArgExpression *>(arg)->expr();
      return expr.toString(true);
      break;
    }
    case LocalLabel: {
      auto id = static_cast<const SC3ArgLocalLabel *>(arg)->id();
      return "LocalLabelRef(" + std::to_string(id) + ")";
      break;
    }
    case FarLabel: {
      const auto &expr =
          static_cast<const SC3ArgFarLabel *>(arg)->scriptBufferId();
      auto id = static_cast<const SC3ArgFarLabel *>(arg)->labelId();
      return "FarLabelRef(" + expr.toString(true) + ", " + std::to_string(id) +
             ")";
    }
    case ReturnAddress: {
      auto id = static_cast<const SC3ArgReturnAddress *>(arg)->id();
      return "ReturnAddressRef(" + std::to_string(id) + ")";
      break;
    }
    case StringRef: {
      auto id = static_cast<const SC3ArgStringRef *>(arg)->id();
      return "StringRef(" + std::to_string(id) + ")";
      break;
    }
    case ExprFlagRef: {
      const auto &expr = static_cast<const SC3ArgExprFlagRef *>(arg)->expr();
      return "FlagRef(" + expr.toString(true) + ")";
      break;
    }
    case ExprGlobalVarRef: {
      const auto &expr =
          static_cast<const SC3ArgExprGlobalVarRef *>(arg)->expr();
      return "GlobalVarRef(" + expr.toString(true) + ")";
      break;
    }
    case ExprThreadVarRef: {
      const auto &expr =
          static_cast<const SC3ArgExprThreadVarRef *>(arg)->expr();
      return "ThreadVarRef(" + expr.toString(true) + ")";
      break;
    }
  }
  return "unrecognized";
}

std::string DumpSC3InstructionToText(const SC3Instruction *inst) {
  std::stringstream out;
  if (inst->name() == "Assign") {
    out << SC3ArgumentToString(inst->args().at(0).get());
  } else {
    out << inst->name();
    int i = 0;
    int argCount = inst->args().size();
    if (argCount > 0) {
      out << "(";
      for (const auto &arg : inst->args()) {
        i++;
        out << arg->name() << ": ";
        out << SC3ArgumentToString(arg.get());
        if (i < argCount) out << ", ";
      }
      out << ")";
    }
  }
  return out.str();
}

std::string DumpSCXFileToText(const SCXFile *file) {
  std::stringstream out;
  int i = 0;
  for (const auto &label : file->disassembly()) {
    out << "\n#label" << i << "_" << label->address() << ":\n";
    i++;
    for (const auto &inst : label->instructions()) {
      out << "\t" << DumpSC3InstructionToText(inst.get()) << "\n";
    }
  }
  return out.str();
}