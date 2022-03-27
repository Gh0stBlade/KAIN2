#ifndef ANMG2ILF_H
#define ANMG2ILF_H

typedef long (*TDRFuncPtr_G2Anim_SetCallback1callback)();

extern short G2Anim_GetElapsedTime(struct _G2Anim_Type *anim); // 0x8008FA10
extern struct _G2AnimKeylist_Type * G2Anim_GetKeylist(struct _G2Anim_Type *anim); // 0x8008FA34
extern void G2Anim_GetRootMotionOverInterval(struct _G2Anim_Type *anim, short intervalStart, short intervalEnd, struct _G2SVector3_Type *motionVector); // 0x8008FA58
extern void G2Anim_InterpToKeylistFrame(struct _G2Anim_Type *anim, struct _G2AnimKeylist_Type *keylist, int keylistID, int targetFrame, int duration); // 0x8008FA88
extern void G2Anim_SetAlphaTable(struct _G2Anim_Type *anim, struct _G2AnimAlphaTable_Type *table); // 0x8008FB2C
extern void G2Anim_SetCallback(struct _G2Anim_Type *anim, unsigned long (*func)(struct _G2Anim_Type*, int, enum _G2AnimCallbackMsg_Enum, long, long, struct _Instance*), void *data); // 0x8008FB9C
extern void G2Anim_SetLooping(struct _G2Anim_Type *anim); // 0x8008FBD8
extern void G2Anim_SetNoLooping(struct _G2Anim_Type *anim); // 0x8008FC38
extern void G2Anim_SetPaused(struct _G2Anim_Type *anim); // 0x8008FC98
extern void G2Anim_SetSpeedAdjustment(struct _G2Anim_Type *anim, long adjustment); // 0x8008FCF8
extern void G2Anim_SetUnpaused(struct _G2Anim_Type *anim); // 0x8008FD2C
extern void G2Anim_SwitchToKeylist(struct _G2Anim_Type *anim, struct _G2AnimKeylist_Type *keylist, int keylistID); // 0x8008FD8C
extern short G2AnimKeylist_GetDuration(struct _G2AnimKeylist_Type *keylist); // 0x8008FE0C
extern int G2AnimKeylist_GetKeyframeCount(struct _G2AnimKeylist_Type *keylist); // 0x8008FE34
extern void G2AnimSection_ClearAlarm(struct _G2AnimSection_Type *section, unsigned long flag); // 0x8008FE6C
extern int G2AnimSection_GetKeyframeNumber(struct _G2AnimSection_Type *section); // 0x8008FE80
extern int G2AnimSection_GetStoredKeyframeNumber(struct _G2AnimSection_Type *section); // 0x8008FEDC
extern void G2AnimSection_InterpToKeylistFrame(struct _G2AnimSection_Type *section, struct _G2AnimKeylist_Type *keylist, int keylistID, int targetFrame, int duration); // 0x8008FEFC
extern enum _G2Bool_Enum G2AnimSection_IsInInterpolation(struct _G2AnimSection_Type *section); // 0x8008FF3C
extern short G2AnimSection_NextKeyframe(struct _G2AnimSection_Type *section); // 0x8008FF6C
extern void G2AnimSection_SetAlphaTable(struct _G2AnimSection_Type *section, struct _G2AnimAlphaTable_Type *table); // 0x8008FFC8
extern void G2AnimSection_SetInterpInfo(struct _G2AnimSection_Type *section, struct _G2AnimInterpInfo_Type *newInfoPtr); // 0x8008FFE4
extern void G2AnimSection_SetLooping(struct _G2AnimSection_Type *section); // 0x80090014
extern void G2AnimSection_SetLoopRangeAll(struct _G2AnimSection_Type *section); // 0x80090054
extern void G2AnimSection_SetNoLooping(struct _G2AnimSection_Type *section); // 0x80090084
extern void G2AnimSection_SetNotRewinding(struct _G2AnimSection_Type *section); // 0x80090098
extern void G2AnimSection_SetPaused(struct _G2AnimSection_Type *section); // 0x800900AC
extern void G2AnimSection_SetUnpaused(struct _G2AnimSection_Type *section); // 0x800900C0
extern void G2AnimSection_SwitchToKeylist(struct _G2AnimSection_Type *section, struct _G2AnimKeylist_Type *keylist, int keylistID); // 0x800900D4

#endif
