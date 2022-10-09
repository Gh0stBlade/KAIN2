#ifndef ATTACK_H
#define ATTACK_H

#include "Game/PLAYER.H"

extern int StateHandlerDecodeHold(int *Message, int *Data); // 0x8009BB00

extern void StateHandlerAttack2(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009BE68

extern void StateHandlerCannedReaction(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009CF38

extern void StateHandlerStumble(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009D1C8

extern void StateHandlerHitReaction(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009D5EC

extern void StateHandlerThrow2(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009D868

extern void PointAt(struct _Instance *instance, _Position *Target, struct _Rotation *Rot1); // 0x8009E390

extern void ThrowSetFocusPoint(struct _Instance *instance, struct _Rotation *rot); // 0x8009E4B0

extern void LimitRotation(struct _Rotation *rot); // 0x8009E5F0

extern void StateHandlerGrab(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009E764


#endif
