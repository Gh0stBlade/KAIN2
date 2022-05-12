#ifndef MONAPI_H
#define MONAPI_H

#include "MONSTER.H"

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void MonsterProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007DA80
extern void MonsterInit(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007DBB8
extern void SendHitObject(struct _Instance *instance, struct _Instance *hit, int type); // 0x8007DDC0
extern void MonsterCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007DE94
extern void MonsterAdditionalCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007E3A8
extern unsigned long MonsterQuery(struct _Instance *instance, unsigned long query); // 0x8007E3D8
extern void MonsterMessage(struct _Instance *instance, unsigned long message, unsigned long data); // 0x8007E638
extern void AnimDistanceAndVel(struct Object *object, struct _MonsterAnim *mAnim); // 0x8007E900
extern void TranslateAnimList(struct Object *object, struct _MonsterAnim *animList, int numAnims); // 0x8007EA68
extern void MonsterTranslateAnim(struct Object *object); // 0x8007EB64
extern void MonsterRelocateTune(struct Object *object, long offset); // 0x8007ED1C
extern void MonsterRelocateInstanceObject(struct _Instance *instance, long offset); // 0x8007EF38
extern void MONAPI_TurnHead(struct _Instance *instance, short *rotx, short *rotz, struct GameTracker *gameTracker); // 0x8007F05C
extern void MONAPI_SetLookAround(struct _Instance *instance); // 0x8007F0D8
extern void MONAPI_ResetLookAround(struct _Instance *instance); // 0x8007F108
extern long MONAPI_OkToLookAround(struct _Instance *instance); // 0x8007F138
extern void MONAPI_DeleteRegen(struct _MONAPI_Regenerator *regen); // 0x8007F14C
extern void MONAPI_ProcessGenerator(); // 0x8007F198
extern void MONAPI_AddToGenerator(struct _Instance *instance); // 0x8007F2D8
extern void MONAPI_CheckGenerator(struct _Instance *instance); // 0x8007F378

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
