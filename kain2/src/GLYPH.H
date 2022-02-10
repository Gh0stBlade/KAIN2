#ifndef GLYPH_H
#define GLYPH_H


extern void GlyphInit(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007A8BC

extern void GlyphCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007A9D8

extern void GlyphProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007A9E0

extern unsigned long GlyphQuery(struct _Instance *instance, unsigned long query); // 0x8007AA68

extern void GlyphPost(struct _Instance *instance, unsigned long message, unsigned long messageData); // 0x8007AA88

extern void _GlyphSwitchProcess(struct _Instance *instance, TDRFuncPtr__GlyphSwitchProcess1process process); // 0x8007AABC

extern int GlyphIsGlyphOpen(struct _Instance *instance); // 0x8007AB5C

extern int _GlyphIsGlyphSet(int glyph); // 0x8007AB70

extern int _GlyphIsGlyphUsable(int glyph); // 0x8007ABB4

extern int _GlyphIsAnyGlyphSet(); // 0x8007ABF0

extern int _GlyphCost(struct _GlyphTuneData *glyphtunedata, int glyphNum); // 0x8007AC20

extern int GlyphIsPuppetShowModeOn(); // 0x8007AC30

extern void _GlyphDefaultProcess(struct _Instance *instance, int data1, int data2); // 0x8007AC54

extern void HUD_GetPlayerScreenPt(struct DVECTOR *center); // 0x8007ACF8

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


typedef void (*TDRFuncPtr__GlyphSwitchProcess1process)();




#endif
