#include "EMULATOR_SPU.H"

extern int _spu_keystat_last;
extern char spuSoundBuffer[524288];

void SPU_Initialise()
{
    for (int i = 0; i < 24; i++)
    {
        channelList[i].voicePlaying = 0;
        channelList[i].s_1 = 0;
        channelList[i].s_2 = 0;
    }

#if defined(SDL2)
    SDL_AddTimer(1000 / 60, SPU_Update, NULL);
#endif
}

unsigned int SPU_Update(unsigned int interval, void* pTimerID)
{
    for (int i = 0; i < 24; i++)
    {
        if (_spu_keystat_last & (1 << i) && channelList[i].voicePlaying == 0)
        {
            Mix_Play(i, (unsigned char*)&spuSoundBuffer[channelList[i].voiceStartAddrs + channelList[i].voicePositions]);
            channelList[i].voicePlaying = 1;
        }
    }

    return interval;
}

int CLAMP16(int x)
{
    if (x > 32767) x = 32767;
    else if (x < -32768) x = -32768;

    return x;
}

unsigned int SPU_DecodeAudioFrame(unsigned char* vag, unsigned short* out, struct Channel* channel)
{
    int predict_nr, shift, flags;
    int i;
    int d, s;
    int s_1 = channel->s_1;
    int s_2 = channel->s_2;
    unsigned int result_length = 0;
    int sample;
    int filter;
    int filterTablePos[5] = { 0, 60, 115, 98, 122 };
    int filterTableNeg[5] = { 0, 0, -52, -55, -60 };


    while (1)
    {
        shift = *vag & 0xF;
        filter = (*vag++ & 0x70) >> 4;
        flags = *vag++;

        if (flags == 7)
        {
            break;
        }

        if (shift > 12) shift = 9;
        if (filter > 4) filter = 4;

        int filterPos = filterTablePos[filter];
        int filterNeg = filterTableNeg[filter];

        for (i = 0; i < 28; i += 2)
        {
            d = *vag++;

            sample = (int)(short)((d & 0xF) << 12);
            sample = (sample >> shift);
            sample += ((s_1 * filterPos + s_2 * filterNeg + 32) >> 6);
            sample = CLAMP16(sample);

            *out++ = sample;

            s_2 = s_1;
            s_1 = sample;

            sample = (int)(short)((d & 0xf0) << 8);
            sample = (sample >> shift);
            sample += ((s_1 * filterPos + s_2 * filterNeg + 32) >> 6);
            sample = CLAMP16(sample);

            *out++ = sample;

            s_2 = s_1;
            s_1 = sample;

            result_length += sizeof(unsigned int);
        }

        channel->s_1 = s_1;
        channel->s_2 = s_2;
        //1 frame
        break;

        if (flags == 1)
            break;
    }

    return result_length;
}
