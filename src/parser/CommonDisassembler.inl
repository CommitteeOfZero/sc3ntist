#include <string>
#include <vector>
#include "SC3Expression.h"
#include "SC3Argument.h"
#include "SC3Instruction.h"
#include "DisassemblerMacros.h"

DECODER_PROC(Assign) {
  uint8_t* dataStart = data;
  data++;
  std::vector<SC3Argument> args;
  ADD_EXPRESSION_ARG("expr");
  RETURN_INSTRUCTION("Assign");
}

/*
DECODER_PROC(EndOfScript) {
DECODER_PROC_INIT();
ADD_BYTE_ARG("zero");
RETURN_INSTRUCTION("EndOfScript");
}
*/

I0(End)

I2(CreateThread, EXPRESSION, "threadId", FAR_LABEL, "entrypoint")
I1(KillThread, EXPRESSION, "threadId")
I2(ScriptLoad, EXPRESSION, "scriptBuffer", EXPRESSION, "scriptFile")
I1(Wait, EXPRESSION, "cycles")
I1(Jump, LOCAL_LABEL, "target")
I2(JumpTable, EXPRESSION, "index", LOCAL_LABEL, "jumpTable")
I1(GetLabelAdr, LOCAL_LABEL, "labelNum")
I3(If, BYTE, "conditionExpectedTrue", EXPRESSION, "condition", LOCAL_LABEL,
   "target")
I2(Loop, LOCAL_LABEL, "target", EXPRESSION, "count")
I3(FlagOnJump, BYTE, "condition", EXPR_FLAG_REF, "flag", LOCAL_LABEL, "target")
I0(Reset)
I0(Halt)
I0(Return)
I2(Call, LOCAL_LABEL, "target", RETURN_ADDRESS, "returnAddress")
I1(JumpFar, FAR_LABEL, "target")
I2(CallFar, FAR_LABEL, "target", RETURN_ADDRESS, "returnAddress")
I2(FlagOnWait, BYTE, "condition", EXPR_FLAG_REF, "flag")
I1(SetFlag, EXPR_FLAG_REF, "flag")
I1(ResetFlag, EXPR_FLAG_REF, "flag")
I2(CopyFlag, EXPR_FLAG_REF, "src", EXPR_FLAG_REF, "dest")

DECODER_PROC(BGMplay) {
  DECODER_PROC_INIT();
  uint8_t loop = *data++;
  { args.push_back(SC3ArgumentFromByte("loop", loop)); }
  ADD_EXPRESSION_ARG("track");
  if (loop == 2) {
    ADD_EXPRESSION_ARG("unk");
  }
  RETURN_INSTRUCTION("BGMplay");
}

I1(BGMstop, BYTE, "zero")

DECODER_PROC(SEplay) {
  DECODER_PROC_INIT();
  ADD_BYTE_ARG("channel");
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  if (type != 2) {
    ADD_EXPRESSION_ARG("effect");
    ADD_EXPRESSION_ARG("loop");
  }
  RETURN_INSTRUCTION("SEplay");
}

I1(SEstop, BYTE, "channel")
I3(PadAct, EXPRESSION, "unused", EXPRESSION, "vib1", EXPRESSION, "vib2")
I1(SSEplay, EXPRESSION, "value")
I0(SSEstop)
I4(KeyOnJump, BYTE, "arg1", EXPRESSION, "arg2", EXPRESSION, "arg3", LOCAL_LABEL,
   "target")
I3(KeyWait, BYTE, "type", EXPRESSION, "arg1", EXPRESSION, "arg2")
I4(KeyWaitTimer, BYTE, "type", EXPRESSION, "timer", EXPRESSION, "arg1",
   EXPRESSION, "arg2")
I2(MemberWrite, EXPRESSION, "destination", EXPRESSION, "value")
I2(ThreadControl, EXPRESSION, "arg1", EXPRESSION, "arg2")
I0(GetSelfPointer)
I2(LoadJump, EXPRESSION, "arg1", UINT16, "arg2")
I0(Vsync)
I1(LongAssign, EXPRESSION, "arg")
I1(Test, EXPRESSION, "arg")

DECODER_PROC(ThreadControlStore) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("ThreadControlRestore");
      break;
    case 1:
      RETURN_INSTRUCTION("ThreadControlStore");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I1(Switch, EXPRESSION, "expr")
I2(Case, EXPRESSION, "case", LOCAL_LABEL, "target")
I4(CopyThreadWork, EXPRESSION, "arg1", EXPRESSION, "arg2", EXPRESSION, "arg3",
   EXPRESSION, "loopCount")
I1(UPLmenuUI, BYTE, "arg")
I1(SaveIconLoad, BYTE, "arg")
I1(BGMflag, EXPRESSION, "arg")
I1(UPLxTitle, BYTE, "arg")

DECODER_PROC(Achievement) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  if (type == 1) {
    ADD_EXPRESSION_ARG("arg1");
  }
  RETURN_INSTRUCTION("Achievement");
}

I1(SetPlayer, BYTE, "arg1")
I0(SetPadCustom)
I2(Mwait, EXPRESSION, "delay", EXPRESSION, "unused")
I0(Terminate)
I2(Presence, BYTE, "notDoSet", EXPRESSION, "mode")
I1(AchievementIcon, BYTE, "arg1")
I3(VoicePlay, BYTE, "channel", EXPRESSION, "arg1", EXPRESSION, "arg2")
I2(VoiceStop, BYTE, "channel", EXPRESSION, "arg1")
I1(VoicePlayWait, BYTE, "channel")

DECODER_PROC(BGMduelPlay) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("BGMduelPlay_00");
      break;
    case 1:
      RETURN_INSTRUCTION("BGMduelPlay_01");
      break;
    case 2:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("BGMduelPlay_02");
      break;
    case 3:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("BGMduelPlay_03");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I1(SNDpause, BYTE, "type")
I1(SEplayWait, BYTE, "channel")
I2(DebugPrint, EXPRESSION, "a", EXPRESSION, "b")
I0(ResetSoundAll)
I0(SNDloadStop)
I0(BGMstopWait)
I1(AwardGamerIcon, BYTE, "arg1")
I1(SetX360SysMesPos, EXPRESSION, "arg1")

DECODER_PROC(SystemMenu) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("SystemMenuInit");
      break;
    case 1:
      RETURN_INSTRUCTION("SystemMenuMain");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I0(GetNowTime)
I1(GetSystemStatus, EXPRESSION, "system")
I0(Reboot)
I0(ReloadScript)
I1(ReloadScriptMenu, BYTE, "arg1")

DECODER_PROC(DebugEditer) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("DebugEditerInit");
      break;
    case 1:
      RETURN_INSTRUCTION("DebugEditerMain");
      break;
    case 2:
      RETURN_INSTRUCTION("DebugEditerLoad");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I0(SysSEload)
I2(SysVoicePlay, BYTE, "arg1", EXPRESSION, "arg2")
I3(PadActEx, EXPRESSION, "arg1", EXPRESSION, "arg2", EXPRESSION, "arg3")
I1(DebugSetup, EXPRESSION, "arg")

I0(HelpDisp)

DECODER_PROC(ClickOnJump) {
  DECODER_PROC_INIT();
  uint8_t arg1 = *data++;
  { args.push_back(SC3ArgumentFromByte("arg1", arg1)); }
  if ((arg1 & 0xFE) == 2) {
    ADD_EXPRESSION_ARG("_arg1");
    ADD_EXPRESSION_ARG("_arg2");
    ADD_EXPRESSION_ARG("_arg3");
  }
  ADD_EXPRESSION_ARG("arg2");
  ADD_LOCAL_LABEL_ARG("arg3");
  RETURN_INSTRUCTION("ClickOnJump");
}

I4(KeyboardOnJump, BYTE, "arg1", EXPRESSION, "arg2", EXPRESSION, "arg3",
   LOCAL_LABEL, "arg4")
I3(ControlOnJump, BYTE, "arg1", EXPRESSION, "arg2", LOCAL_LABEL, "target")
I2(GetControl, EXPRESSION, "arg1", EXPR_GLOBAL_VAR_REF, "destination")
I0(PackFileAddBind)

I1(ReleaseSurf, EXPRESSION, "surfaceId")
I3(LoadPic, EXPRESSION, "surfaceId", EXPRESSION, "archiveId", EXPRESSION,
   "textureFileId")
I5(SurfFill, EXPRESSION, "surfaceId", EXPRESSION, "arg2", EXPRESSION, "arg3",
   EXPRESSION, "arg4", EXPRESSION, "arg5")

DECODER_PROC(Calc) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      ADD_EXPRESSION_ARG("angle");
      RETURN_INSTRUCTION("CalcSin");
      break;
    case 1:
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      ADD_EXPRESSION_ARG("angle");
      RETURN_INSTRUCTION("CalcCos");
      break;
    case 2:
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      ADD_EXPRESSION_ARG("x");
      ADD_EXPRESSION_ARG("y");
      RETURN_INSTRUCTION("CalcAtan");
      break;
    case 3:
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      ADD_EXPRESSION_ARG("base");
      ADD_EXPRESSION_ARG("angle");
      ADD_EXPRESSION_ARG("offset");
      RETURN_INSTRUCTION("CalcSinL");
      break;
    case 4:
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      ADD_EXPRESSION_ARG("base");
      ADD_EXPRESSION_ARG("angle");
      ADD_EXPRESSION_ARG("offset");
      RETURN_INSTRUCTION("CalcCosL");
      break;
    case 5:
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      ADD_EXPRESSION_ARG("value");
      ADD_EXPRESSION_ARG("multiplier");
      ADD_EXPRESSION_ARG("divider");
      RETURN_INSTRUCTION("CalcRound");
      break;
    case 6:
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      ADD_EXPRESSION_ARG("x");
      ADD_EXPRESSION_ARG("a");
      ADD_EXPRESSION_ARG("b");
      RETURN_INSTRUCTION("CalcAccel");
      break;
    default:
      RETURN_UNRECOGNIZED();
  }
}

DECODER_PROC(MesViewFlag) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MesViewFlag_Set");
      break;
    case 1:
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      RETURN_INSTRUCTION("MesViewFlag_Chk");
      break;
    default:
      RETURN_UNRECOGNIZED();
  }
}

I3(SetMesWinPri, EXPRESSION, "arg1", EXPRESSION, "arg2", EXPRESSION, "unused")

DECODER_PROC(MesSetID) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_UINT16_ARG("savePointId");
      RETURN_INSTRUCTION("MesSetID_SetSavePoint");
      break;
    case 1:
      ADD_UINT16_ARG("savePointId");
      ADD_EXPRESSION_ARG("unk");
      RETURN_INSTRUCTION("MesSetID_SetSavePoint1");
      break;
    case 2:
      ADD_EXPRESSION_ARG("unk");
      RETURN_INSTRUCTION("MetSetID_02");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(MesCls) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  if ((type & 0xFE) != 4 && !(type & 1)) {
    ADD_EXPRESSION_ARG("arg1");
  }
  RETURN_INSTRUCTION("MesCls");
}

I0(MesVoiceWait)

DECODER_PROC(Mes) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_EXPRESSION_ARG("characterId");
      ADD_STRING_REF_ARG("line");
      RETURN_INSTRUCTION("Mes_LoadDialogue");
      break;
    case 1:
      ADD_EXPRESSION_ARG("audioId");
      ADD_EXPRESSION_ARG("characterId");
      ADD_STRING_REF_ARG("line");
      RETURN_INSTRUCTION("Mes_LoadVoicedUnactedDialogueMaybe");
      break;
    case 3:
      ADD_EXPRESSION_ARG("audioId");
      ADD_EXPRESSION_ARG("animationId");
      ADD_EXPRESSION_ARG("characterId");
      ADD_STRING_REF_ARG("line");
      RETURN_INSTRUCTION("Mes_LoadVoicedDialogue");
      break;
    case 0x0B:
      ADD_EXPRESSION_ARG("audioId");
      ADD_EXPRESSION_ARG("animationId");
      ADD_EXPRESSION_ARG("characterId");
      ADD_STRING_REF_ARG("line");
      RETURN_INSTRUCTION("Mes_LoadDialogue0B");
      break;
    default:
      RETURN_UNRECOGNIZED();
  }
}

DECODER_PROC(MesMain) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  if (type == 0) {
    RETURN_INSTRUCTION("MesMain_DisplayDialogue");
  }
  RETURN_UNRECOGNIZED();
}

I2(SetMesModeFormat, EXPRESSION, "id", LOCAL_LABEL, "modeDataBlock")
I2(SetNGmoji, STRING_REF, "unk1", STRING_REF, "unk2")

DECODER_PROC(MesRev) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("MesRev_DispInit");
      break;
    case 1:
      RETURN_INSTRUCTION("MesRev_Main");
      break;
    case 2:
      RETURN_INSTRUCTION("MesRev_AllCls");
      break;
    case 3:
      RETURN_INSTRUCTION("MesRev_ChkLoad");
      break;
    case 4:
      RETURN_INSTRUCTION("MesRev_SAVELoad");
      break;
    case 5:
      RETURN_INSTRUCTION("MesRev_SoundUnk");
      break;
    case 0xA:
      RETURN_INSTRUCTION("MesRev_DispInit");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(MessWindow) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("MessWindow_HideCurrent");
      break;
    case 1:
      RETURN_INSTRUCTION("MessWindow_ShowCurrent");
      break;
    case 2:
      RETURN_INSTRUCTION("MessWindow_AwaitShowCurrent");
      break;
    case 3:
      RETURN_INSTRUCTION("MessWindow_AwaitHideCurrent");
      break;
    case 4:
      RETURN_INSTRUCTION("MessWindow_Current04");
      break;
    case 5:
      ADD_EXPRESSION_ARG("messWindowId");
      RETURN_INSTRUCTION("MessWindow_Hide");
      break;
    case 6:
      ADD_EXPRESSION_ARG("messWindowId");
      RETURN_INSTRUCTION("MessWindow_HideSlow");
      break;
    case 7:
      ADD_EXPRESSION_ARG("messWindowId");
      RETURN_INSTRUCTION("MessWindow_HideSlow");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(Sel) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  ADD_STRING_REF_ARG("arg1");
  if (type == 0 || type == 2) {
    ADD_EXPRESSION_ARG("arg2");
  }
  RETURN_INSTRUCTION("Sel");
}

DECODER_PROC(Select) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  if (type == 2) {
    ADD_EXPRESSION_ARG("arg1");
  }
  RETURN_INSTRUCTION("Select");
}

DECODER_PROC(SysSel) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  if (type >= 2) {
    ADD_STRING_REF_ARG("arg1");
  }
  RETURN_INSTRUCTION("SysSel");
}

DECODER_PROC(SysSelect) {
  DECODER_PROC_INIT();
  uint8_t arg1 = *data++;
  { args.push_back(SC3ArgumentFromByte("arg1", arg1)); }
  switch (arg1 & 0xF) {
    case 0:
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      RETURN_INSTRUCTION("SysSelect");
      break;
    case 2:
    case 3:
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      RETURN_INSTRUCTION("SysSelect");
      break;
  }
  RETURN_INSTRUCTION("SysSelect");
}

I1(SCcapture, EXPRESSION, "bufferId")
I2(SetTextTable, EXPRESSION, "instanceId", LOCAL_LABEL, "dataBlock")

DECODER_PROC(PlayMovie) {
  DECODER_PROC_INIT();
  uint8_t playMode = *data++;
  { args.push_back(SC3ArgumentFromByte("playMode", playMode)); }
  if (playMode == 99) {
    ADD_EXPRESSION_ARG("playModeEx");
    ADD_EXPRESSION_ARG("playView");
  } else {
    ADD_BYTE_ARG("playView");
  }
  ADD_EXPRESSION_ARG("playNo");
  ADD_EXPRESSION_ARG("movCancelFlag");
  RETURN_INSTRUCTION("PlayMovie");
}

DECODER_PROC(MovieMain) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 2:
      RETURN_INSTRUCTION("MovieMain_Stop");
      break;
    case 3:
      RETURN_INSTRUCTION("MovieMain_StopWaitForSomething");
      break;
    default: { args.push_back(SC3ArgumentFromByte("type", type)); }
      RETURN_INSTRUCTION("MovieMain_Unk");
      break;
  }
}

I1(LoadMovie, EXPRESSION, "movieId")

DECODER_PROC(SetRevMes) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_STRING_REF_ARG("messageId");
      RETURN_INSTRUCTION("SetRevMes_Default");
      break;
    case 1:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_STRING_REF_ARG("messageId");
      RETURN_INSTRUCTION("SetRevMes");
      break;
    case 2:
      ADD_STRING_REF_ARG("arg1");
      RETURN_INSTRUCTION("SetRevMes_02");
      break;
    case 3:
      ADD_STRING_REF_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      RETURN_INSTRUCTION("SetRevMes_03");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(PlayMovieMemory) {
  DECODER_PROC_INIT();
  uint8_t playMode = *data++;
  { args.push_back(SC3ArgumentFromByte("playMode", playMode)); }
  if (playMode == 99) {
    ADD_EXPRESSION_ARG("playModeEx");
    ADD_EXPRESSION_ARG("playView");
  } else {
    ADD_BYTE_ARG("playView");
  }
  ADD_EXPRESSION_ARG("movCancelFlag");
  RETURN_INSTRUCTION("PlayMovieMemory");
}

I1(SFDpause, BYTE, "paused")
I1(MSinit, EXPRESSION, "subsystemId")
I2(BGload, EXPRESSION, "bufferId", EXPRESSION, "backgroundId")
I2(BGswap, EXPRESSION, "firstBufferId", EXPRESSION, "secondBufferId")
I2(BGsetColor, EXPRESSION, "bufferId", EXPRESSION, "color")

DECODER_PROC(BGsetLink) {
  DECODER_PROC_INIT();
  uint8_t id = *data++;
  { args.push_back(SC3ArgumentFromByte("id", id)); }
  ADD_EXPRESSION_ARG("arg1");
  ADD_EXPRESSION_ARG("arg2");
  if (id >= 4) {
    ADD_EXPRESSION_ARG("arg3");
  }
  ADD_EXPRESSION_ARG("arg4");
  RETURN_INSTRUCTION("BGsetLink");
}

DECODER_PROC(CHAload) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  ADD_EXPRESSION_ARG("bufferId");
  ADD_EXPRESSION_ARG("spriteId");
  if (type == 0) {
    ADD_UINT16_ARG("unused");
  }
  RETURN_INSTRUCTION("CHAload");
}

I2(CHAswap, EXPRESSION, "firstBufferId", EXPRESSION, "secondBufferId")
I2(CHAcopy, EXPRESSION, "srcBufferId", EXPRESSION, "destBufferId")
I0(CharaLayerLoad)
I2(BGcopy, EXPRESSION, "srcBufferId", EXPRESSION, "destBufferId")
I2(SaveSlot, EXPRESSION, "arg1", EXPRESSION, "arg2")
I0(SystemMain)
I1(GameInfoInit, BYTE, "arg")
I2(BGloadEx, EXPRESSION, "arg1", EXPRESSION, "arg2")
I1(BGrelease, EXPRESSION, "bufferId")
I0(ClearFlagChk)

DECODER_PROC(CHAmove) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("CHAmoveInit");
      break;
    case 1:
      ADD_EXPRESSION_ARG("arg1");
      ADD_LOCAL_LABEL_ARG("seqDataBlock");
      RETURN_INSTRUCTION("CHAmoveSetSeq");
      break;
    case 2:
      ADD_EXPRESSION_ARG("arg1");
      RETURN_INSTRUCTION("CHAmoveExec_Blocking");
      break;
    case 3:
      RETURN_INSTRUCTION("CHAmoveExec_NonBlocking");
      break;
    case 4:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
      RETURN_INSTRUCTION("CHAmoveChkEnd");
      break;
    case 5:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      ADD_EXPRESSION_ARG("arg6");
      ADD_EXPRESSION_ARG("arg7");
      RETURN_INSTRUCTION("CHAmoveSetSeqDirect");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I1(CHArelease, EXPRESSION, "bufferId")

DECODER_PROC(Option) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
    case 0xA:
      RETURN_INSTRUCTION("OptionInit");
      break;
    case 1:
      RETURN_INSTRUCTION("OptionMain");
      break;
    case 2:
      RETURN_INSTRUCTION("OptionCancel");
      break;
    case 3:
      RETURN_INSTRUCTION("Option_V2toV1vol");
      break;
    case 4:
      RETURN_INSTRUCTION("OptionDefault");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I1(SystemDataReset, BYTE, "type")
I1(DebugData, BYTE, "arg1")
I2(GetCharaPause, EXPRESSION, "arg1", EXPR_GLOBAL_VAR_REF, "destination")
I1(BGfadeExpInit, EXPRESSION, "arg1")

DECODER_PROC(Help) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("HelpInit");
      break;
    case 1:
      RETURN_INSTRUCTION("HelpMain");
      break;
    case 4:
      RETURN_INSTRUCTION("Help_DisplayModeInit");
      break;
    case 5:
      RETURN_INSTRUCTION("Help_DisplayModeMain");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I1(AchievementMenu, BYTE, "arg1")
I1(ActorVoiceMenu, BYTE, "arg1")

DECODER_PROC(SoundMenu) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("SoundMenu_MusicInit");
      break;
    case 1:
      RETURN_INSTRUCTION("SoundMenu_MusicMain");
      break;
    case 10:
      RETURN_INSTRUCTION("SoundMenu_ProfSetXboxEvent");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(Album) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("Album_EXmenuInit");
      break;
    case 1:
      RETURN_INSTRUCTION("Album_EXmenuMain");
      break;
    case 10:
      RETURN_INSTRUCTION("Album_ProfSetXboxEvent");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(ClistInit) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("ClistInit_PDmenuInit");
      break;
    case 1:
      RETURN_INSTRUCTION("ClistInit_PlayDataMain");
      break;
    case 3:
      RETURN_INSTRUCTION("ClistInit_PDmenuInit2");
      break;
    case 10:
      RETURN_INSTRUCTION("ClistInit_ProfSetXboxEvent");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(AutoSave) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("AutoSave_QuickSave");
      break;
    case 1:
      RETURN_INSTRUCTION("AutoSave_01");
      break;
    case 2:
      RETURN_INSTRUCTION("AutoSave_02");
      break;
    case 3:
      RETURN_INSTRUCTION("AutoSave_03");
      break;
    case 5:
      RETURN_INSTRUCTION("AutoSave_05");
      break;
    case 0x0A:
      ADD_UINT16_ARG("checkpointId");
      RETURN_INSTRUCTION("AutoSave_SaveState");
      break;
    case 0x14:
      RETURN_INSTRUCTION("AutoSave_14");
      break;
    case 0x15:
      RETURN_INSTRUCTION("AutoSave_15");
      break;
    case 0xFF:
      RETURN_INSTRUCTION("AutoSave_FF");
      break;
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
      RETURN_INSTRUCTION("AutoSave_NotImplemented");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I0(AllClear)

DECODER_PROC(MovieMode) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("MovieModeInit");
      break;
    case 1:
      RETURN_INSTRUCTION("MovieModeMain");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(SetDic) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  { args.push_back(SC3ArgumentFromByte("type", type)); }
  ADD_EXPRESSION_ARG("tip");
  if (type == 1) {
    ADD_EXPR_FLAG_REF_ARG("flag");
  }
  if (type == 0) {
    RETURN_INSTRUCTION("SetDic_NewTip");
  }
  RETURN_INSTRUCTION("SetDic");
}

I1(SetPlayMode, EXPRESSION, "arg")
I1(SetEVflag, EXPRESSION, "EVflag")
I4(SetCutin, BYTE, "type", EXPRESSION, "arg1", EXPRESSION, "arg2", EXPRESSION,
   "arg3")
I0(AchChkTitle)

DECODER_PROC(SetSceneViewFlag) {
  DECODER_PROC_INIT();
  ADD_EXPRESSION_ARG("sceneId");
  RETURN_INSTRUCTION("SetSceneViewFlag");
}

I0(ChkClearFlag)

DECODER_PROC(BGeffectWave) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("BGwaveInitWave");
      break;
    case 1:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      RETURN_INSTRUCTION("BGwaveSetWave");
      break;
    case 2:
      RETURN_INSTRUCTION("CHAwaveInitWave");
      break;
    case 3:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      RETURN_INSTRUCTION("CHAwaveSetWave");
      break;
    case 4:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      ADD_EXPRESSION_ARG("arg6");
      RETURN_INSTRUCTION("BGwaveResetWave");
      break;
    case 5:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      ADD_EXPRESSION_ARG("arg6");
      RETURN_INSTRUCTION("CHAwaveResetWave");
      break;
    case 0xA:
      RETURN_INSTRUCTION("EFFwaveInitWave");
      break;
    case 0xB:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      RETURN_INSTRUCTION("EFFwaveSetWave");
      break;
    case 0xC:
      ADD_EXPRESSION_ARG("arg1");
      ADD_EXPRESSION_ARG("arg2");
      ADD_EXPRESSION_ARG("arg3");
      ADD_EXPRESSION_ARG("arg4");
      ADD_EXPRESSION_ARG("arg5");
      ADD_EXPRESSION_ARG("arg6");
      RETURN_INSTRUCTION("EFFwaveResetWave");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(Tips) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      ADD_LOCAL_LABEL_ARG("tipsDataBlock");
      RETURN_INSTRUCTION("TipsDataInit");
      break;
    case 1:
      RETURN_INSTRUCTION("TipsInit");
      break;
    case 2:
      RETURN_INSTRUCTION("TipsMain");
      break;
    case 3:
      RETURN_INSTRUCTION("TipsEnd");
      break;
    case 4:
      RETURN_INSTRUCTION("TipsSet");
      break;
    case 10:
      RETURN_INSTRUCTION("Tips_ProfSetXboxEvent");
      break;
  }
  RETURN_UNRECOGNIZED();
}

DECODER_PROC(TitleMenu) {
  DECODER_PROC_INIT();
  uint8_t type = *data++;
  switch (type) {
    case 0:
      RETURN_INSTRUCTION("TitleMenuInit");
      break;
    case 1:
      RETURN_INSTRUCTION("TitleMenuMain");
      break;
    case 3:
      RETURN_INSTRUCTION("TitleMenuInit2");
      break;
  }
  RETURN_UNRECOGNIZED();
}

I1(BGeffect, BYTE, "arg1")
I0(ScreenChange)
I0(ExitGame)