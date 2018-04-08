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
  std::vector<std::unique_ptr<SC3Argument>> args;

#define RETURN_UNRECOGNIZED() \
  { return Inst__Unrecognized__(dataStart, address, maxLength); }

#define RETURN_INSTRUCTION(name)                                            \
  {                                                                         \
    if ((data - dataStart) > maxLength) RETURN_UNRECOGNIZED();              \
    return new SC3Instruction(address, (SCXOffset)(data - dataStart), name, \
                              args);                                        \
  }

#define ADD_BYTE_ARG(name)                                            \
  {                                                                   \
    args.push_back(                                                   \
        std::unique_ptr<SC3Argument>(new SC3ArgByte(name, *data++))); \
  }
#define ADD_UINT16_ARG(name)                        \
  {                                                 \
    args.push_back(std::unique_ptr<SC3Argument>(    \
        new SC3ArgUInt16(name, *(uint16_t*)data))); \
    data += sizeof(uint16_t);                       \
  }
#define ADD_EXPRESSION_ARG(name)                       \
  {                                                    \
    std::unique_ptr<SC3ArgExpression> arg;             \
    try {                                              \
      arg = std::unique_ptr<SC3ArgExpression>(         \
          new SC3ArgExpression(name, (uint8_t*)data)); \
    } catch (...) {                                    \
      RETURN_UNRECOGNIZED();                           \
    }                                                  \
    data += arg->expr().rawLength();                   \
    args.push_back(std::move(arg));                    \
  }
#define ADD_LOCAL_LABEL_ARG(name)                            \
  {                                                          \
    args.push_back(std::unique_ptr<SC3Argument>(             \
        new SC3ArgLocalLabel(name, *(SCXTableIndex*)data))); \
    data += sizeof(SCXTableIndex);                           \
  }
#define ADD_FAR_LABEL_ARG(name)                                    \
  {                                                                \
    try {                                                          \
      SC3Expression expr(data);                                    \
      data += expr.rawLength();                                    \
      args.push_back(std::unique_ptr<SC3Argument>(                 \
          new SC3ArgFarLabel(name, expr, *(SCXTableIndex*)data))); \
      data += sizeof(SCXTableIndex);                               \
    } catch (...) {                                                \
      RETURN_UNRECOGNIZED();                                       \
    }                                                              \
  }

#define ADD_RETURN_ADDRESS_ARG(name)                            \
  {                                                             \
    args.push_back(std::unique_ptr<SC3Argument>(                \
        new SC3ArgReturnAddress(name, *(SCXTableIndex*)data))); \
    data += sizeof(SCXTableIndex);                              \
  }
#define ADD_STRING_REF_ARG(name)                            \
  {                                                         \
    args.push_back(std::unique_ptr<SC3Argument>(            \
        new SC3ArgStringRef(name, *(SCXTableIndex*)data))); \
    data += sizeof(SCXTableIndex);                          \
  }

#define ADD_EXPR_FLAG_REF_ARG(name)                     \
  {                                                     \
    std::unique_ptr<SC3ArgExprFlagRef> arg;             \
    try {                                               \
      arg = std::unique_ptr<SC3ArgExprFlagRef>(         \
          new SC3ArgExprFlagRef(name, (uint8_t*)data)); \
    } catch (...) {                                     \
      RETURN_UNRECOGNIZED();                            \
    }                                                   \
    data += arg->expr().rawLength();                    \
    args.push_back(std::move(arg));                     \
  }
#define ADD_EXPR_GLOBAL_VAR_REF_ARG(name)                    \
  {                                                          \
    std::unique_ptr<SC3ArgExprGlobalVarRef> arg;             \
    try {                                                    \
      arg = std::unique_ptr<SC3ArgExprGlobalVarRef>(         \
          new SC3ArgExprGlobalVarRef(name, (uint8_t*)data)); \
    } catch (...) {                                          \
      RETURN_UNRECOGNIZED();                                 \
    }                                                        \
    data += arg->expr().rawLength();                         \
    args.push_back(std::move(arg));                          \
  }
#define ADD_EXPR_THREAD_VAR_REF_ARG(name)                    \
  {                                                          \
    std::unique_ptr<SC3ArgExprThreadVarRef> arg;             \
    try {                                                    \
      arg = std::unique_ptr<SC3ArgExprThreadVarRef>(         \
          new SC3ArgExprThreadVarRef(name, (uint8_t*)data)); \
    } catch (...) {                                          \
      RETURN_UNRECOGNIZED();                                 \
    }                                                        \
    data += arg->expr().rawLength();                         \
    args.push_back(std::move(arg));                          \
  }

#define NO_ARGS_DECODER_PROC(name)                                           \
  DECODER_PROC(name) {                                                       \
    DECODER_PROC_INIT();                                                     \
    return new SC3Instruction(address, (SCXOffset)(data - dataStart), #name, \
                              args);                                         \
  }

DECODER_PROC(__Unrecognized__) {
  std::vector<std::unique_ptr<SC3Argument>> args;
  args.push_back(std::unique_ptr<SC3Argument>(new SC3ArgByteArray(
      "data", std::vector<uint8_t>(data, data + maxLength))));
  return new SC3Instruction(address, maxLength, "__Unrecognized__", args);
}
