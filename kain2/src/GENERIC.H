#ifndef GENERIC_H
#define GENERIC_H

union GENERIC_76fake // Hashcode: 0x55C9A583 (dec: 1439278467)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: generic.o, generic.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: generic.o, generic.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: generic.o, generic.o
};

union GENERIC_77fake // Hashcode: 0x95D27D31 (dec: -1781367503)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct GENERIC_114fake // hashcode: 0xDFE61644 (dec: -538569148)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: generic.o, generic.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: generic.o, generic.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: generic.o, generic.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: generic.o, generic.o
};

struct evAnimationInstanceSwitchData // hashcode: 0xEE7F2A6C (dec: -293655956)
{
	struct _Instance* instance; // size=616, offset=0
	int anim; // size=0, offset=4
	int frame; // size=0, offset=8
	int frames; // size=0, offset=12
	int mode; // size=0, offset=16
};

union GENERIC_201fake // Hashcode: 0x7031E694 (dec: 1882318484)
{
	struct _Instance* target; // size=616, offset=0
	_Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	_Position throwVector; // size=6, offset=0
};

struct evControlSaveDataData // hashcode: 0xA76A68A9 (dec: -1486198615)
{
	long length; // size=0, offset=0
	void* data; // size=0, offset=4
};

struct GenericTune // hashcode: 0x1B714C34 (dec: 460409908)
{
	long flags; // size=0, offset=0
	void* shatterData; // size=0, offset=4
};

extern void GenericInit(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8003E194
extern void GenericCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8003E2D0
extern void GenericProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8003E2D8
extern unsigned long GenericQuery(struct _Instance *instance, unsigned long query); // 0x8003E32C
extern void GenericMessage(struct _Instance *instance, unsigned long message, unsigned long data); // 0x8003E4A4
extern void GenericRelocateTune(struct Object *object, long offset); // 0x8003E620

#endif
