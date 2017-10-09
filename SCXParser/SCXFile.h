#pragma once
#include <cstdint>

typedef int32_t SCXOffset;
typedef uint16_t SCXTableIndex;

class SCXFile {
 public:
  SCXFile(uint8_t* data, SCXOffset length);
  SCXOffset getStringOffset(const SCXTableIndex stringId) const;
  SCXOffset getReturnAddressOffset(const SCXTableIndex returnAddressId) const;
  SCXOffset getLabelOffset(const SCXTableIndex labelId) const;
  void* getPString(const SCXTableIndex stringId) const;
  void* getPReturnAddress(const SCXTableIndex returnAddressId) const;
  void* getPLabel(const SCXTableIndex labelId) const;
  uint8_t* getPData() const { return _data; }
  SCXOffset getLength() const { return _length; }
  SCXOffset getCodeEndOffset() const { return _codeEndOffset; }
  SCXTableIndex getStringCount() const { return _stringCount; }
  SCXTableIndex getReturnAddressCount() const { return _returnAddressCount; }
  SCXTableIndex getLabelCount() const { return _labelCount; }
  ~SCXFile();

 private:
  uint8_t* _data;
  SCXOffset _length;
  SCXOffset _codeEndOffset;
  SCXTableIndex _stringCount;
  SCXTableIndex _returnAddressCount;
  SCXTableIndex _labelCount;
  SCXOffset* _stringTable;
  SCXOffset* _returnAddressTable;
  SCXOffset* _labelTable;

  void parseHeader();

  static const SCXOffset SCXStringTableOffsetOffset = 4;
  static const SCXOffset SCXReturnAddressTableOffsetOffset =
      SCXStringTableOffsetOffset + sizeof(SCXOffset);
  static const SCXOffset SCXLabelTableOffset =
      SCXReturnAddressTableOffsetOffset + sizeof(SCXOffset);
};