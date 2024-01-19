// SCXParser.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
//#include <experimental>
#include <filesystem>
#include "parser/SC3Argument.h"
#include "parser/CCDisassembler.h"
#include "parser/SGHDDisassembler.h"
#include "parser/ZeroDisassembler.h"
#include "parser/RNEDisassembler.h"
#include "parser/SC3StringDecoder.h"
#include "parser/SGHDCharset.h"
#include "parser/CCCharset.h"
#include "parser/RNECharset.h"
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

std::string GetFirstSC3String(const std::vector<std::string> &stringTable,
                              const SC3Instruction *inst) {
  for (const auto &arg : inst->args()) {
    if (arg.type == StringRef) {
      if (arg.uint16_value < stringTable.size())
        return "\t; " + stringTable[arg.uint16_value];
      return "";
    }
  }
  return "";
}

void* chooseDisassembler(std::string game, SCXFile& file)
{
    if (game == "CC")
    {
        return new CCDisassembler(file);
    }
    else if (game == "SG0")
    {
        return new ZeroDisassembler(file);
    }
    else if (game == "SGHD")
    {
        return new SGHDDisassembler(file);
    }
    else if (game == "RNE")
    {
        return new RNEDisassembler(file);
    }
    else
    {
        std::cout<<"Game not supported or spelling mistake. Remember capslock";
        return nullptr;
    }
}

SC3StringDecoder* chooseStringDec(std::string game, SCXFile& file)
{
    if (game == "CC")
    {
        return new SC3StringDecoder(file,CCCharset);
    }
    else if (game == "SG0")
    {
        return new SC3StringDecoder(file,CCCharset);
    }
    else if (game == "SGHD")
    {
        return new SC3StringDecoder(file,SGHDCharset);
    }
    else if (game == "RNE")
    {
        return new SC3StringDecoder(file,RNECharset);
    }
    else
    {
        std::cout<<"Game not supported or spelling mistake. Remember capslock";
        return nullptr;
    }
}

int main() {
  std::vector<std::pair<uint8_t *, std::streamsize>> files;

  std::string path = "/path/to/gamescript";
  std::string game = "CC";
  std::cout<<"Please specify the path of the game scripts (folder with all .scx files)" << std::endl;
  std::cin>>path;
  std::cout<<"Please specify the game!\n CC, RNE, SG0 or SGHD"<<std::endl;
  std::cin>>game;
  if (game != "CC" && game != "SGHD" && game != "RNE" && game != "SG0") {
      std::cout << "Game not supported or spelling mistake. Remember capslock";
      return 0;
  }

  int fileId = 0;
  for (auto &p : std::filesystem::directory_iterator(path)) {
    //std::cout << p.path().string()<< std::endl;
    std::string filext = p.path().extension().string();
    if ((filext != ".scx") && (filext != ".SCX")) continue;

    std::ifstream file(p.path(), std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    uint8_t *buf = (uint8_t *)malloc(size);
    file.seekg(0, std::ios::beg);
    file.read((char *)buf, size);
    file.close();

    std::string outPath = p.path().string() + ".txt";
    SCXFile scx(buf, size, p.path().filename().string(), fileId++);
    //SGHDDisassembler dis(scx);
    void* dis = chooseDisassembler(game,scx);
    if (dis != nullptr) {
        if (game == "SGHD")
            static_cast<SGHDDisassembler*>(dis)->DisassembleFile();
        if (game == "SG0")
            static_cast<ZeroDisassembler*>(dis)->DisassembleFile();
        if (game == "CC")
            static_cast<CCDisassembler*>(dis)->DisassembleFile();
        if (game == "RNE")
            static_cast<RNEDisassembler*>(dis)->DisassembleFile();
    }
    else
        return 0;
    SC3StringDecoder* strdec = chooseStringDec(game,scx);
    const std::vector<std::string> stringTable =
        strdec->decodeStringTableToUtf8();

    std::ofstream outFile(outPath,
                          std::ios::out | std::ios::trunc | std::ios::binary);
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
          outFile << GetFirstSC3String(stringTable, inst.get());
        }
        outFile << "\n";
      }
    }
    outFile.close();
    free(dis);
    free(strdec);
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
