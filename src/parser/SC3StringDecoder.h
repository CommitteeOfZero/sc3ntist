#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "SCXTypes.h"
#include "SCXFile.h"

enum SC3StringTokenType {
  LineBreak = 0x00,
  CharacterNameStart = 0x01,
  DialogueLineStart = 0x02,
  Present = 0x03,
  SetColor = 0x04,
  Present_ResetAlignment = 0x08,
  RubyBaseStart = 0x09,
  RubyTextStart = 0x0A,
  RubyTextEnd = 0x0B,
  SetFontSize = 0x0C,
  PrintInParallel = 0x0E,
  CenterText = 0x0F,
  SetTopMargin = 0x11,
  SetLeftMargin = 0x12,
  GetHardcodedValue = 0x13,
  EvaluateExpression = 0x15,
  Present_0x18 = 0x18,
  AutoForward = 0x19,
  AutoForward_1A = 0x1A,
  Unk_1E = 0x1E,
  AltLineBreak = 0x1F
};

class SC3StringDecoder {
 public:
  SC3StringDecoder(const SCXFile& file, const std::vector<std::string>& charset,
                   bool sc3toolsCompat = false)
      : _file(file), _charset(charset), _sc3toolsCompat(sc3toolsCompat){};

  std::vector<std::string> decodeStringTableToUtf8();

 private:
  const SCXFile& _file;
  const std::vector<std::string> _charset;
  uint8_t *_cursor, *_end;
  bool _sc3toolsCompat;

  bool decodeToken(std::ostream& stream);
  void print(std::ostream& stream, uint16_t value);
  uint16_t readUint16BE();
  std::string exprToString(uintptr_t* remaining);
};