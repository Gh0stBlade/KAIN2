#include <windows.h>
#include "snd.h"

struct _G2AppDataVM_Type
{
	HINSTANCE hInstance;
	HWND hWindow;
	int hWnd;
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
};

int NumSNDDevices,
	NumSNDDevicesBase,
	NumSNDDevices2;

SND_DEVICE SNDDeviceList[16];
SND_DEVICE_INFO SndGuids[16];

extern int __cdecl DSOUND_EnumerateDevices(SND_DEVICE* devs);

void (*SND_ShutdownPtr)();
void (*SND_SetSamplePtr)(int voiceNum, BYTE* data);
void (*SND_SetNextSamplePtr)(int voiceNum, BYTE* data);
void (*SND_FreeSamplePtr)(void *data);
void (*SND_SetFrequencyPtr)(int voiceNum, float frequency);
void (*SND_SetVolumePtr)(int voiceNum, int voll, int volr);
void (*SND_SetTimerFuncPtr)(void (*fn)());
void* (*SND_UploadSamplePtr)(const void* data, int samples, int a3, int a4, int a5);
void (*SND_SetChannelInterruptPtr)(int voiceNum, int intr);
void (*SND_KeyOffPtr)(int voiceNum);
void (*SND_StopPtr)(int voiceNum);

//0001 : 0007b190       _SoundG2_Init              0047c190 f   snd.obj
int __cdecl SoundG2_Init(_G2AppDataVM_Type* vm)
{
	return 0;
}
//0001 : 0007b390       _SoundG2_ShutDown          0047c390 f   snd.obj
void SoundG2_ShutDown()
{
	if (SND_ShutdownPtr)
		SND_ShutdownPtr();
}
//0001 : 0007b3a0       _SND_EnumerateDevices      0047c3a0 f   snd.obj
void __cdecl SND_EnumerateDevices()
{
	strcpy_s(SNDDeviceList[0].name, sizeof(SNDDeviceList[0].name), "No Sound");
	NumSNDDevices = 1;
	NumSNDDevicesBase = 1;
	NumSNDDevices = DSOUND_EnumerateDevices(&SNDDeviceList[1]) + 1;
	NumSNDDevices2 = NumSNDDevices;
}
//0001 : 0007b400       _SND_Init                  0047c400 f   snd.obj [unused]
//0001 : 0007b600       _SND_Shutdown              0047c600 f   snd.obj [unused]
//0001 : 0007b610       _SND_SetSample             0047c610 f   snd.obj
void SND_SetSample(int voiceNum, BYTE* data)
{
	if (SND_SetSamplePtr)
		SND_SetSamplePtr(voiceNum, data);
}
//0001 : 0007b630       _SND_SetNextSample         0047c630 f   snd.obj
void SND_SetNextSample(int voiceNum, BYTE* data)
{
	if (SND_SetNextSamplePtr)
		SND_SetNextSamplePtr(voiceNum, data);
}
//0001 : 0007b650       _SND_GetSample             0047c650 f   snd.obj
//0001 : 0007b670       _SND_GetNextSample         0047c670 f   snd.obj
//0001 : 0007b690       _SND_SetFrequency          0047c690 f   snd.obj
void SND_SetFrequency(int voiceNum, float frequency)
{
	if (SND_SetFrequencyPtr)
		SND_SetFrequencyPtr(voiceNum, frequency);
}
//0001 : 0007b6b0       _SND_SetVolume             0047c6b0 f   snd.obj
void SND_SetVolume(int voiceNum, int voll, int volr)
{
	if (SND_SetVolumePtr)
		SND_SetVolumePtr(voiceNum, voll, volr);
}
//0001 : 0007b6d0       _SND_SetLoopMode           0047c6d0 f   snd.obj
//0001 : 0007b6f0       _SND_GetStatus             0047c6f0 f   snd.obj
//0001 : 0007b710       _SND_Start                 0047c710 f   snd.obj
//0001 : 0007b730       _SND_Stop                  0047c730 f   snd.obj
void __cdecl SND_Stop(int voiceNum)
{
	if (SND_StopPtr)
		SND_StopPtr(voiceNum);
}
//0001 : 0007b750       _SND_KeyOff                0047c750 f   snd.obj
void __cdecl SND_KeyOff(int voiceNum)
{
	if (SND_KeyOffPtr)
		SND_KeyOffPtr(voiceNum);
}
//0001 : 0007b770       _SND_SetChannelInterrupt   0047c770 f   snd.obj
void __cdecl SND_SetChannelInterrupt(int voiceNum, int intr)
{
	if (SND_SetChannelInterruptPtr)
		SND_SetChannelInterruptPtr(voiceNum, intr);
}
//0001 : 0007b790       _SND_UploadSample          0047c790 f   snd.obj
void* SND_UploadSample(const void* data, int samples, int a3, int a4, int a5)
{
	if (SND_UploadSamplePtr)
		return SND_UploadSamplePtr(data, samples, a3, a4, a5);
	else
		return 0;
}
//0001 : 0007b7c0       _SND_FreeSample            0047c7c0 f   snd.obj
void SND_FreeSample(void *data)
{
	if (SND_FreeSamplePtr)
		SND_FreeSamplePtr(data);
}
//0001 : 0007b7e0       _SND_SetTimerFunc          0047c7e0 f   snd.obj
void SND_SetTimerFunc(void (*fn)())
{
	if (SND_SetTimerFuncPtr)
		SND_SetTimerFuncPtr(fn);
}
