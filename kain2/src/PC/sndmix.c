#include <windows.h>

typedef struct Channel
{
	int status;
	int frequency;
	short voll, volr;
	int field_C;
	BYTE* sample;
	BYTE* next_sample;
	int loop_mode;
	int field_1C;
	int field_20;
	int interrupt;
} Channel;

typedef struct MIX_SAMPLE
{
	int pData;
	int field_4;
	int samples;
	int field_C;
	int field_10;
	BYTE data[1];
} MIX_SAMPLE;

Channel channels[26];
void(__cdecl* TimerFunc)();
int timeout, timeoutmax;

void __cdecl Mix(unsigned __int16* data, unsigned int time);

//0001 : 0007b800       _SNDMIX_Init               0047c800 f   sndmix.obj
void __cdecl SNDMIX_Init()
{
	timeoutmax = 441;
	timeout = 0;
	for(int i = 0; i<_countof(channels); i++)
		channels[i].status = 0;
}
//0001 : 0007b830       _SNDMIX_Shutdown           0047c830 f   sndmix.obj
void SNDMIX_Shutdown()
{
}
//0001 : 0007b840       _SNDMIX_Mix                0047c840 f   sndmix.obj
void __cdecl SNDMIX_Mix(WORD* sample, int size)
{
	signed int time; // eax

	if (size > 0)
	{
		time = timeout;
		while (time)
		{
			if (time <= size)
			{
				Mix(sample, time);
				size -= timeout;
				sample += 2 * timeout;
				time = 0;
LABEL_10:
				timeout = time;
				goto LABEL_11;
			}
			Mix(sample, size);
			time = timeout - size;
			sample += 2 * size;
			timeout -= size;
			size = 0;
LABEL_11:
			if (size <= 0)
				return;
		}
		if (TimerFunc)
			TimerFunc();
		time = timeoutmax;
		goto LABEL_10;
	}
}
//0001 : 0007bc60       _SNDMIX_SetSample          0047cc60 f   sndmix.obj
void __cdecl SNDMIX_SetSample(int voiceNum, BYTE* sample)
{
	channels[voiceNum].status = 0;
	channels[voiceNum].interrupt = 0;
	channels[voiceNum].sample = sample;
	channels[voiceNum].next_sample = 0;
}
//0001 : 0007bc90       _SNDMIX_SetNextSample      0047cc90 f   sndmix.obj
void __cdecl SNDMIX_SetNextSample(int voiceNum, BYTE* sample)
{
	channels[voiceNum].next_sample = sample;
}
//0001 : 0007bcb0       _SNDMIX_GetSample          0047ccb0 f   sndmix.obj
BYTE* __cdecl SNDMIX_GetSample(int voiceNum)
{
	return channels[voiceNum].sample;
}
//0001 : 0007bcc0       _SNDMIX_GetNextSample      0047ccc0 f   sndmix.obj
BYTE* __cdecl SNDMIX_GetNextSample(int voiceNum)
{
	return channels[voiceNum].next_sample;
}
//0001 : 0007bcd0       _SNDMIX_SetFrequency       0047ccd0 f   sndmix.obj
void __cdecl SNDMIX_SetFrequency(int voiceNum, float frequency)
{
	channels[voiceNum].frequency = (int)(frequency * 0.092879817f);
}
//0001 : 0007bcf0       _SNDMIX_SetVolume          0047ccf0 f   sndmix.obj
void __cdecl SNDMIX_SetVolume(int voiceNum, int left, int right)
{
	int l; // eax
	int r; // ecx

	l = 2 * left;
	r = 2 * right;
	if (2 * left > 0x7FFF) l = 0x7FFF;
	if (r > 0x7FFF) r = 0x7FFF;
	channels[voiceNum].voll = l;
	channels[voiceNum].volr = r;
}
//0001 : 0007bd30       _SNDMIX_SetLoopMode        0047cd30 f   sndmix.obj
void __cdecl SNDMIX_SetLoopMode(int voiceNum, int mode)
{
	channels[voiceNum].loop_mode = mode;
}
//0001 : 0007bd50       _SNDMIX_KeyOff             0047cd50 f   sndmix.obj
void __cdecl SNDMIX_KeyOff(int voiceNum)
{
	if (channels[voiceNum].loop_mode == 1)
	{
		channels[voiceNum].field_1C = 2;
		channels[voiceNum].field_20 = *((DWORD*)channels[voiceNum].sample + 2);
	}
	else
	{
		channels[voiceNum].status = 0;
	}
}
//0001 : 0007bd90       _SNDMIX_Start              0047cd90 f   sndmix.obj
void __cdecl SNDMIX_Start(int voiceNum)
{
	channels[voiceNum].status = 1;
	channels[voiceNum].field_C = 0;
	switch (channels[voiceNum].loop_mode)
	{
	case 0:
		channels[voiceNum].field_1C = 2;
		channels[voiceNum].field_20 = *((DWORD*)channels[voiceNum].sample + 2);
		break;
	case 1:
		channels[voiceNum].field_1C = 1;
		channels[voiceNum].field_20 = *((DWORD*)channels[voiceNum].sample + 4);
		break;
	}
}
//0001 : 0007bdf0       _SNDMIX_Stop               0047cdf0 f   sndmix.obj
void __cdecl SNDMIX_Stop(int voiceNum)
{
	channels[voiceNum].status = 0;
}
//0001 : 0007be10       _SNDMIX_UploadSample       0047ce10 f   sndmix.obj
void* __cdecl SNDMIX_UploadSample(const void* data, int samples, int a3, int a4, int a5)
{
	int block; // esi
	unsigned int size; // esi
	MIX_SAMPLE* sample; // eax

	block = 1;
	if (a3 != 1)
		block = a3;
	if (a3 == 2)
		block = 2;
	if (a3 == 3)
		block = 2;
	if (a3 == 4)
		block = 4;
	size = samples * block;
	sample = (MIX_SAMPLE*)GlobalAlloc(GMEM_NOT_BANKED, size + 20);
	memcpy(sample->data, data, size);
	sample->pData = (int)sample->data;
	sample->field_C = a4 << 12;
	sample->samples = samples << 12;
	sample->field_10 = a5 << 12;
	sample->field_4 = a3;
	return sample;
}
//0001 : 0007bea0       _SNDMIX_SetChannelInterrupt 0047cea0 f   sndmix.obj
void __cdecl SNDMIX_SetChannelInterrupt(int voiceNum, int intr)
{
	channels[voiceNum].interrupt = intr;
}
//0001 : 0007bec0       _SNDMIX_FreeSample         0047cec0 f   sndmix.obj
void __cdecl SNDMIX_FreeSample(void *ptr)
{
	GlobalFree(ptr);
}
//0001 : 0007bed0       _SNDMIX_GetStatus          0047ced0 f   sndmix.obj
int __cdecl SNDMIX_GetStatus(int voiceNum)
{
	return channels[voiceNum].status;
}
//0001 : 0007bee0       _SNDMIX_SetTimerFunc       0047cee0 f   sndmix.obj
void __cdecl SNDMIX_SetTimerFunc(void(__cdecl* fn)())
{
	TimerFunc = fn;
}

// TODO: this is gonna be messy as fuck
void __cdecl Mix(unsigned __int16* data, unsigned int time)
{}
