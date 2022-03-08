#ifndef SIGNAL_H
#define SIGNAL_H

struct SignalInfo // hashcode: 0x5899EF0F (dec: 1486483215)
{
	long (*signalHandleFunc)(struct _Instance* instance, struct Signal* signal); // size=0, offset=0
	short length; // size=0, offset=4
	short onlyPlayer; // size=0, offset=6
	void (*signalRelocateFunc)(struct Signal* signal, int offset); // size=0, offset=8
};

struct Signal // hashcode: 0xA0225F88 (dec: -1608360056)
{
	long id; // size=0, offset=0
	union {
		struct {
			union {
				long l; // size=0, offset=0
				short s[2]; // size=4, offset=0
				char c[4]; // size=4, offset=0
			} size; // size=4, offset=0, found in object files: player.o, player.o
		} misc; // size=12, offset=0, found in object files: player.o, player.o
		struct _CameraKey* cameraKey; // size=24, offset=0
		struct {
			short x; // size=0, offset=0
			short y; // size=0, offset=2
			short z; // size=0, offset=4
			short zrot; // size=0, offset=6
			struct _SVector offset; // size=8, offset=8
		} teleport; // size=16, offset=0, found in object files: player.o, player.o
		struct {
			long index; // size=0, offset=0
			long value; // size=0, offset=4
		} cameraValue; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			long index; // size=0, offset=0
			long value; // size=0, offset=4
		} miscValue; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			long index; // size=0, offset=0
			long value; // size=0, offset=4
		} logicValue; // size=8, offset=0, found in object files: player.o, player.o
		long timer; // size=0, offset=0
		long times; // size=0, offset=0
		long lightGroup; // size=0, offset=0
		long farPlane; // size=0, offset=0
		long cameraAdjust; // size=0, offset=0
		long cameraMode; // size=0, offset=0
		long cameraLock; // size=0, offset=0
		long cameraUnlock; // size=0, offset=0
		long cameraSmooth; // size=0, offset=0
		long cameraTimer; // size=0, offset=0
		struct {
			long time; // size=0, offset=0
			long scale; // size=0, offset=4
		} cameraShake; // size=8, offset=0, found in object files: player.o, player.o
		long cameraSave; // size=0, offset=0
		long cameraRestore; // size=0, offset=0
		struct _BGObject* bgObject; // size=24, offset=0
		long value[2]; // size=8, offset=0
		struct {
			long action; // size=0, offset=0
			long value[2]; // size=8, offset=4
		} sound; // size=12, offset=0, found in object files: player.o, player.o
		void* intro; // size=0, offset=0
		long deathZ; // size=0, offset=0
		struct {
			void* intro; // size=0, offset=0
			void* data; // size=0, offset=4
		} dsignal; // size=8, offset=0, found in object files: player.o, player.o
		void* mirror; // size=0, offset=0
		long fogNear; // size=0, offset=0
		long fogFar; // size=0, offset=0
		struct {
			void* intro; // size=0, offset=0
			long frame; // size=0, offset=4
		} gotoFrame; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			void* intro; // size=0, offset=0
			long model; // size=0, offset=4
		} changeModel; // size=8, offset=0, found in object files: player.o, player.o
		void* vmObject; // size=0, offset=0
		struct {
			unsigned char target; // size=0, offset=0
			unsigned char source1; // size=0, offset=1
			unsigned char source2; // size=0, offset=2
			unsigned char pad; // size=0, offset=3
			struct Signal* gotoLabel; // size=0, offset=4
		} logic; // size=8, offset=0, found in object files: player.o, player.o
		void* callSignal; // size=0, offset=0
		struct _SVector offset; // size=8, offset=0
		struct Signal* gotoLabel; // size=0, offset=0
		struct Signal* gosubLabel; // size=0, offset=0
		struct {
			long timer; // size=0, offset=0
			long action; // size=0, offset=4
		} control; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			short index; // size=0, offset=0
			short z; // size=0, offset=2
			struct MultiSignal* upSignal; // size=0, offset=4
			struct MultiSignal* downSignal; // size=0, offset=8
		} zSignal; // size=12, offset=0, found in object files: player.o, player.o
		struct {
			long currentnum; // size=0, offset=0
			long streamID; // size=0, offset=4
			char toname[16]; // size=16, offset=8
		} StreamLevel; // size=24, offset=0, found in object files: player.o, player.o
		struct {
			char num; // size=0, offset=0
			char vel; // size=0, offset=1
			short misc; // size=0, offset=2
			char flags; // size=0, offset=4
			char timeToLive; // size=0, offset=5
			short dummy2; // size=0, offset=6
			struct Object* object; // size=0, offset=8
		} shards; // size=12, offset=0, found in object files: player.o, player.o
		struct {
			long index; // size=0, offset=0
			void* intro; // size=0, offset=4
		} cameraSpline; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			short type; // size=0, offset=0
			short time; // size=0, offset=2
		} screenWipe; // size=4, offset=0, found in object files: player.o, player.o
		long voiceSitId; // size=0, offset=0
		struct {
			long time; // size=0, offset=0
			void* intro; // size=0, offset=4
		} introFX; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			void* intro; // size=0, offset=0
			struct Signal* gotoLabel; // size=0, offset=4
		} introActive; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			char world[9]; // size=9, offset=0
			char level[9]; // size=9, offset=9
		} levelChange; // size=18, offset=0, found in object files: player.o, player.o
		long blendStart; // size=0, offset=0
		struct {
			struct Signal* signal; // size=0, offset=0
			long times; // size=0, offset=4
		} setTimes; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			unsigned char r; // size=0, offset=0
			unsigned char g; // size=0, offset=1
			unsigned char b; // size=0, offset=2
			unsigned char pad; // size=0, offset=3
		} color; // size=4, offset=0, found in object files: player.o, player.o
		struct {
			short x; // size=0, offset=0
			short y; // size=0, offset=2
			short time; // size=0, offset=4
			short index; // size=0, offset=6
			char* text; // size=0, offset=8
		} print; // size=12, offset=0, found in object files: player.o, player.o
		long slideAngle; // size=0, offset=0
	} data; // size=24, offset=4, found in object files: player.o, player.o
};

struct _MultiSignal
{
	long numSignals; // size=0, offset=0
	short signalNum; // size=0, offset=4
	short flags; // size=0, offset=6
	struct Signal signalList[32]; // size=896, offset=8
};

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
