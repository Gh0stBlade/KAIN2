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

typedef struct Channel
{
    int32_t pitch;
    int32_t volL;
    int32_t volR;
    int32_t reverb;
    int32_t pos;
    int32_t s1, s2;
    int32_t blockPos;
    int16_t block[28];
    uint8_t* data;
    uint8_t* loop;
    int32_t loop_s1, loop_s2;
} channel;

extern short g_spuLeftVol;
extern short g_spuRightVol;

extern struct Channel channelList[SPU_MAX_CHANNELS];

void SPU_Initialise();
void SPU_Destroy();
void SPU_ResetChannel(struct Channel* channel, uint8_t* data);

#endif