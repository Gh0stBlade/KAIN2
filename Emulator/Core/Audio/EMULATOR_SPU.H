#ifndef EMULATOR_SPU_H
#define EMULATOR_SPU_H

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#define SPU_MAX_CHANNELS 24
#define SPU_PLAYBACK_FREQUENCY (44100)

struct Channel
{
    unsigned int voicePitches;
    unsigned int voiceStartAddrs;
    unsigned int voicePositions;
    unsigned int voiceLengths;
    unsigned int voicePlaying;
    int s_1;
    int s_2;
};

extern Channel channelList[SPU_MAX_CHANNELS];

extern void Mix_Play(int vNum, unsigned char* address);
extern unsigned int SPU_DecodeAudioFrame(unsigned char* vag, unsigned short* out, struct Channel* channel);
extern unsigned int SPU_Update(unsigned int interval, void* pTimerID);
extern void SPU_Initialise();

#endif