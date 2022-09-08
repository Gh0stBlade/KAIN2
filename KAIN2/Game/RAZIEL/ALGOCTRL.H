#ifndef ALGOCTRL_H
#define ALGOCTRL_H

struct evAnimationControllerDoneData // hashcode: 0x06BA7BE4 (dec: 112884708)
{
	struct _Instance *instance; // size=616, offset=0
	long segment; // size=0, offset=4
	long type; // size=0, offset=8
	int data; // size=0, offset=12
};

extern void InitAlgorithmicWings(struct _Instance *instance); // 0x800A1C24

extern void DeInitAlgorithmicWings(struct _Instance *instance); // 0x800A1DBC

extern void AlgorithmicWings(struct _Instance *instance, struct evAnimationControllerDoneData *ControllerData); // 0x800A1E4C

extern void AlgorithmicNeck(struct _Instance *Player, struct _Instance *Target); // 0x800A1EBC


#endif
