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
  std::vector<SC3Argument*> args;

#define RETURN_UNRECOGNIZED()                                   \
  {                                                             \
    for (auto& it = args.begin(); it != args.end(); ++it) {     \
      delete *it;                                               \
    }                                                           \
    return Inst__Unrecognized__(dataStart, address, maxLength); \
  }

#define RETURN_INSTRUCTION(name)                                            \
  {                                                                         \
    if ((data - dataStart) > maxLength) RETURN_UNRECOGNIZED();              \
    return new SC3Instruction(address, (SCXOffset)(data - dataStart), name, \
                              args);                                        \
  }

#define ADD_BYTE_ARG(name)                                          \
  {                                                                 \
    SC3Argument* arg = (SC3Argument*)new SC3ArgByte(name, *data++); \
    args.push_back(arg);                                            \
  }
#define ADD_UINT16_ARG(name)                                                   \
  {                                                                            \
    SC3Argument* arg = (SC3Argument*)new SC3ArgUInt16(name, *(uint16_t*)data); \
    data += sizeof(uint16_t);                                                  \
    args.push_back(arg);                                                       \
  }
#define ADD_EXPRESSION_ARG(name)                        \
  {                                                     \
    SC3ArgExpression* arg;                              \
    try {                                               \
      arg = new SC3ArgExpression(name, (uint8_t*)data); \
    } catch (...) {                                     \
      RETURN_UNRECOGNIZED();                            \
    }                                                   \
    data += arg->expr().rawLength();                    \
    args.push_back((SC3Argument*)arg);                  \
  }
#define ADD_LOCAL_LABEL_ARG(name)                                        \
  {                                                                      \
    SC3Argument* arg =                                                   \
        (SC3Argument*)new SC3ArgLocalLabel(name, *(SCXTableIndex*)data); \
    data += sizeof(SCXTableIndex);                                       \
    args.push_back(arg);                                                 \
  }
#define ADD_FAR_LABEL_ARG(name)                                                \
  {                                                                            \
    try {                                                                      \
      SC3Expression expr(data);                                                \
      data += expr.rawLength();                                                \
      SC3Argument* arg =                                                       \
          (SC3Argument*)new SC3ArgFarLabel(name, expr, *(SCXTableIndex*)data); \
      data += sizeof(SCXTableIndex);                                           \
      args.push_back(arg);                                                     \
    } catch (...) {                                                            \
      RETURN_UNRECOGNIZED();                                                   \
    }                                                                          \
  }

#define ADD_RETURN_ADDRESS_ARG(name)                                        \
  {                                                                         \
    SC3Argument* arg =                                                      \
        (SC3Argument*)new SC3ArgReturnAddress(name, *(SCXTableIndex*)data); \
    data += sizeof(SCXTableIndex);                                          \
    args.push_back(arg);                                                    \
  }
#define ADD_STRING_REF_ARG(name)                                        \
  {                                                                     \
    SC3Argument* arg =                                                  \
        (SC3Argument*)new SC3ArgStringRef(name, *(SCXTableIndex*)data); \
    data += sizeof(SCXTableIndex);                                      \
    args.push_back(arg);                                                \
  }
#define ADD_EXPR_FLAG_REF_ARG(name)                      \
  {                                                      \
    SC3ArgExprFlagRef* arg;                              \
    try {                                                \
      arg = new SC3ArgExprFlagRef(name, (uint8_t*)data); \
    } catch (...) {                                      \
      RETURN_UNRECOGNIZED();                             \
    }                                                    \
    data += arg->expr().rawLength();                     \
    args.push_back((SC3Argument*)arg);                   \
  }
#define ADD_EXPR_GLOBAL_VAR_REF_ARG(name)                     \
  {                                                           \
    SC3ArgExprGlobalVarRef* arg;                              \
    try {                                                     \
      arg = new SC3ArgExprGlobalVarRef(name, (uint8_t*)data); \
    } catch (...) {                                           \
      RETURN_UNRECOGNIZED();                                  \
    }                                                         \
    data += arg->expr().rawLength();                          \
    args.push_back((SC3Argument*)arg);                        \
  }
#define ADD_EXPR_THREAD_VAR_REF_ARG(name)                     \
  {                                                           \
    SC3ArgExprThreadVarRef* arg;                              \
    try {                                                     \
      arg = new SC3ArgExprThreadVarRef(name, (uint8_t*)data); \
    } catch (...) {                                           \
      RETURN_UNRECOGNIZED();                                  \
    }                                                         \
    data += arg->expr().rawLength();                          \
    args.push_back((SC3Argument*)arg);                        \
  }

#define NO_ARGS_DECODER_PROC(name)                                           \
  DECODER_PROC(name) {                                                       \
    DECODER_PROC_INIT();                                                     \
    return new SC3Instruction(address, (SCXOffset)(data - dataStart), #name, \
                              args);                                         \
  }

DECODER_PROC(__Unrecognized__) {
  std::vector<SC3Argument*> args = {(SC3Argument*)new SC3ArgByteArray(
      "data", std::vector<uint8_t>(data, data + maxLength))};
  return new SC3Instruction(address, maxLength, "__Unrecognized__", args);
}