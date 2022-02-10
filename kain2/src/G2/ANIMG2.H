#ifndef ANIMG2_H
#define ANIMG2_H


extern enum _G2Bool_Enum G2Anim_Install(); // 0x800929A4

extern void G2Anim_ResetInternalState(); // 0x80092A0C

extern void G2Anim_Init(struct _G2Anim_Type *anim, struct _Model *modelData); // 0x80092A64

extern struct _G2AnimSection_Type * G2Anim_AddSection(struct _G2Anim_Type *anim, int firstSegID, int segCount); // 0x80092AF8

extern void G2Anim_Free(struct _G2Anim_Type *anim); // 0x80092B94

extern void G2Anim_Restore(struct _G2Anim_Type *anim); // 0x80092C50

extern void G2Anim_BuildTransforms(struct _G2Anim_Type *anim); // 0x80092CD0

extern void G2Anim_UpdateStoredFrame(struct _G2Anim_Type *anim); // 0x80092D9C

extern struct _G2AnimSection_Type * G2Anim_GetSectionWithSeg(struct _G2Anim_Type *anim, int segNumber); // 0x80092F18

extern enum _G2Bool_Enum G2Anim_SegmentHasActiveChannels(struct _G2Anim_Type *anim, int segNumber, unsigned short chanMask); // 0x80092F84

extern void G2Anim_GetSegChannelValue(struct _G2Anim_Type *anim, int segIndex, unsigned short *valueTable, unsigned short channelMask); // 0x800930C4

extern void G2Anim_GetRootMotionFromTimeForDuration(struct _G2Anim_Type *anim, short durationStart, short duration, struct _G2SVector3_Type *motionVector); // 0x80093170

extern void G2AnimSection_SwitchToKeylistAtTime(struct _G2AnimSection_Type *section, struct _G2AnimKeylist_Type *keylist, int keylistID, short targetTime); // 0x80093448

extern void G2AnimSection_JumpToTime(struct _G2AnimSection_Type *section, short targetTime); // 0x80093600

extern short G2AnimSection_UpdateOverInterval(struct _G2AnimSection_Type *section, short interval); // 0x800936A0

extern short G2AnimSection_AdvanceOverInterval(struct _G2AnimSection_Type *section, short interval); // 0x80093894

extern short G2AnimSection_RewindOverInterval(struct _G2AnimSection_Type *section, short interval); // 0x80093C6C

extern void _G2Anim_BuildTransformsNoControllers(struct _G2Anim_Type *anim); // 0x80093EAC

extern void _G2Anim_BuildSegTransformNoControllers(struct _G2Matrix_Type *segMatrix, struct _G2Matrix_Type *parentMatrix, enum _G2Bool_Enum bRootTransUpdated, int segIndex); // 0x80094000

extern void _G2Anim_BuildSegLocalRotMatrix(struct _G2AnimSegValue_Type *segValue, struct _G2Matrix_Type *segMatrix); // 0x80094228

extern void wombat(unsigned char *segKeyList, int flagBitOffset, struct _G2AnimSegKeyflagInfo_Type *kfInfo); // 0x80094288

extern unsigned long kangaroo(struct _G2AnimSegKeyflagInfo_Type *kfInfo); // 0x800942C0

extern void _G2Anim_InitializeSegValue(struct _G2Anim_Type *anim, struct _G2AnimSegValue_Type *segValue, int segIndex); // 0x80094348

extern void _G2AnimSection_InitStatus(struct _G2AnimSection_Type *section, struct _G2Anim_Type *anim); // 0x8009439C

extern void FooBar(struct _G2AnimSection_Type *section, struct _G2Anim_Type *anim, int decompressedKey, int targetKey, long timeOffset); // 0x80094648

extern void _G2AnimSection_UpdateStoredFrameFromData(struct _G2AnimSection_Type *section, struct _G2Anim_Type *anim); // 0x80094AD0

extern struct _G2Anim_Type * _G2AnimSection_GetAnim(struct _G2AnimSection_Type *section); // 0x80094BA0

extern void _G2AnimSection_TriggerEffects(struct _G2AnimSection_Type *section, short startTime, short endTime); // 0x80094BC0

extern void _G2Anim_FreeChanStatusBlockList(struct _G2AnimChanStatusBlock_Type *block); // 0x80094CEC

extern long _G2AnimAlphaTable_GetValue(struct _G2AnimAlphaTable_Type *table, long trueAlpha); // 0x80094D2C


#endif
