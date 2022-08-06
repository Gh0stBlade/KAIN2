#include "EMULATOR_MIXER.H"
#include "EMULATOR_SPU.H"

#include "Core/Debug/EMULATOR_LOG.H"

unsigned char MixChannelToSpuChannel[SPU_MAX_CHANNELS];

#if defined(SDL2_MIXER)

SDL_TimerID audioTimer;
Mix_Chunk* mixerChunks[SPU_MAX_CHANNELS];
unsigned char* frequencyConvertedChunks[SPU_MAX_CHANNELS];

int Mix_ChangeFrequency(Mix_Chunk* chunk, int freq, int vNum)
{
    Uint16 format;
    int channels;
    Mix_QuerySpec(NULL, &format, &channels);

    SDL_AudioCVT cvt;
    int channel;
    SDL_BuildAudioCVT(&cvt, format, 1, freq, format, channels, SPU_PLAYBACK_FREQUENCY);

    if (cvt.needed)
    {
        for (channel = 0; channel < SPU_MAX_CHANNELS; channel++)
            if (!Mix_Playing(channel)) break; //Find a free channel

        if (channel == SPU_MAX_CHANNELS)
        {
            eprinterr("Fatal: Channels overloaded!\n");
            return -1;
        }

        //Set converter lenght and buffer
        cvt.len = chunk->alen;
        cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult);
        if (cvt.buf == NULL)
        {
            return -1;
        }

        //Copy the Mix_Chunk data to the new chunk and make the conversion
        SDL_memcpy(cvt.buf, chunk->abuf, chunk->alen);
        if (SDL_ConvertAudio(&cvt) < 0)
        {
            SDL_free(cvt.buf);
            return -1;
        }

        //If it was sucessfull put it on the original Mix_Chunk
        chunk->abuf = cvt.buf;
        chunk->alen = cvt.len_cvt;

        //If there was a chunk in this channel delete it
        if (frequencyConvertedChunks[channel] != NULL)
            SDL_free(frequencyConvertedChunks[channel]);

        frequencyConvertedChunks[channel] = cvt.buf;
        MixChannelToSpuChannel[channel] = vNum;
        return channel;

    }
    else
    {
        return -1;
    }
}

#include <stdio.h>

FILE* f = NULL;

Mix_Chunk* Mix_LoadAudioChunk(int vNum, unsigned char* address)
{
    if (address == NULL)
    {
        return NULL;
    }

    if (f == NULL)
    {
        f = fopen("DEBUG.BIN", "wb+");
    }

    unsigned short pcm[28];
    SPU_DecodeAudioFrame(address, pcm, &channelList[vNum]);

    fwrite(pcm, sizeof(unsigned short) * 28, 1, f);

    Mix_Chunk* waveChunk = Mix_QuickLoad_RAW((unsigned char*)pcm, 56);

    return waveChunk;
}

void Mix_ChannelFinishedPlayingCallback(int channel)
{
    unsigned char* waveChunk = frequencyConvertedChunks[channel];
    frequencyConvertedChunks[channel] = NULL;
    SDL_free(waveChunk);

    Mix_Chunk* mixerChunk = mixerChunks[channel];
    mixerChunks[channel] = NULL;
    Mix_FreeChunk(mixerChunk);

    channelList[MixChannelToSpuChannel[channel]].voicePositions += 32;
    channelList[MixChannelToSpuChannel[channel]].voicePlaying = 0;
}

void Mix_Play(int vNum, unsigned char* address)
{
    Mix_Chunk* waveChunk = Mix_LoadAudioChunk(vNum, address);

    if (waveChunk != NULL)
    {
        Uint8* bkp_buf = waveChunk->abuf;
        Uint32 bkp_len = waveChunk->alen;

        int channel = Mix_ChangeFrequency(waveChunk, channelList[vNum].voicePitches, vNum);
        mixerChunks[channel] = waveChunk;

        Mix_PlayChannel(channel, waveChunk, 0);

        waveChunk->abuf = bkp_buf;
        waveChunk->alen = bkp_len;
    }
}

void Mix_Initialise()
{
    Mix_Init(0);///@TODO MIX_QUIT!

    if (Mix_OpenAudio(SPU_PLAYBACK_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 1024) < 0)
    {
        eprinterr("[SDL2_MIXER]: Failed to Mix_OpenAudio! %s\n", Mix_GetError());
    }

    Mix_AllocateChannels(SPU_MAX_CHANNELS);
    Mix_ChannelFinished(Mix_ChannelFinishedPlayingCallback);
}

#endif
