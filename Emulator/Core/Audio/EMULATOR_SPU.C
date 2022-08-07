#include "EMULATOR_SPU.H"

#include <thread>

extern int _spu_keystat_last;
extern char spuSoundBuffer[524288];

std::thread thread;

void SPU_Initialise()
{
    for (int i = 0; i < SPU_MAX_CHANNELS; i++)
    {
        SPU_InitialiseChannel(i);
    }

#if defined(SDL2)
    thread = std::thread(SPU_Update);
    //SDL_AddTimer(1000/SPU_FPS, SPU_Update, NULL);

#endif
}

void SPU_InitialiseChannel(int vNum)
{
    struct Channel* channel = &channelList[vNum];
    channel->voiceNum = vNum;
    channel->voicePlaying = 0;
    channel->voiceEnd = NULL;
    channel->voicePitches = 0;
    channel->voicePosition = NULL;
    channel->voiceStartAddrs = 0;
    channel->voiceEndFlag = 0;
    channel->voiceNew = TRUE;
    channel->voiceLength = -1;
    channel->s_1 = 0;
    channel->s_2 = 0;
}

void SPU_InitialiseChannelKeepStartAddrAndPitch(int vNum)
{
    struct Channel* channel = &channelList[vNum];
    channel->voiceNum = vNum;
    channel->voicePlaying = 0;
    channel->voiceEnd = NULL;
    channel->voicePosition = NULL;
    channel->voiceEndFlag = 0;
    channel->voiceNew = TRUE;
    channel->voiceLength = -1;
    channel->s_1 = 0;
    channel->s_2 = 0;
}

int SPU_GetADPCMSize(unsigned char* pADPCM)
{
    int resultSize = 0;
    
    //End flag
    while (pADPCM[1] != 7)
    {
        resultSize += 16;
        pADPCM += 16;
    }
    
    return resultSize + 16;
}

void SPU_Update()
{
    while (TRUE)
    {
        for (int i = 0; i < SPU_MAX_CHANNELS; i++)
        {
            if (_spu_keystat_last & (1 << i) && channelList[i].voiceNew == TRUE)
            {
                unsigned char* pADPCM = (unsigned char*)&spuSoundBuffer[channelList[i].voiceStartAddrs];

                if (channelList[i].voicePosition == NULL)
                {
                    channelList[i].voicePosition = pADPCM;
                }

                if (channelList[i].voiceLength == -1)
                {
                    float time = (float)((SPU_GetADPCMSize(pADPCM) / SPU_ADPCM_FRAME_SIZE) * SPU_PCM_FRAME_SIZE) / (float)(channelList[i].voicePitches * 2 * 16 / 8);
                    channelList[i].voiceLength = (int)(time * 1000.0f);
                }

                if (channelList[i].voiceEnd == NULL)
                {
                    channelList[i].voiceEnd = &pADPCM[SPU_GetADPCMSize(pADPCM)];
                }

                Mix_Play(i, channelList[i].voicePosition);

                channelList[i].voicePlaying = TRUE;
                channelList[i].voiceNew = FALSE;
            }
        }
    }
    //return interval;
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

    for(int f = 0; f < SPU_MS_PER_UPDATE; f++)
    {
        shift = *vag & 0xF;
        filter = (*vag++ & 0x70) >> 4;
        flags = *vag++;

        if (flags == 7)
        {
            channel->voiceEndFlag = TRUE;
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
    }

    return result_length;
}
