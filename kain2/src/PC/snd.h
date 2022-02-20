#pragma once

typedef struct _G2AppDataVM_Type
{
	HINSTANCE hInstance;
	HWND hWindow;
	HWND hWnd;
	int Screen_width;
	int Screen_height;
	int Screen_depth;
	int Gamma_level;
	int Is_windowed;
	int Render_device_id;
	int Triple_buffer;
	int VSync;
	int Filter;
	int Sound_device_id;
} _G2AppDataVM_Type;

typedef struct SND_DEVICE
{
	char name[64];
	int index;
} SND_DEVICE;

typedef struct SND_DEVICE_INFO
{
	GUID Guid;
	GUID* pGuid;
} SND_DEVICE_INFO;

extern int(__cdecl* SND_InitPtr)(HWND hWnd, int index);
extern void(__cdecl* SND_ShutdownPtr)();
extern void(__cdecl* SND_SetSamplePtr)(int voiceNum, BYTE* data);
extern BYTE* (__cdecl* SND_GetSamplePtr)(int voiceNum);
extern void(__cdecl* SND_SetNextSamplePtr)(int voiceNum, BYTE* data);
extern BYTE* (__cdecl* SND_GetNextSamplePtr)(int voiceNum);
extern void(__cdecl* SND_FreeSamplePtr)(void* data);
extern void(__cdecl* SND_SetFrequencyPtr)(int voiceNum, float frequency);
extern void(__cdecl* SND_SetVolumePtr)(int voiceNum, int voll, int volr);
extern void(__cdecl* SND_SetTimerFuncPtr)(void (*fn)());
extern void* (__cdecl* SND_UploadSamplePtr)(const void* data, int samples, int a3, int a4, int a5);
extern void(__cdecl* SND_SetChannelInterruptPtr)(int voiceNum, int intr);
extern void(__cdecl* SND_KeyOffPtr)(int voiceNum);
extern void(__cdecl* SND_StopPtr)(int voiceNum);
extern void(__cdecl* SND_SetLoopModePtr)(int voiceNum, int mode);
extern void(__cdecl* SND_StartPtr)(int voiceNum);
extern int(__cdecl* SND_GetStatusPtr)(int voiceNum);

int __cdecl SoundG2_Init(_G2AppDataVM_Type* vm);
void __cdecl SoundG2_ShutDown();
