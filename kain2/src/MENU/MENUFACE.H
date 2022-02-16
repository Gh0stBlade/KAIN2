#ifndef MENUFACE_H
#define MENUFACE_H

union MENUFACE_71fake // Hashcode: 0x1C6ACF56 (dec: 476761942)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: menuface.o, menuface.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: menuface.o, menuface.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: menuface.o, menuface.o
};

union MENUFACE_72fake // Hashcode: 0xFB4CCE52 (dec: -78852526)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MENUFACE_109fake // hashcode: 0xD61A4E22 (dec: -702919134)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: menuface.o, menuface.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: menuface.o, menuface.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: menuface.o, menuface.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: menuface.o, menuface.o
};

enum bdepth // Hashcode: 0xB297A406 (dec: -1298684922)
{
	TIM_4BIT = 0,
	TIM_8BIT = 1,
	TIM_16BIT = 2,
};

struct menuface_t // hashcode: 0xC2732FD8 (dec: -1032638504)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	char w; // size=0, offset=4
	char h; // size=0, offset=5
	char frames; // size=0, offset=6
	short curFrame; // size=0, offset=8
	short transitionDir; // size=0, offset=10
	unsigned short delay; // size=0, offset=12
	unsigned short loaded; // size=0, offset=14
};

typedef enum bdepth bdepth;
typedef struct menuface_t menuface_t;

extern char * NextTimAddr(char *addr, int w, int h, enum bdepth bpp); // 0x800B9538
extern void menuface_initialize(); // 0x800B9580
extern void menuface_terminate(); // 0x800B96AC
extern void MENUFACE_ChangeStateRandomly(int index); // 0x800B9758
extern void MENUFACE_RefreshFaces(); // 0x800B987C

#endif
