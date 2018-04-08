#pragma once
#include <vector>
#include <memory>
#include "SCXTypes.h"
#include "SC3Argument.h"

class SC3Instruction {
 public:
  SC3Instruction(SCXOffset position, SCXOffset length, const std::string& name,
                 std::vector<SC3Argument>& args)
      : _position(position),
        _length(length),
        _name(name),
        _args(std::move(args)){};

  const SCXOffset position() const { return _position; }
  const SCXOffset length() const { return _length; }
  const std::string& name() const { return _name; }
  const std::vector<SC3Argument>& args() const { return _args; }

 private:
  const SCXOffset _position;
  const SCXOffset _length;
  const std::string _name;
  const std::vector<SC3Argument> _args;
};