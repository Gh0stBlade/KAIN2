#if 0 //ndef EMULATOR_RESAMPLE_H
#define EMULATOR_RESAMPLE_H

extern unsigned char* Resample_PCM(int src_rate, int dst_rate, short* in, int numSamples, int* outNumSamples);
extern void Resample_Free(unsigned char* data);

#endif