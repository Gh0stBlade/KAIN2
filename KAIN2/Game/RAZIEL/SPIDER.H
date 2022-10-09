#ifndef SPIDER_H
#define SPIDER_H

#include "Game/PLAYER.H"

extern void StateHandlerWallGrab(struct __CharacterState *In, int CurrentSection, int Data); // 0x800A0E58

extern void StateHandlerWallIdle(struct __CharacterState *In, int CurrentSection, int Data); // 0x800A10C8

extern void StateHandlerWallDismount(struct __CharacterState *In, int CurrentSection, int Data); // 0x800A19B0


#endif
