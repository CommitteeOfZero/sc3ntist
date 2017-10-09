#pragma once
#include <vector>
#include "SC3Argument.h"
#include "SCXFile.h"

class SC3Instruction {
 public:
  ~SC3Instruction() {
    for (auto& it = _args.begin(); it != _args.end(); ++it) {
      delete *it;
    }
  }

  SC3Instruction(SCXOffset position, SCXOffset length,
                 const std::string& name,
                 const std::vector<SC3Argument*>& args)
      : _position(position),
        _length(length),
        _name(name),
        _args(args){};

  const SCXOffset position() const { return _position; }
  const SCXOffset length() const { return _length; }
  const std::string& name() const { return _name; }
  const std::vector<SC3Argument*> args() const { return _args; }

 private:
  const SCXOffset _position;
  const SCXOffset _length;
  const std::string _name;
  const std::vector<SC3Argument*> _args;
};