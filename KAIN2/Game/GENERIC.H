#ifndef GENERIC_H
#define GENERIC_H

struct evAnimationInstanceSwitchData // hashcode: 0xEE7F2A6C (dec: -293655956)
{
	struct _Instance* instance; // size=616, offset=0
	int anim; // size=0, offset=4
	int frame; // size=0, offset=8
	int frames; // size=0, offset=12
	int mode; // size=0, offset=16
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
