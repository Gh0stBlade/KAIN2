#ifndef SWIM_H
#define SWIM_H

#include "PLAYER.H"

extern void StateHandlerInitSwim(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009EEFC

extern void StateHandlerSwimCoil(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009F008

extern void StateHandlerSwimTread(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009F788

extern void StateHandlerSwimDiveIn(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009FF50

extern void StateHandlerSwim(struct __CharacterState *In, int CurrentSection, int Data); // 0x800A02DC


#endif
