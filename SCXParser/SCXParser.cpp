// SCXParser.cpp : Defines the entry point for the console application.
//

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#include "SC3Argument.h"
#include "CCDisassembler.h"
#include "SCXFile.h"

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

std::string SC3ArgumentToString(SC3Argument *arg) {
  switch (arg->type()) {
    case ByteArray: {
      auto data = static_cast<SC3ArgByteArray *>(arg)->data();
      return uint8_vector_to_hex_string(data);
      break;
    }
    case Byte: {
      auto value = static_cast<SC3ArgByte *>(arg)->value();
      return std::to_string((unsigned)value);
      break;
    }
    case UInt16: {
      auto value = static_cast<SC3ArgUInt16 *>(arg)->value();
      return std::to_string((unsigned)value);
      break;
    }
    case Expression: {
      const auto &expr = static_cast<SC3ArgExpression *>(arg)->expr();
      return expr.toString();
      break;
    }
    case LocalLabel: {
      auto id = static_cast<SC3ArgLocalLabel *>(arg)->id();
      return "LocalLabelRef(" + std::to_string(id) + ")";
      break;
    }
    case FarLabel: {
      const auto &expr = static_cast<SC3ArgFarLabel *>(arg)->scriptBufferId();
      auto id = static_cast<SC3ArgFarLabel *>(arg)->labelId();
      return "FarLabelRef(" + expr.toString() + ", " + std::to_string(id) + ")";
    }
    case ReturnAddress: {
      auto id = static_cast<SC3ArgReturnAddress *>(arg)->id();
      return "ReturnAddressRef(" + std::to_string(id) + ")";
      break;
    }
    case StringRef: {
      auto id = static_cast<SC3ArgStringRef *>(arg)->id();
      return "StringRef(" + std::to_string(id) + ")";
      break;
    }
    case ExprFlagRef: {
      const auto &expr = static_cast<SC3ArgExprFlagRef *>(arg)->expr();
      return "FlagRef(" + expr.toString() + ")";
      break;
    }
	case ExprGlobalVarRef: {
		const auto &expr = static_cast<SC3ArgExprGlobalVarRef *>(arg)->expr();
		return "GlobalVarRef(" + expr.toString() + ")";
		break;
	}
	case ExprThreadVarRef: {
		const auto &expr = static_cast<SC3ArgExprThreadVarRef *>(arg)->expr();
		return "ThreadVarRef(" + expr.toString() + ")";
		break;
	}
  }
  return "unrecognized";
}

int main() {
  std::vector<std::pair<uint8_t *, std::streamsize>> files;

  std::string path = "G:\\Games\\SGTL\\cc_work\\orig_script";
  for (auto &p : std::experimental::filesystem::directory_iterator(path)) {
	  if (p.path().extension().string() != ".scx") continue;

    std::ifstream file(p.path(), std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    uint8_t *buf = (uint8_t *)malloc(size);
    file.seekg(0, std::ios::beg);
    file.read((char *)buf, size);
    file.close();

    std::string outPath = p.path().string() + ".txt";
    SCXFile scx(buf, size);
    CCDisassembler dis(scx);
    dis.DisassembleFile();

    std::ofstream outFile(outPath, std::ios::out | std::ios::trunc);
    for (const auto &label : dis.code()) {
      outFile << "\n#label_" << label->address() << ":\n";
      for (const auto &inst : label->instructions()) {
        if (inst->name() == "Assign") {
          outFile << "\t" << SC3ArgumentToString(inst->args().at(0)) << "\n";
          continue;
        }
		outFile << "\t" << inst->name();
        int i = 0;
        int argCount = inst->args().size();
		if (argCount > 0)
		{
			outFile << "(";
			for (const auto &arg : inst->args()) {
				i++;
				outFile << arg->name() << ": ";
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
