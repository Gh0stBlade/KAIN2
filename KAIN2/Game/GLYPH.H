#ifndef GLYPH_H
#define GLYPH_H

#include "MONSTER/MONSTER.H"

struct GLYPH_114fake // hashcode: 0x249332FF (dec: 613626623)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: glyph.o, glyph.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: glyph.o, glyph.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: glyph.o, glyph.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: glyph.o, glyph.o
};

struct __GlyphData // hashcode: 0x9C88C1F1 (dec: -1668759055)
{
	void (*process)(struct _Instance*, int, int); // size=0, offset=0
	struct __MessageQueue messages; // size=136, offset=4
	short selectedGlyph; // size=0, offset=140
	short glyph_open; // size=0, offset=142
	short glyph_time; // size=0, offset=144
	short glyph_radius; // size=0, offset=146
	short glyph_scale; // size=0, offset=148
	short glyph_rotation; // size=0, offset=150
	short target_glyph_rotation; // size=0, offset=152
	short glyph_movement; // size=0, offset=154
};

struct _GlyphTuneData // hashcode: 0x14FFD660 (dec: 352310880)
{
	short glyph_size; // size=0, offset=0
	short glyph_darkness; // size=0, offset=2
	char glyph_costs[8]; // size=8, offset=4
	short glyph_range[8]; // size=16, offset=12
	long color_array[28]; // size=112, offset=28
};

typedef void (*TDRFuncPtr__GlyphSwitchProcess1process)();

extern void GlyphInit(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007A8BC
extern void GlyphCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007A9D8
extern void GlyphProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007A9E0
extern unsigned long GlyphQuery(struct _Instance *instance, unsigned long query); // 0x8007AA68
extern void GlyphPost(struct _Instance *instance, unsigned long message, unsigned long messageData); // 0x8007AA88
extern void _GlyphSwitchProcess(struct _Instance *instance, void (*func)(struct _Instance* instance, int data1, int data2)); // 0x8007AABC
extern int GlyphIsGlyphOpen(struct _Instance *instance); // 0x8007AB5C
extern int _GlyphIsGlyphSet(int glyph); // 0x8007AB70
extern int _GlyphIsGlyphUsable(int glyph); // 0x8007ABB4
extern int _GlyphIsAnyGlyphSet(); // 0x8007ABF0
extern int _GlyphCost(struct _GlyphTuneData *glyphtunedata, int glyphNum); // 0x8007AC20
extern void _GlyphDefaultProcess(struct _Instance *instance, int data1, int data2); // 0x8007AC54
extern void HUD_GetPlayerScreenPt(DVECTOR *center); // 0x8007ACF8
extern void GlyphDrawMenu(struct _Instance *instance); // 0x8007AD88
extern long GlyphTime(int time); // 0x8007B0EC
extern void ShrinkGlyphMenu(struct _Instance *instance); // 0x8007B108
extern void EnlargeGlyphMenu(struct _Instance *instance); // 0x8007B22C
extern void _GlyphOffProcess(struct _Instance *instance, int data1, int data2); // 0x8007B328
extern void _GlyphSelectProcess(struct _Instance *instance, int data1, int data2); // 0x8007B470
extern void Glyph_StartSpell(struct _Instance *instance, int glyphnum); // 0x8007B970
extern void Glyph_Broadcast(struct _Instance *sender, int glyphnum); // 0x8007BA18
extern void Glyph_DoSpell(struct _Instance *instance, int glyphnum); // 0x8007BC70
extern void Glyph_EndFX(); // 0x8007BF14
extern void Glyph_DoFX(struct _Instance *instance); // 0x8007BF28
extern void _GlyphGenericProcess(struct _Instance *instance, int data1, int data2); // 0x8007BFD8
extern void GlyphTrigger(); // 0x8007C110
extern void MANNA_Pickup(); // 0x8007C12C
extern void HEALTHU_Pickup(struct _Instance *instance); // 0x8007C148
extern void HUD_Damp(short *val, short target, short *vel, short max); // 0x8007C1D4
extern void HUD_Init(); // 0x8007C21C
extern void HUD_Setup_Chit_Count(int chits); // 0x8007C254
extern void HUD_Update(); // 0x8007C28C
extern void HUD_Draw(); // 0x8007C4C0

extern int hud_warp_arrow_flash;

#endif
