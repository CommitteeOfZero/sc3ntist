#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include "CommonDisassembler.inl"
#include "CCDisassembler.h"

I1(Save, BYTE, "arg")
I1(VoiceTableLoad, EXPRESSION, "arg1")
I1(SignIn, BYTE,
   "arg1")  // I *think* the length of this actually depends on build

DECODER_PROC(SystemMes) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  ADD_BYTE_ARG("arg2");
  switch (type) {
    case 0:
    case 1:
    case 5:
    case 6:
    case 7:
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
  uint8_t cond = *data++;
  { args.push_back(SC3ArgumentFromByte("condition", cond)); }
  if (cond == 0 || cond == 2 || cond == 3) {
    ADD_LOCAL_LABEL_ARG("target");
    ADD_LOCAL_LABEL_ARG("idk");
  }
  if (cond == 3) {
    ADD_LOCAL_LABEL_ARG("idk2");
  }
  RETURN_INSTRUCTION("PressStart");
}

DECODER_PROC(GlobalSystemMessage) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("GlobalSystemMessage_00");
      break;
    case 1:
      RETURN_INSTRUCTION("GlobalSystemMessage_01");
      break;
    case 2:
      RETURN_INSTRUCTION("GlobalSystemMessage_02");
      break;
    case 3:
      RETURN_INSTRUCTION("GlobalSystemMessage_03");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I4(CreateSurf, BYTE, "format", EXPRESSION, "surfaceId", EXPRESSION, "width",
   EXPRESSION, "height")
I1(MesSync, BYTE, "arg")

DECODER_PROC(SaveMenu) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
    case 0xA:
      ADD_BYTE_ARG("arg1");
      RETURN_INSTRUCTION("SaveMenuInit");
      break;
    case 1:
      RETURN_INSTRUCTION("SaveMenuMain");
      break;
    case 2:
      RETURN_INSTRUCTION("SaveMenu_SaveResetThumnail");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(LoadData) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  if (type == 0) {
    ADD_EXPRESSION_ARG("arg1");
    ADD_EXPRESSION_ARG("arg2");
    RETURN_INSTRUCTION("LoadData_00");
  }
  RETURN_INSTRUCTION("LoadData_01");
}

DECODER_PROC(NameID) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  if (type == 0) {
    ADD_LOCAL_LABEL_ARG("namePlateDataBlock");
    RETURN_INSTRUCTION("NameID");
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(MapSystem) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 1:
      RETURN_INSTRUCTION("MapInit");
      break;
    case 2:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapSetFadein");
    case 3:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      RETURN_INSTRUCTION("MapSetGroup");
      break;
    case 4:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapSetFadeout");
      break;
    case 5:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapSetDisp");
      break;
    case 6:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapSetHide");
      break;
    case 7:
      RETURN_INSTRUCTION("MapFadeEndChk_Wait");
      break;
    case 8:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("MapMoveAnimeInit");
      break;
    case 9:
      RETURN_INSTRUCTION("MapMoveAnimeMain");
      break;
    case 0xA:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapGetPos");
      break;
    case 0xB:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      RETURN_INSTRUCTION("MapSetPool");
      break;
    case 0xC:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("MapResetPoolAll");
      break;
    case 0xD:
      RETURN_INSTRUCTION("MapPoolFadeEndChk_Wait");
      break;
    case 0xE:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("MapPoolShuffle");
      break;
    case 0xF:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapPoolSetDisp");
      break;
    case 0x10:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapPoolSetHide");
      break;
    case 0x11:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapPoolSetFadein");
      break;
    case 0x12:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapPoolSetFadeout");
      break;
    case 0x13:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("MapPlayerPhotoSelect");
      break;
    case 0x14:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MapResetPool");
      break;
    case 0x15:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("MapSetGroupEx");
      break;
    case 0x16:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("MapZoomInit");
      break;
    case 0x17:
      RETURN_INSTRUCTION("MapZoomMain");
      break;
    case 0x18:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("MapZoomInit2");
      break;
    case 0x19:
      RETURN_INSTRUCTION("MapZoomMain3");
      break;
    case 0x1A:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("MapZoomInit3");
      break;
    case 0x1B:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("MapMoveAnimeInit2");
      break;
    case 0x1C:
      RETURN_INSTRUCTION("MapMoveAnimeMain2");
      break;
    case 0x1E:
      RETURN_INSTRUCTION("MapPlayerPotalSelectInit");
      break;
    case 0x1F:
      RETURN_INSTRUCTION("MapPlayerPotalSelect");
      break;
    case 0x28:
      RETURN_INSTRUCTION("MapSystem_28");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(Mtrg) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("MtrgStart");
      break;
    case 1:
      RETURN_INSTRUCTION("MtrgEnd_Wait");
      break;
    case 2:
      RETURN_INSTRUCTION("Mtrg_02");
      break;
    case 3:
      RETURN_INSTRUCTION("MtrgStop_Wait");
      break;
    case 4:
      RETURN_INSTRUCTION("Mtrg_04_Wait");
      break;
    case 5:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("MtrgSetEvent");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(IOS) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("urlId");
      RETURN_INSTRUCTION("IOSStartWebBrowser");
      break;
    case 1:
      RETURN_INSTRUCTION("IOSShowAchievementsUI");
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
    Inst__Unrecognized__,     // 00 09
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
    InstVoiceTableLoad,       // 00 31
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
    InstAwardGamerIcon,       // 00 41
    InstSetX360SysMesPos,     // 00 42
    InstSystemMes,            // 00 43
    InstSystemMenu,           // 00 44
    InstGetNowTime,           // 00 45
    InstGetSystemStatus,      // 00 46
    InstReboot,               // 00 47
    InstReloadScript,         // 00 48
    InstReloadScriptMenu,     // 00 49
    InstDebugEditer,          // 00 4A
    InstSysSEload,            // 00 4B
    Inst__Unrecognized__,     // 00 4C
    InstSysVoicePlay,         // 00 4D
    InstPadActEx,             // 00 4E
    InstDebugSetup,           // 00 4F
    InstPressStart,           // 00 50
    InstGlobalSystemMessage,  // 00 51
    InstHelpDisp,             // 00 52
    InstClickOnJump,          // 00 53
    InstKeyboardOnJump,       // 00 54
    InstControlOnJump,        // 00 55
    InstGetControl,           // 00 56
    Inst__Unrecognized__,     // 00 57
    Inst__Unrecognized__,     // 00 58
    Inst__Unrecognized__,     // 00 59
    Inst__Unrecognized__,     // 00 5A
    Inst__Unrecognized__,     // 00 5B
    Inst__Unrecognized__,     // 00 5C
    Inst__Unrecognized__,     // 00 5D
    Inst__Unrecognized__,     // 00 5E
    InstPackFileAddBind,      // 00 5F
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
    Inst__Unrecognized__,  // 01 1F
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
    Inst__Unrecognized__,  // 01 2C
    Inst__Unrecognized__,  // 01 2D
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
    InstCHAload,           // 10 05
    InstCHAswap,           // 10 06
    InstBGcopy,            // 10 07
    InstCHAcopy,           // 10 08
    InstSaveSlot,          // 10 09
    InstSystemMain,        // 10 0A
    InstCharaLayerLoad,    // 10 0B
    InstGameInfoInit,      // 10 0C
    InstCHAmove,           // 10 0D
    InstBGloadEx,          // 10 0E
    Inst__Unrecognized__,  // 10 0F
    InstBGrelease,         // 10 10
    InstCHArelease,        // 10 11
    InstClearFlagChk,      // 10 12
    InstOption,            // 10 13
    InstSystemDataReset,   // 10 14
    InstDebugData,         // 10 15
    InstGetCharaPause,     // 10 16
    InstBGfadeExpInit,     // 10 17
    Inst__Unrecognized__,  // 10 18
    Inst__Unrecognized__,  // 10 19
    InstHelp,              // 10 1A
    InstAchievementMenu,   // 10 1B
    InstActorVoiceMenu,    // 10 1C
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
    Inst__Unrecognized__,  // 10 31
    InstNameID,            // 10 32
    InstTips,              // 10 33
    InstTitleMenu,         // 10 34
    Inst__Unrecognized__,  // 10 35
    InstBGeffect,          // 10 36
    InstMapSystem,         // 10 37
    InstMtrg,              // 10 38
    InstIOS,               // 10 39
    Inst__Unrecognized__,  // 10 3A
    Inst__Unrecognized__,  // 10 3B
    Inst__Unrecognized__,  // 10 3C
    Inst__Unrecognized__,  // 10 3D
    Inst__Unrecognized__,  // 10 3E
    Inst__Unrecognized__,  // 10 3F
    InstScreenChange,      // 10 40
    InstExitGame,          // 10 41
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

SC3Instruction* CCDisassembler::DisassembleAt(SCXOffset address,
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