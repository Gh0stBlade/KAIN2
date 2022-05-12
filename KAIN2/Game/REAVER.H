#ifndef REAVER_H
#define REAVER_H

struct __ReaverSaveDat // hashcode: 0x18BBFFA1 (dec: 414973857)
{
	unsigned long pad; // size=0, offset=0
};

struct __ReaverData // hashcode: 0xD1EA4FE0 (dec: -773173280)
{
	char ReaverPickedUp; // size=0, offset=0
	char ReaverOn; // size=0, offset=1
	short ReaverSize; // size=0, offset=2
	short CurrentReaver; // size=0, offset=4
	short ReaverDeg; // size=0, offset=6
	long ReaverChargeTime; // size=0, offset=8
	long ReaverShockAmount; // size=0, offset=12
	long ReaverGlowColor; // size=0, offset=16
	long ReaverBladeColor; // size=0, offset=20
	long ReaverBladeGlowColor; // size=0, offset=24
	short ReaverScale; // size=0, offset=28
	short ReaverTargetScale; // size=0, offset=30
	struct __ReaverSaveDat saveData; // size=4, offset=32
};

struct __ReaverTuneData // hashcode: 0xE4E74D43 (dec: -454603453)
{
	unsigned long spectralGlowColor; // size=0, offset=0
	unsigned long materialGlowColor; // size=0, offset=4
	unsigned long sunlightGlowColor; // size=0, offset=8
	unsigned long waterGlowColor; // size=0, offset=12
	unsigned long stoneGlowColor; // size=0, offset=16
	unsigned long fireGlowColor; // size=0, offset=20
	unsigned long spiritGlowColor; // size=0, offset=24
	unsigned long soundGlowColor; // size=0, offset=28
	unsigned long spectralInnerColor; // size=0, offset=32
	unsigned long materialInnerColor; // size=0, offset=36
	unsigned long sunlightInnerColor; // size=0, offset=40
	unsigned long waterInnerColor; // size=0, offset=44
	unsigned long stoneInnerColor; // size=0, offset=48
	unsigned long fireInnerColor; // size=0, offset=52
	unsigned long spiritInnerColor; // size=0, offset=56
	unsigned long soundInnerColor; // size=0, offset=60
	unsigned long spectralInnerGlowColor; // size=0, offset=64
	unsigned long materialInnerGlowColor; // size=0, offset=68
	unsigned long sunlightInnerGlowColor; // size=0, offset=72
	unsigned long waterInnerGlowColor; // size=0, offset=76
	unsigned long stoneInnerGlowColor; // size=0, offset=80
	unsigned long fireInnerGlowColor; // size=0, offset=84
	unsigned long spiritInnerGlowColor; // size=0, offset=88
	unsigned long soundInnerGlowColor; // size=0, offset=92
	unsigned long icon_colors[24]; // size=96, offset=96
};

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
