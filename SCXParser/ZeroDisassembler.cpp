#include "ZeroDisassembler.h"
#include "DisassemblerMacros.h"

#pragma region Instruction decoders

DECODER_PROC(Assign) {
	uint8_t* dataStart = data;
	data++;
	std::vector<SC3Argument*> args;
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

NO_ARGS_DECODER_PROC(EndOfScript);

DECODER_PROC(CreateThread) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("threadId");
	ADD_FAR_LABEL_ARG("entrypoint");
	RETURN_INSTRUCTION("CreateThread");
}

DECODER_PROC(TerminateThread) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("threadId");
	RETURN_INSTRUCTION("TerminateThread");
}

DECODER_PROC(LoadScript) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("scriptBuffer");
	ADD_EXPRESSION_ARG("scriptFile");
	RETURN_INSTRUCTION("LoadScript");
}

DECODER_PROC(Sleep) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("cycles");
	RETURN_INSTRUCTION("Sleep");
}

DECODER_PROC(Jump) {
	DECODER_PROC_INIT();
	ADD_LOCAL_LABEL_ARG("target");
	RETURN_INSTRUCTION("Jump");
}

DECODER_PROC(JumpTable) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("index");
	ADD_LOCAL_LABEL_ARG("jumpTable");
	RETURN_INSTRUCTION("JumpTable");
}

DECODER_PROC(JumpIf) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("conditionExpectedTrue");
	ADD_EXPRESSION_ARG("condition");
	ADD_LOCAL_LABEL_ARG("target");
	RETURN_INSTRUCTION("JumpIf");
}

DECODER_PROC(LoopTimes) {
	DECODER_PROC_INIT();
	ADD_LOCAL_LABEL_ARG("target");
	ADD_EXPRESSION_ARG("count");
	RETURN_INSTRUCTION("LoopTimes");
}

DECODER_PROC(JumpIfFlag) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("condition");
	ADD_EXPR_FLAG_REF_ARG("flag");
	ADD_LOCAL_LABEL_ARG("target");
	RETURN_INSTRUCTION("JumpIfFlag");
}

NO_ARGS_DECODER_PROC(ExitThread);
NO_ARGS_DECODER_PROC(Halt);
NO_ARGS_DECODER_PROC(Return);

DECODER_PROC(Call) {
	DECODER_PROC_INIT();
	ADD_LOCAL_LABEL_ARG("target");
	ADD_RETURN_ADDRESS_ARG("returnAddress");
	RETURN_INSTRUCTION("Call");
}

DECODER_PROC(JumpFar) {
	DECODER_PROC_INIT();
	ADD_FAR_LABEL_ARG("target");
	RETURN_INSTRUCTION("JumpFar");
}

DECODER_PROC(CallFar) {
	DECODER_PROC_INIT();
	ADD_FAR_LABEL_ARG("target");
	ADD_RETURN_ADDRESS_ARG("returnAddress");
	RETURN_INSTRUCTION("CallFar");
}

DECODER_PROC(WaitForFlag) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("condition");
	ADD_EXPR_FLAG_REF_ARG("flag");
	RETURN_INSTRUCTION("WaitForFlag");
}

DECODER_PROC(SetFlag) {
	DECODER_PROC_INIT();
	ADD_EXPR_FLAG_REF_ARG("flag");
	RETURN_INSTRUCTION("SetFlag");
}

DECODER_PROC(ResetFlag) {
	DECODER_PROC_INIT();
	ADD_EXPR_FLAG_REF_ARG("flag");
	RETURN_INSTRUCTION("ResetFlag");
}

DECODER_PROC(CopyFlag) {
	DECODER_PROC_INIT();
	ADD_EXPR_FLAG_REF_ARG("src");
	ADD_EXPR_FLAG_REF_ARG("dest");
	RETURN_INSTRUCTION("CopyFlag");
}

DECODER_PROC(PlayBgm) {
	DECODER_PROC_INIT();
	uint8_t loop = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("loop", loop);
		args.push_back(arg);
	}
	ADD_EXPRESSION_ARG("track");
	if (loop == 2) {
		ADD_EXPRESSION_ARG("unk");
	}
	RETURN_INSTRUCTION("PlayBgm");
}

DECODER_PROC(StopBgm) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("zero");
	RETURN_INSTRUCTION("StopBgm");
}

DECODER_PROC(PlaySoundEffect) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("channel");
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type != 2) {
		ADD_EXPRESSION_ARG("effect");
		ADD_EXPRESSION_ARG("loop");
	}
	RETURN_INSTRUCTION("PlaySoundEffect");
}

DECODER_PROC(StopSoundEffect) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("channel");
	RETURN_INSTRUCTION("StopSoundEffect");
}

DECODER_PROC(SetVibration) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("unused");
	ADD_EXPRESSION_ARG("vib1");
	ADD_EXPRESSION_ARG("vib2");
	RETURN_INSTRUCTION("SetVibration");
}

DECODER_PROC(Unk0026) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("value");
	RETURN_INSTRUCTION("Unk0026");
}

DECODER_PROC(UnkJump0015) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	ADD_EXPRESSION_ARG("arg3");
	ADD_LOCAL_LABEL_ARG("target");
	RETURN_INSTRUCTION("UnkJump0015");
}

DECODER_PROC(SetThreadVar) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("destination");
	ADD_EXPRESSION_ARG("value");
	RETURN_INSTRUCTION("SetThreadVar");
}

DECODER_PROC(Unk0019) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	RETURN_INSTRUCTION("Unk0019");
}

NO_ARGS_DECODER_PROC(Thread001A);

DECODER_PROC(Unk001B) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg1");
	ADD_UINT16_ARG("arg2");
	RETURN_INSTRUCTION("Unk001B");
}

DECODER_PROC(LongAssign) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg");
	RETURN_INSTRUCTION("LongAssign");
}

DECODER_PROC(SetSwitch) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("expr");
	RETURN_INSTRUCTION("SetSwitch");
}

DECODER_PROC(JumpSwitchCase) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("case");
	ADD_LOCAL_LABEL_ARG("target");
	RETURN_INSTRUCTION("JumpSwitchCase");
}

DECODER_PROC(Unk0028) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	ADD_EXPRESSION_ARG("arg3");
	ADD_EXPRESSION_ARG("loopCount");
	RETURN_INSTRUCTION("Unk0028");
}

DECODER_PROC(Useless_0029) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg");
	RETURN_INSTRUCTION("Useless_0029");
}

DECODER_PROC(Unk002A) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg");
	RETURN_INSTRUCTION("Unk002A");
}

DECODER_PROC(Unk002C) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg");
	RETURN_INSTRUCTION("Unk002C");
}

DECODER_PROC(Nop3) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg");
	RETURN_INSTRUCTION("Nop3");
}

DECODER_PROC(Unk002F) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type == 1) {
		ADD_EXPRESSION_ARG("arg1");
	}
	RETURN_INSTRUCTION("Unk002F");
}

DECODER_PROC(Unk0030) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg1");
	RETURN_INSTRUCTION("Unk0030");
}

DECODER_PROC(Unk0031) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg1");
	RETURN_INSTRUCTION("Unk0031");
}

NO_ARGS_DECODER_PROC(ResetSomething0032);

DECODER_PROC(Wait) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("delay");
	ADD_EXPRESSION_ARG("unused");
	RETURN_INSTRUCTION("Wait");
}

DECODER_PROC(SetRichPresence) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("notDoSet");
	ADD_EXPRESSION_ARG("mode");
	RETURN_INSTRUCTION("SetRichPresence");
}

DECODER_PROC(Unk0035) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg1");
	// I *think* the length of this actually depends on build
	RETURN_INSTRUCTION("Unk0035");
}

DECODER_PROC(PlayVoice) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("channel");
	ADD_EXPRESSION_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	RETURN_INSTRUCTION("PlayVoice");
}

DECODER_PROC(StopVoice) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("channel");
	ADD_EXPRESSION_ARG("arg1");
	RETURN_INSTRUCTION("StopVoice");
}

DECODER_PROC(Group003A) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		RETURN_INSTRUCTION("Unk003A00");
		break;
	case 1:
		RETURN_INSTRUCTION("Unk003A01");
		break;
	case 2:
		ADD_EXPRESSION_ARG("arg1");
		RETURN_INSTRUCTION("Unk003A02");
		break;
	case 3:
		ADD_EXPRESSION_ARG("arg1");
		RETURN_INSTRUCTION("Unk003A03");
		break;
	}
	RETURN_UNRECOGNIZED();
}

DECODER_PROC(Audio003B) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	RETURN_INSTRUCTION("Audio003B");
}

DECODER_PROC(EvalTwo) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	RETURN_INSTRUCTION("EvalTwo")
}

NO_ARGS_DECODER_PROC(ResetAudioState);
NO_ARGS_DECODER_PROC(Unk003F);
NO_ARGS_DECODER_PROC(Nop);

DECODER_PROC(Unk0042) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg1");
	RETURN_INSTRUCTION("Unk0042");
}

DECODER_PROC(Unk0043) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	ADD_BYTE_ARG("arg2");
	switch (type) {
	case 0:
	case 1:
	case 5:
	case 6:
	case 7:
		RETURN_INSTRUCTION("Unk0043");
		break;
	case 2:
		ADD_EXPRESSION_ARG("arg2");
		RETURN_INSTRUCTION("Unk0043");
		break;
	case 3:
	case 4:
		ADD_STRING_REF_ARG("arg2");
		RETURN_INSTRUCTION("Unk0043");
		break;
	default:
		RETURN_UNRECOGNIZED();
	}
}

DECODER_PROC(Unk0044) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	RETURN_INSTRUCTION("Unk0044");
}

DECODER_PROC(Unk0046) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("mode");
	RETURN_INSTRUCTION("Unk0046");
}

DECODER_PROC(DebugCtrl) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("mode");
	RETURN_INSTRUCTION("DebugCtrl");
}

NO_ARGS_DECODER_PROC(WaitForSomething004B);

DECODER_PROC(Unk004C) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type == 0) {
		ADD_EXPRESSION_ARG("arg2");
	}
	RETURN_INSTRUCTION("Unk004C");
}

DECODER_PROC(UselessJump) {
	DECODER_PROC_INIT();
	uint8_t cond = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("condition", cond);
		args.push_back(arg);
	}
	if (cond == 0 || cond == 2 || cond == 3) {
		ADD_LOCAL_LABEL_ARG("target");
		ADD_LOCAL_LABEL_ARG("idk");
	}
	if (cond == 3) {
		ADD_LOCAL_LABEL_ARG("idk2");
	}
	RETURN_INSTRUCTION("UselessJump");
}

DECODER_PROC(Group0051) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		ADD_EXPRESSION_ARG("arg1");
		RETURN_INSTRUCTION("Unk005100");
		break;
	case 1:
		RETURN_INSTRUCTION("Unk005101");
		break;
	case 2:
		RETURN_INSTRUCTION("Unk005102");
		break;
	case 3:
		RETURN_INSTRUCTION("Unk005103");
		break;
	}
	RETURN_UNRECOGNIZED();
}

DECODER_PROC(Useless0053) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	ADD_LOCAL_LABEL_ARG("arg3");
	RETURN_INSTRUCTION("Useless0053");
}

DECODER_PROC(CallIfFlag) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("condition");
	ADD_EXPR_FLAG_REF_ARG("flag");
	ADD_LOCAL_LABEL_ARG("target");
	ADD_RETURN_ADDRESS_ARG("returnAddress");
	RETURN_INSTRUCTION("CallIfFlag");
}

DECODER_PROC(CallFarIfFlag) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("condition");
	ADD_EXPR_FLAG_REF_ARG("flag");
	ADD_FAR_LABEL_ARG("target");
	ADD_RETURN_ADDRESS_ARG("returnAddress");
	RETURN_INSTRUCTION("CallFarIfFlag");
}

DECODER_PROC(ReturnIfFlag) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("condition");
	ADD_EXPR_FLAG_REF_ARG("flag");
	RETURN_INSTRUCTION("ReturnIfFlag");
}

DECODER_PROC(Unk0058) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type == 2 || type == 3) {
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_EXPRESSION_ARG("arg3");
		ADD_EXPRESSION_ARG("arg4");
	}
	else {
		ADD_EXPRESSION_ARG("arg1");
	}
	ADD_LOCAL_LABEL_ARG("target");
	RETURN_INSTRUCTION("Unk0058");
}

DECODER_PROC(Unk0059) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	ADD_EXPRESSION_ARG("arg3");
	ADD_LOCAL_LABEL_ARG("target");
	RETURN_INSTRUCTION("Unk0059");
}

DECODER_PROC(CreateSurface) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("format");
	ADD_EXPRESSION_ARG("surfaceId");
	ADD_EXPRESSION_ARG("width");
	ADD_EXPRESSION_ARG("height");
	RETURN_INSTRUCTION("CreateSurface");
}

DECODER_PROC(ReleaseSurface) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("surfaceId");
	RETURN_INSTRUCTION("ReleaseSurface");
}

DECODER_PROC(LoadTexture) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("surfaceId");
	ADD_EXPRESSION_ARG("archiveId");
	ADD_EXPRESSION_ARG("textureFileId");
	RETURN_INSTRUCTION("LoadTexture");
}

DECODER_PROC(Unk0104) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	ADD_EXPRESSION_ARG("arg3");
	ADD_EXPRESSION_ARG("arg4");
	ADD_EXPRESSION_ARG("arg5");
	RETURN_INSTRUCTION("Unk0104");
}

DECODER_PROC(GroupCalc) {
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

DECODER_PROC(Unk0106) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		RETURN_INSTRUCTION("Unk0106NoStore");
		break;
	case 1:
		ADD_EXPR_GLOBAL_VAR_REF_ARG("destination");
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		RETURN_INSTRUCTION("Unk0106Store");
		break;
	default:
		RETURN_UNRECOGNIZED();
	}
}

DECODER_PROC(Unk0107) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	ADD_EXPRESSION_ARG("unused");
	RETURN_INSTRUCTION("Unk0107");
}

DECODER_PROC(Unk0108) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg");
	RETURN_INSTRUCTION("Unk0108");
}

DECODER_PROC(GroupCheckpoint) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		ADD_UINT16_ARG("checkpointId");
		RETURN_INSTRUCTION("Checkpoint00");
		break;
	case 1:
		ADD_UINT16_ARG("checkpointId");
		ADD_EXPRESSION_ARG("unk");
		RETURN_INSTRUCTION("Checkpoint01");
		break;
	case 2:
		ADD_EXPRESSION_ARG("unk");
		RETURN_INSTRUCTION("Checkpoint02");
		break;
	default:
		// probably invalid
		RETURN_INSTRUCTION("CheckpointUnk");
		break;
	}
}

DECODER_PROC(Unk010A) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg");
	RETURN_INSTRUCTION("Unk010A");
}

NO_ARGS_DECODER_PROC(ClearDialogueBox);

DECODER_PROC(GroupLoadDialogue) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		ADD_EXPRESSION_ARG("characterId");
		ADD_STRING_REF_ARG("line");
		RETURN_INSTRUCTION("LoadDialogue");
		break;
	case 1:
		ADD_EXPRESSION_ARG("audioId");
		ADD_EXPRESSION_ARG("characterId");
		ADD_STRING_REF_ARG("line");
		RETURN_INSTRUCTION("LoadVoicedUnactedDialogueMaybe");
		break;
	case 3:
		ADD_EXPRESSION_ARG("audioId");
		ADD_EXPRESSION_ARG("animationId");
		ADD_EXPRESSION_ARG("characterId");
		ADD_STRING_REF_ARG("line");
		RETURN_INSTRUCTION("LoadVoicedDialogue");
		break;
	case 0x0B:
		ADD_EXPRESSION_ARG("audioId");
		ADD_EXPRESSION_ARG("animationId");
		ADD_EXPRESSION_ARG("characterId");
		ADD_STRING_REF_ARG("line");
		RETURN_INSTRUCTION("LoadDialogue0B");
		break;
	default:
		RETURN_UNRECOGNIZED();
	}
}

DECODER_PROC(GroupDisplayDialogue) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	if (type == 0) {
		RETURN_INSTRUCTION("DisplayDialogue");
	}
	RETURN_UNRECOGNIZED();
}

DECODER_PROC(InstantiateTextStyle) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("instanceId");
	ADD_LOCAL_LABEL_ARG("styleDataBlock");
	RETURN_INSTRUCTION("InstantiateTextStyle");
}

DECODER_PROC(SetNGmoji) {
	DECODER_PROC_INIT();
	ADD_STRING_REF_ARG("unk1");
	ADD_STRING_REF_ARG("unk2");
	RETURN_INSTRUCTION("SetNGmoji");
}

DECODER_PROC(Unk0110) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("mode");
	RETURN_INSTRUCTION("Unk0110");
}

DECODER_PROC(GroupDialogueBox) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		RETURN_INSTRUCTION("HideCurrentDialogueBox");
		break;
	case 1:
		RETURN_INSTRUCTION("ShowCurrentDialogueBox");
		break;
	case 2:
		RETURN_INSTRUCTION("AwaitShowCurrentDialogueBox");
		break;
	case 3:
		RETURN_INSTRUCTION("AwaitCurrentHideDialogueBox");
		break;
	case 4:
		RETURN_INSTRUCTION("CurrentDialogueBox04");
		break;
	case 5:
		ADD_EXPRESSION_ARG("dialogueBoxId");
		RETURN_INSTRUCTION("HideNewDialogueBox");
		break;
	case 6:
		ADD_EXPRESSION_ARG("dialogueBoxId");
		RETURN_INSTRUCTION("HideNewDialogueBoxSlow");
		break;
	case 7:
		ADD_EXPRESSION_ARG("dialogueBoxId");
		RETURN_INSTRUCTION("HideNewDialogueBoxFast");
		break;
	default:
		RETURN_INSTRUCTION("DialogueBoxUnk");
		break;
	}
}

DECODER_PROC(Unk0113) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type == 2) {
		ADD_EXPRESSION_ARG("arg1");
	}
	RETURN_INSTRUCTION("Unk0113");
}

DECODER_PROC(Unk0112) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	ADD_STRING_REF_ARG("arg1");
	if (type == 0 || type == 2) {
		ADD_EXPRESSION_ARG("arg2");
	}
	RETURN_INSTRUCTION("Unk0112");
}

DECODER_PROC(Unk0114) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type >= 2) {
		ADD_STRING_REF_ARG("arg1");
	}
	RETURN_INSTRUCTION("Unk0114");
}

DECODER_PROC(Unk0120) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("bufferId");
	RETURN_INSTRUCTION("Unk0120");
}

DECODER_PROC(InstantiateStringArray) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("instanceId");
	ADD_LOCAL_LABEL_ARG("dataBlock");
	RETURN_INSTRUCTION("InstantiateStringArray");
}

DECODER_PROC(ActualLoadCutscene) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type == 99) {
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
	}
	else {
		ADD_BYTE_ARG("arg1");
	}
	ADD_EXPRESSION_ARG("arg3");
	ADD_EXPRESSION_ARG("arg4");
	RETURN_INSTRUCTION("ActualLoadCutscene");
}

DECODER_PROC(GroupCutscene) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 2:
		RETURN_INSTRUCTION("StopCutscene");
		break;
	case 3:
		RETURN_INSTRUCTION("StopCutscene_WaitForSomething");
		break;
	default: {
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
			 RETURN_INSTRUCTION("CutsceneUnk");
			 break;
	}
}

DECODER_PROC(LoadCutscene) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("cutsceneId");
	RETURN_INSTRUCTION("LoadCutscene");
}

DECODER_PROC(Group0125) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		ADD_STRING_REF_ARG("arg1");
		RETURN_INSTRUCTION("Unk012500");
		break;
	case 1:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_STRING_REF_ARG("arg3");
		RETURN_INSTRUCTION("Unk012501");
		break;
	case 2:
		ADD_STRING_REF_ARG("arg1");
		RETURN_INSTRUCTION("Unk012502");
		break;
	case 3:
		ADD_STRING_REF_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_EXPRESSION_ARG("arg3");
		ADD_EXPRESSION_ARG("arg4");
		RETURN_INSTRUCTION("Unk012503");
		break;
	default: {
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
			 RETURN_INSTRUCTION("Unk0125");
			 break;
	}
}

DECODER_PROC(PlayCutscene) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type == 99) {
		ADD_EXPRESSION_ARG("playmode");
		ADD_EXPRESSION_ARG("playview");
	}
	else {
		ADD_BYTE_ARG("playview");
	}
	ADD_EXPRESSION_ARG("cutscene");
	RETURN_INSTRUCTION("PlayCutscene");
}

DECODER_PROC(SetCutscenePaused) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("paused");
	RETURN_INSTRUCTION("SetCutscenePaused");
}

DECODER_PROC(InitSubsystem) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("subsystemId");
	RETURN_INSTRUCTION("InitSubsystem");
}

DECODER_PROC(LoadBackground) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("bufferId");
	ADD_EXPRESSION_ARG("backgroundId");
	RETURN_INSTRUCTION("LoadBackground");
}

DECODER_PROC(SwapBackgrounds) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("firstBufferId");
	ADD_EXPRESSION_ARG("secondBufferId");
	RETURN_INSTRUCTION("SwapBackgrounds");
}

DECODER_PROC(UnkBglink1004) {
	DECODER_PROC_INIT();
	uint8_t id = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("id", id);
		args.push_back(arg);
	}
	ADD_EXPRESSION_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	if (id >= 4) {
		ADD_EXPRESSION_ARG("arg3");
	}
	ADD_EXPRESSION_ARG("arg4");
	RETURN_INSTRUCTION("UnkBglink1004");
}

DECODER_PROC(LoadCharacter) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	ADD_EXPRESSION_ARG("bufferId");
	ADD_EXPRESSION_ARG("spriteId");
	if (type == 0) {
		ADD_UINT16_ARG("unused");
	}
	RETURN_INSTRUCTION("LoadCharacter");
}

DECODER_PROC(Unk1006) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("firstBufferId");
	ADD_EXPRESSION_ARG("secondBufferId");
	RETURN_INSTRUCTION("Unk1006");
}

DECODER_PROC(CopyBackground) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("srcBufferId");
	ADD_EXPRESSION_ARG("destBufferId");
	RETURN_INSTRUCTION("CopyBackground");
}

DECODER_PROC(HideBackgroundMaybe) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("bufferId");
	RETURN_INSTRUCTION("HideBackgroundMaybe");
}

DECODER_PROC(HideCharacterMaybe) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("bufferId");
	RETURN_INSTRUCTION("HideCharacterMaybe");
}

DECODER_PROC(Unk1013) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	RETURN_INSTRUCTION("Unk1013");
}

DECODER_PROC(SaveResetRelated1014) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	RETURN_INSTRUCTION("SaveResetRelated1014");
}

DECODER_PROC(Unk101A) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	RETURN_INSTRUCTION("Unk101A");
}

DECODER_PROC(Unk101D) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	RETURN_INSTRUCTION("Unk101D");
}

NO_ARGS_DECODER_PROC(Unk101E);

DECODER_PROC(Unk101F) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	RETURN_INSTRUCTION("Unk101F");
}

DECODER_PROC(Unk1020) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	RETURN_INSTRUCTION("Unk1020");
}

DECODER_PROC(Unk1021) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	RETURN_INSTRUCTION("Unk1021");
}

DECODER_PROC(Group1022) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		RETURN_INSTRUCTION("QuickSave");
		break;
	case 1:
		RETURN_INSTRUCTION("StateUnk01");
		break;
	case 2:
		RETURN_INSTRUCTION("StateUnk02");
		break;
	case 3:
		RETURN_INSTRUCTION("StateUnk03");
		break;
	case 5:
		RETURN_INSTRUCTION("StateUnk05");
		break;
	case 0x0A:
		ADD_UINT16_ARG("checkpointId");
		RETURN_INSTRUCTION("SaveState");
		break;
	case 0x14:
		RETURN_INSTRUCTION("StateUnk14");
		break;
	case 0x15:
		RETURN_INSTRUCTION("StateUnk15");
		break;
	case 0xFF:
		RETURN_INSTRUCTION("StateUnkFF");
		break;
	}
	RETURN_UNRECOGNIZED();
}

DECODER_PROC(Unk1023) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type == 0 || type == 0xA) {
		ADD_BYTE_ARG("unk");
	}
	RETURN_INSTRUCTION("Unk1023");
}

DECODER_PROC(Unk1024) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	if (type == 0) {
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
	}
	RETURN_INSTRUCTION("Unk1024");
}

DECODER_PROC(Unk1027) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	{
		SC3Argument* arg = (SC3Argument*)new SC3ArgByte("type", type);
		args.push_back(arg);
	}
	ADD_EXPRESSION_ARG("tip");
	if (type == 1) {
		ADD_EXPR_FLAG_REF_ARG("flag");
	}
	if (type == 0) {
		RETURN_INSTRUCTION("OpenTip");
	}
	RETURN_INSTRUCTION("Unk1027");
}

DECODER_PROC(Unk1028) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("arg");
	RETURN_INSTRUCTION("Unk1028");
}

DECODER_PROC(SetEnvFlag) {
	DECODER_PROC_INIT();
	ADD_EXPRESSION_ARG("flag");
	RETURN_INSTRUCTION("SetEnvFlag");
}

DECODER_PROC(Unk102A) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("type");
	ADD_EXPRESSION_ARG("arg1");
	ADD_EXPRESSION_ARG("arg2");
	ADD_EXPRESSION_ARG("arg3");
	RETURN_INSTRUCTION("Unk102A");
}

DECODER_PROC(Group1030) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		RETURN_INSTRUCTION("Unk103000");
		break;
	case 1:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_EXPRESSION_ARG("arg3");
		ADD_EXPRESSION_ARG("arg4");
		ADD_EXPRESSION_ARG("arg5");
		RETURN_INSTRUCTION("Unk103001");
		break;
	case 2:
		RETURN_INSTRUCTION("Unk103002");
		break;
	case 3:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_EXPRESSION_ARG("arg3");
		ADD_EXPRESSION_ARG("arg4");
		ADD_EXPRESSION_ARG("arg5");
		RETURN_INSTRUCTION("Unk103003");
		break;
	case 4:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_EXPRESSION_ARG("arg3");
		ADD_EXPRESSION_ARG("arg4");
		ADD_EXPRESSION_ARG("arg5");
		ADD_EXPRESSION_ARG("arg6");
		RETURN_INSTRUCTION("Unk103004");
		break;
	case 5:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_EXPRESSION_ARG("arg3");
		ADD_EXPRESSION_ARG("arg4");
		ADD_EXPRESSION_ARG("arg5");
		ADD_EXPRESSION_ARG("arg6");
		RETURN_INSTRUCTION("Unk103005");
		break;
	case 0xA:
		RETURN_INSTRUCTION("Unk10300A");
		break;
	case 0xB:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_EXPRESSION_ARG("arg3");
		ADD_EXPRESSION_ARG("arg4");
		ADD_EXPRESSION_ARG("arg5");
		RETURN_INSTRUCTION("Unk10300B");
		break;
	case 0xC:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_EXPRESSION_ARG("arg3");
		ADD_EXPRESSION_ARG("arg4");
		ADD_EXPRESSION_ARG("arg5");
		ADD_EXPRESSION_ARG("arg6");
		RETURN_INSTRUCTION("Unk10300C");
		break;
	default:
		RETURN_UNRECOGNIZED();
		break;
	}
}

DECODER_PROC(GroupTips) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		ADD_LOCAL_LABEL_ARG("block");
		RETURN_INSTRUCTION("TipsUnk103300");
		break;
	case 1:
		RETURN_INSTRUCTION("TipsUnk103301");
		break;
	case 2:
		RETURN_INSTRUCTION("TipsUnk103302");
		break;
	case 3:
		RETURN_INSTRUCTION("TipsUnk103303");
		break;
	case 4:
		RETURN_INSTRUCTION("TipsUnk103304");
		break;
	default:
		RETURN_UNRECOGNIZED();
		break;
	}
}

DECODER_PROC(Unk1034) {
	DECODER_PROC_INIT();
	ADD_BYTE_ARG("arg1");
	RETURN_INSTRUCTION("Unk1034");
}

DECODER_PROC(Group1037) {
	DECODER_PROC_INIT();
	uint8_t type = *data++;
	switch (type) {
	case 0:
		ADD_LOCAL_LABEL_ARG("arg1");
		ADD_LOCAL_LABEL_ARG("arg2");
		ADD_LOCAL_LABEL_ARG("arg3");
		ADD_LOCAL_LABEL_ARG("arg4");
		ADD_LOCAL_LABEL_ARG("arg5");
		ADD_LOCAL_LABEL_ARG("arg6");
		ADD_LOCAL_LABEL_ARG("arg7");
		RETURN_INSTRUCTION("Unk103700");
		break;
	case 1:
		RETURN_INSTRUCTION("Unk103701");
		break;
	case 2:
		RETURN_INSTRUCTION("Unk103702");
		break;
	case 3:
		RETURN_INSTRUCTION("Unk103703");
		break;
	case 4:
		RETURN_INSTRUCTION("Unk103704");
		break;
	case 0xA:
		ADD_LOCAL_LABEL_ARG("arg1");
		RETURN_INSTRUCTION("Unk10370A");
		break;
	case 0xB:
		RETURN_INSTRUCTION("Unk10370B");
		break;
	case 0xC:
		ADD_EXPRESSION_ARG("unused1");
		ADD_EXPRESSION_ARG("unused2");
		RETURN_INSTRUCTION("Unk10370C");
		break;
	case 0xD:
		ADD_EXPRESSION_ARG("unused1");
		// sg0 win32 reads unused2 as an expression. which is wrong.
		ADD_UINT16_ARG("unused2");
		RETURN_INSTRUCTION("Unk10370D");
		break;
	case 0xE:
		RETURN_INSTRUCTION("Unk10370E");
		break;
	case 0xF:
		ADD_EXPRESSION_ARG("unused1");
		ADD_EXPRESSION_ARG("unused2");
		ADD_EXPRESSION_ARG("unused3");
		RETURN_INSTRUCTION("Unk10370F");
		break;
	case 0x28:
		RETURN_INSTRUCTION("Unk103728");
		break;
	case 0x29:
		RETURN_INSTRUCTION("Unk103729");
		break;
	case 0x2A:
		ADD_EXPRESSION_ARG("arg1");
		RETURN_INSTRUCTION("Unk10372A");
		break;
	case 0x2B:
		ADD_EXPRESSION_ARG("arg1");
		RETURN_INSTRUCTION("Unk10372B");
		break;
	case 0x2C:
		ADD_EXPRESSION_ARG("arg1");
		RETURN_INSTRUCTION("Unk10372C");
		break;
	case 0x2D:
		ADD_EXPRESSION_ARG("arg1");
		RETURN_INSTRUCTION("Unk10372D");
		break;
	case 0x2E:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		ADD_EXPRESSION_ARG("arg3");
		RETURN_INSTRUCTION("Unk10372E");
		break;
	case 0x2F:
		RETURN_INSTRUCTION("Unk10372F");
		break;
	case 0x30:
		ADD_EXPRESSION_ARG("arg1");
		RETURN_INSTRUCTION("Unk103730");
		break;
	case 0x31:
		ADD_EXPRESSION_ARG("arg1");
		ADD_EXPRESSION_ARG("arg2");
		RETURN_INSTRUCTION("Unk103731");
		break;
	case 0x32:
		// not sure if this is local or in some other fixed script
		ADD_UINT16_ARG("someLabel");
		RETURN_INSTRUCTION("Unk103732");
		break;
	default:
		RETURN_UNRECOGNIZED();
	}
}

NO_ARGS_DECODER_PROC(Win32_SetResolution);
NO_ARGS_DECODER_PROC(Win32_DestroyWindow);

#pragma endregion

#pragma region Decoder tables

static DecoderProc DecoderTable00[256] = {
	InstEndOfScript,           // 00 00
	InstCreateThread,          // 00 01
	InstTerminateThread,       // 00 02
	InstExitThread,            // 00 03
	InstLoadScript,            // 00 04
	InstSleep,                 // 00 05
	InstHalt,                  // 00 06
	InstJump,                  // 00 07
	InstJumpTable,             // 00 08
	Inst__Unrecognized__,      // 00 09
	InstJumpIf,                // 00 0A
	InstCall,                  // 00 0B
	InstJumpFar,               // 00 0C
	InstCallFar,               // 00 0D
	InstReturn,                // 00 0E
	InstLoopTimes,             // 00 0F
	InstJumpIfFlag,            // 00 10
	InstWaitForFlag,           // 00 11
	InstSetFlag,               // 00 12
	InstResetFlag,             // 00 13
	InstCopyFlag,              // 00 14
	InstUnkJump0015,           // 00 15
	Inst__Unrecognized__,      // 00 16
	Inst__Unrecognized__,      // 00 17
	InstSetThreadVar,          // 00 18
	InstUnk0019,               // 00 19
	InstThread001A,            // 00 1A
	InstUnk001B,               // 00 1B
	Inst__Unrecognized__,      // 00 1C
	InstLongAssign,            // 00 1D
	Inst__Unrecognized__,      // 00 1E
	InstSetSwitch,             // 00 1F
	InstJumpSwitchCase,        // 00 20
	InstPlayBgm,               // 00 21
	InstStopBgm,               // 00 22
	InstPlaySoundEffect,       // 00 23
	InstStopSoundEffect,       // 00 24
	InstSetVibration,          // 00 25
	InstUnk0026,               // 00 26
	Inst__Unrecognized__,      // 00 27
	InstUnk0028,               // 00 28
	InstUseless_0029,          // 00 29
	InstUnk002A,               // 00 2A
	InstLongAssign,            // 00 2B
	InstUnk002C,               // 00 2C
	InstNop3,                  // 00 2D
	InstSetRichPresence,       // 00 2E
	InstUnk002F,               // 00 2F
	InstUnk0030,               // 00 30
	InstUnk0031,               // 00 31
	InstResetSomething0032,    // 00 32
	InstWait,                  // 00 33
	Inst__Unrecognized__,      // 00 34
	InstUnk0035,               // 00 35
	InstNop3,                  // 00 36
	InstPlayVoice,             // 00 37
	InstStopVoice,             // 00 38
	Inst__Unrecognized__,      // 00 39
	InstGroup003A,             // 00 3A
	InstAudio003B,             // 00 3B
	Inst__Unrecognized__,      // 00 3C
	InstEvalTwo,               // 00 3D
	InstResetAudioState,       // 00 3E
	InstUnk003F,               // 00 3F
	InstNop,                   // 00 40
	InstNop3,                  // 00 41
	InstUnk0042,               // 00 42
	InstUnk0043,               // 00 43
	InstUnk0044,               // 00 44
	Inst__Unrecognized__,      // 00 45
	InstUnk0046,               // 00 46
	Inst__Unrecognized__,      // 00 47
	Inst__Unrecognized__,      // 00 48
	InstNop3,                  // 00 49
	InstDebugCtrl,             // 00 4A
	InstWaitForSomething004B,  // 00 4B
	InstUnk004C,               // 00 4C
	Inst__Unrecognized__,      // 00 4D
	Inst__Unrecognized__,      // 00 4E
	InstLongAssign,            // 00 4F
	InstUselessJump,           // 00 50
	InstGroup0051,             // 00 51
	InstNop,                   // 00 52
	InstUseless0053,           // 00 53
	InstCallIfFlag,            // 00 54
	Inst__Unrecognized__,      // 00 55
	InstCallFarIfFlag,         // 00 56
	InstReturnIfFlag,          // 00 57
	InstUnk0058,               // 00 58
	InstUnk0059,               // 00 59
	Inst__Unrecognized__,      // 00 5A
	Inst__Unrecognized__,      // 00 5B
	Inst__Unrecognized__,      // 00 5C
	Inst__Unrecognized__,      // 00 5D
	Inst__Unrecognized__,      // 00 5E
	InstNop,                   // 00 5F
	Inst__Unrecognized__,      // 00 60
	Inst__Unrecognized__,      // 00 61
	Inst__Unrecognized__,      // 00 62
	Inst__Unrecognized__,      // 00 63
	Inst__Unrecognized__,      // 00 64
	Inst__Unrecognized__,      // 00 65
	Inst__Unrecognized__,      // 00 66
	Inst__Unrecognized__,      // 00 67
	Inst__Unrecognized__,      // 00 68
	Inst__Unrecognized__,      // 00 69
	Inst__Unrecognized__,      // 00 6A
	Inst__Unrecognized__,      // 00 6B
	Inst__Unrecognized__,      // 00 6C
	Inst__Unrecognized__,      // 00 6D
	Inst__Unrecognized__,      // 00 6E
	Inst__Unrecognized__,      // 00 6F
	Inst__Unrecognized__,      // 00 70
	Inst__Unrecognized__,      // 00 71
	Inst__Unrecognized__,      // 00 72
	Inst__Unrecognized__,      // 00 73
	Inst__Unrecognized__,      // 00 74
	Inst__Unrecognized__,      // 00 75
	Inst__Unrecognized__,      // 00 76
	Inst__Unrecognized__,      // 00 77
	Inst__Unrecognized__,      // 00 78
	Inst__Unrecognized__,      // 00 79
	Inst__Unrecognized__,      // 00 7A
	Inst__Unrecognized__,      // 00 7B
	Inst__Unrecognized__,      // 00 7C
	Inst__Unrecognized__,      // 00 7D
	Inst__Unrecognized__,      // 00 7E
	Inst__Unrecognized__,      // 00 7F
	Inst__Unrecognized__,      // 00 80
	Inst__Unrecognized__,      // 00 81
	Inst__Unrecognized__,      // 00 82
	Inst__Unrecognized__,      // 00 83
	Inst__Unrecognized__,      // 00 84
	Inst__Unrecognized__,      // 00 85
	Inst__Unrecognized__,      // 00 86
	Inst__Unrecognized__,      // 00 87
	Inst__Unrecognized__,      // 00 88
	Inst__Unrecognized__,      // 00 89
	Inst__Unrecognized__,      // 00 8A
	Inst__Unrecognized__,      // 00 8B
	Inst__Unrecognized__,      // 00 8C
	Inst__Unrecognized__,      // 00 8D
	Inst__Unrecognized__,      // 00 8E
	Inst__Unrecognized__,      // 00 8F
	Inst__Unrecognized__,      // 00 90
	Inst__Unrecognized__,      // 00 91
	Inst__Unrecognized__,      // 00 92
	Inst__Unrecognized__,      // 00 93
	Inst__Unrecognized__,      // 00 94
	Inst__Unrecognized__,      // 00 95
	Inst__Unrecognized__,      // 00 96
	Inst__Unrecognized__,      // 00 97
	Inst__Unrecognized__,      // 00 98
	Inst__Unrecognized__,      // 00 99
	Inst__Unrecognized__,      // 00 9A
	Inst__Unrecognized__,      // 00 9B
	Inst__Unrecognized__,      // 00 9C
	Inst__Unrecognized__,      // 00 9D
	Inst__Unrecognized__,      // 00 9E
	Inst__Unrecognized__,      // 00 9F
	Inst__Unrecognized__,      // 00 A0
	Inst__Unrecognized__,      // 00 A1
	Inst__Unrecognized__,      // 00 A2
	Inst__Unrecognized__,      // 00 A3
	Inst__Unrecognized__,      // 00 A4
	Inst__Unrecognized__,      // 00 A5
	Inst__Unrecognized__,      // 00 A6
	Inst__Unrecognized__,      // 00 A7
	Inst__Unrecognized__,      // 00 A8
	Inst__Unrecognized__,      // 00 A9
	Inst__Unrecognized__,      // 00 AA
	Inst__Unrecognized__,      // 00 AB
	Inst__Unrecognized__,      // 00 AC
	Inst__Unrecognized__,      // 00 AD
	Inst__Unrecognized__,      // 00 AE
	Inst__Unrecognized__,      // 00 AF
	Inst__Unrecognized__,      // 00 B0
	Inst__Unrecognized__,      // 00 B1
	Inst__Unrecognized__,      // 00 B2
	Inst__Unrecognized__,      // 00 B3
	Inst__Unrecognized__,      // 00 B4
	Inst__Unrecognized__,      // 00 B5
	Inst__Unrecognized__,      // 00 B6
	Inst__Unrecognized__,      // 00 B7
	Inst__Unrecognized__,      // 00 B8
	Inst__Unrecognized__,      // 00 B9
	Inst__Unrecognized__,      // 00 BA
	Inst__Unrecognized__,      // 00 BB
	Inst__Unrecognized__,      // 00 BC
	Inst__Unrecognized__,      // 00 BD
	Inst__Unrecognized__,      // 00 BE
	Inst__Unrecognized__,      // 00 BF
	Inst__Unrecognized__,      // 00 C0
	Inst__Unrecognized__,      // 00 C1
	Inst__Unrecognized__,      // 00 C2
	Inst__Unrecognized__,      // 00 C3
	Inst__Unrecognized__,      // 00 C4
	Inst__Unrecognized__,      // 00 C5
	Inst__Unrecognized__,      // 00 C6
	Inst__Unrecognized__,      // 00 C7
	Inst__Unrecognized__,      // 00 C8
	Inst__Unrecognized__,      // 00 C9
	Inst__Unrecognized__,      // 00 CA
	Inst__Unrecognized__,      // 00 CB
	Inst__Unrecognized__,      // 00 CC
	Inst__Unrecognized__,      // 00 CD
	Inst__Unrecognized__,      // 00 CE
	Inst__Unrecognized__,      // 00 CF
	Inst__Unrecognized__,      // 00 D0
	Inst__Unrecognized__,      // 00 D1
	Inst__Unrecognized__,      // 00 D2
	Inst__Unrecognized__,      // 00 D3
	Inst__Unrecognized__,      // 00 D4
	Inst__Unrecognized__,      // 00 D5
	Inst__Unrecognized__,      // 00 D6
	Inst__Unrecognized__,      // 00 D7
	Inst__Unrecognized__,      // 00 D8
	Inst__Unrecognized__,      // 00 D9
	Inst__Unrecognized__,      // 00 DA
	Inst__Unrecognized__,      // 00 DB
	Inst__Unrecognized__,      // 00 DC
	Inst__Unrecognized__,      // 00 DD
	Inst__Unrecognized__,      // 00 DE
	Inst__Unrecognized__,      // 00 DF
	Inst__Unrecognized__,      // 00 E0
	Inst__Unrecognized__,      // 00 E1
	Inst__Unrecognized__,      // 00 E2
	Inst__Unrecognized__,      // 00 E3
	Inst__Unrecognized__,      // 00 E4
	Inst__Unrecognized__,      // 00 E5
	Inst__Unrecognized__,      // 00 E6
	Inst__Unrecognized__,      // 00 E7
	Inst__Unrecognized__,      // 00 E8
	Inst__Unrecognized__,      // 00 E9
	Inst__Unrecognized__,      // 00 EA
	Inst__Unrecognized__,      // 00 EB
	Inst__Unrecognized__,      // 00 EC
	Inst__Unrecognized__,      // 00 ED
	Inst__Unrecognized__,      // 00 EE
	Inst__Unrecognized__,      // 00 EF
	Inst__Unrecognized__,      // 00 F0
	Inst__Unrecognized__,      // 00 F1
	Inst__Unrecognized__,      // 00 F2
	Inst__Unrecognized__,      // 00 F3
	Inst__Unrecognized__,      // 00 F4
	Inst__Unrecognized__,      // 00 F5
	Inst__Unrecognized__,      // 00 F6
	Inst__Unrecognized__,      // 00 F7
	Inst__Unrecognized__,      // 00 F8
	Inst__Unrecognized__,      // 00 F9
	Inst__Unrecognized__,      // 00 FA
	Inst__Unrecognized__,      // 00 FB
	Inst__Unrecognized__,      // 00 FC
	Inst__Unrecognized__,      // 00 FD
	Inst__Unrecognized__,      // 00 FE
	Inst__Unrecognized__       // 00 FF
};

static DecoderProc DecoderTable01[256] = {
	InstCreateSurface,           // 01 00
	InstReleaseSurface,          // 01 01
	InstLoadTexture,             // 01 02
	InstReleaseSurface,          // 01 03
	InstUnk0104,                 // 01 04
	InstGroupCalc,               // 01 05
	InstUnk0106,                 // 01 06
	InstUnk0107,                 // 01 07
	InstUnk0108,                 // 01 08
	InstGroupCheckpoint,         // 01 09
	InstUnk010A,                 // 01 0A
	InstClearDialogueBox,        // 01 0B
	InstGroupLoadDialogue,       // 01 0C
	InstGroupDisplayDialogue,    // 01 0D
	InstInstantiateTextStyle,    // 01 0E
	InstSetNGmoji,               // 01 0F
	InstUnk0110,                 // 01 10
	InstGroupDialogueBox,        // 01 11
	InstUnk0112,                 // 01 12
	InstUnk0113,                 // 01 13
	InstUnk0114,                 // 01 14
	Inst__Unrecognized__,        // 01 15
	Inst__Unrecognized__,        // 01 16
	Inst__Unrecognized__,        // 01 17
	Inst__Unrecognized__,        // 01 18
	Inst__Unrecognized__,        // 01 19
	Inst__Unrecognized__,        // 01 1A
	Inst__Unrecognized__,        // 01 1B
	Inst__Unrecognized__,        // 01 1C
	Inst__Unrecognized__,        // 01 1D
	Inst__Unrecognized__,        // 01 1E
	Inst__Unrecognized__,        // 01 1F
	InstUnk0120,                 // 01 20
	InstInstantiateStringArray,  // 01 21
	InstActualLoadCutscene,      // 01 22
	InstGroupCutscene,           // 01 23
	InstLoadCutscene,            // 01 24
	InstGroup0125,               // 01 25
	InstPlayCutscene,            // 01 26
	InstActualLoadCutscene,      // 01 27
	InstGroupCutscene,           // 01 28
	InstLoadCutscene,            // 01 29
	InstPlayCutscene,            // 01 2A
	InstSetCutscenePaused,       // 01 2B
	Inst__Unrecognized__,        // 01 2C
	Inst__Unrecognized__,        // 01 2D
	Inst__Unrecognized__,        // 01 2E
	Inst__Unrecognized__,        // 01 2F
	Inst__Unrecognized__,        // 01 30
	Inst__Unrecognized__,        // 01 31
	Inst__Unrecognized__,        // 01 32
	Inst__Unrecognized__,        // 01 33
	Inst__Unrecognized__,        // 01 34
	Inst__Unrecognized__,        // 01 35
	Inst__Unrecognized__,        // 01 36
	Inst__Unrecognized__,        // 01 37
	Inst__Unrecognized__,        // 01 38
	Inst__Unrecognized__,        // 01 39
	Inst__Unrecognized__,        // 01 3A
	Inst__Unrecognized__,        // 01 3B
	Inst__Unrecognized__,        // 01 3C
	Inst__Unrecognized__,        // 01 3D
	Inst__Unrecognized__,        // 01 3E
	Inst__Unrecognized__,        // 01 3F
	Inst__Unrecognized__,        // 01 40
	Inst__Unrecognized__,        // 01 41
	Inst__Unrecognized__,        // 01 42
	Inst__Unrecognized__,        // 01 43
	Inst__Unrecognized__,        // 01 44
	Inst__Unrecognized__,        // 01 45
	Inst__Unrecognized__,        // 01 46
	Inst__Unrecognized__,        // 01 47
	Inst__Unrecognized__,        // 01 48
	Inst__Unrecognized__,        // 01 49
	Inst__Unrecognized__,        // 01 4A
	Inst__Unrecognized__,        // 01 4B
	Inst__Unrecognized__,        // 01 4C
	Inst__Unrecognized__,        // 01 4D
	Inst__Unrecognized__,        // 01 4E
	Inst__Unrecognized__,        // 01 4F
	Inst__Unrecognized__,        // 01 50
	Inst__Unrecognized__,        // 01 51
	Inst__Unrecognized__,        // 01 52
	Inst__Unrecognized__,        // 01 53
	Inst__Unrecognized__,        // 01 54
	Inst__Unrecognized__,        // 01 55
	Inst__Unrecognized__,        // 01 56
	Inst__Unrecognized__,        // 01 57
	Inst__Unrecognized__,        // 01 58
	Inst__Unrecognized__,        // 01 59
	Inst__Unrecognized__,        // 01 5A
	Inst__Unrecognized__,        // 01 5B
	Inst__Unrecognized__,        // 01 5C
	Inst__Unrecognized__,        // 01 5D
	Inst__Unrecognized__,        // 01 5E
	Inst__Unrecognized__,        // 01 5F
	Inst__Unrecognized__,        // 01 60
	Inst__Unrecognized__,        // 01 61
	Inst__Unrecognized__,        // 01 62
	Inst__Unrecognized__,        // 01 63
	Inst__Unrecognized__,        // 01 64
	Inst__Unrecognized__,        // 01 65
	Inst__Unrecognized__,        // 01 66
	Inst__Unrecognized__,        // 01 67
	Inst__Unrecognized__,        // 01 68
	Inst__Unrecognized__,        // 01 69
	Inst__Unrecognized__,        // 01 6A
	Inst__Unrecognized__,        // 01 6B
	Inst__Unrecognized__,        // 01 6C
	Inst__Unrecognized__,        // 01 6D
	Inst__Unrecognized__,        // 01 6E
	Inst__Unrecognized__,        // 01 6F
	Inst__Unrecognized__,        // 01 70
	Inst__Unrecognized__,        // 01 71
	Inst__Unrecognized__,        // 01 72
	Inst__Unrecognized__,        // 01 73
	Inst__Unrecognized__,        // 01 74
	Inst__Unrecognized__,        // 01 75
	Inst__Unrecognized__,        // 01 76
	Inst__Unrecognized__,        // 01 77
	Inst__Unrecognized__,        // 01 78
	Inst__Unrecognized__,        // 01 79
	Inst__Unrecognized__,        // 01 7A
	Inst__Unrecognized__,        // 01 7B
	Inst__Unrecognized__,        // 01 7C
	Inst__Unrecognized__,        // 01 7D
	Inst__Unrecognized__,        // 01 7E
	Inst__Unrecognized__,        // 01 7F
	Inst__Unrecognized__,        // 01 80
	Inst__Unrecognized__,        // 01 81
	Inst__Unrecognized__,        // 01 82
	Inst__Unrecognized__,        // 01 83
	Inst__Unrecognized__,        // 01 84
	Inst__Unrecognized__,        // 01 85
	Inst__Unrecognized__,        // 01 86
	Inst__Unrecognized__,        // 01 87
	Inst__Unrecognized__,        // 01 88
	Inst__Unrecognized__,        // 01 89
	Inst__Unrecognized__,        // 01 8A
	Inst__Unrecognized__,        // 01 8B
	Inst__Unrecognized__,        // 01 8C
	Inst__Unrecognized__,        // 01 8D
	Inst__Unrecognized__,        // 01 8E
	Inst__Unrecognized__,        // 01 8F
	Inst__Unrecognized__,        // 01 90
	Inst__Unrecognized__,        // 01 91
	Inst__Unrecognized__,        // 01 92
	Inst__Unrecognized__,        // 01 93
	Inst__Unrecognized__,        // 01 94
	Inst__Unrecognized__,        // 01 95
	Inst__Unrecognized__,        // 01 96
	Inst__Unrecognized__,        // 01 97
	Inst__Unrecognized__,        // 01 98
	Inst__Unrecognized__,        // 01 99
	Inst__Unrecognized__,        // 01 9A
	Inst__Unrecognized__,        // 01 9B
	Inst__Unrecognized__,        // 01 9C
	Inst__Unrecognized__,        // 01 9D
	Inst__Unrecognized__,        // 01 9E
	Inst__Unrecognized__,        // 01 9F
	Inst__Unrecognized__,        // 01 A0
	Inst__Unrecognized__,        // 01 A1
	Inst__Unrecognized__,        // 01 A2
	Inst__Unrecognized__,        // 01 A3
	Inst__Unrecognized__,        // 01 A4
	Inst__Unrecognized__,        // 01 A5
	Inst__Unrecognized__,        // 01 A6
	Inst__Unrecognized__,        // 01 A7
	Inst__Unrecognized__,        // 01 A8
	Inst__Unrecognized__,        // 01 A9
	Inst__Unrecognized__,        // 01 AA
	Inst__Unrecognized__,        // 01 AB
	Inst__Unrecognized__,        // 01 AC
	Inst__Unrecognized__,        // 01 AD
	Inst__Unrecognized__,        // 01 AE
	Inst__Unrecognized__,        // 01 AF
	Inst__Unrecognized__,        // 01 B0
	Inst__Unrecognized__,        // 01 B1
	Inst__Unrecognized__,        // 01 B2
	Inst__Unrecognized__,        // 01 B3
	Inst__Unrecognized__,        // 01 B4
	Inst__Unrecognized__,        // 01 B5
	Inst__Unrecognized__,        // 01 B6
	Inst__Unrecognized__,        // 01 B7
	Inst__Unrecognized__,        // 01 B8
	Inst__Unrecognized__,        // 01 B9
	Inst__Unrecognized__,        // 01 BA
	Inst__Unrecognized__,        // 01 BB
	Inst__Unrecognized__,        // 01 BC
	Inst__Unrecognized__,        // 01 BD
	Inst__Unrecognized__,        // 01 BE
	Inst__Unrecognized__,        // 01 BF
	Inst__Unrecognized__,        // 01 C0
	Inst__Unrecognized__,        // 01 C1
	Inst__Unrecognized__,        // 01 C2
	Inst__Unrecognized__,        // 01 C3
	Inst__Unrecognized__,        // 01 C4
	Inst__Unrecognized__,        // 01 C5
	Inst__Unrecognized__,        // 01 C6
	Inst__Unrecognized__,        // 01 C7
	Inst__Unrecognized__,        // 01 C8
	Inst__Unrecognized__,        // 01 C9
	Inst__Unrecognized__,        // 01 CA
	Inst__Unrecognized__,        // 01 CB
	Inst__Unrecognized__,        // 01 CC
	Inst__Unrecognized__,        // 01 CD
	Inst__Unrecognized__,        // 01 CE
	Inst__Unrecognized__,        // 01 CF
	Inst__Unrecognized__,        // 01 D0
	Inst__Unrecognized__,        // 01 D1
	Inst__Unrecognized__,        // 01 D2
	Inst__Unrecognized__,        // 01 D3
	Inst__Unrecognized__,        // 01 D4
	Inst__Unrecognized__,        // 01 D5
	Inst__Unrecognized__,        // 01 D6
	Inst__Unrecognized__,        // 01 D7
	Inst__Unrecognized__,        // 01 D8
	Inst__Unrecognized__,        // 01 D9
	Inst__Unrecognized__,        // 01 DA
	Inst__Unrecognized__,        // 01 DB
	Inst__Unrecognized__,        // 01 DC
	Inst__Unrecognized__,        // 01 DD
	Inst__Unrecognized__,        // 01 DE
	Inst__Unrecognized__,        // 01 DF
	Inst__Unrecognized__,        // 01 E0
	Inst__Unrecognized__,        // 01 E1
	Inst__Unrecognized__,        // 01 E2
	Inst__Unrecognized__,        // 01 E3
	Inst__Unrecognized__,        // 01 E4
	Inst__Unrecognized__,        // 01 E5
	Inst__Unrecognized__,        // 01 E6
	Inst__Unrecognized__,        // 01 E7
	Inst__Unrecognized__,        // 01 E8
	Inst__Unrecognized__,        // 01 E9
	Inst__Unrecognized__,        // 01 EA
	Inst__Unrecognized__,        // 01 EB
	Inst__Unrecognized__,        // 01 EC
	Inst__Unrecognized__,        // 01 ED
	Inst__Unrecognized__,        // 01 EE
	Inst__Unrecognized__,        // 01 EF
	Inst__Unrecognized__,        // 01 F0
	Inst__Unrecognized__,        // 01 F1
	Inst__Unrecognized__,        // 01 F2
	Inst__Unrecognized__,        // 01 F3
	Inst__Unrecognized__,        // 01 F4
	Inst__Unrecognized__,        // 01 F5
	Inst__Unrecognized__,        // 01 F6
	Inst__Unrecognized__,        // 01 F7
	Inst__Unrecognized__,        // 01 F8
	Inst__Unrecognized__,        // 01 F9
	Inst__Unrecognized__,        // 01 FA
	Inst__Unrecognized__,        // 01 FB
	Inst__Unrecognized__,        // 01 FC
	Inst__Unrecognized__,        // 01 FD
	Inst__Unrecognized__,        // 01 FE
	Inst__Unrecognized__         // 01 FF
};

static DecoderProc DecoderTable10[256] = {
	InstInitSubsystem,         // 10 00
	InstLoadBackground,        // 10 01
	InstSwapBackgrounds,       // 10 02
	Inst__Unrecognized__,      // 10 03
	InstUnkBglink1004,         // 10 04
	InstLoadCharacter,         // 10 05
	InstUnk1006,               // 10 06
	InstCopyBackground,        // 10 07
	Inst__Unrecognized__,      // 10 08
	InstEvalTwo,               // 10 09
	InstNop,                   // 10 0A
	InstNop,                   // 10 0B
	InstNop3,                  // 10 0C
	Inst__Unrecognized__,      // 10 0D
	InstEvalTwo,               // 10 0E
	Inst__Unrecognized__,      // 10 0F
	InstHideBackgroundMaybe,   // 10 10
	InstHideCharacterMaybe,    // 10 11
	InstNop,                   // 10 12
	InstUnk1013,               // 10 13
	InstSaveResetRelated1014,  // 10 14
	InstNop3,                  // 10 15
	Inst__Unrecognized__,      // 10 16
	Inst__Unrecognized__,      // 10 17
	Inst__Unrecognized__,      // 10 18
	Inst__Unrecognized__,      // 10 19
	InstUnk101A,               // 10 1A
	InstNop3,                  // 10 1B
	InstNop3,                  // 10 1C
	InstUnk101D,               // 10 1D
	InstUnk101E,               // 10 1E
	InstUnk101F,               // 10 1F
	InstUnk1020,               // 10 20
	InstUnk1021,               // 10 21
	InstGroup1022,             // 10 22
	InstUnk1023,               // 10 23
	InstUnk1024,               // 10 24
	Inst__Unrecognized__,      // 10 25
	Inst__Unrecognized__,      // 10 26
	InstUnk1027,               // 10 27
	InstUnk1028,               // 10 28
	InstSetEnvFlag,            // 10 29
	InstUnk102A,               // 10 2A
	Inst__Unrecognized__,      // 10 2B
	Inst__Unrecognized__,      // 10 2C
	InstNop,                   // 10 2D
	Inst__Unrecognized__,      // 10 2E
	InstNop,                   // 10 2F
	InstGroup1030,             // 10 30
	Inst__Unrecognized__,      // 10 31
	Inst__Unrecognized__,      // 10 32
	InstGroupTips,             // 10 33
	InstUnk1034,               // 10 34
	Inst__Unrecognized__,      // 10 35
	InstNop3,                  // 10 36
	InstGroup1037,             // 10 37
	Inst__Unrecognized__,      // 10 38
	Inst__Unrecognized__,      // 10 39
	Inst__Unrecognized__,      // 10 3A
	Inst__Unrecognized__,      // 10 3B
	Inst__Unrecognized__,      // 10 3C
	Inst__Unrecognized__,      // 10 3D
	Inst__Unrecognized__,      // 10 3E
	Inst__Unrecognized__,      // 10 3F
	InstWin32_SetResolution,   // 10 40
	InstWin32_DestroyWindow,   // 10 41
	Inst__Unrecognized__,      // 10 42
	Inst__Unrecognized__,      // 10 43
	Inst__Unrecognized__,      // 10 44
	Inst__Unrecognized__,      // 10 45
	Inst__Unrecognized__,      // 10 46
	Inst__Unrecognized__,      // 10 47
	Inst__Unrecognized__,      // 10 48
	Inst__Unrecognized__,      // 10 49
	Inst__Unrecognized__,      // 10 4A
	Inst__Unrecognized__,      // 10 4B
	Inst__Unrecognized__,      // 10 4C
	Inst__Unrecognized__,      // 10 4D
	Inst__Unrecognized__,      // 10 4E
	Inst__Unrecognized__,      // 10 4F
	Inst__Unrecognized__,      // 10 50
	Inst__Unrecognized__,      // 10 51
	Inst__Unrecognized__,      // 10 52
	Inst__Unrecognized__,      // 10 53
	Inst__Unrecognized__,      // 10 54
	Inst__Unrecognized__,      // 10 55
	Inst__Unrecognized__,      // 10 56
	Inst__Unrecognized__,      // 10 57
	Inst__Unrecognized__,      // 10 58
	Inst__Unrecognized__,      // 10 59
	Inst__Unrecognized__,      // 10 5A
	Inst__Unrecognized__,      // 10 5B
	Inst__Unrecognized__,      // 10 5C
	Inst__Unrecognized__,      // 10 5D
	Inst__Unrecognized__,      // 10 5E
	Inst__Unrecognized__,      // 10 5F
	Inst__Unrecognized__,      // 10 60
	Inst__Unrecognized__,      // 10 61
	Inst__Unrecognized__,      // 10 62
	Inst__Unrecognized__,      // 10 63
	Inst__Unrecognized__,      // 10 64
	Inst__Unrecognized__,      // 10 65
	Inst__Unrecognized__,      // 10 66
	Inst__Unrecognized__,      // 10 67
	Inst__Unrecognized__,      // 10 68
	Inst__Unrecognized__,      // 10 69
	Inst__Unrecognized__,      // 10 6A
	Inst__Unrecognized__,      // 10 6B
	Inst__Unrecognized__,      // 10 6C
	Inst__Unrecognized__,      // 10 6D
	Inst__Unrecognized__,      // 10 6E
	Inst__Unrecognized__,      // 10 6F
	Inst__Unrecognized__,      // 10 70
	Inst__Unrecognized__,      // 10 71
	Inst__Unrecognized__,      // 10 72
	Inst__Unrecognized__,      // 10 73
	Inst__Unrecognized__,      // 10 74
	Inst__Unrecognized__,      // 10 75
	Inst__Unrecognized__,      // 10 76
	Inst__Unrecognized__,      // 10 77
	Inst__Unrecognized__,      // 10 78
	Inst__Unrecognized__,      // 10 79
	Inst__Unrecognized__,      // 10 7A
	Inst__Unrecognized__,      // 10 7B
	Inst__Unrecognized__,      // 10 7C
	Inst__Unrecognized__,      // 10 7D
	Inst__Unrecognized__,      // 10 7E
	Inst__Unrecognized__,      // 10 7F
	Inst__Unrecognized__,      // 10 80
	Inst__Unrecognized__,      // 10 81
	Inst__Unrecognized__,      // 10 82
	Inst__Unrecognized__,      // 10 83
	Inst__Unrecognized__,      // 10 84
	Inst__Unrecognized__,      // 10 85
	Inst__Unrecognized__,      // 10 86
	Inst__Unrecognized__,      // 10 87
	Inst__Unrecognized__,      // 10 88
	Inst__Unrecognized__,      // 10 89
	Inst__Unrecognized__,      // 10 8A
	Inst__Unrecognized__,      // 10 8B
	Inst__Unrecognized__,      // 10 8C
	Inst__Unrecognized__,      // 10 8D
	Inst__Unrecognized__,      // 10 8E
	Inst__Unrecognized__,      // 10 8F
	Inst__Unrecognized__,      // 10 90
	Inst__Unrecognized__,      // 10 91
	Inst__Unrecognized__,      // 10 92
	Inst__Unrecognized__,      // 10 93
	Inst__Unrecognized__,      // 10 94
	Inst__Unrecognized__,      // 10 95
	Inst__Unrecognized__,      // 10 96
	Inst__Unrecognized__,      // 10 97
	Inst__Unrecognized__,      // 10 98
	Inst__Unrecognized__,      // 10 99
	Inst__Unrecognized__,      // 10 9A
	Inst__Unrecognized__,      // 10 9B
	Inst__Unrecognized__,      // 10 9C
	Inst__Unrecognized__,      // 10 9D
	Inst__Unrecognized__,      // 10 9E
	Inst__Unrecognized__,      // 10 9F
	Inst__Unrecognized__,      // 10 A0
	Inst__Unrecognized__,      // 10 A1
	Inst__Unrecognized__,      // 10 A2
	Inst__Unrecognized__,      // 10 A3
	Inst__Unrecognized__,      // 10 A4
	Inst__Unrecognized__,      // 10 A5
	Inst__Unrecognized__,      // 10 A6
	Inst__Unrecognized__,      // 10 A7
	Inst__Unrecognized__,      // 10 A8
	Inst__Unrecognized__,      // 10 A9
	Inst__Unrecognized__,      // 10 AA
	Inst__Unrecognized__,      // 10 AB
	Inst__Unrecognized__,      // 10 AC
	Inst__Unrecognized__,      // 10 AD
	Inst__Unrecognized__,      // 10 AE
	Inst__Unrecognized__,      // 10 AF
	Inst__Unrecognized__,      // 10 B0
	Inst__Unrecognized__,      // 10 B1
	Inst__Unrecognized__,      // 10 B2
	Inst__Unrecognized__,      // 10 B3
	Inst__Unrecognized__,      // 10 B4
	Inst__Unrecognized__,      // 10 B5
	Inst__Unrecognized__,      // 10 B6
	Inst__Unrecognized__,      // 10 B7
	Inst__Unrecognized__,      // 10 B8
	Inst__Unrecognized__,      // 10 B9
	Inst__Unrecognized__,      // 10 BA
	Inst__Unrecognized__,      // 10 BB
	Inst__Unrecognized__,      // 10 BC
	Inst__Unrecognized__,      // 10 BD
	Inst__Unrecognized__,      // 10 BE
	Inst__Unrecognized__,      // 10 BF
	Inst__Unrecognized__,      // 10 C0
	Inst__Unrecognized__,      // 10 C1
	Inst__Unrecognized__,      // 10 C2
	Inst__Unrecognized__,      // 10 C3
	Inst__Unrecognized__,      // 10 C4
	Inst__Unrecognized__,      // 10 C5
	Inst__Unrecognized__,      // 10 C6
	Inst__Unrecognized__,      // 10 C7
	Inst__Unrecognized__,      // 10 C8
	Inst__Unrecognized__,      // 10 C9
	Inst__Unrecognized__,      // 10 CA
	Inst__Unrecognized__,      // 10 CB
	Inst__Unrecognized__,      // 10 CC
	Inst__Unrecognized__,      // 10 CD
	Inst__Unrecognized__,      // 10 CE
	Inst__Unrecognized__,      // 10 CF
	Inst__Unrecognized__,      // 10 D0
	Inst__Unrecognized__,      // 10 D1
	Inst__Unrecognized__,      // 10 D2
	Inst__Unrecognized__,      // 10 D3
	Inst__Unrecognized__,      // 10 D4
	Inst__Unrecognized__,      // 10 D5
	Inst__Unrecognized__,      // 10 D6
	Inst__Unrecognized__,      // 10 D7
	Inst__Unrecognized__,      // 10 D8
	Inst__Unrecognized__,      // 10 D9
	Inst__Unrecognized__,      // 10 DA
	Inst__Unrecognized__,      // 10 DB
	Inst__Unrecognized__,      // 10 DC
	Inst__Unrecognized__,      // 10 DD
	Inst__Unrecognized__,      // 10 DE
	Inst__Unrecognized__,      // 10 DF
	Inst__Unrecognized__,      // 10 E0
	Inst__Unrecognized__,      // 10 E1
	Inst__Unrecognized__,      // 10 E2
	Inst__Unrecognized__,      // 10 E3
	Inst__Unrecognized__,      // 10 E4
	Inst__Unrecognized__,      // 10 E5
	Inst__Unrecognized__,      // 10 E6
	Inst__Unrecognized__,      // 10 E7
	Inst__Unrecognized__,      // 10 E8
	Inst__Unrecognized__,      // 10 E9
	Inst__Unrecognized__,      // 10 EA
	Inst__Unrecognized__,      // 10 EB
	Inst__Unrecognized__,      // 10 EC
	Inst__Unrecognized__,      // 10 ED
	Inst__Unrecognized__,      // 10 EE
	Inst__Unrecognized__,      // 10 EF
	Inst__Unrecognized__,      // 10 F0
	Inst__Unrecognized__,      // 10 F1
	Inst__Unrecognized__,      // 10 F2
	Inst__Unrecognized__,      // 10 F3
	Inst__Unrecognized__,      // 10 F4
	Inst__Unrecognized__,      // 10 F5
	Inst__Unrecognized__,      // 10 F6
	Inst__Unrecognized__,      // 10 F7
	Inst__Unrecognized__,      // 10 F8
	Inst__Unrecognized__,      // 10 F9
	Inst__Unrecognized__,      // 10 FA
	Inst__Unrecognized__,      // 10 FB
	Inst__Unrecognized__,      // 10 FC
	Inst__Unrecognized__,      // 10 FD
	Inst__Unrecognized__,      // 10 FE
	Inst__Unrecognized__       // 10 FF
};

#pragma endregion

ZeroDisassembler::~ZeroDisassembler()
{
}

SC3Instruction* ZeroDisassembler::DisassembleAt(SCXOffset address,
	SCXOffset maxLength) {
	uint8_t* inst = _file.getPData() + address;
	uint8_t opcodeMSB = inst[0];
	switch (opcodeMSB) {
	case 0:
		return DecoderTable00[inst[1]](inst, address, maxLength);
		break;
	case 1:
		return DecoderTable01[inst[1]](inst, address, maxLength);
		break;
	case 0x10:
		return DecoderTable10[inst[1]](inst, address, maxLength);
		break;
	case 0xFE:
		return InstAssign(inst, address, maxLength);
		break;
	default:
		return Inst__Unrecognized__(inst, address, maxLength);
		break;
	}
}