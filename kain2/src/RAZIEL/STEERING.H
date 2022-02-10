#ifndef STEERING_H
#define STEERING_H


extern int UpdateZoneDelta(int rc, int LastRC); // 0x800A213C

extern int GetControllerInput(int *ZDirection, long *controlCommand); // 0x800A2178

extern int DecodeDirection(int Source, int Destination, short *Difference, short *Zone); // 0x800A2358

extern int ProcessMovement(struct _Instance *instance, long *controlCommand, struct GameTracker *GT); // 0x800A2448

extern void SteerTurn(struct _Instance *instance, int rc); // 0x800A298C

extern void SteerMove(struct _Instance *instance, int rc); // 0x800A2A20

extern int SteerAutoFace(struct _Instance *instance, long *controlCommand); // 0x800A2AD0

extern void SteerSwim(struct _Instance *instance); // 0x800A2D64

extern void SteerWallcrawling(struct _Instance *instance); // 0x800A2E54

extern void SteerDisableAutoFace(struct _Instance *instance); // 0x800A2E88

extern void SteerSwitchMode(struct _Instance *instance, int mode); // 0x800A2F1C

extern void razInitWallCrawlSteering(struct _Instance *instance); // 0x800A3178

extern void razDeinitWallCrawlSteering(struct _Instance *instance); // 0x800A32B4


#endif
