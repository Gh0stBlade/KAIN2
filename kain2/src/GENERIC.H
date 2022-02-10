#ifndef GENERIC_H
#define GENERIC_H


extern void GenericInit(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8003E194

extern void GenericCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8003E2D0

extern void GenericProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8003E2D8

extern unsigned long GenericQuery(struct _Instance *instance, unsigned long query); // 0x8003E32C

extern void GenericMessage(struct _Instance *instance, unsigned long message, unsigned long data); // 0x8003E4A4

extern void GenericRelocateTune(struct Object *object, long offset); // 0x8003E620


#endif
