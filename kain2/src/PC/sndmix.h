#pragma once

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void SNDMIX_Init();
extern void SNDMIX_Shutdown();
extern void SNDMIX_Mix(WORD* sample, int size);

extern void SNDMIX_SetSample(int voiceNum, BYTE* sample);
extern void SNDMIX_SetNextSample(int voiceNum, BYTE* sample);
extern BYTE* SNDMIX_GetSample(int voiceNum);
extern BYTE* SNDMIX_GetNextSample(int voiceNum);
extern void SNDMIX_SetFrequency(int voiceNum, float frequency);
extern void SNDMIX_SetVolume(int voiceNum, int left, int right);
extern void SNDMIX_SetLoopMode(int voiceNum, int mode);
extern void SNDMIX_SetChannelInterrupt(int voiceNum, int intr);
extern int SNDMIX_GetStatus(int voiceNum);
extern void SNDMIX_Start(int voiceNum);
extern void SNDMIX_Stop(int voiceNum);
extern void SNDMIX_KeyOff(int voiceNum);
extern void* SNDMIX_UploadSample(const void* data, int samples, int a3, int a4, int a5);
extern void SNDMIX_FreeSample(void* ptr);
extern void SNDMIX_SetTimerFunc(void(__cdecl* fn)());

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif