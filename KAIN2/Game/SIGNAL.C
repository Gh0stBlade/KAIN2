#include "CORE.H"
#include "SIGNAL.H"
#include "GAMELOOP.H"
#include "Game/CAMERA.H"
#include "Game/STREAM.H"
#include "Game/LIGHT3D.H"
#include "Game/SAVEINFO.H"

extern struct Camera theCamera;

struct SignalInfo signalInfoList[] =
{
	{SIGNAL_HandleLightGroup,			1, 0, NULL},
	{SIGNAL_HandleCameraAdjust,			1, 1, NULL},
	{SIGNAL_HandleCameraMode,			1, 1, NULL},
	{SIGNAL_HandleCamera,				1, 1, SIGNAL_RelocateCamera},
	{SIGNAL_HandleCameraTimer,			1, 1, NULL},
	{SIGNAL_HandleCameraSmooth,			1, 1, NULL},
	{SIGNAL_HandleCameraValue,			2, 1, NULL},
	{SIGNAL_HandleCameraLock,			1, 1, NULL},
	{SIGNAL_HandleCameraUnlock,			1, 1, NULL},
	{SIGNAL_HandleCameraSave,			1, 1, NULL},
	{SIGNAL_HandleCameraRestore,		1, 1, NULL},
	{SIGNAL_HandleFogNear,				1, 1, NULL},
	{SIGNAL_HandleFogFar,				1, 1, NULL},
	{SIGNAL_HandleCameraShake,			2, 1, NULL},
	{SIGNAL_HandleCallSignal,			1, 1, NULL},
	{SIGNAL_HandleEnd,					0, 0, NULL},
	{SIGNAL_HandleStopPlayerControl,	0, 1, NULL},
	{SIGNAL_HandleStartPlayerControl,	0, 1, NULL},
	{SIGNAL_HandleStreamLevel,			6, 0, NULL},
	{SIGNAL_HandleCameraSpline,			2, 1, SIGNAL_RelocateCameraSpline},
	{SIGNAL_HandleScreenWipe,			1, 1, NULL},
	{SIGNAL_HandleBlendStart,			1, 1, NULL},
	{SIGNAL_HandleScreenWipeColor,		1, 1, NULL},
	{SIGNAL_HandleSetSlideAngle,		1, 1, NULL},
	{SIGNAL_HandleResetSlideAngle,		0, 1, NULL},
	{SIGNAL_HandleSetCameraTilt,		1, 1, NULL},
	{SIGNAL_HandleSetCameraDistance,	1, 1, NULL}
};

long SIGNAL_HandleLightGroup(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	if (instance != NULL)
	{
		instance->lightGroup = signal->data.misc[0].size.c[0];
	}

	return 1;
}

long SIGNAL_HandleCameraAdjust(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_Adjust(&theCamera, signal->data.cameraAdjust);
	return 1;
}

long SIGNAL_HandleCamera(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_ChangeTo(&theCamera, signal->data.cameraKey);
	return 1;
}

void SIGNAL_RelocateCamera(struct Signal* signal, long offset) // Matching - 100%
{
	signal->data.cameraKey = (struct _CameraKey*)OFFSET_DATA(signal->data.cameraKey, offset);
}

long SIGNAL_HandleCameraMode(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_SetMode(&theCamera, signal->data.cameraMode);
	return 1;
}

long SIGNAL_HandleCameraLock(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_Lock(&theCamera, signal->data.cameraLock);
	return 1;
}

long SIGNAL_HandleCameraUnlock(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_Unlock(&theCamera, signal->data.cameraUnlock);
	return 1;
}

long SIGNAL_HandleCameraSmooth(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_SetSmoothValue(&theCamera, signal->data.cameraSmooth);
	return 1;
}

long SIGNAL_HandleCameraTimer(struct _Instance* instance, struct Signal* signal) // Matching - 100%
{
	CAMERA_SetTimer(&theCamera, signal->data.cameraTimer);
	return 1;
}

long SIGNAL_HandleCameraSave(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_Save(&theCamera, signal->data.cameraSave);
	return 1;
}

long SIGNAL_HandleCameraRestore(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_Restore(&theCamera, signal->data.cameraRestore);
	return 1;
}

long SIGNAL_HandleCameraValue(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_SetValue(&theCamera, signal->data.cameraValue.index, signal->data.cameraValue.value);
	return 1;
}


long SIGNAL_HandleStreamLevel(struct _Instance* instance, struct Signal* signal)  // Matching - 98.68%
{
	char areaName[16];
	int signalnum;
	char* commapos;
	long newStreamID;
	static int lastTimeCrossed;
	long doingWarpRoom;

	signalnum = -1;
	doingWarpRoom = 0;

	strcpy(areaName, &signal->data.StreamLevel.toname[0]);

	commapos = (char*)strchr(areaName, ',');

	if (commapos != NULL)
	{
		signalnum = atoi(commapos + 1);

		commapos[0] = 0;
	}

	if (strcmpi(areaName, "warpgate") == 0)
	{
		struct _StreamUnit* curStreamUnit;

		curStreamUnit = STREAM_GetStreamUnitWithID(instance->currentStreamUnitID);

		if (gameTrackerX.currentTime < 101U)
		{
			return 1;
		}

		if (!(curStreamUnit->flags & 8))
		{
			return 1;
		}

		newStreamID = WarpRoomArray[CurrentWarpNumber].streamUnit->StreamUnitID;

		strcpy(areaName, (char*)&WarpRoomArray[CurrentWarpNumber]);

		doingWarpRoom = 1;

		if ((WarpRoomArray[CurrentWarpNumber].streamUnit == NULL) || (!(WarpRoomArray[CurrentWarpNumber].streamUnit->flags & 8)))
		{
			return 1;
		}
	}
	else
	{
		newStreamID = signal->data.StreamLevel.streamID;
	}

	if (instance->currentStreamUnitID != newStreamID)
	{
		if (instance == gameTrackerX.playerInstance)
		{
			gameTrackerX.SwitchToNewStreamUnit = 1;
			lastTimeCrossed = (int)gameTrackerX.currentTime;

			strcpy(gameTrackerX.S_baseAreaName, areaName);

			gameTrackerX.toSignal = signalnum;
			gameTrackerX.fromSignal = (short)signal->data.StreamLevel.currentnum;
			gameTrackerX.moveRazielToStreamID = newStreamID;

			if (doingWarpRoom != 0)
			{
				if ((gameTrackerX.gameData.asmData.MorphType == 0) && (strcmpi(areaName, "under3") == 0))
				{
					INSTANCE_Post(gameTrackerX.playerInstance, 0x10002001, 0);
				}

				gameTrackerX.SwitchToNewWarpIndex = (short)WARPGATE_GetWarpRoomIndex(gameTrackerX.baseAreaName);
				return 1;
			}

			gameTrackerX.SwitchToNewWarpIndex = -1;
			return 1;
		}

		if (instance->LinkParent == NULL)
		{
			if (STREAM_GetLevelWithID(newStreamID) == 0)
			{
				SAVE_Instance(instance, STREAM_GetLevelWithID(instance->currentStreamUnitID));

				instance->flags |= 0x20;
			}
			else
			{
				instance->cachedTFace = -1;
				instance->cachedTFaceLevel = NULL;
				instance->currentStreamUnitID = newStreamID;

				INSTANCE_UpdateFamilyStreamUnitID(instance);
			}
		}

		return 1;
	}

	return 1;
}

long SIGNAL_HandleFogNear(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	struct Level* level;

	level = STREAM_GetLevelWithID(gameTrackerX.playerInstance->currentStreamUnitID);
	level->fogNear = (unsigned short)signal->data.fogNear;
	
	SetFogNearFar(level->fogNear, level->fogFar, theCamera.core.projDistance);
	
	LIGHT_CalcDQPTable(level);
	
	return 1;
}

long SIGNAL_HandleFogFar(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	struct Level* level;

	level = STREAM_GetLevelWithID(gameTrackerX.playerInstance->currentStreamUnitID);

	level->fogFar = (unsigned short)signal->data.fogFar;

	SetFogNearFar(level->fogNear, level->fogFar, theCamera.core.projDistance);

	LIGHT_CalcDQPTable(level);

	return 1;
}

long SIGNAL_HandleCameraShake(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	CAMERA_SetShake(&theCamera, signal->data.cameraShake.time, signal->data.cameraShake.scale);
	return 1;
}

long SIGNAL_HandleCallSignal(struct _Instance* instance, struct Signal* signal)
{
	SIGNAL_HandleSignal(instance,(struct Signal*)signal->data.callSignal, 0);
	return 1;
}

long SIGNAL_HandleStopPlayerControl(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	gameTrackerX.gameFlags |= 0x90;
	return 1;
}

long SIGNAL_HandleStartPlayerControl(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	gameTrackerX.gameFlags &= 0xFFFFFF7F;
	gameTrackerX.gameFlags &= 0xFFFFFFEF;
	return 1;
}

void SIGNAL_RelocateCameraSpline(struct Signal* signal, long offset) // Matching - 100%
{
	signal->data.cameraSpline.intro = (void*)OFFSET_DATA(signal->data.cameraSpline.intro, offset);
}

long SIGNAL_HandleCameraSpline(struct _Instance* instance, struct Signal* signal) // Matching - 100%
{
	switch (signal->data.cameraSpline.index)
	{
	case 0:
		if (signal->data.cameraSpline.intro != NULL)
		{
			theCamera.Spline00 = ((struct Intro*)signal->data.cameraSpline.intro)->multiSpline;
		}
		else
		{
			theCamera.Spline00 = 0;
		}
		break;
	case 1:
		if ((struct Intro*)signal->data.cameraSpline.intro != NULL)
		{
			theCamera.Spline01 = ((struct Intro*)signal->data.cameraSpline.intro)->multiSpline;
		}
		else
		{
			theCamera.Spline01 = 0;
		}
	}
	return 1;
}

long SIGNAL_HandleScreenWipe(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	gameTrackerX.wipeTime = signal->data.screenWipe.time;
	gameTrackerX.maxWipeTime = signal->data.screenWipe.time < 0 ? -signal->data.screenWipe.time : signal->data.screenWipe.time;
	gameTrackerX.wipeType = signal->data.screenWipe.type;

	return 1;
}

long SIGNAL_HandleBlendStart(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	return 1;
}

long SIGNAL_HandleScreenWipeColor(struct _Instance* instance, struct Signal* signal)  // Matching - 100%
{
	gameTrackerX.wipeColor.r = signal->data.color.r;
	gameTrackerX.wipeColor.g = signal->data.color.g;
	gameTrackerX.wipeColor.b = signal->data.color.b;
	return 1;
}

long SIGNAL_HandleSetSlideAngle(struct _Instance* instance, struct Signal* signal)
{
	if (instance != NULL)
	{
		INSTANCE_Post(instance, 0x4000005, signal->data.slideAngle);
	}

	return 1;
}

long SIGNAL_HandleResetSlideAngle(struct _Instance* instance, struct Signal* signal)
{
	if (instance != NULL)
	{
		INSTANCE_Post(instance, 0x4000006, 0);
	}

	return 1;
}


// autogenerated function stub: 
// long /*$ra*/ SIGNAL_HandleSetCameraTilt(struct _Instance *instance /*$a0*/, struct Signal *signal /*$a1*/)
long SIGNAL_HandleSetCameraTilt(struct _Instance *instance, struct Signal *signal)
{ // line 595, offset 0x8001df10
	UNIMPLEMENTED();
	return 0;
}

long SIGNAL_HandleSetCameraDistance(struct _Instance* instance, struct Signal* signal)
{
	if (instance != NULL)
	{
		CAMERA_Adjust_distance(&theCamera, signal->data.cameraShake.time);
	}

	return 1;
}

long SIGNAL_HandleEnd(struct _Instance* instance, struct Signal* signal)
{
	return 0;
}

void COLLIDE_HandleSignal(struct _Instance* instance, struct Signal* signal, long numSignals, int dontForceDoSignal)//Matching - 90.00%
{
	long signalNumber;

	if (numSignals != 0)
	{
		do
		{
			signalNumber = signal->id & 0x7FFFFFFF;

			if ((signalInfoList[signalNumber].onlyPlayer == 0 || instance == gameTrackerX.playerInstance)
				&& (!(gameTrackerX.gameFlags & 0x40) || signal->id >= 0)
				&& ((signalNumber) >= 27
					|| !(signalInfoList[signal->id & 0x7FFFFFFF].signalHandleFunc)(instance, signal)))
			{
				break;
			}
			signal = (struct Signal*)((char*)signal + (signalInfoList[signal->id & 0x7FFFFFFF].length + 1) * 4);

		} while (1);
	}
}

long SIGNAL_IsThisStreamAWarpGate(struct Signal* signal)  // Matching - 100%
{
	long result;
	char areaName[32];
	char* commapos;

	result = 0;

	strcpy(areaName, signal->data.StreamLevel.toname);

	commapos = strchr(areaName, ',');
	
	if (commapos != NULL)
	{
		commapos[0] = 0;
	}

	if (strcmpi(areaName, "warpgate") == 0)
	{
		result = 1;
	}

	return result;
}

long SIGNAL_IsStreamSignal(struct Signal* signal, long* isWarpGate)
{
	long result;
	long done;
	long signalNumber;

	result = 0;
	done = 0;
	isWarpGate[0] = 0;

	while (!done)
	{
		signalNumber = signal->id & 0x7FFFFFFF;

		if (signalNumber == 0xF)
		{
			done = 1;
		}
		else if (signalNumber == 0x12)
		{
			done = 1;

			result = 1;

			if (SIGNAL_IsThisStreamAWarpGate(signal) != 0)
			{
				isWarpGate[0] = 1;
			}
		}

		signal = (struct Signal*)(((char*)signal) + ((signalInfoList[signalNumber].length + 1) << 2));
	}

	return result;
}

void SIGNAL_HandleSignal(struct _Instance* instance, struct Signal* signal, int dontForceDoSignal)  // Matching - 100%
{
	COLLIDE_HandleSignal(instance, signal, 1, dontForceDoSignal);
}


// autogenerated function stub: 
// struct _MultiSignal * /*$ra*/ SIGNAL_RelocateSignal(struct _MultiSignal *multiSignal /*$s4*/, long offset /*$s5*/)
struct _MultiSignal * SIGNAL_RelocateSignal(struct _MultiSignal *multiSignal, long offset)
{ // line 976, offset 0x8001e234
	UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// struct _MultiSignal * /*$ra*/ SIGNAL_FindSignal(struct Level *level /*$a0*/, long id /*$a1*/)
struct _MultiSignal * SIGNAL_FindSignal(struct Level *level, long id)
{ // line 1002, offset 0x8001e310
	UNIMPLEMENTED();
	return 0;
}

void SIGNAL_OutOfWater(struct _Instance* instance)  // Matching - 100%
{
	struct Level* level;

	level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

	if (level != NULL)
	{
		if (level->startGoingOutOfWaterSignal != NULL)
		{
			SIGNAL_HandleSignal(instance, &level->startGoingOutOfWaterSignal->signalList[0], 0);
		}
	}
}

void SIGNAL_InWater(struct _Instance* instance)  // Matching - 100%
{
	struct Level* level;

	level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

	if (level != NULL)
	{
		if (level->startGoingIntoWaterSignal != NULL)
		{
			SIGNAL_HandleSignal(instance, &level->startGoingIntoWaterSignal->signalList[0], 0);
		}
	}
}