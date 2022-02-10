#ifndef REAVER_H
#define REAVER_H


extern void SoulReaverInit(struct _Instance *instance, struct GameTracker *gameTracker); // 0x80079D48

extern void SoulReaverCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x80079DE8

extern void SoulReaverProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x80079F58

extern void CollideReaverProjectile(struct _Instance *instance, struct GameTracker *gameTracker); // 0x80079FA4

extern unsigned long SoulReaverQuery(struct _Instance *instance, unsigned long query); // 0x8007A100

extern void SoulReaverImbue(struct _Instance *instance, int number); // 0x8007A184

extern void SoulReaverCharge(struct _Instance *instance, struct __ReaverData *data); // 0x8007A210

extern void StopSoulReaverCharge(struct __ReaverData *data, struct _Instance *instance); // 0x8007A334

extern void SoulReaverPost(struct _Instance *instance, unsigned long message, unsigned long data); // 0x8007A374

extern unsigned long REAVER_GetGlowColor(struct _Instance *instance); // 0x8007A6D0

extern void _SoulReaverAnimate(struct _Instance *instance); // 0x8007A6F4

extern int SoulReaverFire(); // 0x8007A8B0


#endif
