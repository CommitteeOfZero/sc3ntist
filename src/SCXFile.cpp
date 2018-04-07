#include "SCXFile.h"
#include <malloc.h>

SCXFile::SCXFile(uint8_t * data, SCXOffset length) :
	_data(data),
	_length(length)
{
	parseHeader();
}

SCXOffset SCXFile::getStringOffset(const SCXTableIndex stringId) const
{
	return _stringTable[stringId];
}

SCXOffset SCXFile::getReturnAddressOffset(const SCXTableIndex returnAddressId) const
{
	return _returnAddressTable[returnAddressId];
}

SCXOffset SCXFile::getLabelOffset(const SCXTableIndex labelId) const
{
	return _labelTable[labelId];
}

SCXFile::~SCXFile()
{
	free(_data);
}

void SCXFile::appendLabel(SC3CodeBlock * label)
{
  _disassembly.push_back(std::unique_ptr<SC3CodeBlock>(label));
}

void SCXFile::parseHeader()
{
	// TODO exception on wrong magic
	SCXOffset stringTableOffset = *(SCXOffset*)(_data + SCXStringTableOffsetOffset);
	_codeEndOffset = stringTableOffset;
	_stringTable = (SCXOffset*)(_data + stringTableOffset);
	SCXOffset returnAddressTableOffset = *(SCXOffset*)(_data + SCXReturnAddressTableOffsetOffset);
	_returnAddressTable = (SCXOffset*)(_data + returnAddressTableOffset);
	_labelTable = (SCXOffset*)(_data + SCXLabelTableOffset);

	_stringCount = (SCXTableIndex)((returnAddressTableOffset - stringTableOffset) / sizeof(SCXOffset));
	if (_stringCount > 0)
	{
		SCXOffset firstStringOffset = _stringTable[0];
		_returnAddressCount = (SCXTableIndex)((firstStringOffset - returnAddressTableOffset) / sizeof(SCXOffset));
	}
	else
	{
		SCXOffset firstReturnAddressOffset = _returnAddressTable[0];
		_returnAddressCount = (SCXTableIndex)((firstReturnAddressOffset - returnAddressTableOffset) / sizeof(SCXOffset));
	}
	_labelCount = (SCXTableIndex)((_labelTable[0] - SCXLabelTableOffset) / sizeof(SCXOffset));
}

void * SCXFile::getPString(const SCXTableIndex stringId) const
{
	// TODO bounds checking
	return (void *)(_data + _stringTable[stringId]);
}

void * SCXFile::getPReturnAddress(const SCXTableIndex returnAddressId) const
{
	// TODO bounds checking
	return (void *)(_data + _returnAddressTable[returnAddressId]);
}

void * SCXFile::getPLabel(const SCXTableIndex labelId) const
{
	// TODO bounds checking
	return (void *)(_data + _labelTable[labelId]);
}
