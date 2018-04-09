#include "textdump.h"
#include <iomanip>
#include <sstream>
#include <string>

#include "parser/SC3Argument.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"

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
      return expr.toString(true);
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
      return "FarLabelRef(" + expr.toString(true) + ", " + std::to_string(id) +
             ")";
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
      return "FlagRef(" + expr.toString(true) + ")";
      break;
    }
    case ExprGlobalVarRef: {
      const auto &expr = arg.exprValue;
      return "GlobalVarRef(" + expr.toString(true) + ")";
      break;
    }
    case ExprThreadVarRef: {
      const auto &expr = arg.exprValue;
      return "ThreadVarRef(" + expr.toString(true) + ")";
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