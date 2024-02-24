#ifndef OBTABLE_H
#define OBTABLE_H

struct _ObjectOwnerInfo // hashcode: 0x04BCED5C (dec: 79490396)
{
	long magicnum; // size=0, offset=0
	short animID; // size=0, offset=4
	char objectName[1]; // size=0, offset=6
};

struct ObjectAccess // hashcode: 0x1268C513 (dec: 308856083)
{
	char* objectName; // size=0, offset=0
	void* object; // size=0, offset=4
};

typedef struct _ObjectOwnerInfo ObjectOwnerInfo;

typedef struct ObjectAccess ObjectAccess;

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
