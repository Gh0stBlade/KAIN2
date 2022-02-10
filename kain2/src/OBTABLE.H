#ifndef OBTABLE_H
#define OBTABLE_H


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
