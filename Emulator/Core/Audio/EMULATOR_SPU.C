#include "EMULATOR_SPU.H"
#include "Debug/EMULATOR_LOG.H"

#if !defined(SN_TARGET_PSP2) && !defined(__ANDROID__)
#include <thread>
std::thread audioThread;
#endif

#if defined(OPENAL)
extern ALCdevice* alDevice;
extern ALCcontext* alContext;

extern ALuint alSources[SPU_MAX_CHANNELS];
extern ALuint alBuffers[SPU_MAX_CHANNELS];
extern ALboolean alHasAudioData[24];
#endif

int bufferReady;

extern int _spu_keystat_last;
extern char spuSoundBuffer[524288];
extern char MixChannelToSPUChannel[SPU_MAX_CHANNELS];

short audioBuffer[28 * 2 * 4];
unsigned short audioBufferPos = 0;


short finalAudioBuffer[512 * 32];
unsigned short finalAudioBufferPos = 0;

short gauss[512] = {
     -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, 0x001,  -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, 0x001,  0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0003, 0x0003, 0x0003, 0x0004,
     0x0004, 0x0005, 0x0005, 0x0006, 0x0007, 0x0007, 0x0008, 0x0009, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F, 0x0010, 0x0011,
     0x0012, 0x0013, 0x0015, 0x0016, 0x0018, 0x0019, 0x001B, 0x001C, 0x001E, 0x0020, 0x0021, 0x0023, 0x0025, 0x0027, 0x0029, 0x002C, 0x002E,
     0x0030, 0x0033, 0x0035, 0x0038, 0x003A, 0x003D, 0x0040, 0x0043, 0x0046, 0x0049, 0x004D, 0x0050, 0x0054, 0x0057, 0x005B, 0x005F, 0x0063,
     0x0067, 0x006B, 0x006F, 0x0074, 0x0078, 0x007D, 0x0082, 0x0087, 0x008C, 0x0091, 0x0096, 0x009C, 0x00A1, 0x00A7, 0x00AD, 0x00B3, 0x00BA,
     0x00C0, 0x00C7, 0x00CD, 0x00D4, 0x00DB, 0x00E3, 0x00EA, 0x00F2, 0x00FA, 0x0101, 0x010A, 0x0112, 0x011B, 0x0123, 0x012C, 0x0135, 0x013F,
     0x0148, 0x0152, 0x015C, 0x0166, 0x0171, 0x017B, 0x0186, 0x0191, 0x019C, 0x01A8, 0x01B4, 0x01C0, 0x01CC, 0x01D9, 0x01E5, 0x01F2, 0x0200,
     0x020D, 0x021B, 0x0229, 0x0237, 0x0246, 0x0255, 0x0264, 0x0273, 0x0283, 0x0293, 0x02A3, 0x02B4, 0x02C4, 0x02D6, 0x02E7, 0x02F9, 0x030B,
     0x031D, 0x0330, 0x0343, 0x0356, 0x036A, 0x037E, 0x0392, 0x03A7, 0x03BC, 0x03D1, 0x03E7, 0x03FC, 0x0413, 0x042A, 0x0441, 0x0458, 0x0470,
     0x0488, 0x04A0, 0x04B9, 0x04D2, 0x04EC, 0x0506, 0x0520, 0x053B, 0x0556, 0x0572, 0x058E, 0x05AA, 0x05C7, 0x05E4, 0x0601, 0x061F, 0x063E,
     0x065C, 0x067C, 0x069B, 0x06BB, 0x06DC, 0x06FD, 0x071E, 0x0740, 0x0762, 0x0784, 0x07A7, 0x07CB, 0x07EF, 0x0813, 0x0838, 0x085D, 0x0883,
     0x08A9, 0x08D0, 0x08F7, 0x091E, 0x0946, 0x096F, 0x0998, 0x09C1, 0x09EB, 0x0A16, 0x0A40, 0x0A6C, 0x0A98, 0x0AC4, 0x0AF1, 0x0B1E, 0x0B4C,
     0x0B7A, 0x0BA9, 0x0BD8, 0x0C07, 0x0C38, 0x0C68, 0x0C99, 0x0CCB, 0x0CFD, 0x0D30, 0x0D63, 0x0D97, 0x0DCB, 0x0E00, 0x0E35, 0x0E6B, 0x0EA1,
     0x0ED7, 0x0F0F, 0x0F46, 0x0F7F, 0x0FB7, 0x0FF1, 0x102A, 0x1065, 0x109F, 0x10DB, 0x1116, 0x1153, 0x118F, 0x11CD, 0x120B, 0x1249, 0x1288,
     0x12C7, 0x1307, 0x1347, 0x1388, 0x13C9, 0x140B, 0x144D, 0x1490, 0x14D4, 0x1517, 0x155C, 0x15A0, 0x15E6, 0x162C, 0x1672, 0x16B9, 0x1700,
     0x1747, 0x1790, 0x17D8, 0x1821, 0x186B, 0x18B5, 0x1900, 0x194B, 0x1996, 0x19E2, 0x1A2E, 0x1A7B, 0x1AC8, 0x1B16, 0x1B64, 0x1BB3, 0x1C02,
     0x1C51, 0x1CA1, 0x1CF1, 0x1D42, 0x1D93, 0x1DE5, 0x1E37, 0x1E89, 0x1EDC, 0x1F2F, 0x1F82, 0x1FD6, 0x202A, 0x207F, 0x20D4, 0x2129, 0x217F,
     0x21D5, 0x222C, 0x2282, 0x22DA, 0x2331, 0x2389, 0x23E1, 0x2439, 0x2492, 0x24EB, 0x2545, 0x259E, 0x25F8, 0x2653, 0x26AD, 0x2708, 0x2763,
     0x27BE, 0x281A, 0x2876, 0x28D2, 0x292E, 0x298B, 0x29E7, 0x2A44, 0x2AA1, 0x2AFF, 0x2B5C, 0x2BBA, 0x2C18, 0x2C76, 0x2CD4, 0x2D33, 0x2D91,
     0x2DF0, 0x2E4F, 0x2EAE, 0x2F0D, 0x2F6C, 0x2FCC, 0x302B, 0x308B, 0x30EA, 0x314A, 0x31AA, 0x3209, 0x3269, 0x32C9, 0x3329, 0x3389, 0x33E9,
     0x3449, 0x34A9, 0x3509, 0x3569, 0x35C9, 0x3629, 0x3689, 0x36E8, 0x3748, 0x37A8, 0x3807, 0x3867, 0x38C6, 0x3926, 0x3985, 0x39E4, 0x3A43,
     0x3AA2, 0x3B00, 0x3B5F, 0x3BBD, 0x3C1B, 0x3C79, 0x3CD7, 0x3D35, 0x3D92, 0x3DEF, 0x3E4C, 0x3EA9, 0x3F05, 0x3F62, 0x3FBD, 0x4019, 0x4074,
     0x40D0, 0x412A, 0x4185, 0x41DF, 0x4239, 0x4292, 0x42EB, 0x4344, 0x439C, 0x43F4, 0x444C, 0x44A3, 0x44FA, 0x4550, 0x45A6, 0x45FC, 0x4651,
     0x46A6, 0x46FA, 0x474E, 0x47A1, 0x47F4, 0x4846, 0x4898, 0x48E9, 0x493A, 0x498A, 0x49D9, 0x4A29, 0x4A77, 0x4AC5, 0x4B13, 0x4B5F, 0x4BAC,
     0x4BF7, 0x4C42, 0x4C8D, 0x4CD7, 0x4D20, 0x4D68, 0x4DB0, 0x4DF7, 0x4E3E, 0x4E84, 0x4EC9, 0x4F0E, 0x4F52, 0x4F95, 0x4FD7, 0x5019, 0x505A,
     0x509A, 0x50DA, 0x5118, 0x5156, 0x5194, 0x51D0, 0x520C, 0x5247, 0x5281, 0x52BA, 0x52F3, 0x532A, 0x5361, 0x5397, 0x53CC, 0x5401, 0x5434,
     0x5467, 0x5499, 0x54CA, 0x54FA, 0x5529, 0x5558, 0x5585, 0x55B2, 0x55DE, 0x5609, 0x5632, 0x565B, 0x5684, 0x56AB, 0x56D1, 0x56F6, 0x571B,
     0x573E, 0x5761, 0x5782, 0x57A3, 0x57C3, 0x57E2, 0x57FF, 0x581C, 0x5838, 0x5853, 0x586D, 0x5886, 0x589E, 0x58B5, 0x58CB, 0x58E0, 0x58F4,
     0x5907, 0x5919, 0x592A, 0x593A, 0x5949, 0x5958, 0x5965, 0x5971, 0x597C, 0x5986, 0x598F, 0x5997, 0x599E, 0x59A4, 0x59A9, 0x59AD, 0x59B0,
     0x59B2, 0x59B3 
};


int16_t sample(Channel* v, int p) 
{
    if (p < 0)
    {
        if (v->previousDecodedAudioDataSize == 0)
        {
            return 0;
        }

        return v->previousDecodedAudioData[v->previousDecodedAudioDataSize - abs(p)];
    }

    return v->decodedAudioData[p];
}

short SPU_GetMainLeftVol()
{
    return g_spuLeftVol;
}

short SPU_GetMainRightVol()
{
    return g_spuRightVol;
}

short SPU_GetChannelLeftVol(Channel* c)
{
    if (c->volL & 0x8000)
    {
        return 0x7fff;
    }

    return (int16_t)c->volL * 2;
}

short SPU_GetChannelRightVol(Channel* c)
{
    if (c->volR & 0x8000)
    {
        return 0x7fff;
    }

    return (int16_t)c->volR * 2;
}

short SPU_Interpolate(Channel* v, int pos, int i)
{
    // Store two ADPCM rows? zero out if outside?
    short oldest = sample(v, pos - 3);
    short older = sample(v, pos - 2);
    short old = sample(v, pos - 1);
    short new_ = sample(v, pos);

    short out = 0;
    out += (gauss[0x0ff - i] * oldest) >> 15;
    out += (gauss[0x1ff - i] * older) >> 15;
    out += (gauss[0x100 + i] * old) >> 15;
    out += (gauss[0x000 + i] * new_) >> 15;

    return out;
}

void SPU_Initialise()
{


#if defined(SDL2_MIXER)

    Mix_Initialise();

#elif defined(OPENAL)

    alDevice = alcOpenDevice(NULL);

    if (alDevice == NULL)
    {
        eprinterr("Failed to create OpenAL device!\n");
        return;
    }

    alContext = alcCreateContext(alDevice, NULL);
    if (alcMakeContextCurrent(alContext) == NULL)
    {
        eprinterr("Failed to create OpenAL context!\n");
        return;
    }

    ALfloat orient[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    alListener3f(AL_POSITION, 0.0f, 0.0f, 1.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, orient);

    for (int i = 0; i < SPU_MAX_CHANNELS; i++)
    {
        alGenSources(1, &alSources[i]);
        alSourcef(alSources[i], AL_PITCH, 1);
        alSourcef(alSources[i], AL_GAIN, 1);
        alSource3f(alSources[i], AL_POSITION, 0, 0, 0);
        alSource3f(alSources[i], AL_VELOCITY, 0, 0, 0);
        alSourcei(alSources[i], AL_LOOPING, AL_FALSE);
    }
#elif defined(XAUDIO2)

    CoInitializeEx(0, 0);

    HRESULT hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

    assert(SUCCEEDED(hr));

    hr = pXAudio2->CreateMasteringVoice(&pMasterVoice);
    assert(SUCCEEDED(hr));
#endif

    for (int i = 0; i < SPU_MAX_CHANNELS; i++)
    {
        bufferReady = FALSE;

        audioBufferPos = 0;

        SPU_InitialiseChannel(i);
    }

#if defined(SDL2)
#if defined(SPU_USE_TIMER)
    SDL_AddTimer(1000 / SPU_FPS, SPU_Update, NULL);
#else
#if !defined(SINGLE_THREADED_AUDIO) || 1
    //audioThread = std::thread(SPU_Update);
#endif
#endif

#endif
}

void SPU_Destroy()
{
#if defined(OPENAL)
    for (int i = 0; i < SPU_MAX_CHANNELS; i++)
    {
        alSourceStop(alSources[i]);
        alDeleteSources(1, &alSources[i]);
        alDeleteBuffers(1, &alBuffers[i]);
        alSources[i] = 0;
        alBuffers[i] = 0;
    }

    alcDestroyContext(alContext);
    alcCloseDevice(alDevice);

    alContext = NULL;
    alDevice = NULL;
#endif

    for (int i = 0; i < SPU_MAX_CHANNELS; i++)
    {
        SPU_InitialiseChannel(i);
    }
}

void SPU_InitialiseChannel(int vNum)
{
    struct Channel* channel = &channelList[vNum];
    channel->voiceNum = vNum;
    channel->voiceEnd = NULL;
    channel->voicePitches = 0;
    channel->voiceSteps = 0;
    channel->voicePosition = NULL;
    channel->voiceStartAddrs = 0;
    channel->voiceEndFlag = 0;
    channel->voiceFlags = VOICE_INITIAL;
    channel->voiceLength = -1;
    channel->s_1 = 0;
    channel->s_2 = 0;
    channel->decodedAudioDataSize = 0;
    channel->previousDecodedAudioDataSize = 0;
    channel->sample = 0;
    channel->reverb = 0;
    channel->volL = 0;
    channel->volR = 0;
    channel->samplePos = 0;
    channel->frame = 0;
    channel->previousFrame = 0;

    memset(channel->decodedAudioData, 0, sizeof(channel->decodedAudioData));
    memset(channel->previousDecodedAudioData, 0, sizeof(channel->previousDecodedAudioData));
}

void SPU_InitialiseChannelKeepStartAddrAndPitch(int vNum)
{
    struct Channel* channel = &channelList[vNum];
    channel->voiceNum = vNum;
    channel->voiceEnd = NULL;
    channel->voicePosition = NULL;
    channel->voiceSteps = NULL;
    channel->voiceEndFlag = 0;
    channel->voiceFlags = VOICE_INITIAL;
    channel->voiceLength = -1;
    channel->s_1 = 0;
    channel->s_2 = 0;
    channel->decodedAudioDataSize = 0;
    channel->previousDecodedAudioDataSize = 0;
    channel->sample = 0;
    channel->reverb = 0;
    channel->volL = 0;
    channel->volR = 0;
    channel->samplePos = 0;
    channel->frame = 0;
    channel->previousFrame = 0;
    channel->counter._reg = 0;

    memset(channel->decodedAudioData, 0, sizeof(channel->decodedAudioData));
    memset(channel->previousDecodedAudioData, 0, sizeof(channel->previousDecodedAudioData));
}

int SPU_GetADPCMSize(unsigned char* pADPCM)
{
    int resultSize = 0;
    
    //End flag
    while (pADPCM[1] != 7)
    {
        resultSize += 56;
        pADPCM += 16;
    }
    
    return resultSize;
}

int noiseFrequency;
int noiseLevel;

unsigned char noiseHalfCycle[5] = { 0, 84, 140, 180, 210 };

unsigned char noiseAddition[64] = {
    1, 0, 0, 1, 0, 1, 1, 0,  //
    1, 0, 0, 1, 0, 1, 1, 0,  //
    1, 0, 0, 1, 0, 1, 1, 0,  //
    1, 0, 0, 1, 0, 1, 1, 0,  //
    0, 1, 1, 0, 1, 0, 0, 1,  //
    0, 1, 1, 0, 1, 0, 0, 1,  //
    0, 1, 1, 0, 1, 0, 0, 1,  //
    0, 1, 1, 0, 1, 0, 0, 1   //
};

void SPU_DoNoise(unsigned short step, unsigned short shift)
{
    int freq = (0x8000 >> shift) << 16;
    noiseFrequency += 0x10000;
    noiseFrequency += noiseHalfCycle[step];

    if ((noiseFrequency & 0xFFFF) >= noiseHalfCycle[4]) 
    {
        noiseFrequency += 0x10000;
        noiseFrequency -= noiseHalfCycle[step];
    }

    if (noiseFrequency >= freq) 
    {
        noiseFrequency %= freq;
        int index = (noiseLevel >> 10) & 0x3F;
        noiseLevel = (noiseLevel << 1) + noiseAddition[index];
    }
}

#if defined(SPU_USE_TIMER)
unsigned int SPU_Update(unsigned int interval, void* pTimerID)
#else
void SPU_Update()
#endif
{
    SPU_DoNoise(0, 0);

    while(bufferReady == FALSE)
    {
        short sumLeft = 0;
        short sumReverbLeft = 0;
        short sumRight = 0;
        short sumReverbRight = 0;

        short sample = 0;

        for (int i = 0; i < SPU_MAX_CHANNELS; i++)
        {
            Channel* channel = &channelList[i];

            if (_spu_keystat_last & (1 << i) && (channel->voiceFlags & VOICE_NEW))
            {
                unsigned char* pADPCM = (unsigned char*)&spuSoundBuffer[channel->voiceStartAddrs];

                if (channel->voicePosition == NULL && (channel->voiceFlags & VOICE_NEW))
                {
                    channel->voicePosition = pADPCM;
                }

                if (channel->voiceLength == -1 && (channel->voiceFlags & VOICE_NEW))//TODO should pass voicePosition instead to get actual size per chunk
                {
                    float time = (float)((SPU_GetADPCMSize(pADPCM) / SPU_ADPCM_FRAME_SIZE) * SPU_PCM_FRAME_SIZE) / (float)(channelList[i].voicePitches * 2 * 16 / 8);
                    channel->voiceLength = (int)(time * 1000.0f);
                }

                if (channel->voiceEnd == NULL && (channel->voiceFlags & VOICE_NEW))
                {
                    channel->voiceEnd = &pADPCM[SPU_GetADPCMSize(pADPCM)];
                }

#if defined(OPENAL) || defined(SDL2_MIXER)
                channel->previousFrame = channel->frame;

                if (channel->decodedAudioDataSize == 0)
                {
                    SPU_DecodeAudioFrame(channel->voicePosition, channel);
                }

                //processEnvelope

                unsigned int step = channel->voiceSteps;

                if (step > 0x3fff)
                {
                    step = 0x4000;
                }
#endif
#if 1
                sample = SPU_Interpolate(channel, channel->counter.sample, channel->counter.index);

                channel->sample = sample;

                if ((channel->voiceFlags & VOICE_NEW))
                {
                    sumLeft += (sample * SPU_GetChannelLeftVol(channel)) >> 15;
                    sumRight += (sample * SPU_GetChannelRightVol(channel)) >> 15;

                    if (channel->reverb)
                    {
                        sumReverbLeft += (sample * SPU_GetChannelLeftVol(channel)) >> 15;
                        sumReverbRight += (sample * SPU_GetChannelRightVol(channel)) >> 15;
                    }
                }
#else
                sample = channel->decodedAudioData[channel->samplePos++];
                sumLeft = sample;
                sumRight = sample;
#endif
                channel->counter._reg += step;
                if (channel->counter.sample >= 28) {
                    // Overflow, parse next ADPCM block
                    channel->counter.sample -= 28;
                    //voice.currentAddress._reg += 2;
                    memcpy(channel->previousDecodedAudioData, channel->decodedAudioData, sizeof(channel->decodedAudioData));
                    channel->previousDecodedAudioDataSize = channel->decodedAudioDataSize;
                    channel->decodedAudioDataSize = 0;

                    //if (voice.loadRepeatAddress) {
                    //    voice.loadRepeatAddress = false;
                    //    voice.currentAddress = voice.repeatAddress;
                    //}
                }

                sumLeft += 0;
                sumRight += 0;

                sumLeft = (sumLeft * (min(0x3fff, SPU_GetMainLeftVol()) * 2)) >> 15;
                sumRight = (sumRight * (min(0x3fff, SPU_GetMainRightVol()) * 2)) >> 15;

                
            }

            if (channel->voiceEndFlag)
            {
                _spu_keystat_last &= ~(1 << i);
                SPU_InitialiseChannel(i);
            }
        }

        audioBuffer[audioBufferPos] = sumLeft;
        audioBuffer[audioBufferPos + 1] = sumRight;

        audioBufferPos += 2;

        if (audioBufferPos >= 28 * 2 * 4)
        {
            audioBufferPos = 0;
            bufferReady = TRUE;
            break;
        }
    }

    memcpy(finalAudioBuffer + finalAudioBufferPos, audioBuffer, sizeof(audioBuffer));
    finalAudioBufferPos += sizeof(audioBuffer) / sizeof(short);

    bufferReady = FALSE;

    if (finalAudioBufferPos >= (sizeof(finalAudioBuffer) / sizeof(short)) - 16)
    {
        //Add to buffer
        alGenBuffers(1, &alBuffers[0]);
        alBufferData(alBuffers[0], AL_FORMAT_STEREO16, finalAudioBuffer, sizeof(finalAudioBuffer), 44100);

        memset(finalAudioBuffer, 0, sizeof(finalAudioBuffer));

        FILE* f = fopen("AUDIO.BIN", "wb+");

        if (f != NULL)
        {
            fwrite(audioBuffer, 28 * 2 * 4 * sizeof(short), 1, f);
            fclose(f);
        }

        alSourcei(alSources[0], AL_BUFFER, alBuffers[0]);
        alSourcePlay(alSources[0]);

        while (1)
        {
            ALint state;
            alGetSourcei(alSources[0], AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING)
            {
                alDeleteBuffers(1, &alBuffers[0]);
                alBuffers[0] = NULL;
                audioBufferPos = 0;
                break;
            }
        }

        finalAudioBufferPos = 0;
    }

}

int CLAMP16(int x)
{
    if (x > 32767) x = 32767;
    else if (x < -32768) x = -32768;

    return x;
}

void SPU_DecodeAudioFrame(unsigned char* vag, struct Channel* channel)
{
    int shift, flags;
    int i;
    int d;
    int s_1 = channel->s_1;
    int s_2 = channel->s_2;
    int sample;
    int filter;
    int filterTablePos[5] = { 0, 60, 115, 98, 122 };
    int filterTableNeg[5] = { 0, 0, -52, -55, -60 };

    channel->decodedAudioDataSize = 0;

    shift = *vag & 0xF;
    filter = (*vag++ & 0x70) >> 4;
    flags = *vag++;

    if (flags == 7)
    {
        channel->voiceEndFlag = 1;//TRUE;
        return;
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

        channel->decodedAudioData[i] = sample;

        s_2 = s_1;
        s_1 = sample;

        sample = (int)(short)((d & 0xf0) << 8);
        sample = (sample >> shift);
        sample += ((s_1 * filterPos + s_2 * filterNeg + 32) >> 6);
        sample = CLAMP16(sample);

        channel->decodedAudioData[i + 1] = sample;

        s_2 = s_1;
        s_1 = sample;

        channel->decodedAudioDataSize += 2;
    }

    channel->frame++;

    channel->voicePosition += SPU_ADPCM_FRAME_SIZE;

    if (!(channel->voiceFlags & VOICE_PROCESSING))
    {
        channel->counter._reg = 0;
        channel->voiceFlags |= VOICE_PROCESSING;
    }

    channel->s_1 = s_1;
    channel->s_2 = s_2;
}
