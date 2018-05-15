#pragma once

#include <string>
#include <vector>
#include "SC3BaseDisassembler.h"
#include "SC3StringDecoder.h"

class SupportedGame {
 public:
  virtual ~SupportedGame() {}
  virtual SC3BaseDisassembler *createDisassembler(SCXFile &file) const = 0;
  virtual SC3StringDecoder *createStringDecoder(SCXFile &file) const;
  virtual int id() const = 0;
  virtual const std::string name() const = 0;
  virtual const std::vector<std::string> &charset() const = 0;
};

class CCGame : public SupportedGame {
 public:
  SC3BaseDisassembler *createDisassembler(SCXFile &file) const override;
  int id() const override;
  const std::string name() const override;
  const std::vector<std::string> &charset() const override;
};

class RNEGame : public SupportedGame {
 public:
  SC3BaseDisassembler *createDisassembler(SCXFile &file) const override;
  int id() const override;
  const std::string name() const override;
  const std::vector<std::string> &charset() const override;
};

extern const std::vector<SupportedGame *> SupportedGames;