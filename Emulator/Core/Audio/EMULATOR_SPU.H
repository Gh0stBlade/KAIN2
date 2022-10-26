#ifndef EMULATOR_SPU_H
#define EMULATOR_SPU_H

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#define SPU_MAX_CHANNELS 24
#define SPU_PLAYBACK_FREQUENCY (44100)
#define SPU_MS_PER_UPDATE 336*2
#define SPU_FPS 60
#define SPU_ADPCM_FRAME_SIZE 16
#define SPU_PCM_FRAME_SIZE 56
#define AUDIO_CHUNK_SIZE SPU_ADPCM_FRAME_SIZE * SPU_MS_PER_UPDATE
#define AUDIO_CHUNK_SIZE_PCM SPU_PCM_FRAME_SIZE * SPU_MS_PER_UPDATE
#define SPU_FINAL_BUFFER_SIZE 368*2

#define AUDIO_BUFFER_SIZE 28 * 2 * 4

#if defined(__ANDROID__)
#define SPU_USE_TIMER 1
#endif
#define OLD_SYSTEM

enum Flags
{
    VOICE_INITIAL = 0,
    VOICE_NEW = (1 << 0),
    VOICE_PLAYING = (1 << 1),
    VOICE_PROCESSING = (1 << 2),
};

union Counter {
    struct {
        uint32_t : 4;
        uint32_t index : 8;  // Interpolation index
        uint32_t sample : 5;
        uint32_t : 15;
    };
    uint32_t _reg = 0;
};

struct Channel
{
    int voiceNum;
    unsigned int voicePitches;
    unsigned int voiceSteps;
    unsigned int voiceStartAddrs;
    unsigned char* voicePosition;
    unsigned char* voiceEnd;
    unsigned int voiceFlags;
    unsigned int voiceEndFlag;
    int voiceLength;
    int s_1;
    int s_2;
    short decodedAudioData[28];
    short decodedAudioDataSize;
    short previousDecodedAudioData[28];
    short previousDecodedAudioDataSize;
    short sample;
    int reverb;
    short volL;
    short volR;
    short samplePos;
    short frame;
    short previousFrame;
    Counter counter;
};

extern short g_spuLeftVol;
extern short g_spuRightVol;

extern struct Channel channelList[SPU_MAX_CHANNELS];

extern void Mix_Play(int vNum, unsigned char* address, int timeMs);
extern void SPU_DecodeAudioFrame(unsigned char* vag, struct Channel* channel);
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

#if !defined(SN_TARGET_PSP2) && !defined(__ANDROID__)
#include <thread>
extern std::thread audioThread;
#endif

extern void SPU_Destroy();


#endif