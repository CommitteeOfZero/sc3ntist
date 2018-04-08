// SCXParser.cpp : Defines the entry point for the console application.
//

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#include "parser/SC3Argument.h"
#include "parser/CCDisassembler.h"
#include "parser/SCXFile.h"
#include "parser/SC3CodeBlock.h"

std::string uint8_vector_to_hex_string(const std::vector<uint8_t> &v) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  std::vector<uint8_t>::const_iterator it;

  for (it = v.begin(); it != v.end(); it++) {
    if (it != v.begin()) ss << " ";
    ss << std::setw(2) << static_cast<unsigned>(*it);
  }

  return ss.str();
}

std::string SC3ArgumentToString(const SC3Argument &arg) {
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
      return "LocalLabelRef(" + std::to_string(id) + ")";
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

int main() {
  std::vector<std::pair<uint8_t *, std::streamsize>> files;

  std::string path = "G:\\Games\\SGTL\\CCEnVitaPatch101\\script_dis";

  int fileId = 0;
  for (auto &p : std::experimental::filesystem::directory_iterator(path)) {
    if (p.path().extension().string() != ".scx") continue;

    std::ifstream file(p.path(), std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    uint8_t *buf = (uint8_t *)malloc(size);
    file.seekg(0, std::ios::beg);
    file.read((char *)buf, size);
    file.close();

    std::string outPath = p.path().string() + ".txt";
    SCXFile scx(buf, size, p.path().filename().string(), fileId++);
    CCDisassembler dis(scx);
    dis.DisassembleFile();

    std::ofstream outFile(outPath, std::ios::out | std::ios::trunc);
    int i = 0;
    for (const auto &label : scx.disassembly()) {
      outFile << "\n#label" << i << "_" << label->address() << ":\n";
      i++;
      for (const auto &inst : label->instructions()) {
        if (inst->name() == "Assign") {
          outFile << "\t" << SC3ArgumentToString(inst->args().at(0)) << "\n";
          continue;
        }
        outFile << "\t" << inst->name();
        int i = 0;
        int argCount = inst->args().size();
        if (argCount > 0) {
          outFile << "(";
          for (const auto &arg : inst->args()) {
            i++;
            outFile << arg.name << ": ";
            outFile << SC3ArgumentToString(arg);
            if (i < argCount) outFile << ", ";
          }
          outFile << ")";
        }
        outFile << "\n";
      }
    }
    outFile.close();
  }

#if 0
	int unrecognizedCount = 0;
	/*for (auto &it : dis.code())
	{
		for (auto &inst : it->instructions())
		{
			if (inst->name() == "__Unrecognized__"
				&& inst->args().size() > 0
				&& inst->args().at(0)->type() == SC3ArgumentType::ByteArray
				&& static_cast<SC3ArgByteArray*>(inst->args().at(0))->data().size() > 0)
			{
				uint8_t firstByte = static_cast<SC3ArgByteArray*>(inst->args().at(0))->data().at(0);
				if (firstByte == 0 || firstByte == 1 || firstByte == 0x10)
				{
					unrecognizedCount++;
				}
			}
		}
	}*/
#endif

  return 0;
}
