#ifndef RAZIEL_H
#define RAZIEL_H


extern void InitStates(struct _Instance *PlayerInstance); // 0x800A80C8

extern void StateInitIdle(struct __CharacterState *In, int CurrentSection, int Ptr); // 0x800A83B0

extern void StateHandlerIdle(struct __CharacterState *In, int CurrentSection, int Data); // 0x800A8668

extern void StateHandlerLookAround(struct __CharacterState *In, int CurrentSection, int Data); // 0x800A8F28

extern void StateHandlerCrouch(struct __CharacterState *In, int CurrentSection, int Data); // 0x800A9270

extern void StateHandlerDropAction(struct __CharacterState *In, int CurrentSection, int Data); // 0x800A99E8

extern void StateHandlerSoulSuck(struct __CharacterState *In, int CurrentSection, int Data); // 0x800A9C68

extern void StateHandlerStartTurn(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AA1A0

extern void StateInitStartMove(struct __CharacterState *In, int CurrentSection, int Frame); // 0x800AA3A0

extern void StateHandlerStartMove(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AA44C

extern void StateInitMove(struct __CharacterState *In, int CurrentSection, int Frames); // 0x800AAAEC

extern void StateHandlerMove(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AAD30

extern void StateHandlerStopMove(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AB430

extern void StateHandlerCompression(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AB788

extern void StateHandlerJump(struct __CharacterState *In, int CurrentSection, int Data); // 0x800ABB4C

extern void StateHandlerFall(struct __CharacterState *In, int CurrentSection, int Data); // 0x800ABF84

extern void StateHandlerSlide(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AC3F8

extern void StateHandlerBlock(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AC5D8

extern void StateHandlerDeCompression(struct __CharacterState *In, int CurrentSection, int Data); // 0x800ACA44

extern void StateHandlerGlide(struct __CharacterState *In, int CurrentSection, int Data); // 0x800ACDB4

extern void StateHandlerHang(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AD520

extern void StateHandlerPushObject(struct __CharacterState *In, int CurrentSection, int Data); // 0x800ADAC4

extern void StateHandlerBreakOff(struct __CharacterState *In, int CurrentSection, int Data); // 0x800ADDF0

extern void StateHandlerPullSwitch(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AE1F4

extern void StateHandlerDragObject(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AE804

extern void StateHandlerPickupObject(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AEF2C

extern void StateHandlerAutoFace(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AF148

extern void StateHandlerGlyphs(struct __CharacterState *In, int CurrentSection, int Data); // 0x800AF888

extern void DefaultStateHandler(struct __CharacterState *In, int CurrentSection, int Data); // 0x800B0114

extern long RazielAnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, void *data); // 0x800B0AAC

extern long RazielAnimCallbackDuringPause(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, void *data); // 0x800B10B0

extern unsigned long RazielQuery(struct _Instance *instance, unsigned long Query); // 0x800B1164

extern void RazielPost(struct _Instance *instance, unsigned long Message, unsigned long Data); // 0x800B1528

extern int SetStates(struct _Instance *instance, struct GameTracker *GT, long *controlCommand, int AnalogLength); // 0x800B1EF8

extern void ProcessConstrict(); // 0x800B2588

extern void RelocateConstrict(struct _SVector *offset); // 0x800B285C

extern void ProcessEffects(struct _Instance *instance); // 0x800B29BC

extern void ProcessHints(); // 0x800B2BD0

extern void ProcessInteractiveMusic(struct _Instance *instance); // 0x800B2D08

extern void ProcessTimers(struct _Instance *instance); // 0x800B2EB8

extern void SetTimer(int ticks); // 0x800B33AC

extern void ProcessSpecialAbilities(struct _Instance *instance); // 0x800B33BC

extern int GetControllerMessages(long *controlCommand); // 0x800B3574

extern void RazielAdditionalCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x800B377C

extern int GetEngageEvent(struct _Instance *instance); // 0x800B3C78

extern int SetupReaction(struct _Instance *player, struct _Instance *instance); // 0x800B3CD0

extern int CheckHolding(struct _Instance *instance); // 0x800B3DC0

extern void DisableWristCollision(struct _Instance *instance, int Side); // 0x800B3DCC

extern void EnableWristCollision(struct _Instance *instance, int Side); // 0x800B3E1C

extern int GetCollisionType(struct _Instance *instance); // 0x800B3E6C

extern void InitGlyphSystem(struct _Instance *instance); // 0x800B4014

extern void mdRazielProcess(struct _Instance *playerInstance, struct GameTracker *gameTracker, long *controlCommand); // 0x800B404C

extern void RazielProcess(struct _Instance *playerInstance, struct GameTracker *gameTracker); // 0x800B4104

extern void RazielInit(struct _Instance *instance, struct GameTracker *gameTracker); // 0x800B4148

extern void RazielCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x800B420C

extern void RAZIEL_TurnHead(struct _Instance *instance, short *rotx, short *rotz, struct GameTracker *gameTracker); // 0x800B4470

extern void RAZIEL_SetLookAround(struct _Instance *instance); // 0x800B4694

extern void RAZIEL_ResetLookAround(struct _Instance *instance); // 0x800B470C

extern long RAZIEL_OkToLookAround(struct _Instance *playerInstance); // 0x800B4788

extern void TrailWaterFX(struct _Instance *instance, int Segment, int Bubbles, int Type); // 0x800B4810


#endif
