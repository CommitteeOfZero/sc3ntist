#pragma once
#include <string>
#include <vector>
#include "SC3Expression.h"
#include "SCXFile.h"

enum SC3ArgumentType {
  ByteArray,
  Byte,
  UInt16,
  Expression,
  LocalLabel,
  FarLabel,
  ReturnAddress,
  StringRef,
  ExprFlagRef,
  ExprGlobalVarRef,
  ExprThreadVarRef
};

class SC3Argument {
 public:
  virtual ~SC3Argument(){};
  virtual SC3ArgumentType type() const = 0;
  const std::string& name() const { return _name; }

 protected:
  SC3Argument(const std::string& name) : _name(name){};
  const std::string _name;
};

class SC3ArgByteArray : public SC3Argument {
 public:
  ~SC3ArgByteArray(){};
  SC3ArgByteArray(const std::string& name, const std::vector<uint8_t>& data)
      : SC3Argument(name), _data(data){};
  SC3ArgumentType type() const override { return ByteArray; }
  const std::vector<uint8_t>& data() const { return _data; }

 private:
  const std::vector<uint8_t> _data;
};

class SC3ArgByte : public SC3Argument {
 public:
  ~SC3ArgByte(){};
  SC3ArgByte(const std::string& name, const uint8_t value)
      : SC3Argument(name), _value(value){};
  SC3ArgumentType type() const override { return Byte; }
  uint8_t value() const { return _value; }

 private:
  const uint8_t _value;
};

class SC3ArgUInt16 : public SC3Argument {
 public:
  ~SC3ArgUInt16(){};
  SC3ArgUInt16(const std::string& name, const uint16_t value)
      : SC3Argument(name), _value(value){};
  SC3ArgumentType type() const override { return UInt16; }
  uint16_t value() const { return _value; }

 private:
  const uint16_t _value;
};

class SC3ArgExpression : public SC3Argument {
 public:
  ~SC3ArgExpression(){};
  SC3ArgExpression(const std::string& name, uint8_t* rawExpression)
      : SC3Argument(name), _expr(rawExpression){};
  SC3ArgExpression(const std::string& name, const SC3Expression& expr)
      : SC3Argument(name), _expr(expr){};
  SC3ArgumentType type() const override { return Expression; }
  const SC3Expression& expr() const { return _expr; }

 private:
  const SC3Expression _expr;
};

class SC3ArgLocalLabel : public SC3Argument {
 public:
  ~SC3ArgLocalLabel(){};
  SC3ArgLocalLabel(const std::string& name, const SCXTableIndex id)
      : SC3Argument(name), _id(id){};
  SC3ArgumentType type() const override { return LocalLabel; }
  SCXTableIndex id() const { return _id; }

 private:
  const SCXTableIndex _id;
};

class SC3ArgFarLabel : public SC3Argument {
 public:
  ~SC3ArgFarLabel(){};
  SC3ArgFarLabel(const std::string& name, uint8_t* scriptBufferIdExpr,
                 const SCXTableIndex labelId)
      : SC3Argument(name),
        _scriptBufferId(scriptBufferIdExpr),
        _labelId(labelId){};
  SC3ArgFarLabel(const std::string& name,
                 const SC3Expression& scriptBufferIdExpr,
                 const SCXTableIndex labelId)
      : SC3Argument(name),
        _scriptBufferId(scriptBufferIdExpr),
        _labelId(labelId){};
  SC3ArgumentType type() const override { return FarLabel; }
  const SC3Expression& scriptBufferId() const { return _scriptBufferId; }
  SCXTableIndex labelId() const { return _labelId; }

 private:
  const SC3Expression _scriptBufferId;
  const SCXTableIndex _labelId;
};

class SC3ArgReturnAddress : public SC3Argument {
 public:
  ~SC3ArgReturnAddress(){};
  SC3ArgReturnAddress(const std::string& name, const SCXTableIndex id)
      : SC3Argument(name), _id(id){};
  SC3ArgumentType type() const override { return ReturnAddress; }
  SCXTableIndex id() const { return _id; }

 private:
  const SCXTableIndex _id;
};

class SC3ArgStringRef : public SC3Argument {
 public:
  ~SC3ArgStringRef(){};
  SC3ArgStringRef(const std::string& name, const SCXTableIndex id)
      : SC3Argument(name), _id(id){};
  SC3ArgumentType type() const override { return StringRef; }
  SCXTableIndex id() const { return _id; }

 private:
  const SCXTableIndex _id;
};

class SC3ArgExprFlagRef : public SC3Argument {
 public:
  ~SC3ArgExprFlagRef(){};
  SC3ArgExprFlagRef(const std::string& name, uint8_t* rawExpression)
      : SC3Argument(name), _expr(rawExpression){};
  SC3ArgExprFlagRef(const std::string& name, const SC3Expression& expr)
      : SC3Argument(name), _expr(expr){};
  SC3ArgumentType type() const override { return ExprFlagRef; }
  const SC3Expression& expr() const { return _expr; }

 private:
  const SC3Expression _expr;
};

class SC3ArgExprGlobalVarRef : public SC3Argument {
 public:
  ~SC3ArgExprGlobalVarRef(){};
  SC3ArgExprGlobalVarRef(const std::string& name, uint8_t* rawExpression)
      : SC3Argument(name), _expr(rawExpression){};
  SC3ArgExprGlobalVarRef(const std::string& name, const SC3Expression& expr)
      : SC3Argument(name), _expr(expr){};
  SC3ArgumentType type() const override { return ExprGlobalVarRef; }
  const SC3Expression& expr() const { return _expr; }

 private:
  const SC3Expression _expr;
};

class SC3ArgExprThreadVarRef : public SC3Argument {
public:
	~SC3ArgExprThreadVarRef() {};
	SC3ArgExprThreadVarRef(const std::string& name, uint8_t* rawExpression)
		: SC3Argument(name), _expr(rawExpression) {};
	SC3ArgExprThreadVarRef(const std::string& name, const SC3Expression& expr)
		: SC3Argument(name), _expr(expr) {};
	SC3ArgumentType type() const override { return ExprThreadVarRef; }
	const SC3Expression& expr() const { return _expr; }

private:
	const SC3Expression _expr;
};