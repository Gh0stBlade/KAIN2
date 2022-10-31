#include "EMULATOR_SPU.H"
#include "Debug/EMULATOR_LOG.H"

#define SND_SAMPLES  1024
#define PITCH_SHIFT  12
#define VOL_SHIFT    15
#define BLOCK_END    (28 << PITCH_SHIFT)

int32_t gSamples[SND_SAMPLES * 2]; // stereo
#if defined(SDL2)
SDL_AudioDeviceID gAudioDevice;
#endif

const int8_t SPU_POS[] = { 0, 60, 115,  98, 122 };
const int8_t SPU_NEG[] = { 0,  0, -52, -55, -60 };

int32_t clamp(int32_t value)
{
    if (value < -32768)
        return -32768;
    if (value > 32767)
        return 32767;
    return value;
}

int16_t vagPredicate(int16_t value, uint8_t pred, uint8_t shift, int32_t* s1, int32_t* s2)
{
    int32_t s = ((*s1) * SPU_POS[pred] + (*s2) * SPU_NEG[pred]) >> 6;
    s += (value >> shift);
    s = clamp(s);

    *s2 = *s1;
    *s1 = s;

    return s;
}

int32_t vagProcessBlock(Channel* channel, int16_t* dst)
{
    int32_t i;
    uint8_t d;
    uint8_t* src = channel->data + channel->pos;
    int32_t s1 = channel->s1;
    int32_t s2 = channel->s2;

    uint8_t pred = *src++;
    uint8_t flags = *src++;
    uint8_t shift = pred & 0x0F;
    pred >>= 4;

    for (i = 0; i < 14; i++)
    {
        d = *src++;
        *dst++ = vagPredicate((d & 0x0F) << 12, pred, shift, &s1, &s2);
        *dst++ = vagPredicate((d & 0xF0) <<  8, pred, shift, &s1, &s2);
    }

    if (flags & 4) // set loop start
    {
        channel->loop = channel->data + channel->pos;
        channel->loop_s1 = channel->s1;
        channel->loop_s2 = channel->s2;
    }

    if (flags & 1) // end
    {
        if (flags & 2) // goto loop start
        {
            eassert(channel->loop);
            src = channel->loop;
            s1 = channel->loop_s1;
            s2 = channel->loop_s2;
        }
        else
        {
            return 0; // stop TODO Release
        }
    }

    channel->pos = src - channel->data;
    channel->s1 = s1;
    channel->s2 = s2;

    return 1;
}

void fillVAG(Channel* channel, int32_t count)
{
    int32_t i, value;

    int32_t* samples = gSamples;
    int32_t volL = channel->volL * g_spuLeftVol >> VOL_SHIFT;
    int32_t volR = channel->volR * g_spuRightVol >> VOL_SHIFT;
    int32_t blockPos = channel->blockPos;
    int32_t posInc = channel->pitch;

    for (i = 0; i < count; i++)
    {
        if (blockPos >= BLOCK_END)
        {
            blockPos -= BLOCK_END;

            if (!vagProcessBlock(channel, channel->block))
            {
                channel->data = NULL;
                blockPos = BLOCK_END;
                break;
            }
        }

        value = channel->block[blockPos >> PITCH_SHIFT];

        *samples++ += value * volL >> VOL_SHIFT;
        *samples++ += value * volR >> VOL_SHIFT;

        blockPos += posInc;
    }

    channel->blockPos = blockPos;
}

void convertSamples(int16_t* dst, int32_t* src, int32_t count)
{
    int32_t i;

    for (i = 0; i < count; i++)
    {
        *dst++ = clamp(*src++);
        *dst++ = clamp(*src++);
    }
}

void fillSamples(int16_t* buffer, int32_t count)
{
    int32_t i;
    Channel* channel = channelList;

    memset(gSamples, 0, sizeof(gSamples));

    for (i = 0; i < SPU_MAX_CHANNELS; i++, channel++)
    {
        if (!channel->data)
            continue;

        fillVAG(channel, count);
    }

    convertSamples(buffer, gSamples, count);
}

void sndFill(void* udata, uint8_t* stream, int32_t len)
{
    fillSamples((int16_t*)stream, SND_SAMPLES);
}

void SPU_Initialise()
{
    int32_t i;
#if defined(SDL2)
    SDL_AudioSpec desired, obtained;
#endif

    for (i = 0; i < SPU_MAX_CHANNELS; i++)
    {
        channelList[i].blockPos = BLOCK_END;
    }
#if defined(SDL2)
    desired.freq     = 44100;
    desired.format   = AUDIO_S16SYS;
    desired.channels = 2;
    desired.samples  = SND_SAMPLES;
    desired.callback = sndFill;
    desired.userdata = NULL;

    gAudioDevice = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0);
    SDL_PauseAudioDevice(gAudioDevice, 0);
#endif
}

void SPU_Destroy()
{
#if defined(SDL2)
    SDL_PauseAudioDevice(gAudioDevice,1);
    SDL_CloseAudioDevice(gAudioDevice);
#endif
}

void SPU_ResetChannel(Channel* channel, uint8_t* data)
{
    channel->pos = 0;
    channel->s1 = 0;
    channel->s2 = 0;
    channel->data = data;
    channel->loop = NULL;
}
