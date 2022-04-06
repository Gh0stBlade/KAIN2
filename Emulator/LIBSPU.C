#include "LIBSPU.H"
#include "LIBETC.H"
#include <stdio.h>
#include "EMULATOR.H"
#include "LIBAPI.H"

#if defined(OPENAL)
#include <al.h>
#include <alc.h>

ALCdevice* alDevice = NULL;
ALCcontext* alContext = NULL;

ALuint alSources[24];
ALuint alBuffers[24];
ALuint alBufferSizes[24];

#elif defined(XAUDIO2)

#include <xaudio2.h>
#include <assert.h>

IXAudio2* pXAudio2 = NULL;
IXAudio2MasteringVoice* pMasterVoice = NULL;
IXAudio2SourceVoice* pSourceVoices[24];
#endif

unsigned int voicePitces[24];
unsigned int voiceStartAddrs[24];

#include <string.h>

#define SPU_CENTERNOTE (-32768 / 2)

SpuTransferCallbackProc __spu_transferCallback = NULL;

short _spu_voice_centerNote[24] =
{
	SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE,
	SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE,
	SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE, SPU_CENTERNOTE
};

unsigned short word_300[] = 
{
  0x8000,0x879c,0x8fac,0x9837,0xa145,0xaadc,0xb504,0xbfc8,0xcb2f,0xd744,0xe411,0xf1a1 
};

unsigned short word_318[] =
{
  0x8000,0x800e,0x801d,0x802c,0x803b,0x804a,0x8058,0x8067,0x8076,0x8085,0x8094,0x80a3
};

unsigned short E40[] =
{
    1799, 1799, 1799, 1799, 1799, 1799, 1799, 1799
};

unsigned short _spu_rev_attr[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

short _spu_RQ[10];

SpuCommonAttr dword_424;//Might be wrong struct, need to check
int _spu_isCalled = 0;
int _spu_FiDMA = 0;///@TODO decl as extern find initial value
int _spu_EVdma = 0;
int _spu_rev_flag = 0;
int _spu_rev_reserve_wa = 0;
int _spu_rev_offsetaddr = 0;
int _spu_rev_startaddr = 0;
int _spu_AllocBlockNum = 0;
int _spu_AllocLastNum = 0;
int _spu_memList = 0;
int _spu_trans_mode = 0;
int _spu_transMode = 0;
int _spu_addrMode = 0;
int _spu_keystat = 0;
int _spu_RQmask = 0;
int _spu_RQvoice = 0;
int _spu_env = 0;
int _spu_mem_mode = 2;
int _spu_mem_mode_unit = 8;
int _spu_mem_mode_unitM = 7;
char spu[440];//0x1F801C00 is base address
unsigned short* _spu_RXX = (unsigned short*)&spu[0];
int _spu_mem_mode_plus = 3;
void* _spu_transferCallback = NULL;///@TODO initial value check
int _spu_inTransfer = 0;///@TODO initial value check
int _spu_IRQCallback = 0;
unsigned short _spu_tsa = 0;
int PrimaryDMAControlRegister = 0;///@TODO check initials wil likely be stripped though
int* dword_E10 = &PrimaryDMAControlRegister;//Base address is 1F8010F0.

char spuSoundBuffer[520191];

unsigned int decodeVAG(unsigned char* vag, unsigned int length, unsigned char* out)
{
    int predict_nr, shift_factor, flags;
    int d, s;
    double s_1 = 0.0f;
    double s_2 = 0.0f;
    unsigned int result_length = 0;
    unsigned short* wav = (unsigned short*)out;
    double samples[28];

    double f[5][2] = { { 0.0f, 0.0 },
                    {   60.0f / 64.0f,  0.0f },
                    {  115.0f / 64.0f, -52.0f / 64.0f },
                    {   98.0f / 64.0f, -55.0f / 64.0f },
                    {  122.0f / 64.0f, -60.0f / 64.0f } };


    for(int i = 0; i < length; i++)
    {
        predict_nr = *vag >> 4;
        shift_factor = *vag++ & 0xF;
        flags = *vag++;

        if (flags == 7)
        {
            break;
        }

        for (i = 0; i < 28; i += 2, vag++)
        {
            s = (*vag & 0xF) << 12;
            if (s & 0x8000)
            {
                s |= 0xFFFF0000;
            }

            samples[i] = (double)(s >> shift_factor);
            s = (*vag & 0xF0) << 8;

            if (s & 0x8000)
            {
                s |= 0xFFFF0000;
            }

            samples[i + 1] = (double)(s >> shift_factor);
        }

        for (i = 0; i < 28; i++) 
        {
            samples[i] = samples[i] + s_1 * f[predict_nr][0] + s_2 * f[predict_nr][1];
            s_2 = s_1;
            s_1 = samples[i];
            *wav++ = (int)(s_1 + 0.5f);
            result_length += sizeof(unsigned short);
        }
    }

    return result_length;
}

int _spu_note2pitch(int a0, int a1, int a2, int a3)
{
    a3 += a1;
    a1 = 0x2AAAAAAB;
    int v1 = a3 & 0xFFFF;
    int v0 = v1 >> 7;
    a2 += v0;
    a2 -= a0;
    a2 <<= 16;
    a0 = a2 >> 16;
    int t0 = a0 % a1;
    a3 = v1 & 0x7F;
    a2 >>= 31;
    v0 = t0 >> 1;
    a1 = v0 - a2;
    a2 = a1 - 2;
    v0 = a1 << 1;
    v0 += a1;
    v0 <<= 2;
    a0 -= v0;
    v0 = a0 << 16;
    v1 = a0;
    if (v0 < 0)
    {
        v1 = a0 + 12;
        a2 = a1 - 3;
    }//loc_164

    v0 = a3 & 0xFFFF;

    v1 = word_300[v1];
    v0 = word_300[v0];

    t0 = v1 * v0;
    v0 = a2;
    a1 = t0 >> 16;

    if (v0 >= 0)
    {
        a1 = 0x3FFF;
    }
    else
    {
        a0 = -v0;
        v1 = a0 - 1;
        v0 = 1 << v1;
        a1 += v0;
        a1 = a1 >> a0;
    }

    return a1 & 0xFFFF;
}

int _spu_FsetRXXa(long flag, long addr)
{
    if (_spu_mem_mode != 0)
    {
        if (_spu_mem_mode_unit != 0)
        {
            if (addr % _spu_mem_mode_unit != 0)
            {
                addr += _spu_mem_mode_unit;
                addr & ~_spu_mem_mode_unitM;
            }
            //loc_BA0
        }
    }
    //loc_BA0
    if (flag == -2)
    {
        return addr;
    }
    else if (flag == -1)
    {
        return (addr >> _spu_mem_mode_plus) & 0xFFFF;
    }
    else
    {
        //loc_BD8
        _spu_RXX[flag] = (short)(addr >> _spu_mem_mode_plus);
    }

    return addr;
}

void SpuGetAllKeysStatus(char* status)
{
	//loc_2EC
	for (int i = 0; i < 24; i++, status++)
	{
		if ((_spu_keystat & (1 << i)))
		{
			if ((unsigned short)_spu_RXX[(i << 3) + 6] != 0)
			{
				*status = 1;
			}
			else
			{
				*status = 3;
			}
		}
		else
		{
			//loc_330
			if (_spu_RXX[(i << 3) + 6] != 0)
			{
				*status = 2;
			}
			else
			{
				//loc_340
				*status = 0;
			}
		}
	}
}

void SpuSetVoiceAttr(SpuVoiceAttr* arg)//
{
#if 0//TRC only ;-(
    _SpuRSetVoiceAttr();
#else

    //s0 = arg
    int a0 = 0;
    int a1 = 0;
    int a2 = 0;
    //s1 = arg->mask
    //s5 = &_spu_voice_centerNote[0];
    //s2 = s1 < 1 ? 1 : 0;

    //loc_238
    for(int i = 0; i < 24; i++)
    {
        //v0 = 1
        //v1 = arg->voice
        //v0 = 1 << i
        //v1 = arg->voice & (1 << i);

        if ((arg->voice & (1 << i)))
        {
            //s3 = i << 3
            if ((arg->mask < 1) || (arg->mask & 0x10))
            {
                //loc_264
                //v0 = i << 4
                //v1 = &_spu_RXX[0];
                //a0 = arg->pitch
                _spu_RXX[(i << 3) + 2] = arg->pitch;
            }
            //loc_27C
            if (arg->mask < 1 || (arg->mask & 0x40))
            {
                _spu_voice_centerNote[i] = arg->sample_note;
            }
            //loc_298
            if (arg->mask < 1 || (arg->mask & 0x20))
            {
                //a1 = _spu_voice_centerNote[i] & 0xFF
                //a3 = arg->note
                //a0 = _spu_voice_centerNote[i] >> 8
                //a2 = arg->note >> 8
                //a3 = arg->note & 0xFF
                //v0 = 
                _spu_RXX[(i << 3) + 2] = _spu_note2pitch((_spu_voice_centerNote[i] >> 8), (_spu_voice_centerNote[i] & 0xFF), (arg->note >> 8), (arg->note & 0xFF));
            }
            //loc_2D8
            if (arg->mask < 1 || (arg->mask & 0x1))
            {
                //v0 = arg->volume.left
                //a0 = 0
                //a1 = arg->volume.left & 0x7FFF
                //v0 = arg->mask & 4

                if (!(arg->mask & 0x4))
                {
                    //v0 = arg->volmode.left
                    switch (arg->volmode.left)
                    {
                    case 1:
                        //loc_33C
                        a0 = 0x8000;
                        break;
                    case 2:
                        //loc_344
                        a0 = 0x9000;
                        break;
                    case 3:
                        //loc_34C
                        a0 = 0xA000;
                        break;
                    case 4:
                        //loc_354
                        a0 = 0xB000;
                        break;
                    case 5:
                        //loc_35C
                        a0 = 0xC000;
                        break;
                    case 6:
                        //loc_364
                        a0 = 0xD000;
                        break;
                    case 7:
                        //loc_36C
                        a0 = 0xE000;
                        break;
                    }
                }
                //def_334
                if (a0 != 0)
                {
                    //v1 = arg->volume.left
                    if (arg->volume.left >= 128)
                    {
                        a1 = 127;
                    }
                    else if (arg->volume.left < 0)
                    {
                        a1 = 0;
                    }
                }
                //loc_3A0
                //v0 = &_spu_RXX[0];
                //v1 = &spu_RXX[i << 3];

                //v0 = a1 | a0
                _spu_RXX[(i << 3)] = a1 | a0;
            }//loc_3B8

            if ((arg->mask < 1) || (arg->mask & 0x2))
            {
                //v0 = arg->volume.right
                //a0 = 0
                //a1 = v0 & 0x7FFF

                if ((arg->mask < 1) || (arg->mask & 0x8))
                {
                    switch (arg->volmode.right)
                    {
                    case 1:
                        //loc_41C
                        a0 = 0x8000;
                        break;
                    case 2:
                        //loc_424
                        a0 = 0x9000;
                        break;
                    case 3:
                        //loc_42C
                        a0 = 0xA000;
                        break;
                    case 4:
                        //loc_434
                        a0 = 0xB000;
                        break;
                    case 5:
                        //loc_43C
                        a0 = 0xC000;
                        break;
                    case 6:
                        //loc_444
                        a0 = 0xD000;
                        break;
                    case 7:
                        //loc_44C
                        a0 = 0xE000;
                        break;
                    }
                }

                //def_414
                if (a0 != 0)
                {
                    if (arg->volume.right >= 128)
                    {
                        a1 = 127;
                    }
                    else if (arg->volume.right < 0)
                    {
                        a1 = 0;
                    }
                }//loc_480

                //v0 = &_spu_RXX[0];
                //v1 = &_spu_RXX[i << 3];
                _spu_RXX[(i << 3) + 1] = a1 | a0;
            }//loc_498

            if ((arg->mask < 1) || (arg->mask & 0x80))
            {
                _spu_FsetRXXa(((i << 3) | 3), arg->addr);
            }
            //loc_4B4
            if ((arg->mask < 1) || (arg->mask & 0x10000))
            {
                _spu_FsetRXXa((i << 3) | 7, arg->loop_addr);
            }//loc_4D4

            if ((arg->mask < 1) || (arg->mask & 0x20000))
            {
                _spu_RXX[i << 3] = arg->adsr1;
            }

            //loc_500
            if ((arg->mask < 1) || (arg->mask & 0x40000))
            {
                _spu_RXX[i << 3] = arg->adsr2;
            }
            //loc_52C

            if ((arg->mask < 1) || (arg->mask & 0x800))
            {
                //a1 = arg->ar
                if (arg->ar >= 128)
                {
                    a1 = 127;
                }

                a2 = 0;

                if ((arg->mask < 1) || (arg->mask & 0x100))
                {
                    //v1 = arg->a_mode
                    //v0 = 5
                    if (arg->a_mode == 5)
                    {
                        a2 = 128;
                    }
                }//loc_57C

                //v0 = &_spu_RXX[0];
                //s3 = &_spu_RXX[i << 3];
                //v0 = _spu_RXX[(i << 3) + 4];
                //v1 = _spu_RXX[(i << 3) + 4] & 0xFF;
                //v0 = ((a1 | a2) << 8)
                _spu_RXX[(i << 3) + 4] = (_spu_RXX[(i << 3) + 4] & 0xFF) | ((a1 | a2) << 8);

            }//loc_5A8

            if ((arg->mask < 1) || arg->mask & 0x1000)
            {
                a1 = arg->dr;
                if (arg->dr >= 16)
                {
                    a1 = 15;
                }//loc_5D0

                _spu_RXX[(i << 3) + 4] = (_spu_RXX[(i << 3) + 4] & 0xFF0F) | (a1 << 4);

            }//loc_5F4

            if ((arg->mask < 1) || (arg->mask & 0x2000))
            {
                a1 = arg->sr;

                if (arg->sr >= 128)
                {
                    a1 = 127;
                }

                a2 = 256;

                if ((arg->mask < 1) || (arg->mask & 0x200))
                {
                    //v1 = arg->s_mode
                    if ((arg->s_mode) == 5)
                    {
                        //loc_674
                        a2 = 512;
                    }
                    else if (arg->s_mode >= 6)
                    {
                        //loc_658
                        if (arg->s_mode == 7)
                        {
                            //loc_67C
                            a2 = 768;
                        }
                    }
                    else if (arg->s_mode == 1)
                    {
                        //loc_66C
                        a2 = 0;
                    }
                }//loc_680
                //v0 = _spu_RXX[(i << 3) + 5];
                //v1 = (_spu_RXX[(i << 3) + 5] & 0x3F) | ((a1 | a2) << 6);
                //v0 = ((a1 | a2) << 6)
                _spu_RXX[(i << 3) + 5] = (_spu_RXX[(i << 3) + 5] & 0x3F) | ((a1 | a2) << 6);
            }//loc_6AC

            if ((arg->mask < 1) || (arg->mask & 0x4000))
            {
                a1 = arg->rr;

                if (arg->rr >= 32)
                {
                    a1 = 31;
                }
                //loc_6D4
                a2 = 0;

                if ((arg->mask < 1) || (arg->mask & 0x400))
                {
                    //v1 = arg->r_mode
                    if (arg->r_mode != 3 && arg->r_mode == 7)
                    {
                        a2 = 32;
                    }//loc_704
                }//loc_704

                //a0 = &_spu_RXX[(i << 3) + 5];
                _spu_RXX[(i << 3) + 5] = (_spu_RXX[(i << 3) + 5] & 0xFFC0) | (a1 | a2);
                //v1 = (a1 | a2)
            }//loc_728

            if ((arg->mask < 1) || arg->mask & 0x8000)
            {
                a1 = arg->sl;
                if ((arg->sl >= 16))
                {
                    a1 = 15;
                }//loc_750

                //v0 = _spu_RXX
                _spu_RXX[(i << 3) + 4] = (_spu_RXX[(i << 3) + 4] & 0xFFF0) | a1;
            }//loc_774
        }//loc_774
    }

    ///@? Not sure what the below is doing yet.
    int scratchPad[256];

    scratchPad[9] = 1;
    scratchPad[10] = 0;

    while (scratchPad[10] < 2)
    {
        //loc_794
        scratchPad[9] <<= 13;
        scratchPad[10]++;
    }
#endif
}

void SpuSetKey(long on_off, unsigned long voice_bit)
{
    voice_bit &= 0xFFFFFF;
    int a2 = voice_bit >> 16;

    if (on_off != 0)
    {
        if (on_off != SPU_ON)
        {
            return;
        }

        if ((_spu_env & 0x1))
        {
            _spu_RQ[0] = voice_bit;
            _spu_RQ[1] = a2;
            _spu_RQmask |= 0x1;
            _spu_RQvoice |= voice_bit;

            if ((_spu_RQ[_spu_RQmask] & voice_bit))
            {
                _spu_RQ[_spu_RQmask] = _spu_RQ[_spu_RQmask] & ~voice_bit;

            }//loc_29C

            if ((_spu_RQ[_spu_RQmask + 1] & voice_bit))
            {
                _spu_RQ[_spu_RQmask + 1] = _spu_RQ[_spu_RQmask + 1] & ~a2;

            }//locret_3B4
        }
        else
        {
            //loc_2C4
            _spu_RXX[196] = _spu_keystat | voice_bit;
            _spu_RXX[197] = a2;
            _spu_keystat |= voice_bit;
        }
    }
    else
    {
        //loc_2E4
        if ((_spu_env & 0x1))
        {
            _spu_RQ[_spu_RQmask] = voice_bit;
            _spu_RQ[_spu_RQmask + 1] = a2;
            _spu_RQmask |= 0x1;
            _spu_RQvoice &= (voice_bit ^ -1);

            if ((_spu_RQ[0] & voice_bit))
            {
                _spu_RQ[0] &= voice_bit;
            }
            //loc_360
            if ((_spu_RQ[1] & a2))
            {
                _spu_RQ[1] &= (a2 ^ -1);
            }
            //locret_3B4
        }
        else
        {
            //loc_388
            _spu_RXX[198] = voice_bit;
            _spu_RXX[199] = a2;
            _spu_keystat &= ~voice_bit;
        }
    }

    for (int i = 0; i < 24; i++)
    {
        if (_spu_keystat & (1 << i))
        {

#if defined(OPENAL) || defined(XAUDIO2)
            unsigned long vagSize = ((unsigned long*)&spuSoundBuffer[voiceStartAddrs[i]])[-1];
            unsigned char* wave = new unsigned char[vagSize * 8];
            unsigned int waveSize = decodeVAG((unsigned char*)&spuSoundBuffer[voiceStartAddrs[i]], vagSize, wave);
           
#if defined(OPENAL)
            alGenBuffers(1, &alBuffers[i]);
            alBufferData(alBuffers[i], AL_FORMAT_MONO16, wave, waveSize, voicePitces[i]);
            alSourcei(alSources[i], AL_BUFFER, alBuffers[i]);
            alSourcePlay(alSources[i]);

            ALint state;
            alGetSourcei(alSources[i], AL_SOURCE_STATE, &state);

            while (state == AL_PLAYING) 
            {
                alGetSourcei(alSources[i], AL_SOURCE_STATE, &state);
            }

            alDeleteBuffers(1, &alBuffers[i]);
#elif defined(XAUDIO2)
            WAVEFORMATEX wfex;
            wfex.wFormatTag = WAVE_FORMAT_PCM;
            wfex.nChannels = 1;
            wfex.nSamplesPerSec = voicePitces[i];
            wfex.wBitsPerSample = 16;
            wfex.nBlockAlign = (unsigned short)((wfex.nChannels * wfex.wBitsPerSample) / 8);
            wfex.cbSize = 0;
            wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;
            
            HRESULT hr = pXAudio2->CreateSourceVoice(&pSourceVoices[i], &wfex, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);

            assert(SUCCEEDED(hr));

            XAUDIO2_BUFFER buffer;
            buffer.AudioBytes = waveSize;
            buffer.pAudioData = (BYTE*)wave;
            buffer.Flags = XAUDIO2_END_OF_STREAM;
            buffer.PlayBegin = 0;
            buffer.PlayLength = 0;
            buffer.LoopBegin = 0;
            buffer.LoopLength = 0;
            buffer.LoopCount = 0;
            buffer.pContext = NULL;

            hr = pSourceVoices[i]->SubmitSourceBuffer(&buffer);
            hr = pSourceVoices[i]->Start();

            XAUDIO2_VOICE_STATE state;
            pSourceVoices[i]->GetState(&state);
            while (state.BuffersQueued > 0) 
            {
                pSourceVoices[i]->GetState(&state);
            }

            pSourceVoices[i]->DestroyVoice();

#endif
            delete[] wave;
#endif
        }
    }
}

void SpuSetKeyOnWithAttr(SpuVoiceAttr* attr)//(F)
{
	SpuSetVoiceAttr(attr);
	SpuSetKey(SPU_ON, attr->voice);
}

long SpuGetKeyStatus(unsigned long voice_bit)
{
	int a1 = -1;

	//loc_210
	for (int i = 0; i < 24; i++)
	{
		int v0 = 1 << i;

		if ((voice_bit & (1 << i)))
		{
			//loc_240
			if (i != -1)
			{
				//loc_248
				if ((_spu_keystat & (1 << i)) == 0)
				{
					return (0 < _spu_RXX[(i << 3) + 6]) << 1;
				}
				else
				{
					if (_spu_RXX[(i << 3) + 6] == 0)
					{
						return 3;
					}
					else
					{
						return 1;
					}
				}
			}
			else
			{
				return -1;
			}
		}
	}
	if (a1 != -1)
	{
		//loc_248
		if ((_spu_keystat & (1 << a1)) == 0)
		{
			return (0 < _spu_RXX[a1 << 3]) << 1;
		}
		else
		{
			if (_spu_RXX[a1 << 3] == 0)
			{
				return 3;
			}
			else
			{
				return 1;
			}
		}
	}
	else
	{
		return -1;
	}

	return 0;
}

void _spu_t(int mode, int flag)
{
	UNIMPLEMENTED();
}

void _spu_Fw(unsigned char* addr, unsigned long size)
{
#if defined(OPENAL) || defined(XAUDIO2)
    ((unsigned long*)&spuSoundBuffer[_spu_tsa])[-1] = size;
    memcpy(&spuSoundBuffer[_spu_tsa], addr, size);
    __spu_transferCallback();
#else

	if (_spu_trans_mode == 0)
	{
		//v0 = _spu_tsa
		//a1 = _spu_mem_mode_plus
		//a0 = 2
		_spu_t(2, _spu_tsa << _spu_mem_mode_plus);
		_spu_t(1, _spu_tsa << _spu_mem_mode_plus);

    }
#endif
}

unsigned long SpuWrite(unsigned char* addr, unsigned long size)
{
	if (0x7EFF0 < size)
	{
		size = 0x7EFF0;
	}
	
	//loc_228
	_spu_Fw(addr, size);

	if (_spu_transferCallback == NULL)
	{
		_spu_inTransfer = 0;
	}

	return size;
}

long SpuSetTransferMode(long mode)//(F)
{
    int trans_mode = 1;

    if (mode != 0)
    {
        if (mode == 1)
        {
            trans_mode = 1;
        }
        else
        {
            trans_mode = 0;
        }
    }
    else
    {
        //loc_218
        trans_mode = 0;
    }

    _spu_trans_mode = mode;
    _spu_transMode = trans_mode;

	return trans_mode;
}

unsigned long SpuSetTransferStartAddr(unsigned long addr)
{
    if (0x7EFE8 >= addr + 0x1010)
    {
        _spu_tsa = _spu_FsetRXXa(-1, addr);

        return _spu_tsa << _spu_mem_mode_plus;
    }

    return 0;
}

long SpuIsTransferCompleted(long flag)//(F)
{
    long event = 0;

    if (_spu_trans_mode == 1 || _spu_inTransfer == 1)
    {
        return 1;
    }

    event = TestEvent(_spu_EVdma);

    if (flag == 1)
    {
        if (event != 0)
        {
            _spu_inTransfer = 1;
            return 1;
        }
        else
        {
            //loc_260
            do
            {
                event = TestEvent(_spu_EVdma);
            } while (event == 0);

            _spu_inTransfer = 1;
            return 1;
        }
    }
    //loc_280
    if (event == 1)
    {
        _spu_inTransfer = 1;
    }

	return event;
}

void DMACallback(int a0, int callback)
{
    UNIMPLEMENTED();
}

void _SpuDataCallback(int callback)//(F)
{
    DMACallback(4, callback);
}

void SpuStart()//(F)
{
	long event = 0;

	if (_spu_isCalled == 0)
	{
		_spu_isCalled = 1;
		EnterCriticalSection();
		_SpuDataCallback(_spu_FiDMA);
		//event = OpenEvent(HwSPU, EvSpCOMP, EvMdNOINTR, NULL);
		_spu_EVdma = event;
		EnableEvent(event);
		ExitCriticalSection();
	}
	//loc_348
}

void _spu_Fw1ts()//(F)
{
    unsigned int result = 1;

    for (int i = 0; i < 60; i++)
    {
        result *= 13;
    }

    return;
}

void sub_480(unsigned short* buffer, int count)//(F)
{
    int s0 = 0;
    int v1 = 0;
    int s1 = 0;
    int a1 = 0;
    //v0 = _spu_RXX
    //v1 = (unsigned short)_spu_tsa
    s1 = count;
    //a1 = (unsigned short)_spu_RXX[215];
    //s2 = buffer
    _spu_RXX[211] = _spu_tsa;
    _spu_Fw1ts();

    //s3 = (unsigned short)_spu_RXX[215] & 0x7FF;
    a1 = (unsigned short)_spu_RXX[215] & 0x7FF;
    //v0 = s1 < 0x41 ? 1 : 0
    if (s1 != 0)
    {
        do
        {
            if (s1 <= 64)
            {
                s0 = s1;
            }
            else
            {
                s0 = 64;
            }

            //loc_4D4
            v1 = 0;
            if (s0 > 0)
            {
                //loc_4E4
                do
                {
                    v1 += 2;
                    _spu_RXX[212] = *buffer++;
                } while (v1 < s0);
            }//loc_500

            //v1 = _spu_RXX
            _spu_RXX[213] = (_spu_RXX[213] & 0xFFCF) | 0x10;
            _spu_Fw1ts();

            v1 = 0;
            if ((_spu_RXX[215] & 0x400))
            {
                v1 = 1;
            }//loc_590
            else
            {
                //loc_548
                do
                {
                    if (v1 >= 3841)
                    {
                        printf("SPU:T/O [%s]\n", "wait (wrdy H -> L)");
                        break;
                    }
                    //loc_570
                    v1++;
                } while ((_spu_RXX[215] & 0x400));
            }
            //loc_590
            _spu_Fw1ts();
            s1 -= s0;
            _spu_Fw1ts();
        } while (s1 != 0);
    }
    //loc_5A8
    //v0 = _spu_RXX
    _spu_RXX[213] &= 0xFFCF;
    //a1 = s3 & 0xFFFF

    v1 = 0;
    if ((_spu_RXX[215] & 0x7FF) != (a1 & 0xFFFF))
    {
        do
        {
            v1 = 1;

            //loc_5DC
            if (v1 >= 3841)
            {
                printf("SPU:T/O [%s]\n", "wait (dmaf clear/W)");
                break;
            }
            //loc_604
            v1++;
        } while ((_spu_RXX[215] & 0x7FF) != a1);///@FIXME suspected lock?
    }
    //loc_624
}

void _spu_init(int a0)//(F)
{
    unsigned int v1 = 0;

    //int s0 = a0
    //a0 = dword_E10
    //v0 = dword_E10[0];
    dword_E10[0] |= 0xB0000;

    //v0 = _spu_RXX
    _spu_transMode = 0;
    _spu_addrMode = 0;
    _spu_tsa = 0;
    _spu_RXX[192] = 0;
    _spu_RXX[193] = 0;
    _spu_RXX[213] = 0;

    _spu_Fw1ts();
    //v0 = _spu_RXX
    _spu_RXX[192] = 0;
    _spu_RXX[193] = 0;

    v1 = 0;
    if ((_spu_RXX[215] & 0x7FF))
    {
        v1 = 1;

        //loc_288
        do
        {
            if (v1 >= 3841)
            {
                printf("SPU:T/O [%s]\n", "wait (reset)");
                break;
            }//loc_2B0
            v1++;
        } while ((_spu_RXX[215] & 0x7FF));
    }
    //loc_2D0
    //a0 = 0

    //loc_2D4
    //a1 = &_spu_RQ[0];
    _spu_mem_mode = 2;
    _spu_mem_mode_plus = 3;
    _spu_mem_mode_unit = 8;
    _spu_mem_mode_unitM = 7;
    //v0 = _spu_RXX
    _spu_RXX[214] = 4;
    //v1 = 0xFFFF;
    _spu_RXX[194] = 0;
    _spu_RXX[195] = 0;
    _spu_RXX[198] = 65535;
    _spu_RXX[199] = 65535;
    _spu_RXX[204] = 0;
    _spu_RXX[205] = 0;

    //loc_338
    for (int i = 0; i < 10; i++)
    {
        _spu_RQ[i] = 0;
    }

    //v0 = 0;
    if (a0 == 0)
    {
        //a0 = E40

        //v0 = _spu_RXX
        _spu_tsa = 512;
        _spu_RXX[200] = 0;
        _spu_RXX[201] = 0;
        _spu_RXX[202] = 0;
        _spu_RXX[203] = 0;
        _spu_RXX[216] = 0;
        _spu_RXX[217] = 0;
        _spu_RXX[218] = 0;
        _spu_RXX[219] = 0;

        sub_480(&E40[0], 16);
        //a0 = 0
        //a2 = 0x3FFF
        //a1 = 0x200
        //v1 = _spu_RXX

        //loc_3B0
        for (int i = 0; i < 24; i++)
        {
            _spu_RXX[0 + (i * 8)] = 0;
            _spu_RXX[1 + (i * 8)] = 0;
            _spu_RXX[2 + (i * 8)] = 16383;
            _spu_RXX[3 + (i * 8)] = 512;
            _spu_RXX[4 + (i * 8)] = 0;
            _spu_RXX[5 + (i * 8)] = 0;
        }

        //s1 = 0xFFFF
        //v0 = _spu_RXX
        //s0 = 0xFF
        _spu_RXX[196] = 65535;
        _spu_Fw1ts();
        _spu_RXX[197] = 255;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_RXX[198] = 65535;
        _spu_Fw1ts();
        _spu_RXX[199] = 255;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        //v0 = 0;
    }
    //loc_440
    //a0 = _spu_RXX
    _spu_inTransfer = 1;
    _spu_RXX[213] = 49152;
    _spu_transferCallback = 0;
    _spu_IRQCallback = 0;

	return /*0*/;
}

void _spu_FsetRXX(int a0, int a1, int a2)//(F)
{
	if (a2 == 0)
	{
		_spu_RXX[a0] = a1;
	}
	else
	{
		_spu_RXX[a0] = a1 >> _spu_mem_mode_plus;
	}
}

void _SpuInit(int a0)
{
	ResetCallback();
	_spu_init(a0);

	if (a0 == 0)
	{
		for (int i = 0; i < sizeof(_spu_voice_centerNote) / sizeof(short); i++)
		{
			_spu_voice_centerNote[i] = SPU_CENTERNOTE;
		}
	}
	//loc_240
	SpuStart();

	_spu_rev_flag = 0;
	_spu_rev_reserve_wa = 0;
	dword_424.mask = 0;
	dword_424.mvol.left = 0;
	dword_424.mvol.right = 0;
	dword_424.mvolmode.left = 0;
	dword_424.mvolmode.right = 0;
	dword_424.mvolx.left = 0;
	dword_424.mvolx.right = 0;
	_spu_rev_offsetaddr = _spu_rev_startaddr;
	_spu_FsetRXX(209, _spu_rev_startaddr, 0);
	_spu_AllocBlockNum = 0;
	_spu_AllocLastNum = 0;
	_spu_memList = 0;
	_spu_trans_mode = 0;
	_spu_transMode = 0;
	_spu_keystat = 0;
	_spu_RQmask = 0;
	_spu_RQvoice = 0;
	_spu_env = 0;
}

void SpuInit(void)//(F)
{
    _SpuInit(0);

#if defined(OPENAL)
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

    for (int i = 0; i < 24; i++)
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
}

long SpuSetMute(long on_off)
{
	UNIMPLEMENTED();
	return 0;
}

long SpuSetReverb(long on_off)
{
	UNIMPLEMENTED();
	return 0;
}

unsigned long _SpuSetAnyVoice(long on_off, unsigned long voice_bit, int a2, int a3)
{
    UNIMPLEMENTED();
    return 0;
}

unsigned long SpuSetReverbVoice(long on_off, unsigned long voice_bit)
{
    return _SpuSetAnyVoice(on_off, voice_bit, 204, 205);
}

void SpuSetCommonAttr(SpuCommonAttr* attr)
{
	UNIMPLEMENTED();
}

long SpuInitMalloc(long num, char* top)//(F)
{
	if (num > 0)
	{
		//loc_214
		((int*)top)[0] = 0x40001010;
		_spu_memList = (uintptr_t)top;
		_spu_AllocLastNum = 0;
		_spu_AllocBlockNum = num;
		((int*)top)[1] = (0x10000000 << _spu_mem_mode_plus) - 0x1010;
	}

	return num;
}

long SpuMalloc(long size)
{
	return 0/*(long)(uintptr_t)malloc(size)*/;
}

long SpuMallocWithStartAddr(unsigned long addr, long size)
{
	UNIMPLEMENTED();
	return 0;
}

void SpuFree(unsigned long addr)
{
	/*free((void*)(uintptr_t)addr)*/;
}

void SpuSetCommonMasterVolume(short mvol_left, short mvol_right)//(F)
{
    _spu_RXX[192] = mvol_left & 0x7FFF;
    _spu_RXX[193] = mvol_right & 0x7FFF;
}

void SpuGetCommonCDMix(long* cd_mix)
{
    UNIMPLEMENTED();
}

long SpuSetReverbModeType(long mode)
{
	UNIMPLEMENTED();
	return 0;
}

void SpuSetReverbModeDepth(short depth_left, short depth_right)//(F)
{
    _spu_RXX[194] = depth_left;
    _spu_RXX[195] = depth_right;
    _spu_rev_attr[4] = depth_left;
    _spu_rev_attr[5] = depth_right;
}

void SpuGetVoicePitch(int vNum, unsigned short* pitch)
{

}

void SpuSetVoicePitch(int vNum, unsigned short pitch)
{
    short* p = (short*)&_spu_RXX[vNum << 2];
    p[3] = pitch;

#if defined(OPENAL) || defined(XAUDIO2)

    switch (pitch)
    {
    case 0x400:
        voicePitces[vNum] = 11025;
        break;
    default:
        voicePitces[vNum] = 0;
        eprinterr("[EMU-SPU]: Unknown pitch: %d\n", pitch);
        break;
    }
#endif
}

void SpuSetCommonCDMix(long cd_mix)
{
    UNIMPLEMENTED();
}

SpuTransferCallbackProc SpuSetTransferCallback(SpuTransferCallbackProc func)
{
    SpuTransferCallbackProc prev = __spu_transferCallback;

    if (func != __spu_transferCallback)
    {
        __spu_transferCallback = func;
    }

    return prev;
}

void SpuSetVoiceADSRAttr(int vNum, unsigned short AR, unsigned short DR, unsigned short SR, unsigned short RR, unsigned short SL, long ARmode, long SRmode, long RRmode)
{
    UNIMPLEMENTED();
}

void SpuSetVoiceStartAddr(int vNum, unsigned long startAddr)
{
    long var_4;

    var_4 = _spu_FsetRXXa((vNum << 3) | 0x3, startAddr);

    for (int i = 0; i < 2; i++)
    {
        var_4 *= 13;
    }
#if defined(OPENAL) || defined(XAUDIO2)
    voiceStartAddrs[vNum] = startAddr / 8;
#endif
}

void SpuSetVoiceVolume(int vNum, short volL, short volR)
{
    volL &= 0x7FFF;
    volR &= 0x7FFF;

    short* p = (short*)&_spu_RXX[vNum << 2];
    p[0] = volL;
    p[1] = volR;

#if defined(OPENAL)
    alSourcef(alSources[vNum], AL_GAIN, volL / 32767.0f);///@FIXME only left supported?
#elif defined(XAUDIO2)
   pMasterVoice->SetVolume(volL / 32767.0f, 0);
#endif
}

long SpuClearReverbWorkArea(long mode)
{
    UNIMPLEMENTED();
    return 0;
}