#ifndef UNDRWRLD_H
#define UNDRWRLD_H

struct UW_ScreenXY 
{
	short sx; // size=0, offset=0
	short sy; // size=0, offset=2
	short dx; // size=0, offset=4
	short dy; // size=0, offset=6
};

extern void UNDERWORLD_StartProcess(); // 0x800B4B90

extern long UNDERWORLD_RotateScreenStep(long time); // 0x800B4C14

extern void UNDERWORLD_DoUV(unsigned char *uv, struct UW_ScreenXY *p0, int tx); // 0x800B4D80

extern POLY_GT3 * UNDERWORLD_Poly(POLY_GT3 *last, struct UW_ScreenXY *p0, struct UW_ScreenXY *p1, struct UW_ScreenXY *p2, int drawY); // 0x800B4DCC

extern void UNDERWORLD_DisplayFrame(long *primStart, long drawY); // 0x800B4F44

extern void UNDERWORLD_SetupSource(); // 0x800B509C

extern void UNDERWORLD_InitDisplayProcess(); // 0x800B5148

extern struct _StreamUnit * UNDERWORLD_LoadLevel(char *baseAreaName, struct GameTracker *gameTracker); // 0x800B5208

extern void UNDERWORLD_UpdatePlayer(struct Intro *playerIntro, struct _Instance *instance); // 0x800B5500


#endif
