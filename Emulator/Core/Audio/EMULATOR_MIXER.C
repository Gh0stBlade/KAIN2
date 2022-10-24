#include "EMULATOR_MIXER.H"
#include "EMULATOR_SPU.H"
#include "EMULATOR_RESAMPLE.H"

#include "Core/Debug/EMULATOR_LOG.H"

extern int _spu_keystat_last;

#if defined(SDL2_MIXER)

SDL_TimerID audioTimer;
Mix_Chunk* mixerChunks[SPU_MAX_CHANNELS];
char MixChannelToSPUChannel[SPU_MAX_CHANNELS];

extern int SPU_GetADPCMSize(unsigned char* pADPCM);

Mix_Chunk* Mix_LoadAudioChunk(int vNum, unsigned char* address)
{
    if (address == NULL)
    {
        return NULL;
    }

#if defined(OLD_SYSTEM)
    unsigned int tempPcmLength = SPU_GetADPCMSize(address) * 2;
    unsigned short* pcm = new unsigned short[tempPcmLength];
#else
    unsigned short pcm[AUDIO_CHUNK_SIZE_PCM / 2];
#endif
    unsigned int pcmLength = SPU_DecodeAudioFrame(address, pcm, &channelList[vNum]);

    int resampledPCMLength = 0;
    unsigned char* resampledPCM = Resample_PCM(channelList[vNum].voicePitches, SPU_PLAYBACK_FREQUENCY, (short*)&pcm[0], pcmLength, &resampledPCMLength);

#if defined(OLD_SYSTEM)
    delete[] pcm;
    pcm = NULL;
#endif

#if 1
    Mix_Chunk* waveChunk = Mix_QuickLoad_RAW(resampledPCM, resampledPCMLength);
#else
    Mix_Chunk* waveChunk = Mix_QuickLoad_RAW((unsigned char*)pcm, pcmLength);
#endif

    return waveChunk;
}

void Mix_ChannelFinishedPlayingCallback(int channel)
{
    Mix_Chunk* mixerChunk = mixerChunks[channel];
    int vNum = MixChannelToSPUChannel[channel];

    if (mixerChunk != NULL)
    {
        Resample_Free(mixerChunk->abuf);

        mixerChunks[channel] = NULL;
        Mix_FreeChunk(mixerChunk);
    }

    if (channelList[vNum].voiceEndFlag)//End flag
    {
        _spu_keystat_last &= ~(1 << vNum);
        SPU_InitialiseChannelKeepStartAddrAndPitch(vNum);
        MixChannelToSPUChannel[channel] = 0;
    }
    else
    {
        channelList[vNum].voicePosition += AUDIO_CHUNK_SIZE;
        channelList[vNum].voiceFlags |= Channel::Flags::VOICE_NEW;
    }
}

void Mix_Play(int vNum, unsigned char* address, int timeMs)
{
    //Stops if channel assigned twice which is wrong.
    //while (Mix_Playing(vNum))//Hack?
    //{
    //    Mix_HaltChannel(vNum);
    //}

    Mix_Chunk* waveChunk = Mix_LoadAudioChunk(vNum, address);

    if (waveChunk != NULL)
    {
        int actualChannel;
        for (actualChannel = 0; actualChannel < SPU_MAX_CHANNELS; actualChannel++)
        {
            if(!Mix_Playing(actualChannel))
            {
                break;
            }
        }
        
        //Mix_PlayChannel(actualChannel, waveChunk, 0);
        Mix_PlayChannelTimed(actualChannel, waveChunk, 0, timeMs);
        mixerChunks[actualChannel] = waveChunk;

        MixChannelToSPUChannel[actualChannel] = vNum;
    }
}

void Mix_Initialise()
{
    Mix_Init(0);///@TODO MIX_QUIT!

    if (Mix_OpenAudio(SPU_PLAYBACK_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        eprinterr("[SDL2_MIXER]: Failed to Mix_OpenAudio! %s\n", Mix_GetError());
    }

    Mix_AllocateChannels(SPU_MAX_CHANNELS);
    Mix_ChannelFinished(Mix_ChannelFinishedPlayingCallback);
}

#elif defined(OPENAL)

SDL_TimerID audioTimer;
ALuint mixerChunks[SPU_MAX_CHANNELS];
char MixChannelToSPUChannel[SPU_MAX_CHANNELS];

extern int SPU_GetADPCMSize(unsigned char* pADPCM);

ALuint Mix_LoadAudioChunk(int vNum, unsigned char* address)
{
    if (address == NULL)
    {
        return NULL;
    }

#if defined(OLD_SYSTEM)
    unsigned int tempPcmLength = SPU_GetADPCMSize(address) * 2;
    unsigned short* pcm = new unsigned short[tempPcmLength];
#else
    unsigned short pcm[AUDIO_CHUNK_SIZE_PCM / 2];
#endif
    unsigned int pcmLength = SPU_DecodeAudioFrame(address, pcm, &channelList[vNum]);

#if !defined(OPENAL)
    int resampledPCMLength = 0;
    unsigned char* resampledPCM = Resample_PCM(channelList[vNum].voicePitches, SPU_PLAYBACK_FREQUENCY, (short*)&pcm[0], pcmLength, &resampledPCMLength);
#endif

#if 1
    alGenBuffers(1, &alBuffers[vNum]);
    alBufferData(alBuffers[vNum], AL_FORMAT_MONO16, pcm, pcmLength, channelList[vNum].voicePitches);
    unsigned int err = alGetError();
    err++;
#else
    Mix_Chunk* waveChunk = Mix_QuickLoad_RAW((unsigned char*)pcm, pcmLength);
#endif

#if defined(OLD_SYSTEM)
    delete[] pcm;
    pcm = NULL;
#endif

    return alBuffers[vNum];
}

void Mix_ChannelFinishedPlayingCallback(int channel)
{
    alSourceStop(alSources[channel]);
    alDeleteBuffers(1, &alBuffers[channel]);
    alBuffers[channel] = 0;

    if (channelList[channel].voiceEndFlag)//End flag
    {
        _spu_keystat_last &= ~(1 << channel);
        SPU_InitialiseChannelKeepStartAddrAndPitch(channel);
        MixChannelToSPUChannel[channel] = 0;
    }
    else
    {
        channelList[channel].voicePosition += AUDIO_CHUNK_SIZE;
        channelList[channel].voiceFlags |= VOICE_NEW;
    }
}

void Mix_Play(int vNum, unsigned char* address, int timeMs)
{
    ALuint waveChunk = Mix_LoadAudioChunk(vNum, address);

    if (waveChunk != NULL)
    {
        alSourcei(alSources[vNum], AL_BUFFER, alBuffers[vNum]);
        alSourcePlay(alSources[vNum]);
    }
}

void Mix_Initialise()
{
   
}

#endif
