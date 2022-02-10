#ifndef MBMISS_H
#define MBMISS_H


extern void WCBEGG_Message(struct _Instance *instance, unsigned long message, unsigned long data); // 0x8008EFA8

extern int WCBEGG_ShouldIgniteEgg(struct _Instance *egg, struct _walbossAttributes *wa); // 0x8008F04C

extern void WCBEGG_Process(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F144

extern void WCBEGG_ExplodeProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F338

extern void WCBEGG_SplitProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F478

extern void WCBEGG_CommonPostProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F574

extern void WCBEGG_CommonPostProcess2(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F604

extern void WCBEGG_ExplodeCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F678

extern void WCBEGG_Collide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F7FC

extern long WALBOSC_AnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, struct _Instance *instance); // 0x8008F884

extern void WALBOSC_Collide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F904

extern void WALBOSC_Message(struct _Instance *instance, unsigned long message, unsigned long data); // 0x8008F9C4


#endif
