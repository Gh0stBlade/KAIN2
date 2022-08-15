#ifndef EMULATOR_SPU_H
#define EMULATOR_SPU_H

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#define SPU_MAX_CHANNELS 24
#define SPU_PLAYBACK_FREQUENCY (44100)
#define SPU_MS_PER_UPDATE 368*2
#define SPU_FPS 60
#define SPU_ADPCM_FRAME_SIZE 16
#define SPU_PCM_FRAME_SIZE 56
#define AUDIO_CHUNK_SIZE SPU_ADPCM_FRAME_SIZE * SPU_MS_PER_UPDATE
#define AUDIO_CHUNK_SIZE_PCM SPU_PCM_FRAME_SIZE * SPU_MS_PER_UPDATE
//#define SPU_USE_TIMER 1
#define OLD_SYSTEM

struct Channel
{
    enum Flags
    {
        VOICE_NEW = (1 << 0),
        VOICE_PLAYING = (1 << 1),
    };
    int voiceNum;
    unsigned int voicePitches;
    unsigned int voiceStartAddrs;
    unsigned char* voicePosition;
    unsigned char* voiceEnd;
    unsigned int voiceFlags;
    unsigned int voiceEndFlag;
    int voiceLength;
    int s_1;
    int s_2;

};

extern Channel channelList[SPU_MAX_CHANNELS];

extern void Mix_Play(int vNum, unsigned char* address, int timeMs);
extern unsigned int SPU_DecodeAudioFrame(unsigned char* vag, unsigned short* out, struct Channel* channel);
#if defined(SPU_USE_TIMER)
unsigned int SPU_Update(unsigned int interval, void* pTimerID);
#else
extern void SPU_Update();
#endif
extern void SPU_Initialise();
extern void SPU_InitialiseChannel(int vNum);
extern void SPU_InitialiseChannelKeepStartAddrAndPitch(int vNum);

#if defined(OPENAL)
extern ALuint mixerChunks[SPU_MAX_CHANNELS];
extern ALuint alSources[SPU_MAX_CHANNELS];
extern ALuint alBuffers[SPU_MAX_CHANNELS];

extern void Mix_ChannelFinishedPlayingCallback(int channel);
#endif


#endif