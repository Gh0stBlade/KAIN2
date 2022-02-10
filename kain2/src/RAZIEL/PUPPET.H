#ifndef PUPPET_H
#define PUPPET_H


extern void StateHandlerPuppetShow(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009A984

extern void StateHandlerMoveToPosition(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009AD20

extern void DefaultPuppetStateHandler(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009AFC8

extern void StateHandlerWarpGate(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009B27C

extern void StateHandlerForcedGlide(struct __CharacterState *In, int CurrentSection, int Data); // 0x8009B710


#endif
