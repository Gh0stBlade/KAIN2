#ifndef UNDRWRLD_H
#define UNDRWRLD_H

union UNDRWRLD_76fake // Hashcode: 0x6E59B9C2 (dec: 1851374018)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: undrwrld.o, undrwrld.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: undrwrld.o, undrwrld.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: undrwrld.o, undrwrld.o
};

union UNDRWRLD_77fake // Hashcode: 0x395B97AA (dec: 962303914)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct UNDRWRLD_114fake // hashcode: 0x3CF3A383 (dec: 1022600067)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: undrwrld.o, undrwrld.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: undrwrld.o, undrwrld.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: undrwrld.o, undrwrld.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: undrwrld.o, undrwrld.o
};

union UNDRWRLD_195fake // Hashcode: 0xA8618D78 (dec: -1470001800)
{
	struct _Instance* target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct UW_ScreenXY // hashcode: 0x88757B16 (dec: -2005566698)
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
