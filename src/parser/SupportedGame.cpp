#include "SupportedGame.h"
#include "CCCharset.h"
#include "RNECharset.h"
#include "CCDisassembler.h"
#include "RNEDisassembler.h"

SC3StringDecoder* SupportedGame::createStringDecoder(SCXFile& file) const {
  return new SC3StringDecoder(file, charset());
}

SC3BaseDisassembler* CCGame::createDisassembler(SCXFile& file) const {
  return new CCDisassembler(file);
}
int CCGame::id() const { return 0; }
const std::string CCGame::name() const { return "Chaos;Child"; }
const std::vector<std::string>& CCGame::charset() const { return CCCharset; }

SC3BaseDisassembler* RNEGame::createDisassembler(SCXFile& file) const {
  return new RNEDisassembler(file);
}
int RNEGame::id() const { return 1; }
const std::string RNEGame::name() const { return "Robotics;Notes Elite"; }
const std::vector<std::string>& RNEGame::charset() const { return RNECharset; }

const std::vector<SupportedGame*> SupportedGames{new CCGame(), new RNEGame()};