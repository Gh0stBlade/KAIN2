#ifndef MENUUTIL_H
#define MENUUTIL_H

#include "MENU.H"

union MENUUTIL_71fake // Hashcode: 0x6C5644DB (dec: 1817593051)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: menuutil.o, menuutil.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: menuutil.o, menuutil.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: menuutil.o, menuutil.o
};

union MENUUTIL_72fake // Hashcode: 0x9906017D (dec: -1727659651)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MENUUTIL_109fake // hashcode: 0x36B25773 (dec: 917657459)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: menuutil.o, menuutil.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: menuutil.o, menuutil.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: menuutil.o, menuutil.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: menuutil.o, menuutil.o
};

extern enum menu_ctrl_t menu_get_ctrl(void *gt); // 0x800B87FC
extern void menu_print(int xpos, int ypos, char *text, int color); // 0x800B887C
extern int menu_text_width(char *text); // 0x800B88E4
extern void menu_sound(enum menu_sound_t sound); // 0x800B8904

#endif
