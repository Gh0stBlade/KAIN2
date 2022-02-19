#include <windows.h>
#include <dsound.h>
#include "../snd.h"

EXTERN_C int NumSNDDevices,
	NumSNDDevicesBase,
	NumSNDDevices2;
EXTERN_C SND_DEVICE SNDDeviceList[16];
EXTERN_C SND_DEVICE_INFO SndGuids[16];

void __cdecl SNDMIX_Init();
void __cdecl SNDMIX_Mix(WORD* sample, int size);
void __cdecl DBG_Print(const char* fmt, ...);

DWORD WINAPI PlayThread(LPVOID lpThreadParameter);
void StreamSamples();

int SndDevCurIndex,
	SndBufferAlign;
DWORD SndBufferBytes,
	SndBufferHalf;
SND_DEVICE* pSndDevs;

LPDIRECTSOUND ppDS;
LPDIRECTSOUNDBUFFER pSndBuffer;

HANDLE hThread;

BOOL WINAPI DSCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext)
{
	int index; // edx
	SND_DEVICE_INFO* guid; // edi

	index = SndDevCurIndex;
	if (lpGuid)
	{
		guid = &SndGuids[SndDevCurIndex];
		guid->Guid = *lpGuid;
		SndGuids[SndDevCurIndex].pGuid = &guid->Guid;
	}
	else SndGuids[SndDevCurIndex].pGuid = nullptr;

	pSndDevs->index = index;
	strcpy_s(pSndDevs->name, sizeof(pSndDevs->name), lpcstrDescription);
	++SndDevCurIndex;
	++pSndDevs;
	return 1;
}

int __cdecl DSOUND_EnumerateDevices(SND_DEVICE* devs)
{
	pSndDevs = devs;
	SndDevCurIndex = 0;
	DirectSoundEnumerateA(DSCallback, nullptr);
	return SndDevCurIndex;
}

//0001:0007ad40       _DSOUND_Init               0047bd40 f   dsound.obj
int __cdecl DSOUND_Init(HWND hWnd, int index)
{
	if (FAILED(DirectSoundCreate(SndGuids[index].pGuid, &ppDS, nullptr)))
		return 0;

	DSCAPS caps = { 0 };
	caps.dwSize = sizeof(caps);
	ppDS->GetCaps(&caps);

	int dds_full;
	if ((caps.dwFlags & DSCAPS_EMULDRIVER) != 0)
	{
		dds_full = 1;
		ppDS->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE);
	}
	else
	{
		dds_full = 0;
		ppDS->SetCooperativeLevel(hWnd, DSSCL_WRITEPRIMARY);
	}

	WAVEFORMATEX fmt = { 0 };
	fmt.wFormatTag = WAVE_FORMAT_PCM;
	fmt.nChannels = 2;
	fmt.nSamplesPerSec = 44100;
	fmt.nBlockAlign = 4;
	fmt.wBitsPerSample = 16;
	fmt.nAvgBytesPerSec = fmt.nSamplesPerSec * fmt.nBlockAlign;

	DSBUFFERDESC desc = { 0 };
	desc.dwSize = sizeof(desc);
	if (dds_full)
	{
		desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
		desc.lpwfxFormat = &fmt;
	}
	else
	{
		desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
		desc.dwBufferBytes = 0;
	}

	if(FAILED(ppDS->CreateSoundBuffer(&desc, &pSndBuffer, nullptr)))
		return 0;

	if (!dds_full)
		pSndBuffer->SetFormat(&fmt);
	SNDMIX_Init();

	DSBCAPS dbcaps = { 0 };
	dbcaps.dwSize = sizeof(dbcaps);
	pSndBuffer->GetCaps(&dbcaps);

	SndBufferBytes = dbcaps.dwBufferBytes;
	SndBufferAlign = 3 * ((signed int)dbcaps.dwBufferBytes / 4);
	if (3 * ((signed int)dbcaps.dwBufferBytes / 4) > 16384)
		SndBufferAlign = 16384;
	pSndBuffer->GetCurrentPosition(nullptr, &SndBufferHalf);
	DWORD size = SndBufferAlign + SndBufferHalf;
	SndBufferHalf += SndBufferAlign;
	if (SndBufferHalf >= SndBufferBytes)
	{
		do
			size -= SndBufferBytes;
		while (size >= SndBufferBytes);
		SndBufferHalf = size;
	}

	StreamSamples();
	pSndBuffer->Play(0, 0, DSBPLAY_LOOPING);

	DWORD ThreadID;
	hThread = CreateThread(nullptr, 0, PlayThread, nullptr, 0, &ThreadID);

	return 1;
}

//0001:0007b130       _DSOUND_Shutdown           0047c130 f   dsound.obj
EXTERN_C void SNDMIX_Shutdown();

void __cdecl DSOUND_Shutdown()
{
#if 0
	// unsafe way
	TerminateThread(hThread, 0);
#else
	// safe way to do this
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
#endif
	SNDMIX_Shutdown();
	if (pSndBuffer)
	{
		pSndBuffer->Stop();
		pSndBuffer->Release();
		pSndBuffer = nullptr;
	}
	if (ppDS)
	{
		ppDS->Release();
		ppDS = nullptr;
	}
}

void StreamSamples()
{
	DWORD status, pos;
	pSndBuffer->GetStatus(&status);

	if (status & DSBSTATUS_BUFFERLOST)
	{
		pSndBuffer->Restore();
		pSndBuffer->Play(0, 0, DSBPLAY_LOOPING);
		pSndBuffer->GetCurrentPosition(0, &pos);
		SndBufferHalf = pos;
	}

	pSndBuffer->GetCurrentPosition(0, &pos);
	DWORD dwOffset = SndBufferHalf;
	int range = SndBufferHalf + SndBufferBytes - pos;
	int range2 = SndBufferHalf - pos;
	if (range > 0 && range < SndBufferAlign)
		range2 = SndBufferHalf + SndBufferBytes - pos;
	if (range2 < SndBufferAlign)
	{
		DWORD dwBytes;
		if (range2 >= 0)
			dwBytes = SndBufferAlign - range2;
		else
		{
			DBG_Print("Mixer is behind :(\n");
			dwOffset = SndBufferHalf - range2;
			dwBytes = SndBufferAlign;
			SndBufferHalf = dwOffset;
		}

		if (dwBytes >= 1024)
		{
			if (dwOffset >= SndBufferBytes)
			{
				DBG_Print("bufferpos is too big :P\n");
				dwOffset = 0;
				SndBufferHalf = 0;
			}

			DWORD ppvAudioPtr1 = 0, ppvAudioPtr2 = 0,
				pdwAudioBytes1 = 0, pdwAudioBytes2 = 0;
			if (SUCCEEDED(pSndBuffer->Lock(dwOffset, dwBytes, (LPVOID*)&ppvAudioPtr1, &pdwAudioBytes1, (LPVOID*)&ppvAudioPtr2, &pdwAudioBytes2, 0)))
			{
				if (ppvAudioPtr1)
				{
					if (pdwAudioBytes1)
					{
						SNDMIX_Mix((unsigned short*)ppvAudioPtr1, pdwAudioBytes1 >> 2);
						SndBufferHalf += pdwAudioBytes1;
						if (SndBufferHalf == SndBufferBytes)
							SndBufferHalf = 0;
					}
				}
				if (ppvAudioPtr2)
				{
					if (pdwAudioBytes2)
					{
						SNDMIX_Mix((unsigned short*)ppvAudioPtr2, pdwAudioBytes2 >> 2);
						SndBufferHalf = pdwAudioBytes2;
					}
				}
				pSndBuffer->Unlock((LPVOID)ppvAudioPtr1, pdwAudioBytes1, (LPVOID)ppvAudioPtr2, pdwAudioBytes2);
			}
		}
	}
}

DWORD WINAPI PlayThread(LPVOID lpThreadParameter)
{
	while (1)
	{
		Sleep(15);
		StreamSamples();
	}

	return 0;
}
