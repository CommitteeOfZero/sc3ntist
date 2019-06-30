#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include "CommonDisassembler.inl"
#include "RNEDisassembler.h"

DECODER_PROC(Save) {
  DECODER_PROC_INIT();
  uint8_t arg = *data++;
  { args.push_back(SC3ArgumentFromByte("arg", arg)); }
  if (arg == 40) {
    ADD_EXPRESSION_ARG("arg2");
    ADD_EXPRESSION_ARG("arg3");
    ADD_EXPRESSION_ARG("arg4");
    ADD_EXPRESSION_ARG("arg5");
  } else if (arg == 82) {
    ADD_EXPRESSION_ARG("arg2");
  }
  RETURN_INSTRUCTION("Save");
}

I1(VoiceTableLoadMaybe, EXPRESSION, "arg1")
I0(SignIn)
I0(Unk0041)

DECODER_PROC(SystemMes) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  switch (type) {
    case 0:
    case 1:
    case 5:
    case 6:
    case 7:
    case 8:
    case 20:
      RETURN_INSTRUCTION("SystemMes");
      break;
    case 2:
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("SystemMes");
      break;
    case 3:
    case 4:
      ADD_STRING_REF_ARG("arg2");
      RETURN_INSTRUCTION("SystemMes");
      break;
    default:
      RETURN_UNRECOGNIZED();
  }
}

DECODER_PROC(PressStart) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("PressStart_0");
      break;
    case 1:
      RETURN_INSTRUCTION("PressStart_1");
      break;
    case 2:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("PressStart_2");
      break;
    case 3:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("PressStart_3");
      break;
    case 4:
      RETURN_INSTRUCTION("PressStart_4");
      break;
    case 5:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("PressStart_5");
      break;
    case 6:
      RETURN_INSTRUCTION("PressStart_6");
      break;
    case 7:
      RETURN_INSTRUCTION("PressStart_7");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I1(GlobalSystemMessage, EXPRESSION, "arg1")
I1(Unk0052, EXPRESSION, "arg1")
I2(Unk0053, EXPRESSION, "arg1", EXPRESSION, "arg2")
I2(Unk0054, EXPRESSION, "arg1", EXPRESSION, "arg2")
I2(Unk0055, BYTE, "arg1", EXPR_FLAG_REF, "arg2")

DECODER_PROC(CreateSurf) {
  DECODER_PROC_INIT();
  uint8_t format = *data++;
  { args.push_back(SC3ArgumentFromByte("format", format)); }
  ADD_EXPRESSION_ARG("surfaceId");
  if (format == 10) {
    ADD_EXPRESSION_ARG("arg2");
  }
  ADD_EXPRESSION_ARG("width");
  ADD_EXPRESSION_ARG("height");
  RETURN_INSTRUCTION("CreateSurf");
}

DECODER_PROC(MesSync) {
  DECODER_PROC_INIT();
  uint8_t arg1 = *data++;
  { args.push_back(SC3ArgumentFromByte("arg1", arg1)); }
  switch (arg1) {
    case 10:
      ADD_EXPRESSION_ARG("arg2");
      break;
  }
  RETURN_INSTRUCTION("MesSync");
}

I0(Unk011F)
I0(Unk012D)

DECODER_PROC(SaveMenu) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_BYTE_ARG("arg1");
      RETURN_INSTRUCTION("SaveMenuInit");
      break;
    case 1:
      RETURN_INSTRUCTION("SaveMenuMain");
      break;
    case 2:
      RETURN_INSTRUCTION("SaveMenu_SaveResetThumnail");
      break;
    case 10:
      RETURN_INSTRUCTION("SaveMenu_Unk10");
      break;
    case 11:
      RETURN_INSTRUCTION("SaveMenu_Unk11");
      break;
    case 12:
      RETURN_INSTRUCTION("SaveMenu_Unk12");
      break;
    case 13:
      RETURN_INSTRUCTION("SaveMenu_Unk13");
      break;
    case 20:
      RETURN_INSTRUCTION("SaveMenu_Unk20");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(LoadData) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  switch (type) {
    case 0:
    case 10:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("LoadData");
      break;
    case 1:
    case 2:
    case 11:
      RETURN_INSTRUCTION("LoadData");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(Unk1031) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_LOCAL_LABEL_ARG("label1");
      ADD_LOCAL_LABEL_ARG("label2");
      RETURN_INSTRUCTION("Unk1031_00");
      break;
    case 1:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("Unk1031_01");
      break;
    case 2:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("Unk1031_02");
      break;
    case 3:
      ADD_EXPRESSION_ARG("arg1");
      ADD_LOCAL_LABEL_ARG("label1");
      RETURN_INSTRUCTION("Unk1031_03");
      break;
    case 4:
      ADD_EXPRESSION_ARG("arg1");
      ADD_LOCAL_LABEL_ARG("label1");
      RETURN_INSTRUCTION("Unk1031_04");
      break;
    case 5:
      ADD_EXPRESSION_ARG("arg1");
      ADD_LOCAL_LABEL_ARG("label1");
      RETURN_INSTRUCTION("Unk1031_05");
      break;
    case 6:
      ADD_EXPRESSION_ARG("arg1");
      ADD_LOCAL_LABEL_ARG("label1");
      RETURN_INSTRUCTION("Unk1031_06");
      break;
    case 7:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("Unk1031_07");
      break;
    case 8:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("Unk1031_08");
      break;
    case 9:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("Unk1031_09");
      break;
    case 10:
      RETURN_INSTRUCTION("Unk1031_10");
      break;
    case 11:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      RETURN_INSTRUCTION("Unk1031_11");
      break;
    case 12:
      RETURN_INSTRUCTION("Unk1031_12");
      break;
    case 13:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("Unk1031_13");
      break;
    case 14:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("Unk1031_14");
      break;
    case 15:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      RETURN_INSTRUCTION("Unk1031_15");
      break;
    case 20:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      ADD_EXPRESSION_ARG("arg6");
      ADD_EXPRESSION_ARG("arg7");
      ADD_EXPRESSION_ARG("arg8");
      ADD_EXPRESSION_ARG("arg9");
      ADD_EXPRESSION_ARG("arg10");
      ADD_EXPRESSION_ARG("arg11");
      ADD_EXPRESSION_ARG("arg12");
      RETURN_INSTRUCTION("Unk1031_20");
      break;
    case 21:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("Unk1031_21");
      break;
    case 22:
      RETURN_INSTRUCTION("Unk1031_22");
      break;
  }
  RETURN_INSTRUCTION("Unk1031");
}

DECODER_PROC(NameID) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("NameID_00");
      break;
    case 1:
      ADD_LOCAL_LABEL_ARG("namePlateDataBlock");
      RETURN_INSTRUCTION("NameID_01");
      break;
    case 2:
      RETURN_INSTRUCTION("NameID_02");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I3(Unk1037, EXPRESSION, "arg1", EXPRESSION, "arg2", EXPRESSION, "arg3")

DECODER_PROC(Unk1038) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("Unk1038_00");
      break;
    case 1:
      RETURN_INSTRUCTION("Unk1038_01");
      break;
    case 2:
      RETURN_INSTRUCTION("Unk1038_02");
      break;
    case 3:
      ADD_LOCAL_LABEL_ARG("label1");
      ADD_LOCAL_LABEL_ARG("label2");
      ADD_LOCAL_LABEL_ARG("label3");
      ADD_LOCAL_LABEL_ARG("label4");
      ADD_LOCAL_LABEL_ARG("label5");
      RETURN_INSTRUCTION("Unk1038_03");
      break;
    case 20:
      RETURN_INSTRUCTION("Unk1038_20");
      break;
    case 21:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      RETURN_INSTRUCTION("Unk1038_21");
      break;
    case 22:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      RETURN_INSTRUCTION("Unk1038_22");
      break;
    case 40:
      RETURN_INSTRUCTION("Unk1038_40");
      break;
    case 50:
      RETURN_INSTRUCTION("Unk1038_50");
      break;
    case 51:
      RETURN_INSTRUCTION("Unk1038_51");
      break;
    case 60:
      RETURN_INSTRUCTION("Unk1038_60");
      break;
    case 61:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("Unk1038_61");
      break;
    case 70:
      RETURN_INSTRUCTION("Unk1038_70");
      break;
    case 71:
      ADD_BYTE_ARG("arg1");
      RETURN_INSTRUCTION("Unk1038_71");
      break;
    case 72:
      ADD_BYTE_ARG("arg1");
      RETURN_INSTRUCTION("Unk1038_72");
      break;
    case 73:
      RETURN_INSTRUCTION("Unk1038_73");
      break;
    case 74:
      RETURN_INSTRUCTION("Unk1038_74");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(Unk1039) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_LOCAL_LABEL_ARG("label1");
      ADD_LOCAL_LABEL_ARG("label2");
      RETURN_INSTRUCTION("Unk1039_00");
      break;
    case 1:
      RETURN_INSTRUCTION("Unk1039_01");
      break;
    case 2:
      ADD_EXPRESSION_ARG("arg1");
      ADD_LOCAL_LABEL_ARG("label1");
      RETURN_INSTRUCTION("Unk1039_02");
      break;
    case 3:
      ADD_EXPRESSION_ARG("arg1");
      ADD_LOCAL_LABEL_ARG("label1");
      RETURN_INSTRUCTION("Unk1039_03");
      break;
    case 4:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("Unk1039_04");
      break;
    case 5:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("Unk1039_05");
      break;
    case 6:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("Unk1039_06");
      break;
    case 7:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("Unk1039_07");
      break;
    case 10:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("Unk1039_10");
      break;
    case 11:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("Unk1039_11");
      break;
  }
  RETURN_INSTRUCTION("Unk1039");
}

DECODER_PROC(Unk103A) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("Unk103A_00");
      break;
    case 1:
      RETURN_INSTRUCTION("Unk103A_01");
      break;
    case 2:
      RETURN_INSTRUCTION("Unk103A_02");
      break;
  }
  RETURN_INSTRUCTION("Unk103A");
}

I3(CHAload3D, EXPRESSION, "bufferId", EXPRESSION, "unk01", EXPRESSION,
   "modelId")
I1(CHArelease3D, EXPRESSION, "bufferId")
I2(Unk0204, EXPRESSION, "arg1", EXPRESSION, "arg2")
I2(CHAswap3DMaybe, EXPRESSION, "dstBufferId", EXPRESSION, "srcBufferId")
I3(CHAplayAnim3DMaybe, EXPRESSION, "bufferId", EXPRESSION, "animationId", BYTE,
   "skipAnimation")
I2(CHAUnk02073D, EXPRESSION, "bufferId", BYTE, "arg2")
I5(Unk0208, EXPRESSION, "arg1", EXPRESSION, "arg2", EXPRESSION, "arg3",
   EXPRESSION, "arg4", EXPRESSION, "arg5")
I2(CHAsetAnim3D, EXPRESSION, "bufferId", EXPRESSION, "animationId")

DECODER_PROC(Unk0210) {
  DECODER_PROC_INIT();
  uint8_t arg1 = *data++;
  { args.push_back(SC3ArgumentFromByte("arg1", arg1)); }
  ADD_EXPRESSION_ARG("arg2");
  if (arg1 & 0x10) {
    ADD_EXPRESSION_ARG("arg3");
    ADD_EXPRESSION_ARG("arg4");
  }
  RETURN_INSTRUCTION("Unk0210");
}

I5(Unk0211, EXPRESSION, "arg1", EXPRESSION, "arg2", EXPRESSION, "arg3",
   EXPRESSION, "arg4", EXPRESSION, "arg5")
I1(Unk0212, EXPRESSION, "arg1")

DECODER_PROC(Unk0213) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("Unk0213_00");
      break;
    case 1:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      RETURN_INSTRUCTION("Unk0213_01");
      break;
    case 2:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      RETURN_INSTRUCTION("Unk0213_02");
      break;
    case 3:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("Unk0213_03");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I3(Unk0214, BYTE, "arg1", EXPRESSION, "arg2", EXPRESSION, "arg3")

DECODER_PROC(Unk0215) {
  DECODER_PROC_INIT();
  ADD_EXPRESSION_ARG("arg1");
  ADD_EXPRESSION_ARG("arg2");
  ADD_EXPRESSION_ARG("arg3");
  ADD_EXPRESSION_ARG("arg4");
  ADD_EXPRESSION_ARG("arg5");
  ADD_EXPRESSION_ARG("arg6");
  ADD_EXPRESSION_ARG("arg7");
  ADD_EXPRESSION_ARG("arg8");
  ADD_EXPRESSION_ARG("arg9");
  ADD_EXPRESSION_ARG("arg10");
  ADD_EXPRESSION_ARG("arg11");
  ADD_EXPRESSION_ARG("arg12");
  ADD_EXPRESSION_ARG("arg13");
  ADD_EXPRESSION_ARG("arg14");
  ADD_EXPRESSION_ARG("arg15");
  ADD_EXPRESSION_ARG("arg16");
  ADD_EXPRESSION_ARG("arg17");
  RETURN_INSTRUCTION("Unk0215");
}

DECODER_PROC(Unk0216) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      ADD_EXPRESSION_ARG("arg6");
      ADD_EXPRESSION_ARG("arg7");
      ADD_EXPRESSION_ARG("arg8");
      RETURN_INSTRUCTION("Unk0216_00");
      break;
    case 1:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("Unk0216_01");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I1(Unk0217, BYTE, "arg1")

DECODER_PROC(Unk0218) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
    case 1:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("Unk0218_00");
      break;
    case 2:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      ADD_EXPRESSION_ARG("arg6");
      RETURN_INSTRUCTION("Unk0218_01");
      break;
    case 3:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      ADD_EXPRESSION_ARG("arg6");
      RETURN_INSTRUCTION("Unk0218_01");
      break;
    case 4:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      ADD_EXPRESSION_ARG("arg6");
      ADD_EXPRESSION_ARG("arg7");
      RETURN_INSTRUCTION("Unk0218_01");
      break;
    default:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("Unk0218_Default");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I1(Unk0219, EXPRESSION, "arg1")

DECODER_PROC(Unk0240) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("Unk0240_00");
      break;
    case 1:
      RETURN_INSTRUCTION("Unk0240_01");
      break;
    case 2:
      RETURN_INSTRUCTION("Unk0240_02");
      break;
    case 3:
      RETURN_INSTRUCTION("Unk0240_03");
      break;
    case 4:
      RETURN_INSTRUCTION("Unk0240_04");
      break;
    case 5:
      RETURN_INSTRUCTION("Unk0240_05");
      break;
    case 6:
      RETURN_INSTRUCTION("Unk0240_06");
      break;
  }
  RETURN_UNRECOGNIZED();
}

static DecoderProc DecoderTableSystem[256] = {
    InstEnd,                  // 00 00
    InstCreateThread,         // 00 01
    InstKillThread,           // 00 02
    InstReset,                // 00 03
    InstScriptLoad,           // 00 04
    InstWait,                 // 00 05
    InstHalt,                 // 00 06
    InstJump,                 // 00 07
    InstJumpTable,            // 00 08
    InstGetLabelAdr,          // 00 09
    InstIf,                   // 00 0A
    InstCall,                 // 00 0B
    InstJumpFar,              // 00 0C
    InstCallFar,              // 00 0D
    InstReturn,               // 00 0E
    InstLoop,                 // 00 0F
    InstFlagOnJump,           // 00 10
    InstFlagOnWait,           // 00 11
    InstSetFlag,              // 00 12
    InstResetFlag,            // 00 13
    InstCopyFlag,             // 00 14
    InstKeyOnJump,            // 00 15
    InstKeyWait,              // 00 16
    InstKeyWaitTimer,         // 00 17
    InstMemberWrite,          // 00 18
    InstThreadControl,        // 00 19
    InstGetSelfPointer,       // 00 1A
    InstLoadJump,             // 00 1B
    InstVsync,                // 00 1C
    InstTest,                 // 00 1D
    InstThreadControlStore,   // 00 1E
    InstSwitch,               // 00 1F
    InstCase,                 // 00 20
    InstBGMplay,              // 00 21
    InstBGMstop,              // 00 22
    InstSEplay,               // 00 23
    InstSEstop,               // 00 24
    InstPadAct,               // 00 25
    InstSSEplay,              // 00 26
    InstSSEstop,              // 00 27
    InstCopyThreadWork,       // 00 28
    InstUPLmenuUI,            // 00 29
    InstSave,                 // 00 2A
    InstSaveIconLoad,         // 00 2B
    InstBGMflag,              // 00 2C
    InstUPLxTitle,            // 00 2D
    InstPresence,             // 00 2E
    InstAchievement,          // 00 2F
    InstSetPlayer,            // 00 30
    InstVoiceTableLoadMaybe,  // 00 31
    InstSetPadCustom,         // 00 32
    InstMwait,                // 00 33
    InstTerminate,            // 00 34
    InstSignIn,               // 00 35
    InstAchievementIcon,      // 00 36
    InstVoicePlay,            // 00 37
    InstVoiceStop,            // 00 38
    InstVoicePlayWait,        // 00 39
    InstBGMduelPlay,          // 00 3A
    InstSNDpause,             // 00 3B
    InstSEplayWait,           // 00 3C
    InstDebugPrint,           // 00 3D
    InstResetSoundAll,        // 00 3E
    InstSNDloadStop,          // 00 3F
    InstBGMstopWait,          // 00 40
    InstUnk0041,              // 00 41
    InstSetX360SysMesPos,     // 00 42
    InstSystemMes,            // 00 43
    InstSystemMenu,           // 00 44
    InstGetNowTime,           // 00 45
    InstGetSystemStatus,      // 00 46
    InstReboot,               // 00 47
    InstReloadScript,         // 00 48
    InstReloadScriptMenu,     // 00 49
    InstDebugEditer,          // 00 4A
    Inst__Unrecognized__,     // 00 4B
    Inst__Unrecognized__,     // 00 4C
    InstSysVoicePlay,         // 00 4D
    InstPadActEx,             // 00 4E
    InstDebugSetup,           // 00 4F
    InstPressStart,           // 00 50
    InstGlobalSystemMessage,  // 00 51
    InstUnk0052,              // 00 52
    InstUnk0053,              // 00 53
    InstUnk0054,              // 00 54
    InstUnk0055,              // 00 55
    Inst__Unrecognized__,     // 00 56
    Inst__Unrecognized__,     // 00 57
    Inst__Unrecognized__,     // 00 58
    Inst__Unrecognized__,     // 00 59
    Inst__Unrecognized__,     // 00 5A
    Inst__Unrecognized__,     // 00 5B
    Inst__Unrecognized__,     // 00 5C
    Inst__Unrecognized__,     // 00 5D
    Inst__Unrecognized__,     // 00 5E
    Inst__Unrecognized__,     // 00 5F
    Inst__Unrecognized__,     // 00 60
    Inst__Unrecognized__,     // 00 61
    Inst__Unrecognized__,     // 00 62
    Inst__Unrecognized__,     // 00 63
    Inst__Unrecognized__,     // 00 64
    Inst__Unrecognized__,     // 00 65
    Inst__Unrecognized__,     // 00 66
    Inst__Unrecognized__,     // 00 67
    Inst__Unrecognized__,     // 00 68
    Inst__Unrecognized__,     // 00 69
    Inst__Unrecognized__,     // 00 6A
    Inst__Unrecognized__,     // 00 6B
    Inst__Unrecognized__,     // 00 6C
    Inst__Unrecognized__,     // 00 6D
    Inst__Unrecognized__,     // 00 6E
    Inst__Unrecognized__,     // 00 6F
    Inst__Unrecognized__,     // 00 70
    Inst__Unrecognized__,     // 00 71
    Inst__Unrecognized__,     // 00 72
    Inst__Unrecognized__,     // 00 73
    Inst__Unrecognized__,     // 00 74
    Inst__Unrecognized__,     // 00 75
    Inst__Unrecognized__,     // 00 76
    Inst__Unrecognized__,     // 00 77
    Inst__Unrecognized__,     // 00 78
    Inst__Unrecognized__,     // 00 79
    Inst__Unrecognized__,     // 00 7A
    Inst__Unrecognized__,     // 00 7B
    Inst__Unrecognized__,     // 00 7C
    Inst__Unrecognized__,     // 00 7D
    Inst__Unrecognized__,     // 00 7E
    Inst__Unrecognized__,     // 00 7F
    Inst__Unrecognized__,     // 00 80
    Inst__Unrecognized__,     // 00 81
    Inst__Unrecognized__,     // 00 82
    Inst__Unrecognized__,     // 00 83
    Inst__Unrecognized__,     // 00 84
    Inst__Unrecognized__,     // 00 85
    Inst__Unrecognized__,     // 00 86
    Inst__Unrecognized__,     // 00 87
    Inst__Unrecognized__,     // 00 88
    Inst__Unrecognized__,     // 00 89
    Inst__Unrecognized__,     // 00 8A
    Inst__Unrecognized__,     // 00 8B
    Inst__Unrecognized__,     // 00 8C
    Inst__Unrecognized__,     // 00 8D
    Inst__Unrecognized__,     // 00 8E
    Inst__Unrecognized__,     // 00 8F
    Inst__Unrecognized__,     // 00 90
    Inst__Unrecognized__,     // 00 91
    Inst__Unrecognized__,     // 00 92
    Inst__Unrecognized__,     // 00 93
    Inst__Unrecognized__,     // 00 94
    Inst__Unrecognized__,     // 00 95
    Inst__Unrecognized__,     // 00 96
    Inst__Unrecognized__,     // 00 97
    Inst__Unrecognized__,     // 00 98
    Inst__Unrecognized__,     // 00 99
    Inst__Unrecognized__,     // 00 9A
    Inst__Unrecognized__,     // 00 9B
    Inst__Unrecognized__,     // 00 9C
    Inst__Unrecognized__,     // 00 9D
    Inst__Unrecognized__,     // 00 9E
    Inst__Unrecognized__,     // 00 9F
    Inst__Unrecognized__,     // 00 A0
    Inst__Unrecognized__,     // 00 A1
    Inst__Unrecognized__,     // 00 A2
    Inst__Unrecognized__,     // 00 A3
    Inst__Unrecognized__,     // 00 A4
    Inst__Unrecognized__,     // 00 A5
    Inst__Unrecognized__,     // 00 A6
    Inst__Unrecognized__,     // 00 A7
    Inst__Unrecognized__,     // 00 A8
    Inst__Unrecognized__,     // 00 A9
    Inst__Unrecognized__,     // 00 AA
    Inst__Unrecognized__,     // 00 AB
    Inst__Unrecognized__,     // 00 AC
    Inst__Unrecognized__,     // 00 AD
    Inst__Unrecognized__,     // 00 AE
    Inst__Unrecognized__,     // 00 AF
    Inst__Unrecognized__,     // 00 B0
    Inst__Unrecognized__,     // 00 B1
    Inst__Unrecognized__,     // 00 B2
    Inst__Unrecognized__,     // 00 B3
    Inst__Unrecognized__,     // 00 B4
    Inst__Unrecognized__,     // 00 B5
    Inst__Unrecognized__,     // 00 B6
    Inst__Unrecognized__,     // 00 B7
    Inst__Unrecognized__,     // 00 B8
    Inst__Unrecognized__,     // 00 B9
    Inst__Unrecognized__,     // 00 BA
    Inst__Unrecognized__,     // 00 BB
    Inst__Unrecognized__,     // 00 BC
    Inst__Unrecognized__,     // 00 BD
    Inst__Unrecognized__,     // 00 BE
    Inst__Unrecognized__,     // 00 BF
    Inst__Unrecognized__,     // 00 C0
    Inst__Unrecognized__,     // 00 C1
    Inst__Unrecognized__,     // 00 C2
    Inst__Unrecognized__,     // 00 C3
    Inst__Unrecognized__,     // 00 C4
    Inst__Unrecognized__,     // 00 C5
    Inst__Unrecognized__,     // 00 C6
    Inst__Unrecognized__,     // 00 C7
    Inst__Unrecognized__,     // 00 C8
    Inst__Unrecognized__,     // 00 C9
    Inst__Unrecognized__,     // 00 CA
    Inst__Unrecognized__,     // 00 CB
    Inst__Unrecognized__,     // 00 CC
    Inst__Unrecognized__,     // 00 CD
    Inst__Unrecognized__,     // 00 CE
    Inst__Unrecognized__,     // 00 CF
    Inst__Unrecognized__,     // 00 D0
    Inst__Unrecognized__,     // 00 D1
    Inst__Unrecognized__,     // 00 D2
    Inst__Unrecognized__,     // 00 D3
    Inst__Unrecognized__,     // 00 D4
    Inst__Unrecognized__,     // 00 D5
    Inst__Unrecognized__,     // 00 D6
    Inst__Unrecognized__,     // 00 D7
    Inst__Unrecognized__,     // 00 D8
    Inst__Unrecognized__,     // 00 D9
    Inst__Unrecognized__,     // 00 DA
    Inst__Unrecognized__,     // 00 DB
    Inst__Unrecognized__,     // 00 DC
    Inst__Unrecognized__,     // 00 DD
    Inst__Unrecognized__,     // 00 DE
    Inst__Unrecognized__,     // 00 DF
    Inst__Unrecognized__,     // 00 E0
    Inst__Unrecognized__,     // 00 E1
    Inst__Unrecognized__,     // 00 E2
    Inst__Unrecognized__,     // 00 E3
    Inst__Unrecognized__,     // 00 E4
    Inst__Unrecognized__,     // 00 E5
    Inst__Unrecognized__,     // 00 E6
    Inst__Unrecognized__,     // 00 E7
    Inst__Unrecognized__,     // 00 E8
    Inst__Unrecognized__,     // 00 E9
    Inst__Unrecognized__,     // 00 EA
    Inst__Unrecognized__,     // 00 EB
    Inst__Unrecognized__,     // 00 EC
    Inst__Unrecognized__,     // 00 ED
    Inst__Unrecognized__,     // 00 EE
    Inst__Unrecognized__,     // 00 EF
    Inst__Unrecognized__,     // 00 F0
    Inst__Unrecognized__,     // 00 F1
    Inst__Unrecognized__,     // 00 F2
    Inst__Unrecognized__,     // 00 F3
    Inst__Unrecognized__,     // 00 F4
    Inst__Unrecognized__,     // 00 F5
    Inst__Unrecognized__,     // 00 F6
    Inst__Unrecognized__,     // 00 F7
    Inst__Unrecognized__,     // 00 F8
    Inst__Unrecognized__,     // 00 F9
    Inst__Unrecognized__,     // 00 FA
    Inst__Unrecognized__,     // 00 FB
    Inst__Unrecognized__,     // 00 FC
    Inst__Unrecognized__,     // 00 FD
    Inst__Unrecognized__,     // 00 FE
    Inst__Unrecognized__      // 00 FF
};

static DecoderProc DecoderTableGraph[256] = {
    InstCreateSurf,        // 01 00
    InstReleaseSurf,       // 01 01
    InstLoadPic,           // 01 02
    InstReleaseSurf,       // 01 03
    InstSurfFill,          // 01 04
    InstCalc,              // 01 05
    InstMesViewFlag,       // 01 06
    InstSetMesWinPri,      // 01 07
    InstMesSync,           // 01 08
    InstMesSetID,          // 01 09
    InstMesCls,            // 01 0A
    InstMesVoiceWait,      // 01 0B
    InstMes,               // 01 0C
    InstMesMain,           // 01 0D
    InstSetMesModeFormat,  // 01 0E
    InstSetNGmoji,         // 01 0F
    InstMesRev,            // 01 10
    InstMessWindow,        // 01 11
    InstSel,               // 01 12
    InstSelect,            // 01 13
    InstSysSel,            // 01 14
    InstSysSelect,         // 01 15
    Inst__Unrecognized__,  // 01 16
    Inst__Unrecognized__,  // 01 17
    Inst__Unrecognized__,  // 01 18
    Inst__Unrecognized__,  // 01 19
    Inst__Unrecognized__,  // 01 1A
    Inst__Unrecognized__,  // 01 1B
    Inst__Unrecognized__,  // 01 1C
    Inst__Unrecognized__,  // 01 1D
    Inst__Unrecognized__,  // 01 1E
    InstUnk011F,           // 01 1F
    InstSCcapture,         // 01 20
    InstSetTextTable,      // 01 21
    InstPlayMovie,         // 01 22
    InstMovieMain,         // 01 23
    InstLoadMovie,         // 01 24
    InstSetRevMes,         // 01 25
    InstPlayMovieMemory,   // 01 26
    InstPlayMovie,         // 01 27
    InstMovieMain,         // 01 28
    InstLoadMovie,         // 01 29
    InstPlayMovieMemory,   // 01 2A
    InstSFDpause,          // 01 2B
    InstPlayMovie,         // 01 2C
    InstUnk012D,           // 01 2D
    Inst__Unrecognized__,  // 01 2E
    Inst__Unrecognized__,  // 01 2F
    Inst__Unrecognized__,  // 01 30
    Inst__Unrecognized__,  // 01 31
    Inst__Unrecognized__,  // 01 32
    Inst__Unrecognized__,  // 01 33
    Inst__Unrecognized__,  // 01 34
    Inst__Unrecognized__,  // 01 35
    Inst__Unrecognized__,  // 01 36
    Inst__Unrecognized__,  // 01 37
    Inst__Unrecognized__,  // 01 38
    Inst__Unrecognized__,  // 01 39
    Inst__Unrecognized__,  // 01 3A
    Inst__Unrecognized__,  // 01 3B
    Inst__Unrecognized__,  // 01 3C
    Inst__Unrecognized__,  // 01 3D
    Inst__Unrecognized__,  // 01 3E
    Inst__Unrecognized__,  // 01 3F
    Inst__Unrecognized__,  // 01 40
    Inst__Unrecognized__,  // 01 41
    Inst__Unrecognized__,  // 01 42
    Inst__Unrecognized__,  // 01 43
    Inst__Unrecognized__,  // 01 44
    Inst__Unrecognized__,  // 01 45
    Inst__Unrecognized__,  // 01 46
    Inst__Unrecognized__,  // 01 47
    Inst__Unrecognized__,  // 01 48
    Inst__Unrecognized__,  // 01 49
    Inst__Unrecognized__,  // 01 4A
    Inst__Unrecognized__,  // 01 4B
    Inst__Unrecognized__,  // 01 4C
    Inst__Unrecognized__,  // 01 4D
    Inst__Unrecognized__,  // 01 4E
    Inst__Unrecognized__,  // 01 4F
    Inst__Unrecognized__,  // 01 50
    Inst__Unrecognized__,  // 01 51
    Inst__Unrecognized__,  // 01 52
    Inst__Unrecognized__,  // 01 53
    Inst__Unrecognized__,  // 01 54
    Inst__Unrecognized__,  // 01 55
    Inst__Unrecognized__,  // 01 56
    Inst__Unrecognized__,  // 01 57
    Inst__Unrecognized__,  // 01 58
    Inst__Unrecognized__,  // 01 59
    Inst__Unrecognized__,  // 01 5A
    Inst__Unrecognized__,  // 01 5B
    Inst__Unrecognized__,  // 01 5C
    Inst__Unrecognized__,  // 01 5D
    Inst__Unrecognized__,  // 01 5E
    Inst__Unrecognized__,  // 01 5F
    Inst__Unrecognized__,  // 01 60
    Inst__Unrecognized__,  // 01 61
    Inst__Unrecognized__,  // 01 62
    Inst__Unrecognized__,  // 01 63
    Inst__Unrecognized__,  // 01 64
    Inst__Unrecognized__,  // 01 65
    Inst__Unrecognized__,  // 01 66
    Inst__Unrecognized__,  // 01 67
    Inst__Unrecognized__,  // 01 68
    Inst__Unrecognized__,  // 01 69
    Inst__Unrecognized__,  // 01 6A
    Inst__Unrecognized__,  // 01 6B
    Inst__Unrecognized__,  // 01 6C
    Inst__Unrecognized__,  // 01 6D
    Inst__Unrecognized__,  // 01 6E
    Inst__Unrecognized__,  // 01 6F
    Inst__Unrecognized__,  // 01 70
    Inst__Unrecognized__,  // 01 71
    Inst__Unrecognized__,  // 01 72
    Inst__Unrecognized__,  // 01 73
    Inst__Unrecognized__,  // 01 74
    Inst__Unrecognized__,  // 01 75
    Inst__Unrecognized__,  // 01 76
    Inst__Unrecognized__,  // 01 77
    Inst__Unrecognized__,  // 01 78
    Inst__Unrecognized__,  // 01 79
    Inst__Unrecognized__,  // 01 7A
    Inst__Unrecognized__,  // 01 7B
    Inst__Unrecognized__,  // 01 7C
    Inst__Unrecognized__,  // 01 7D
    Inst__Unrecognized__,  // 01 7E
    Inst__Unrecognized__,  // 01 7F
    Inst__Unrecognized__,  // 01 80
    Inst__Unrecognized__,  // 01 81
    Inst__Unrecognized__,  // 01 82
    Inst__Unrecognized__,  // 01 83
    Inst__Unrecognized__,  // 01 84
    Inst__Unrecognized__,  // 01 85
    Inst__Unrecognized__,  // 01 86
    Inst__Unrecognized__,  // 01 87
    Inst__Unrecognized__,  // 01 88
    Inst__Unrecognized__,  // 01 89
    Inst__Unrecognized__,  // 01 8A
    Inst__Unrecognized__,  // 01 8B
    Inst__Unrecognized__,  // 01 8C
    Inst__Unrecognized__,  // 01 8D
    Inst__Unrecognized__,  // 01 8E
    Inst__Unrecognized__,  // 01 8F
    Inst__Unrecognized__,  // 01 90
    Inst__Unrecognized__,  // 01 91
    Inst__Unrecognized__,  // 01 92
    Inst__Unrecognized__,  // 01 93
    Inst__Unrecognized__,  // 01 94
    Inst__Unrecognized__,  // 01 95
    Inst__Unrecognized__,  // 01 96
    Inst__Unrecognized__,  // 01 97
    Inst__Unrecognized__,  // 01 98
    Inst__Unrecognized__,  // 01 99
    Inst__Unrecognized__,  // 01 9A
    Inst__Unrecognized__,  // 01 9B
    Inst__Unrecognized__,  // 01 9C
    Inst__Unrecognized__,  // 01 9D
    Inst__Unrecognized__,  // 01 9E
    Inst__Unrecognized__,  // 01 9F
    Inst__Unrecognized__,  // 01 A0
    Inst__Unrecognized__,  // 01 A1
    Inst__Unrecognized__,  // 01 A2
    Inst__Unrecognized__,  // 01 A3
    Inst__Unrecognized__,  // 01 A4
    Inst__Unrecognized__,  // 01 A5
    Inst__Unrecognized__,  // 01 A6
    Inst__Unrecognized__,  // 01 A7
    Inst__Unrecognized__,  // 01 A8
    Inst__Unrecognized__,  // 01 A9
    Inst__Unrecognized__,  // 01 AA
    Inst__Unrecognized__,  // 01 AB
    Inst__Unrecognized__,  // 01 AC
    Inst__Unrecognized__,  // 01 AD
    Inst__Unrecognized__,  // 01 AE
    Inst__Unrecognized__,  // 01 AF
    Inst__Unrecognized__,  // 01 B0
    Inst__Unrecognized__,  // 01 B1
    Inst__Unrecognized__,  // 01 B2
    Inst__Unrecognized__,  // 01 B3
    Inst__Unrecognized__,  // 01 B4
    Inst__Unrecognized__,  // 01 B5
    Inst__Unrecognized__,  // 01 B6
    Inst__Unrecognized__,  // 01 B7
    Inst__Unrecognized__,  // 01 B8
    Inst__Unrecognized__,  // 01 B9
    Inst__Unrecognized__,  // 01 BA
    Inst__Unrecognized__,  // 01 BB
    Inst__Unrecognized__,  // 01 BC
    Inst__Unrecognized__,  // 01 BD
    Inst__Unrecognized__,  // 01 BE
    Inst__Unrecognized__,  // 01 BF
    Inst__Unrecognized__,  // 01 C0
    Inst__Unrecognized__,  // 01 C1
    Inst__Unrecognized__,  // 01 C2
    Inst__Unrecognized__,  // 01 C3
    Inst__Unrecognized__,  // 01 C4
    Inst__Unrecognized__,  // 01 C5
    Inst__Unrecognized__,  // 01 C6
    Inst__Unrecognized__,  // 01 C7
    Inst__Unrecognized__,  // 01 C8
    Inst__Unrecognized__,  // 01 C9
    Inst__Unrecognized__,  // 01 CA
    Inst__Unrecognized__,  // 01 CB
    Inst__Unrecognized__,  // 01 CC
    Inst__Unrecognized__,  // 01 CD
    Inst__Unrecognized__,  // 01 CE
    Inst__Unrecognized__,  // 01 CF
    Inst__Unrecognized__,  // 01 D0
    Inst__Unrecognized__,  // 01 D1
    Inst__Unrecognized__,  // 01 D2
    Inst__Unrecognized__,  // 01 D3
    Inst__Unrecognized__,  // 01 D4
    Inst__Unrecognized__,  // 01 D5
    Inst__Unrecognized__,  // 01 D6
    Inst__Unrecognized__,  // 01 D7
    Inst__Unrecognized__,  // 01 D8
    Inst__Unrecognized__,  // 01 D9
    Inst__Unrecognized__,  // 01 DA
    Inst__Unrecognized__,  // 01 DB
    Inst__Unrecognized__,  // 01 DC
    Inst__Unrecognized__,  // 01 DD
    Inst__Unrecognized__,  // 01 DE
    Inst__Unrecognized__,  // 01 DF
    Inst__Unrecognized__,  // 01 E0
    Inst__Unrecognized__,  // 01 E1
    Inst__Unrecognized__,  // 01 E2
    Inst__Unrecognized__,  // 01 E3
    Inst__Unrecognized__,  // 01 E4
    Inst__Unrecognized__,  // 01 E5
    Inst__Unrecognized__,  // 01 E6
    Inst__Unrecognized__,  // 01 E7
    Inst__Unrecognized__,  // 01 E8
    Inst__Unrecognized__,  // 01 E9
    Inst__Unrecognized__,  // 01 EA
    Inst__Unrecognized__,  // 01 EB
    Inst__Unrecognized__,  // 01 EC
    Inst__Unrecognized__,  // 01 ED
    Inst__Unrecognized__,  // 01 EE
    Inst__Unrecognized__,  // 01 EF
    Inst__Unrecognized__,  // 01 F0
    Inst__Unrecognized__,  // 01 F1
    Inst__Unrecognized__,  // 01 F2
    Inst__Unrecognized__,  // 01 F3
    Inst__Unrecognized__,  // 01 F4
    Inst__Unrecognized__,  // 01 F5
    Inst__Unrecognized__,  // 01 F6
    Inst__Unrecognized__,  // 01 F7
    Inst__Unrecognized__,  // 01 F8
    Inst__Unrecognized__,  // 01 F9
    Inst__Unrecognized__,  // 01 FA
    Inst__Unrecognized__,  // 01 FB
    Inst__Unrecognized__,  // 01 FC
    Inst__Unrecognized__,  // 01 FD
    Inst__Unrecognized__,  // 01 FE
    Inst__Unrecognized__   // 01 FF
};

static DecoderProc DecoderTableUser1[256] = {
    InstMSinit,            // 10 00
    InstBGload,            // 10 01
    InstBGswap,            // 10 02
    InstBGsetColor,        // 10 03
    InstBGsetLink,         // 10 04
    InstBGrelease,         // 10 05
    Inst__Unrecognized__,  // 10 06
    InstBGcopy,            // 10 07
    Inst__Unrecognized__,  // 10 08
    InstSaveSlot,          // 10 09
    InstSystemMain,        // 10 0A
    Inst__Unrecognized__,  // 10 0B
    InstGameInfoInit,      // 10 0C
    Inst__Unrecognized__,  // 10 0D
    InstBGloadEx,          // 10 0E
    Inst__Unrecognized__,  // 10 0F
    Inst__Unrecognized__,  // 10 10
    Inst__Unrecognized__,  // 10 11
    InstClearFlagChk,      // 10 12
    InstOption,            // 10 13
    InstSystemDataReset,   // 10 14
    InstDebugData,         // 10 15
    InstGetCharaPause,     // 10 16
    InstBGfadeExpInit,     // 10 17
    Inst__Unrecognized__,  // 10 18
    Inst__Unrecognized__,  // 10 19
    Inst__Unrecognized__,  // 10 1A
    InstHelp,              // 10 1B
    InstAchievementMenu,   // 10 1C
    InstSoundMenu,         // 10 1D
    InstAllClear,          // 10 1E
    InstAlbum,             // 10 1F
    InstMovieMode,         // 10 20
    InstClistInit,         // 10 21
    InstAutoSave,          // 10 22
    InstSaveMenu,          // 10 23
    InstLoadData,          // 10 24
    Inst__Unrecognized__,  // 10 25
    Inst__Unrecognized__,  // 10 26
    InstSetDic,            // 10 27
    InstSetPlayMode,       // 10 28
    InstSetEVflag,         // 10 29
    InstSetCutin,          // 10 2A
    Inst__Unrecognized__,  // 10 2B
    Inst__Unrecognized__,  // 10 2C
    InstAchChkTitle,       // 10 2D
    InstSetSceneViewFlag,  // 10 2E
    InstChkClearFlag,      // 10 2F
    InstBGeffectWave,      // 10 30
    InstUnk1031,           // 10 31
    InstNameID,            // 10 32
    InstTips,              // 10 33
    InstTitleMenu,         // 10 34
    Inst__Unrecognized__,  // 10 35
    InstBGeffect,          // 10 36
    InstUnk1037,           // 10 37
    InstUnk1038,           // 10 38
    InstUnk1039,           // 10 39
    InstUnk103A,           // 10 3A
    Inst__Unrecognized__,  // 10 3B
    Inst__Unrecognized__,  // 10 3C
    Inst__Unrecognized__,  // 10 3D
    Inst__Unrecognized__,  // 10 3E
    Inst__Unrecognized__,  // 10 3F
    Inst__Unrecognized__,  // 10 40
    Inst__Unrecognized__,  // 10 41
    Inst__Unrecognized__,  // 10 42
    Inst__Unrecognized__,  // 10 43
    Inst__Unrecognized__,  // 10 44
    Inst__Unrecognized__,  // 10 45
    Inst__Unrecognized__,  // 10 46
    Inst__Unrecognized__,  // 10 47
    Inst__Unrecognized__,  // 10 48
    Inst__Unrecognized__,  // 10 49
    Inst__Unrecognized__,  // 10 4A
    Inst__Unrecognized__,  // 10 4B
    Inst__Unrecognized__,  // 10 4C
    Inst__Unrecognized__,  // 10 4D
    Inst__Unrecognized__,  // 10 4E
    Inst__Unrecognized__,  // 10 4F
    Inst__Unrecognized__,  // 10 50
    Inst__Unrecognized__,  // 10 51
    Inst__Unrecognized__,  // 10 52
    Inst__Unrecognized__,  // 10 53
    Inst__Unrecognized__,  // 10 54
    Inst__Unrecognized__,  // 10 55
    Inst__Unrecognized__,  // 10 56
    Inst__Unrecognized__,  // 10 57
    Inst__Unrecognized__,  // 10 58
    Inst__Unrecognized__,  // 10 59
    Inst__Unrecognized__,  // 10 5A
    Inst__Unrecognized__,  // 10 5B
    Inst__Unrecognized__,  // 10 5C
    Inst__Unrecognized__,  // 10 5D
    Inst__Unrecognized__,  // 10 5E
    Inst__Unrecognized__,  // 10 5F
    Inst__Unrecognized__,  // 10 60
    Inst__Unrecognized__,  // 10 61
    Inst__Unrecognized__,  // 10 62
    Inst__Unrecognized__,  // 10 63
    Inst__Unrecognized__,  // 10 64
    Inst__Unrecognized__,  // 10 65
    Inst__Unrecognized__,  // 10 66
    Inst__Unrecognized__,  // 10 67
    Inst__Unrecognized__,  // 10 68
    Inst__Unrecognized__,  // 10 69
    Inst__Unrecognized__,  // 10 6A
    Inst__Unrecognized__,  // 10 6B
    Inst__Unrecognized__,  // 10 6C
    Inst__Unrecognized__,  // 10 6D
    Inst__Unrecognized__,  // 10 6E
    Inst__Unrecognized__,  // 10 6F
    Inst__Unrecognized__,  // 10 70
    Inst__Unrecognized__,  // 10 71
    Inst__Unrecognized__,  // 10 72
    Inst__Unrecognized__,  // 10 73
    Inst__Unrecognized__,  // 10 74
    Inst__Unrecognized__,  // 10 75
    Inst__Unrecognized__,  // 10 76
    Inst__Unrecognized__,  // 10 77
    Inst__Unrecognized__,  // 10 78
    Inst__Unrecognized__,  // 10 79
    Inst__Unrecognized__,  // 10 7A
    Inst__Unrecognized__,  // 10 7B
    Inst__Unrecognized__,  // 10 7C
    Inst__Unrecognized__,  // 10 7D
    Inst__Unrecognized__,  // 10 7E
    Inst__Unrecognized__,  // 10 7F
    Inst__Unrecognized__,  // 10 80
    Inst__Unrecognized__,  // 10 81
    Inst__Unrecognized__,  // 10 82
    Inst__Unrecognized__,  // 10 83
    Inst__Unrecognized__,  // 10 84
    Inst__Unrecognized__,  // 10 85
    Inst__Unrecognized__,  // 10 86
    Inst__Unrecognized__,  // 10 87
    Inst__Unrecognized__,  // 10 88
    Inst__Unrecognized__,  // 10 89
    Inst__Unrecognized__,  // 10 8A
    Inst__Unrecognized__,  // 10 8B
    Inst__Unrecognized__,  // 10 8C
    Inst__Unrecognized__,  // 10 8D
    Inst__Unrecognized__,  // 10 8E
    Inst__Unrecognized__,  // 10 8F
    Inst__Unrecognized__,  // 10 90
    Inst__Unrecognized__,  // 10 91
    Inst__Unrecognized__,  // 10 92
    Inst__Unrecognized__,  // 10 93
    Inst__Unrecognized__,  // 10 94
    Inst__Unrecognized__,  // 10 95
    Inst__Unrecognized__,  // 10 96
    Inst__Unrecognized__,  // 10 97
    Inst__Unrecognized__,  // 10 98
    Inst__Unrecognized__,  // 10 99
    Inst__Unrecognized__,  // 10 9A
    Inst__Unrecognized__,  // 10 9B
    Inst__Unrecognized__,  // 10 9C
    Inst__Unrecognized__,  // 10 9D
    Inst__Unrecognized__,  // 10 9E
    Inst__Unrecognized__,  // 10 9F
    Inst__Unrecognized__,  // 10 A0
    Inst__Unrecognized__,  // 10 A1
    Inst__Unrecognized__,  // 10 A2
    Inst__Unrecognized__,  // 10 A3
    Inst__Unrecognized__,  // 10 A4
    Inst__Unrecognized__,  // 10 A5
    Inst__Unrecognized__,  // 10 A6
    Inst__Unrecognized__,  // 10 A7
    Inst__Unrecognized__,  // 10 A8
    Inst__Unrecognized__,  // 10 A9
    Inst__Unrecognized__,  // 10 AA
    Inst__Unrecognized__,  // 10 AB
    Inst__Unrecognized__,  // 10 AC
    Inst__Unrecognized__,  // 10 AD
    Inst__Unrecognized__,  // 10 AE
    Inst__Unrecognized__,  // 10 AF
    Inst__Unrecognized__,  // 10 B0
    Inst__Unrecognized__,  // 10 B1
    Inst__Unrecognized__,  // 10 B2
    Inst__Unrecognized__,  // 10 B3
    Inst__Unrecognized__,  // 10 B4
    Inst__Unrecognized__,  // 10 B5
    Inst__Unrecognized__,  // 10 B6
    Inst__Unrecognized__,  // 10 B7
    Inst__Unrecognized__,  // 10 B8
    Inst__Unrecognized__,  // 10 B9
    Inst__Unrecognized__,  // 10 BA
    Inst__Unrecognized__,  // 10 BB
    Inst__Unrecognized__,  // 10 BC
    Inst__Unrecognized__,  // 10 BD
    Inst__Unrecognized__,  // 10 BE
    Inst__Unrecognized__,  // 10 BF
    Inst__Unrecognized__,  // 10 C0
    Inst__Unrecognized__,  // 10 C1
    Inst__Unrecognized__,  // 10 C2
    Inst__Unrecognized__,  // 10 C3
    Inst__Unrecognized__,  // 10 C4
    Inst__Unrecognized__,  // 10 C5
    Inst__Unrecognized__,  // 10 C6
    Inst__Unrecognized__,  // 10 C7
    Inst__Unrecognized__,  // 10 C8
    Inst__Unrecognized__,  // 10 C9
    Inst__Unrecognized__,  // 10 CA
    Inst__Unrecognized__,  // 10 CB
    Inst__Unrecognized__,  // 10 CC
    Inst__Unrecognized__,  // 10 CD
    Inst__Unrecognized__,  // 10 CE
    Inst__Unrecognized__,  // 10 CF
    Inst__Unrecognized__,  // 10 D0
    Inst__Unrecognized__,  // 10 D1
    Inst__Unrecognized__,  // 10 D2
    Inst__Unrecognized__,  // 10 D3
    Inst__Unrecognized__,  // 10 D4
    Inst__Unrecognized__,  // 10 D5
    Inst__Unrecognized__,  // 10 D6
    Inst__Unrecognized__,  // 10 D7
    Inst__Unrecognized__,  // 10 D8
    Inst__Unrecognized__,  // 10 D9
    Inst__Unrecognized__,  // 10 DA
    Inst__Unrecognized__,  // 10 DB
    Inst__Unrecognized__,  // 10 DC
    Inst__Unrecognized__,  // 10 DD
    Inst__Unrecognized__,  // 10 DE
    Inst__Unrecognized__,  // 10 DF
    Inst__Unrecognized__,  // 10 E0
    Inst__Unrecognized__,  // 10 E1
    Inst__Unrecognized__,  // 10 E2
    Inst__Unrecognized__,  // 10 E3
    Inst__Unrecognized__,  // 10 E4
    Inst__Unrecognized__,  // 10 E5
    Inst__Unrecognized__,  // 10 E6
    Inst__Unrecognized__,  // 10 E7
    Inst__Unrecognized__,  // 10 E8
    Inst__Unrecognized__,  // 10 E9
    Inst__Unrecognized__,  // 10 EA
    Inst__Unrecognized__,  // 10 EB
    Inst__Unrecognized__,  // 10 EC
    Inst__Unrecognized__,  // 10 ED
    Inst__Unrecognized__,  // 10 EE
    Inst__Unrecognized__,  // 10 EF
    Inst__Unrecognized__,  // 10 F0
    Inst__Unrecognized__,  // 10 F1
    Inst__Unrecognized__,  // 10 F2
    Inst__Unrecognized__,  // 10 F3
    Inst__Unrecognized__,  // 10 F4
    Inst__Unrecognized__,  // 10 F5
    Inst__Unrecognized__,  // 10 F6
    Inst__Unrecognized__,  // 10 F7
    Inst__Unrecognized__,  // 10 F8
    Inst__Unrecognized__,  // 10 F9
    Inst__Unrecognized__,  // 10 FA
    Inst__Unrecognized__,  // 10 FB
    Inst__Unrecognized__,  // 10 FC
    Inst__Unrecognized__,  // 10 FD
    Inst__Unrecognized__,  // 10 FE
    Inst__Unrecognized__   // 10 FF
};

static DecoderProc DecoderTableGraph3D[256] = {
    Inst__Unrecognized__,    // 02 00
    InstCHAload3D,           // 02 01
    InstCHArelease3D,        // 02 02
    Inst__Unrecognized__,    // 02 03
    InstUnk0204,             // 02 04
    InstCHAswap3DMaybe,      // 02 05
    InstCHAplayAnim3DMaybe,  // 02 06
    InstCHAUnk02073D,        // 02 07
    InstUnk0208,             // 02 08
    InstCHAsetAnim3D,        // 02 09
    Inst__Unrecognized__,    // 02 0A
    Inst__Unrecognized__,    // 02 0B
    Inst__Unrecognized__,    // 02 0C
    Inst__Unrecognized__,    // 02 0D
    Inst__Unrecognized__,    // 02 0E
    Inst__Unrecognized__,    // 02 0F
    InstUnk0210,             // 02 10
    InstUnk0211,             // 02 11
    InstUnk0212,             // 02 12
    InstUnk0213,             // 02 13
    InstUnk0214,             // 02 14
    InstUnk0215,             // 02 15
    InstUnk0216,             // 02 16
    InstUnk0217,             // 02 17
    InstUnk0218,             // 02 18
    InstUnk0219,             // 02 19
    Inst__Unrecognized__,    // 02 1A
    Inst__Unrecognized__,    // 02 1B
    Inst__Unrecognized__,    // 02 1C
    Inst__Unrecognized__,    // 02 1D
    Inst__Unrecognized__,    // 02 1E
    Inst__Unrecognized__,    // 02 1F
    Inst__Unrecognized__,    // 02 20
    Inst__Unrecognized__,    // 02 21
    Inst__Unrecognized__,    // 02 22
    Inst__Unrecognized__,    // 02 23
    Inst__Unrecognized__,    // 02 24
    Inst__Unrecognized__,    // 02 25
    Inst__Unrecognized__,    // 02 26
    Inst__Unrecognized__,    // 02 27
    Inst__Unrecognized__,    // 02 28
    Inst__Unrecognized__,    // 02 29
    Inst__Unrecognized__,    // 02 2A
    Inst__Unrecognized__,    // 02 2B
    Inst__Unrecognized__,    // 02 2C
    Inst__Unrecognized__,    // 02 2D
    Inst__Unrecognized__,    // 02 2E
    Inst__Unrecognized__,    // 02 2F
    Inst__Unrecognized__,    // 02 30
    Inst__Unrecognized__,    // 02 31
    Inst__Unrecognized__,    // 02 32
    Inst__Unrecognized__,    // 02 33
    Inst__Unrecognized__,    // 02 34
    Inst__Unrecognized__,    // 02 35
    Inst__Unrecognized__,    // 02 36
    Inst__Unrecognized__,    // 02 37
    Inst__Unrecognized__,    // 02 38
    Inst__Unrecognized__,    // 02 39
    Inst__Unrecognized__,    // 02 3A
    Inst__Unrecognized__,    // 02 3B
    Inst__Unrecognized__,    // 02 3C
    Inst__Unrecognized__,    // 02 3D
    Inst__Unrecognized__,    // 02 3E
    Inst__Unrecognized__,    // 02 3F
    InstUnk0240,             // 02 40
    Inst__Unrecognized__,    // 02 41
    Inst__Unrecognized__,    // 02 42
    Inst__Unrecognized__,    // 02 43
    Inst__Unrecognized__,    // 02 44
    Inst__Unrecognized__,    // 02 45
    Inst__Unrecognized__,    // 02 46
    Inst__Unrecognized__,    // 02 47
    Inst__Unrecognized__,    // 02 48
    Inst__Unrecognized__,    // 02 49
    Inst__Unrecognized__,    // 02 4A
    Inst__Unrecognized__,    // 02 4B
    Inst__Unrecognized__,    // 02 4C
    Inst__Unrecognized__,    // 02 4D
    Inst__Unrecognized__,    // 02 4E
    Inst__Unrecognized__,    // 02 4F
    Inst__Unrecognized__,    // 02 50
    Inst__Unrecognized__,    // 02 51
    Inst__Unrecognized__,    // 02 52
    Inst__Unrecognized__,    // 02 53
    Inst__Unrecognized__,    // 02 54
    Inst__Unrecognized__,    // 02 55
    Inst__Unrecognized__,    // 02 56
    Inst__Unrecognized__,    // 02 57
    Inst__Unrecognized__,    // 02 58
    Inst__Unrecognized__,    // 02 59
    Inst__Unrecognized__,    // 02 5A
    Inst__Unrecognized__,    // 02 5B
    Inst__Unrecognized__,    // 02 5C
    Inst__Unrecognized__,    // 02 5D
    Inst__Unrecognized__,    // 02 5E
    Inst__Unrecognized__,    // 02 5F
    Inst__Unrecognized__,    // 02 60
    Inst__Unrecognized__,    // 02 61
    Inst__Unrecognized__,    // 02 62
    Inst__Unrecognized__,    // 02 63
    Inst__Unrecognized__,    // 02 64
    Inst__Unrecognized__,    // 02 65
    Inst__Unrecognized__,    // 02 66
    Inst__Unrecognized__,    // 02 67
    Inst__Unrecognized__,    // 02 68
    Inst__Unrecognized__,    // 02 69
    Inst__Unrecognized__,    // 02 6A
    Inst__Unrecognized__,    // 02 6B
    Inst__Unrecognized__,    // 02 6C
    Inst__Unrecognized__,    // 02 6D
    Inst__Unrecognized__,    // 02 6E
    Inst__Unrecognized__,    // 02 6F
    Inst__Unrecognized__,    // 02 70
    Inst__Unrecognized__,    // 02 71
    Inst__Unrecognized__,    // 02 72
    Inst__Unrecognized__,    // 02 73
    Inst__Unrecognized__,    // 02 74
    Inst__Unrecognized__,    // 02 75
    Inst__Unrecognized__,    // 02 76
    Inst__Unrecognized__,    // 02 77
    Inst__Unrecognized__,    // 02 78
    Inst__Unrecognized__,    // 02 79
    Inst__Unrecognized__,    // 02 7A
    Inst__Unrecognized__,    // 02 7B
    Inst__Unrecognized__,    // 02 7C
    Inst__Unrecognized__,    // 02 7D
    Inst__Unrecognized__,    // 02 7E
    Inst__Unrecognized__,    // 02 7F
    Inst__Unrecognized__,    // 02 80
    Inst__Unrecognized__,    // 02 81
    Inst__Unrecognized__,    // 02 82
    Inst__Unrecognized__,    // 02 83
    Inst__Unrecognized__,    // 02 84
    Inst__Unrecognized__,    // 02 85
    Inst__Unrecognized__,    // 02 86
    Inst__Unrecognized__,    // 02 87
    Inst__Unrecognized__,    // 02 88
    Inst__Unrecognized__,    // 02 89
    Inst__Unrecognized__,    // 02 8A
    Inst__Unrecognized__,    // 02 8B
    Inst__Unrecognized__,    // 02 8C
    Inst__Unrecognized__,    // 02 8D
    Inst__Unrecognized__,    // 02 8E
    Inst__Unrecognized__,    // 02 8F
    Inst__Unrecognized__,    // 02 90
    Inst__Unrecognized__,    // 02 91
    Inst__Unrecognized__,    // 02 92
    Inst__Unrecognized__,    // 02 93
    Inst__Unrecognized__,    // 02 94
    Inst__Unrecognized__,    // 02 95
    Inst__Unrecognized__,    // 02 96
    Inst__Unrecognized__,    // 02 97
    Inst__Unrecognized__,    // 02 98
    Inst__Unrecognized__,    // 02 99
    Inst__Unrecognized__,    // 02 9A
    Inst__Unrecognized__,    // 02 9B
    Inst__Unrecognized__,    // 02 9C
    Inst__Unrecognized__,    // 02 9D
    Inst__Unrecognized__,    // 02 9E
    Inst__Unrecognized__,    // 02 9F
    Inst__Unrecognized__,    // 02 A0
    Inst__Unrecognized__,    // 02 A1
    Inst__Unrecognized__,    // 02 A2
    Inst__Unrecognized__,    // 02 A3
    Inst__Unrecognized__,    // 02 A4
    Inst__Unrecognized__,    // 02 A5
    Inst__Unrecognized__,    // 02 A6
    Inst__Unrecognized__,    // 02 A7
    Inst__Unrecognized__,    // 02 A8
    Inst__Unrecognized__,    // 02 A9
    Inst__Unrecognized__,    // 02 AA
    Inst__Unrecognized__,    // 02 AB
    Inst__Unrecognized__,    // 02 AC
    Inst__Unrecognized__,    // 02 AD
    Inst__Unrecognized__,    // 02 AE
    Inst__Unrecognized__,    // 02 AF
    Inst__Unrecognized__,    // 02 B0
    Inst__Unrecognized__,    // 02 B1
    Inst__Unrecognized__,    // 02 B2
    Inst__Unrecognized__,    // 02 B3
    Inst__Unrecognized__,    // 02 B4
    Inst__Unrecognized__,    // 02 B5
    Inst__Unrecognized__,    // 02 B6
    Inst__Unrecognized__,    // 02 B7
    Inst__Unrecognized__,    // 02 B8
    Inst__Unrecognized__,    // 02 B9
    Inst__Unrecognized__,    // 02 BA
    Inst__Unrecognized__,    // 02 BB
    Inst__Unrecognized__,    // 02 BC
    Inst__Unrecognized__,    // 02 BD
    Inst__Unrecognized__,    // 02 BE
    Inst__Unrecognized__,    // 02 BF
    Inst__Unrecognized__,    // 02 C0
    Inst__Unrecognized__,    // 02 C1
    Inst__Unrecognized__,    // 02 C2
    Inst__Unrecognized__,    // 02 C3
    Inst__Unrecognized__,    // 02 C4
    Inst__Unrecognized__,    // 02 C5
    Inst__Unrecognized__,    // 02 C6
    Inst__Unrecognized__,    // 02 C7
    Inst__Unrecognized__,    // 02 C8
    Inst__Unrecognized__,    // 02 C9
    Inst__Unrecognized__,    // 02 CA
    Inst__Unrecognized__,    // 02 CB
    Inst__Unrecognized__,    // 02 CC
    Inst__Unrecognized__,    // 02 CD
    Inst__Unrecognized__,    // 02 CE
    Inst__Unrecognized__,    // 02 CF
    Inst__Unrecognized__,    // 02 D0
    Inst__Unrecognized__,    // 02 D1
    Inst__Unrecognized__,    // 02 D2
    Inst__Unrecognized__,    // 02 D3
    Inst__Unrecognized__,    // 02 D4
    Inst__Unrecognized__,    // 02 D5
    Inst__Unrecognized__,    // 02 D6
    Inst__Unrecognized__,    // 02 D7
    Inst__Unrecognized__,    // 02 D8
    Inst__Unrecognized__,    // 02 D9
    Inst__Unrecognized__,    // 02 DA
    Inst__Unrecognized__,    // 02 DB
    Inst__Unrecognized__,    // 02 DC
    Inst__Unrecognized__,    // 02 DD
    Inst__Unrecognized__,    // 02 DE
    Inst__Unrecognized__,    // 02 DF
    Inst__Unrecognized__,    // 02 E0
    Inst__Unrecognized__,    // 02 E1
    Inst__Unrecognized__,    // 02 E2
    Inst__Unrecognized__,    // 02 E3
    Inst__Unrecognized__,    // 02 E4
    Inst__Unrecognized__,    // 02 E5
    Inst__Unrecognized__,    // 02 E6
    Inst__Unrecognized__,    // 02 E7
    Inst__Unrecognized__,    // 02 E8
    Inst__Unrecognized__,    // 02 E9
    Inst__Unrecognized__,    // 02 EA
    Inst__Unrecognized__,    // 02 EB
    Inst__Unrecognized__,    // 02 EC
    Inst__Unrecognized__,    // 02 ED
    Inst__Unrecognized__,    // 02 EE
    Inst__Unrecognized__,    // 02 EF
    Inst__Unrecognized__,    // 02 F0
    Inst__Unrecognized__,    // 02 F1
    Inst__Unrecognized__,    // 02 F2
    Inst__Unrecognized__,    // 02 F3
    Inst__Unrecognized__,    // 02 F4
    Inst__Unrecognized__,    // 02 F5
    Inst__Unrecognized__,    // 02 F6
    Inst__Unrecognized__,    // 02 F7
    Inst__Unrecognized__,    // 02 F8
    Inst__Unrecognized__,    // 02 F9
    Inst__Unrecognized__,    // 02 FA
    Inst__Unrecognized__,    // 02 FB
    Inst__Unrecognized__,    // 02 FC
    Inst__Unrecognized__,    // 02 FD
    Inst__Unrecognized__,    // 02 FE
    Inst__Unrecognized__     // 02 FF
};

SC3Instruction* RNEDisassembler::DisassembleAt(SCXOffset address,
                                               SCXOffset maxLength) {
  uint8_t* inst = _file.getPData() + address;
  uint8_t opcodeMSB = inst[0];
  switch (opcodeMSB) {
    case 0:
      return DecoderTableSystem[inst[1]](inst, address, maxLength);
      break;
    case 1:
      return DecoderTableGraph[inst[1]](inst, address, maxLength);
      break;
    case 2:
      return DecoderTableGraph3D[inst[1]](inst, address, maxLength);
      break;
    case 0x10:
      return DecoderTableUser1[inst[1]](inst, address, maxLength);
      break;
    case 0xFE:
      return InstAssign(inst, address, maxLength);
      break;
    default:
      return Inst__Unrecognized__(inst, address, maxLength);
      break;
  }
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
