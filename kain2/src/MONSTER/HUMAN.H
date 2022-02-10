#ifndef HUMAN_H
#define HUMAN_H


extern void HUMAN_WaitForWeapon(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007CB40

extern struct _Instance * HUMAN_CreateWeapon(struct _Instance *instance, int weaponid, int segment); // 0x8007CBB0

extern void HUMAN_Init(struct _Instance *instance); // 0x8007CC84

extern void HUMAN_CleanUp(struct _Instance *instance); // 0x8007CD7C

extern unsigned long HUMAN_Query(struct _Instance *instance, unsigned long query); // 0x8007CD9C

extern void HUMAN_DeadEntry(struct _Instance *instance); // 0x8007CE8C

extern void HUMAN_Dead(struct _Instance *instance); // 0x8007CF04

extern void HUMAN_StunnedEntry(struct _Instance *instance); // 0x8007CFEC

extern void HUMAN_Stunned(struct _Instance *instance); // 0x8007D064

extern void HUMAN_EmbraceEntry(struct _Instance *instance); // 0x8007D10C

extern void HUMAN_Embrace(struct _Instance *instance); // 0x8007D168

extern void HUMAN_IdleEntry(struct _Instance *instance); // 0x8007D330

extern void HUMAN_Idle(struct _Instance *instance); // 0x8007D378

extern void HUMAN_Flee(struct _Instance *instance); // 0x8007D530

extern void HUMAN_GetAngry(); // 0x8007D688

extern int HUMAN_TypeOfHuman(struct _Instance *instance); // 0x8007D750


#endif
