#ifndef AADSQCMD_H
#define AADSQCMD_H


extern void aadSubstituteVariables(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055928

extern void metaCmdSelectChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800559E0

extern void metaCmdSelectSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055A00

extern void metaCmdAssignSequence(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055A50

extern void metaCmdUsePrimaryTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055AC4

extern void metaCmdUseSecondaryTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055ACC

extern void metaCmdSetTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055AD4

extern void metaCmdChangeTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055B54

extern void metaCmdSetTempoFromSequence(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055BC4

extern void metaCmdStartSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055C44

extern void metaCmdStopSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055C68

extern void metaCmdPauseSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055C8C

extern void metaCmdResumeSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055CB0

extern void metaCmdSetSlotBendRange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055CD4

extern void metaCmdSetChannelBendRange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055CDC

extern void metaCmdSetSlotVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055CE4

extern void metaCmdSetSlotPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055D18

extern void metaCmdSetChannelVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055D4C

extern void metaCmdSetChannelPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055D88

extern void metaCmdEnableSustainUpdate(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055DC4

extern void metaCmdDisableSustainUpdate(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055DE4

extern void metaCmdMuteChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055E08

extern void metaCmdUnMuteChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055E34

extern void metaCmdMuteChannelList(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055E60

extern void metaCmdUnMuteChannelList(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055E90

extern void metaCmdSetChannelMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055EC0

extern void metaCmdDelayMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055F14

extern void metaCmdUpdateMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055F38

extern void metaCmdChannelVolumeFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055FCC

extern void metaCmdChannelPanFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055FD4

extern void metaCmdSlotVolumeFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055FDC

extern void metaCmdSlotPanFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055FE4

extern void metaCmdSetChannelProgram(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055FEC

extern void metaCmdSetChannelBasePriority(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056004

extern void metaCmdSetChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x8005600C

extern void metaCmdIgnoreChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056024

extern void metaCmdRespectChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056044

extern void metaCmdSetChannelPitchMap(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056068

extern void metaCmdIgnoreChannelPitchMap(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056070

extern void metaCmdRespectChannelPitchMap(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056078

extern void metaCmdGetSequenceAssigned(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056080

extern void metaCmdGetTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800560B0

extern void metaCmdGetSlotStatus(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056114

extern void metaCmdGetChannelMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056144

extern void metaCmdGetChannelVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056194

extern void metaCmdGetChannelPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800561CC

extern void metaCmdGetChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056204

extern void metaCmdGetChannelProgram(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x8005620C

extern void metaCmdGetChannelBasePriority(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056244

extern void metaCmdGetChannelBendRange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x8005624C

extern void metaCmdGetSlotVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056254

extern void metaCmdGetSlotPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056284

extern void metaCmdSetVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800562B4

extern void metaCmdCopyVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800562E0

extern void metaCmdAddVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x8005631C

extern void metaCmdSubtractVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056354

extern void metaCmdSetVariableBits(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x8005638C

extern void metaCmdClearVariableBits(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800563C4

extern void aadGotoSequencePosition(struct _AadSequenceSlot *slot, int track, unsigned char *newPosition); // 0x800563FC

extern void aadGotoSequenceLabel(struct _AadSequenceSlot *slot, int track, int labelNumber); // 0x80056468

extern void metaCmdLoopStart(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056520

extern void metaCmdLoopEnd(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056590

extern void metaCmdLoopBreak(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056620

extern void metaCmdDefineLabel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056628

extern void metaCmdGotoLabel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056630

extern void metaCmdSetSequencePosition(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056660

extern void metaCmdBranchIfVarEqual(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056690

extern void metaCmdBranchIfVarNotEqual(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800566DC

extern void metaCmdBranchIfVarLess(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056728

extern void metaCmdBranchIfVarGreater(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056778

extern void metaCmdBranchIfVarLessOrEqual(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800567C8

extern void metaCmdBranchIfVarGreaterOrEqual(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056818

extern void metaCmdBranchIfVarBitsSet(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056868

extern void metaCmdBranchIfVarBitsClear(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800568B8

extern void metaCmdSubstituteVariableParam1(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056908

extern void metaCmdSubstituteVariableParam2(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056928

extern void metaCmdSubstituteVariableParam3(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056948

extern void metaCmdEndSequence(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80056968

extern void metaCmdPlaySoundEffect(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800569C4

extern void metaCmdStopSoundEffect(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800569CC

extern void metaCmdSetSoundEffectVolumePan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800569D4


#endif
