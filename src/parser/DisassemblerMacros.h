#pragma once

typedef SC3Instruction* (*DecoderProc)(uint8_t* data, SCXOffset address,
                                       SCXOffset maxLength);

#define DECODER_PROC(name)                                            \
  static SC3Instruction* Inst##name(uint8_t* data, SCXOffset address, \
                                    SCXOffset maxLength)

DECODER_PROC(__Unrecognized__);

#define DECODER_PROC_INIT()  \
  uint8_t* dataStart = data; \
  data += 2;                 \
  std::vector<SC3Argument> args;

#define RETURN_UNRECOGNIZED() \
  { return Inst__Unrecognized__(dataStart, address, maxLength); }

#define RETURN_INSTRUCTION(name)                                            \
  {                                                                         \
    if ((data - dataStart) > maxLength) RETURN_UNRECOGNIZED();              \
    return new SC3Instruction(address, (SCXOffset)(data - dataStart), name, \
                              args);                                        \
  }

#define ADD_BYTE_ARG(aname)                    \
  {                                            \
    SC3Argument arg = {SC3ArgumentType::Byte}; \
    arg.byteValue = *data++;                   \
    arg.name = aname;                          \
    args.push_back(std::move(arg));            \
  }
#define ADD_UINT16_ARG(aname)                    \
  {                                              \
    SC3Argument arg = {SC3ArgumentType::UInt16}; \
    arg.uint16_value = *(uint16_t*)data;         \
    data += sizeof(uint16_t);                    \
    arg.name = aname;                            \
    args.push_back(std::move(arg));              \
  }
#define ADD_EXPRESSION_ARG(aname)                      \
  {                                                    \
    try {                                              \
      SC3Argument arg = {SC3ArgumentType::Expression}; \
      arg.exprValue = SC3Expression(data);             \
      arg.name = aname;                                \
      data += arg.exprValue.rawLength();               \
      args.push_back(std::move(arg));                  \
    } catch (...) {                                    \
      RETURN_UNRECOGNIZED();                           \
    }                                                  \
  }
#define ADD_LOCAL_LABEL_ARG(aname)                   \
  {                                                  \
    SC3Argument arg = {SC3ArgumentType::LocalLabel}; \
    arg.uint16_value = *(uint16_t*)data;             \
    data += sizeof(uint16_t);                        \
    arg.name = aname;                                \
    args.push_back(std::move(arg));                  \
  }
#define ADD_FAR_LABEL_ARG(aname)                     \
  {                                                  \
    try {                                            \
      SC3Argument arg = {SC3ArgumentType::FarLabel}; \
      arg.exprValue = SC3Expression(data);           \
      arg.name = aname;                              \
      data += arg.exprValue.rawLength();             \
      arg.uint16_value = *(uint16_t*)data;           \
      data += 2;                                     \
      args.push_back(std::move(arg));                \
    } catch (...) {                                  \
      RETURN_UNRECOGNIZED();                         \
    }                                                \
  }

#define ADD_RETURN_ADDRESS_ARG(aname)                   \
  {                                                     \
    SC3Argument arg = {SC3ArgumentType::ReturnAddress}; \
    arg.uint16_value = *(uint16_t*)data;                \
    data += sizeof(uint16_t);                           \
    arg.name = aname;                                   \
    args.push_back(std::move(arg));                     \
  }
#define ADD_STRING_REF_ARG(aname)                   \
  {                                                 \
    SC3Argument arg = {SC3ArgumentType::StringRef}; \
    arg.uint16_value = *(uint16_t*)data;            \
    data += sizeof(uint16_t);                       \
    arg.name = aname;                               \
    args.push_back(std::move(arg));                 \
  }

#define ADD_EXPR_FLAG_REF_ARG(aname)                    \
  {                                                     \
    try {                                               \
      SC3Argument arg = {SC3ArgumentType::ExprFlagRef}; \
      arg.exprValue = SC3Expression(data);              \
      arg.name = aname;                                 \
      data += arg.exprValue.rawLength();                \
      args.push_back(std::move(arg));                   \
    } catch (...) {                                     \
      RETURN_UNRECOGNIZED();                            \
    }                                                   \
  }
#define ADD_EXPR_GLOBAL_VAR_REF_ARG(aname)                   \
  {                                                          \
    try {                                                    \
      SC3Argument arg = {SC3ArgumentType::ExprGlobalVarRef}; \
      arg.exprValue = SC3Expression(data);                   \
      arg.name = aname;                                      \
      data += arg.exprValue.rawLength();                     \
      args.push_back(std::move(arg));                        \
    } catch (...) {                                          \
      RETURN_UNRECOGNIZED();                                 \
    }                                                        \
  }
#define ADD_EXPR_THREAD_VAR_REF_ARG(aname)                   \
  {                                                          \
    try {                                                    \
      SC3Argument arg = {SC3ArgumentType::ExprThreadVarRef}; \
      arg.exprValue = SC3Expression(data);                   \
      arg.name = aname;                                      \
      data += arg.exprValue.rawLength();                     \
      args.push_back(std::move(arg));                        \
    } catch (...) {                                          \
      RETURN_UNRECOGNIZED();                                 \
    }                                                        \
  }

#define NO_ARGS_DECODER_PROC(name)                                           \
  DECODER_PROC(name) {                                                       \
    DECODER_PROC_INIT();                                                     \
    return new SC3Instruction(address, (SCXOffset)(data - dataStart), #name, \
                              args);                                         \
  }

DECODER_PROC(__Unrecognized__) {
  std::vector<SC3Argument> args;
  SC3Argument arg = {SC3ArgumentType::ByteArray};
  arg.name = "data";
  arg.byteArrayValue = std::vector<uint8_t>(data, data + maxLength);
  args.push_back(std::move(arg));
  return new SC3Instruction(address, maxLength, "__Unrecognized__", args);
}
