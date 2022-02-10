#ifndef SIGNAL_H
#define SIGNAL_H


extern long SIGNAL_HandleLightGroup(struct _Instance *instance, struct Signal *signal); // 0x8001D840

extern long SIGNAL_HandleCameraAdjust(struct _Instance *instance, struct Signal *signal); // 0x8001D85C

extern long SIGNAL_HandleCamera(struct _Instance *instance, struct Signal *signal); // 0x8001D884

extern void SIGNAL_RelocateCamera(struct Signal *signal, long offset); // 0x8001D8AC

extern long SIGNAL_HandleCameraMode(struct _Instance *instance, struct Signal *signal); // 0x8001D8C8

extern long SIGNAL_HandleCameraLock(struct _Instance *instance, struct Signal *signal); // 0x8001D8F0

extern long SIGNAL_HandleCameraUnlock(struct _Instance *instance, struct Signal *signal); // 0x8001D918

extern long SIGNAL_HandleCameraSmooth(struct _Instance *instance, struct Signal *signal); // 0x8001D940

extern long SIGNAL_HandleCameraTimer(struct _Instance *instance, struct Signal *signal); // 0x8001D968

extern long SIGNAL_HandleCameraSave(struct _Instance *instance, struct Signal *signal); // 0x8001D990

extern long SIGNAL_HandleCameraRestore(struct _Instance *instance, struct Signal *signal); // 0x8001D9B8

extern long SIGNAL_HandleCameraValue(struct _Instance *instance, struct Signal *signal); // 0x8001D9E0

extern long SIGNAL_HandleStreamLevel(struct _Instance *instance, struct Signal *signal); // 0x8001DA10

extern long SIGNAL_HandleFogNear(struct _Instance *instance, struct Signal *signal); // 0x8001DC6C

extern long SIGNAL_HandleFogFar(struct _Instance *instance, struct Signal *signal); // 0x8001DCC0

extern long SIGNAL_HandleCameraShake(struct _Instance *instance, struct Signal *signal); // 0x8001DD1C

extern long SIGNAL_HandleCallSignal(struct _Instance *instance, struct Signal *signal); // 0x8001DD4C

extern long SIGNAL_HandleStopPlayerControl(struct _Instance *instance, struct Signal *signal); // 0x8001DD74

extern long SIGNAL_HandleStartPlayerControl(struct _Instance *instance, struct Signal *signal); // 0x8001DD8C

extern void SIGNAL_RelocateCameraSpline(struct Signal *signal, long offset); // 0x8001DDAC

extern long SIGNAL_HandleCameraSpline(struct _Instance *instance, struct Signal *signal); // 0x8001DDC8

extern long SIGNAL_HandleScreenWipe(struct _Instance *instance, struct Signal *signal); // 0x8001DE48

extern long SIGNAL_HandleBlendStart(struct _Instance *instance, struct Signal *signal); // 0x8001DE80

extern long SIGNAL_HandleScreenWipeColor(struct _Instance *instance, struct Signal *signal); // 0x8001DE88

extern long SIGNAL_HandleSetSlideAngle(struct _Instance *instance, struct Signal *signal); // 0x8001DEB4

extern long SIGNAL_HandleResetSlideAngle(struct _Instance *instance, struct Signal *signal); // 0x8001DEE4

extern long SIGNAL_HandleSetCameraTilt(struct _Instance *instance, struct Signal *signal); // 0x8001DF10

extern long SIGNAL_HandleSetCameraDistance(struct _Instance *instance, struct Signal *signal); // 0x8001DF9C

extern long SIGNAL_HandleEnd(struct _Instance *instance, struct Signal *signal); // 0x8001DFCC

extern void COLLIDE_HandleSignal(struct _Instance *instance, struct Signal *signal, long numSignals, int dontForceDoSignal); // 0x8001DFD4

extern long SIGNAL_IsThisStreamAWarpGate(struct Signal *signal); // 0x8001E0D8

extern long SIGNAL_IsStreamSignal(struct Signal *signal, long *isWarpGate); // 0x8001E138

extern void SIGNAL_HandleSignal(struct _Instance *instance, struct Signal *signal, int dontForceDoSignal); // 0x8001E210

extern struct _MultiSignal * SIGNAL_RelocateSignal(struct _MultiSignal *multiSignal, long offset); // 0x8001E234

extern struct _MultiSignal * SIGNAL_FindSignal(struct Level *level, long id); // 0x8001E310

extern void SIGNAL_OutOfWater(struct _Instance *instance); // 0x8001E3C0

extern void SIGNAL_InWater(struct _Instance *instance); // 0x8001E410


#endif
