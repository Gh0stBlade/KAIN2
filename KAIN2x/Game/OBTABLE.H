#ifndef OBTABLE_H
#define OBTABLE_H

union OBTABLE_76fake // Hashcode: 0x95622E13 (dec: -1788727789)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: obtable.o, obtable.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: obtable.o, obtable.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: obtable.o, obtable.o
};

union OBTABLE_77fake // Hashcode: 0x8EF4F21E (dec: -1896549858)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct _ObjectOwnerInfo // hashcode: 0x04BCED5C (dec: 79490396)
{
	long magicnum; // size=0, offset=0
	short animID; // size=0, offset=4
	char objectName[0]; // size=0, offset=6
};

struct OBTABLE_114fake // hashcode: 0xA297FC58 (dec: -1567097768)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: obtable.o, obtable.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: obtable.o, obtable.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: obtable.o, obtable.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: obtable.o, obtable.o
};

struct ObjectAccess // hashcode: 0x1268C513 (dec: 308856083)
{
	char* objectName; // size=0, offset=0
	void* object; // size=0, offset=4
};

union OBTABLE_196fake // Hashcode: 0x921E9F5C (dec: -1843486884)
{
	struct _Instance* target; // size=616, offset=0
	_Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	_Position throwVector; // size=6, offset=0
};

typedef struct _ObjectOwnerInfo ObjectOwnerInfo;

typedef struct ObjectAccess ObjectAccess;

union OBTABLE2_76fake // Hashcode: 0x1E9F108F (dec: 513740943)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: obtable2.o, obtable2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: obtable2.o, obtable2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: obtable2.o, obtable2.o
};

union OBTABLE2_77fake // Hashcode: 0xA0B14AC6 (dec: -1598993722)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct OBTABLE2_114fake // hashcode: 0xAA6A3EB1 (dec: -1435877711)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: obtable2.o, obtable2.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: obtable2.o, obtable2.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: obtable2.o, obtable2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: obtable2.o, obtable2.o
};

struct ObjectFunc // hashcode: 0xFDE6E2DA (dec: -35200294)
{
	char* scriptName; // size=0, offset=0
	void (*initFunc)(struct _Instance*, struct GameTracker*); // size=0, offset=4
	void (*processFunc)(struct _Instance* playerInstance, struct GameTracker* gameTracker); // size=0, offset=8
	void (*collideFunc)(struct _Instance* instance, struct GameTracker* gameTracker); // size=0, offset=12
	unsigned long (*queryFunc)(struct _Instance* instance, unsigned long Query); // size=0, offset=16
	void (*messageFunc)(struct _Instance* instance, unsigned long Message, unsigned long Data); // size=0, offset=20
	void (*additionalCollideFunc)(struct _Instance* instance, struct GameTracker* gameTracker); // size=0, offset=24
	void (*relocateTuneFunc)(struct Object* object, long offset); // size=0, offset=28
	void (*relocateInstObFunc)(struct _Instance* instance, long offset); // size=0, offset=32
};

union OBTABLE2_209fake // Hashcode: 0xB29B0D0C (dec: -1298461428)
{
	struct _Instance* target; // size=616, offset=0
	_Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	_Position throwVector; // size=6, offset=0
};


extern struct ObjectAccess objectAccess[28];
typedef struct ObjectFunc ObjectFunc;

extern void OBTABLE_InstanceInit(struct _Instance *instance); // 0x8003D9AC
extern void OBTABLE_GetInstanceCollideFunc(struct _Instance *instance); // 0x8003DA90
extern void OBTABLE_GetInstanceAdditionalCollideFunc(struct _Instance *instance); // 0x8003DAD8
extern void OBTABLE_GetInstanceProcessFunc(struct _Instance *instance); // 0x8003DB18
extern void OBTABLE_GetInstanceQueryFunc(struct _Instance *instance); // 0x8003DB60
extern void OBTABLE_GetInstanceMessageFunc(struct _Instance *instance); // 0x8003DBA8
extern void OBTABLE_InitObjectWithID(struct Object *object); // 0x8003DBF0
extern void OBTABLE_ClearObjectReferences(); // 0x8003DDC8
extern void OBTABLE_RemoveObjectEntry(struct Object *object); // 0x8003DDFC
extern struct Object * OBTABLE_FindObject(char *objectName); // 0x8003DE44
extern void OBTABLE_ChangeObjectAccessPointers(struct Object *oldObject, struct Object *newObject); // 0x8003DEF0
extern void OBTABLE_RelocateObjectTune(struct Object *object, long offset); // 0x8003DF38
extern void OBTABLE_RelocateInstanceObject(struct _Instance *instance, long offset); // 0x8003DF9C
extern void OBTABLE_InitAnimPointers(struct _ObjectTracker *objectTracker); // 0x8003DFF8

#endif
