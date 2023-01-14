#include "Game/CORE.H"
#include "AADLIB.H"
#include "Game/SOUND.H"
#include "Game/LOAD3D.H"
#include "AADSEQEV.H"
#include "AADSFX.H"
#include "AADLIB.H"

unsigned long aadReverbModeSize[10] = { 0x00000000, 0x000026C0, 0x00001F40, 0x00004840, 0x00006FE0, 0x00000ADE0, 0x0000F6C0, 0x00018040, 0x00018040, 0x00003C00 };
unsigned short aadHblanksPerUpdate[4] = { 0x0106, 0x0083, 0x0138, 0x009C };
struct AadMemoryStruct* aadMem; // offset 0x800CECD8
unsigned long aadGp;
int gDefragRequest;
unsigned char* smfDataPtr;
unsigned long smfBytesLeft;
struct AadDynamicSfxLoadInfo* smfInfo;
unsigned long __hblankEvent;
unsigned long aadUpdateRate[4] = { 0x411AAAAB, 0x208D5555, 0x4E200000, 0x27100000 };

#include <stddef.h>

unsigned long aadGetMemorySize(struct AadInitAttr *attributes)
{
#if defined(PC_VERSION)
	return 1488 * attributes->numSlots + 23720;
#elif defined(PSX_VERSION)
	return 1488 * attributes->numSlots + 7304;
#endif
}

int aadInit(struct AadInitAttr *attributes, unsigned char *memoryPtr)
{
	struct _AadSequenceSlot* slot;
	unsigned long size;
	int slotNumber;
	int i;

#ifndef PC_VERSION
	aadGp = GetGp();
#endif
	
	PSX_EnterCriticalSection();
	size = aadGetMemorySize(attributes);
	aadMem = (struct AadMemoryStruct*)memoryPtr;
	
	if (aadMem == NULL)
	{
		return 0x1009;
	}
	else
	{
		memset(memoryPtr, 0, size);

		if (attributes->nonBlockLoadProc == NULL ||
			attributes->nonBlockBufferedLoadProc == NULL ||
			attributes->memoryMallocProc == NULL)
		{
			return 0x1008;
		}

		aadMem->nonBlockLoadProc = attributes->nonBlockLoadProc;
		aadMem->nonBlockBufferedLoadProc = attributes->nonBlockBufferedLoadProc;
		aadMem->memoryMallocProc = attributes->memoryMallocProc;
		aadMem->memoryFreeProc = attributes->memoryFreeProc;
		
		memset(aadMem->sfxToneMasterList, -1, sizeof(aadMem->sfxToneMasterList));
		memset(aadMem->sfxWaveMasterList, -1, sizeof(aadMem->sfxWaveMasterList));
		
		aadMem->sramDescriptorTbl[0].prevIndex = 255;
		aadMem->nextSramDescIndex = 1;
		aadMem->sramDescriptorTbl[0].waveID = 32768;
		aadMem->sramDescriptorTbl[0].address = 514;
		aadMem->firstSramBlockDescIndex = 0;
		aadMem->sramDescriptorTbl[0].size = 37336;
		aadMem->sramDescriptorTbl[0].nextIndex = 255;

		aadPurgeLoadQueue();

		SpuInit();
		SpuSetCommonMasterVolume(0, 0);

		if (attributes->numSlots > 0)
		{
			for (slotNumber = 0; slotNumber < attributes->numSlots; slotNumber++)
			{
				slot = (struct _AadSequenceSlot*)(aadMem + 1) + slotNumber;

				aadMem->sequenceSlots[slotNumber] = slot;
				aadMem->sequenceSlots[slotNumber]->thisSlotNumber = slotNumber;
				aadMem->sequenceSlots[slotNumber]->sequenceNumberAssigned = 255;
				aadMem->sequenceSlots[slotNumber]->slotID = slotNumber << 4;
				aadMem->sequenceSlots[slotNumber]->slotVolume = 127;
				aadMem->sequenceSlots[slotNumber]->slotPan = 63;
				aadMem->sequenceSlots[slotNumber]->masterVolPtr = &aadMem->musicMasterVol;
				
				for (i = 0; i < 16; i++)
				{
					slot->currentProgram[i] = 255;
					slot->volume[i] = 127;
					slot->panPosition[i] = 63;
					aadMem->sequenceSlots[slotNumber]->pitchWheel[i] = 8192;
				}
				
				slot->selectedSlotPtr = slot;
				slot->selectedSlotNum = slotNumber;
			}
		}

		aadMem->sfxSlot.handleCounter = 12345;
		aadMem->sfxSlot.sfxVolume = 127;
		aadMem->numSlots = attributes->numSlots & 0xFF;
		aadMem->sfxMasterVol = 127;
		aadMem->musicMasterVol = 127;
		aadMem->endSequenceCallback = NULL;
		aadMem->controller11Callback = NULL;
		aadMem->updateMode = attributes->updateMode & 0xFF;

		for (slotNumber = 0; slotNumber < 24; slotNumber++)
		{
			aadMem->synthVoice[slotNumber].voiceMask = 1 << slotNumber;
			aadMem->synthVoice[slotNumber].voiceID = 255;
			aadMem->synthVoice[slotNumber].voiceNum = slotNumber;
		}

		aadMem->voiceKeyOffRequest = 0;
		aadMem->voiceKeyOnRequest = 0;
		aadMem->voiceReverbRequest = 0;

		if (aadMem->updateMode < 4)
		{
			aadInstallUpdateFunc(aadSlotUpdateWrapper, aadHblanksPerUpdate[aadMem->updateMode]);
		}

		aadMem->flags = 0;
		PSX_ExitCriticalSection();
	}

	return 0;
}

void aadInstallUpdateFunc(TDRFuncPtr_aadInstallUpdateFunc0updateFuncPtr updateFuncPtr, int hblanksPerUpdate)
{ 
	EnterCriticalSection();
	
	__hblankEvent = OpenEvent(0xF2000001, 2, 0x1000, updateFuncPtr);
	
	EnableEvent(__hblankEvent);
	
	SetRCnt(0xF2000001, hblanksPerUpdate, 0x1000);
	
	StartRCnt(0xF2000001);
	
	ExitCriticalSection();
}

void aadInitVolume()
{ 
	aadMem->masterVolume = 0;
	SpuSetCommonCDMix(0);
	SpuSetCommonMasterVolume(0, 0);
}

void aadSetMasterVolume(int volume)
{
	aadMem->masterVolume = volume;
	SpuSetCommonMasterVolume(volume, volume);
}

void aadStartMasterVolumeFade(int targetVolume, int volumeStep, TDRFuncPtr_aadStartMasterVolumeFade2fadeCompleteCallback fadeCompleteCallback)
{
#if defined(PSX_VERSION)
	aadMem->masterVolFader.volumeStep = volumeStep;
	aadMem->masterVolFader.targetVolume = targetVolume;
	aadMem->masterVolFader.fadeCompleteCallback = fadeCompleteCallback;
#elif defined(PC_VERSION)
	aadMem->masterVolFader.volumeStep = volumeStep;
	aadMem->masterVolFader.targetVolume = targetVolume;
	aadMem->masterVolFader.fadeCompleteCallback = fadeCompleteCallback;
#endif
}

void aadSetSfxMasterVolume(int volume)
{
	aadMem->sfxMasterVol = volume;
}

void aadSetMusicMasterVolume(int volume)
{
	int slotNumber;
	
	aadMem->musicMasterVol = volume;

	if (aadMem->numSlots > 0)
	{
		for (slotNumber = 0; slotNumber < aadMem->numSlots; slotNumber++)
		{
			aadUpdateSlotVolPan(aadMem->sequenceSlots[slotNumber]);
		}
	}
}

void aadStartMusicMasterVolFade(int targetVolume, int volumeStep, TDRFuncPtr_aadStartMusicMasterVolFade2fadeCompleteCallback fadeCompleteCallback)
{
	aadMem->musicMasterVolFader.volumeStep = volumeStep;
	aadMem->musicMasterVolFader.targetVolume = targetVolume;
	aadMem->musicMasterVolFader.fadeCompleteCallback = fadeCompleteCallback;
}

// autogenerated function stub: 
// void /*$ra*/ aadShutdown()
void aadShutdown()
{ // line 285, offset 0x80051924
	/* begin block 1 */
		// Start line: 570
	/* end block 1 */
	// End Line: 571
	UNIMPLEMENTED();
}

long aadSlotUpdateWrapper()
{ 
	unsigned long curGp; // $s0

	curGp = GetGp();
	
#if !defined(PSXPC_VERSION) && defined(PSX_VERSION)
	int forceCompilerErrorUntilThisIsUncommented.
#endif
	//SetGp(aadGp);

	aadSlotUpdate();

	//SetGp(curGp);

	return 0;
}


void aadSlotUpdate()//Matching - 97.37%
{
	struct _AadSequenceSlot* slot;
	struct AadSeqEvent* seqEventPtr;
	int slotNumber;
	int i;
	int fadeComplete;
	int track;
	int newVol;
	int slotDone;
	unsigned long vmask;

	if (aadMem != NULL)
	{
		if (!(aadMem->flags & 0x2))
		{
			SpuGetAllKeysStatus(aadMem->voiceStatus);

			for (i = 0, vmask = 1; i < 24; i++, vmask <<= 1)
			{
				if (aadMem->voiceStatus[i] == 3)
				{
					aadMem->voiceKeyOffRequest |= vmask;
				}
				else
				{
					if ((aadMem->voiceKeyOffRequest & vmask) != 0)
					{
						if (aadMem->voiceStatus[i] != 0)
						{
							if (aadMem->voiceStatus[i] == 2)
							{
								aadMem->voiceKeyOffRequest &= ~vmask;
							}
						}
						else
						{
							aadMem->voiceKeyOffRequest &= ~vmask;
						}
					}
				}
			}

			if (!(aadMem->flags & 0x4))
			{
				for (slotNumber = 0; slotNumber < aadMem->numSlots; slotNumber++)
				{
					slot = aadMem->sequenceSlots[slotNumber];

					if ((slot->status & 0x1) && !(slot->slotFlags & 0x1))
					{
						slot->tempo.currentTick += slot->tempo.ticksPerUpdate;
						slot->tempo.currentError += slot->tempo.errorPerUpdate;

						if (slot->tempo.currentError >= slot->tempo.tickTimeFixed)
						{
							slot->tempo.currentError -= slot->tempo.tickTimeFixed;
							slot->tempo.currentTick++;
						}

						do
						{
#if defined(AKUJI)
							for (track = 0; track < 1; track++)
#else
							for (track = 0; track < 16; track++)
#endif
							{
								if (slot->sequencePosition[track] != NULL)
								{
									while (slot->eventsInQueue[track] < 3)
									{
										if (aadQueueNextEvent(slot, track) != 0)
										{
											break;
										}
									}
								}
								else
								{
									break;
								}
							}

							slotDone = 1;

#if defined(AKUJI)
							for (track = 0; track < 1; track++)
#else
							for (track = 0; track < 16; track++)
#endif
							{
								if (slot->sequencePosition[track] != NULL)
								{
									while (slot->eventsInQueue[track] != 0)
									{
										seqEventPtr = &slot->eventQueue[slot->eventOut[track]][track];

										if (slot->tempo.currentTick >= seqEventPtr->deltaTime + slot->lastEventExecutedTime[track])
										{
											slot->lastEventExecutedTime[track] += seqEventPtr->deltaTime;
											slot->eventsInQueue[track]--;
											slot->eventOut[track]++;

											if (slot->eventOut[track] == 4)
											{
												slot->eventOut[track] = 0;
											}

											aadExecuteEvent(seqEventPtr, slot);

											slotDone = 0;
										}
										else
										{
											break;
										}
									}
								}
								else
								{
									break;
								}
							}
						} while (slotDone == 0);
					}
				}
			}

			newVol = 32;

			while (aadMem->sfxSlot.commandsInQueue != 0)
			{
				aadExecuteSfxCommand(&aadMem->sfxSlot.commandQueue[aadMem->sfxSlot.commandOut]);

				aadMem->sfxSlot.commandsInQueue--;

				if (++aadMem->sfxSlot.commandOut == newVol)
				{
					aadMem->sfxSlot.commandOut = 0;
				}
			}

			aadMem->voiceKeyOffRequest = aadMem->voiceKeyOffRequest & ~aadMem->voiceKeyOnRequest;

			if (aadMem->voiceKeyOffRequest != 0)
			{
				SpuSetKey(0, aadMem->voiceKeyOffRequest);
			}

			SpuSetReverbVoice(1, aadMem->voiceReverbRequest);
			SpuSetReverbVoice(0, ~aadMem->voiceReverbRequest);

			if (aadMem->voiceKeyOnRequest != 0)
			{
				SpuSetKey(1, aadMem->voiceKeyOnRequest);
				aadMem->voiceKeyOnRequest = 0;
			}
		}

		fadeComplete = 0;

		if (aadMem->masterVolFader.volumeStep != 0)
		{
			newVol = aadMem->masterVolume;

			newVol += aadMem->masterVolFader.volumeStep;

			if (aadMem->masterVolFader.volumeStep < 0)
			{
				fadeComplete = newVol < aadMem->masterVolFader.targetVolume;
			}
			else
			{
				if (aadMem->masterVolFader.targetVolume < newVol)
				{
					fadeComplete = 1;
				}
			}

			if (fadeComplete != 0)
			{
				aadMem->masterVolFader.targetVolume = newVol;

				aadMem->masterVolFader.volumeStep = 0;

				if (aadMem->masterVolFader.fadeCompleteCallback != NULL)
				{
					aadMem->masterVolFader.fadeCompleteCallback();
				}
			}

			aadSetMasterVolume(newVol);
		}

		if (aadMem->musicMasterVolFader.volumeStep != 0)
		{
			fadeComplete = 0;

			if (!(aadMem->updateCounter & 0x1))
			{
				newVol = aadMem->musicMasterVol;
				newVol += aadMem->musicMasterVolFader.volumeStep;

				if (aadMem->musicMasterVolFader.volumeStep < 0)
				{
					fadeComplete = (aadMem->musicMasterVolFader.targetVolume < newVol) ^ 1;
				}
				else
				{
					if (newVol >= aadMem->musicMasterVolFader.targetVolume)
					{
						fadeComplete = 1;
					}
				}

				if (fadeComplete != 0)
				{
					newVol = aadMem->musicMasterVolFader.targetVolume;

					aadMem->musicMasterVolFader.volumeStep = 0;

					if (aadMem->musicMasterVolFader.fadeCompleteCallback != NULL)
					{
						aadMem->musicMasterVolFader.fadeCompleteCallback();
					}
				}

				aadSetMusicMasterVolume(newVol);
			}
		}

		aadMem->updateCounter++;
	}
}


unsigned long aadCreateFourCharID(char a, char b, char c, char d)
{ 
	return  d | (c << 8) | (b << 16) | (a << 24);
}

int aadLoadDynamicSoundBank(char *sndFileName, char *smpFileName, int dynamicBankIndex, int loadOption, void (*func)(int, int))
{
	int i;
	struct AadDynamicBankLoadInfo* info;
	
	i = 0;
	info = &aadMem->dynamicBankLoadInfo;

	while (i < 2)
	{
		if (aadMem->dynamicBankStatus[i] == 1)
		{
			return 0x1006;
		}

		i++;

		if (i >= 2)
		{
			if (dynamicBankIndex >= 2)
			{
				return 0x1005;
			}
		}
	}

	if (aadMem->dynamicBankStatus[dynamicBankIndex] == 2)
	{
		if (aadMem->dynamicSoundBankData[dynamicBankIndex] != NULL)
		{
			aadFreeDynamicSoundBank(dynamicBankIndex);
		}
	}
	
	aadMem->dynamicBankStatus[dynamicBankIndex] = 1;

	strncpy(info->sndFileName, sndFileName, sizeof(info->sndFileName) - 1);
	strncpy(info->smpFileName, smpFileName, sizeof(info->smpFileName) - 1);

	info->dynamicBankIndex = dynamicBankIndex;
	info->loadOption = loadOption;
	info->flags = 0;
	info->userCallbackProc = func;

	aadMem->nonBlockLoadProc(sndFileName, (void*)&aadLoadDynamicSoundBankReturn, info, NULL, (void**)&aadMem->dynamicSoundBankData[0], 4);
	
	return 0;
}

void aadLoadDynamicSoundBankReturn(void *loadedDataPtr, void *data, void *data2)
{
	int dynamicBankIndex;
	int error;

	dynamicBankIndex = ((struct AadDynamicBankLoadInfo*)data)->dynamicBankIndex;

	error = aadOpenDynamicSoundBank((unsigned char*)loadedDataPtr, dynamicBankIndex);

	if (error != 0)
	{
		aadMem->dynamicBankStatus[dynamicBankIndex] = (error | 0x80) & 0xFF;

		if (aadMem->dynamicSoundBankData[dynamicBankIndex] != NULL)
		{
			aadMem->memoryFreeProc((char*)aadMem->dynamicSoundBankData[dynamicBankIndex]);

			aadMem->dynamicSoundBankData[dynamicBankIndex] = NULL;
		}

		if (((struct AadDynamicBankLoadInfo*)data)->userCallbackProc != NULL)
		{
			((struct AadDynamicBankLoadInfo*)data)->userCallbackProc(dynamicBankIndex & 0xFFFF, error);
		}
	}
	else
	{
		aadMem->nonBlockBufferedLoadProc(&((struct AadDynamicBankLoadInfo*)data)->smpFileName[0], (void*)&aadLoadDynamicSoundBankReturn2, data, NULL);
	}
}

void aadLoadDynamicSoundBankReturn2(void* loadedDataPtr, long loadedDataSize, short status, void* data1, void* data2)//Matching - 96.94%
{
	unsigned char* dataPtr;
	struct AadDynamicBankLoadInfo* info;
	int dynamicBankIndex;
	int error;
	int i;

	info = (struct AadDynamicBankLoadInfo*)data1;
	error = status < 256;
	dynamicBankIndex = info->dynamicBankIndex;

	if (error == 0)
	{
		if (!(info->flags & 0x2))
		{
			info->flags |= 0x2;

			error = (status >> 8) | 0x80;

			aadMem->dynamicBankStatus[dynamicBankIndex] = error;

			if (aadMem->dynamicSoundBankData[dynamicBankIndex] != NULL)
			{
				aadMem->memoryFreeProc((char*)aadMem->dynamicSoundBankData[dynamicBankIndex]);
				aadMem->dynamicSoundBankData[dynamicBankIndex] = NULL;
			}

			if (info->userCallbackProc != NULL)
			{
				info->userCallbackProc(dynamicBankIndex, status);
			}
		}
	}
	else
	{
		if (!(info->flags & 0x1))
		{
			dataPtr = (unsigned char*)loadedDataPtr;

			dataPtr = dataPtr + 4;

			info->flags |= 0x1;

			info->sramDataSize = ((unsigned long*)dataPtr)[0];

			dataPtr += 4;

			loadedDataSize -= 8;

			if (info->loadOption == 0 || info->sramDataSize != 1)
			{
#if defined(AKUJI)
				info->nextSramAddr = 0;
#else
				info->nextSramAddr = 302800;
#endif
			}
			else
			{
				info->nextSramAddr = (524288 - aadGetReverbSize() - info->sramDataSize);
			}


			aadMem->dynamicSoundBankSramData[dynamicBankIndex] = info->nextSramAddr;
		}

		aadWaitForSramTransferComplete();

		SpuSetTransferStartAddr(info->nextSramAddr);

		SpuWrite(dataPtr, loadedDataSize);

		info->nextSramAddr += loadedDataSize;

		if (status == 1)
		{
			for (i = 0; i < aadMem->dynamicSoundBankHdr[dynamicBankIndex]->numWaves; i++)
			{
				aadMem->dynamicWaveAddr[dynamicBankIndex][i] += aadMem->dynamicSoundBankSramData[dynamicBankIndex];
			}

			aadMem->dynamicBankStatus[dynamicBankIndex] = 2;

			if (info->userCallbackProc != NULL)
			{
				info->userCallbackProc(dynamicBankIndex & 0xFFFF, 0);
			}
		}
	}
}

int aadFreeDynamicSoundBank(int dynamicBankIndex)
{ 
	if (dynamicBankIndex < 2)
	{
		return 0x1005;
	}
	
	if (aadMem->dynamicBankStatus[dynamicBankIndex] != 2)
	{
		return 0x1007;
	}

	if (aadMem->dynamicSoundBankData[dynamicBankIndex] == NULL)
	{
		return 0x1007;
	}

	aadMem->dynamicBankStatus[dynamicBankIndex] = 0;
	aadMem->memoryFreeProc((char*)aadMem->dynamicSoundBankData[dynamicBankIndex]);
	aadMem->dynamicSoundBankData[dynamicBankIndex] = NULL;

	return 0;
}

int aadOpenDynamicSoundBank(unsigned char *soundBank, int dynamicBankIndex)
{
	struct AadSoundBankHdr *soundBankHdr;
	struct AadProgramAtr *programAtr;
	struct AadToneAtr *toneAtr;
	unsigned long *waveAddr;
	unsigned long *sequenceOffsetTbl;
	unsigned long *sequenceLabelOffsetTbl;
	unsigned char *sequenceBase;
	int i;

	if (dynamicBankIndex >= 2)
	{
		return 0x1005;
	}

	soundBankHdr = (struct AadSoundBankHdr*)soundBank;

	if (soundBankHdr->bankID != aadCreateFourCharID('a', 'S', 'N', 'D'))
	{
		return 0x1001;
	}

#if !defined(AKUJI)
	if (soundBankHdr->bankVersion != 262)
	{
		return 0x1002;
	}
#endif

#if defined(AKUJI)
	programAtr = (struct AadProgramAtr*)(soundBankHdr + 1);
#else
	programAtr = (struct AadProgramAtr*)((char*)soundBankHdr + soundBankHdr->headerSize);
#endif

	aadMem->dynamicProgramAtr[dynamicBankIndex] = programAtr;

	toneAtr = (struct AadToneAtr*)((char*)programAtr + soundBankHdr->numPrograms * sizeof(struct AadProgramAtr));
	aadMem->dynamicToneAtr[dynamicBankIndex] = toneAtr;

	aadMem->dynamicSoundBankHdr[dynamicBankIndex] = soundBankHdr;

	waveAddr = (unsigned long*)((char*)toneAtr + soundBankHdr->numTones * sizeof(struct AadToneAtr));
	aadMem->dynamicWaveAddr[dynamicBankIndex] = waveAddr;

	sequenceOffsetTbl = (unsigned long*)((char*)waveAddr + soundBankHdr->numWaves * sizeof(unsigned int));
	aadMem->dynamicSequenceAddressTbl[dynamicBankIndex] = (unsigned char* PTR_32* PTR_32)sequenceOffsetTbl;

	sequenceLabelOffsetTbl = (unsigned long*)((char*)sequenceOffsetTbl + soundBankHdr->numSequences * sizeof(unsigned int));
	aadMem->dynamicSequenceLabelOffsetTbl[dynamicBankIndex] = sequenceLabelOffsetTbl;

	sequenceBase = (unsigned char*)((char*)sequenceLabelOffsetTbl + soundBankHdr->numLabels * sizeof(unsigned int));

	if (soundBankHdr->numSequences > 0)
	{
		for (i = 0; i < soundBankHdr->numSequences; i++)
		{
			aadMem->dynamicSequenceAddressTbl[dynamicBankIndex][i] = &sequenceBase[sequenceOffsetTbl[i]];
		}
	}

	return 0;
}

int aadLoadDynamicSfx(char *fileName, long directoryID, long flags)//Matching - 99.78%
{ 
	struct AadDynamicLoadRequest* loadReq;

	if (aadMem->numLoadReqsQueued < 16)
	{
		loadReq = &aadMem->loadRequestQueue[aadMem->nextLoadReqIn];
		loadReq->type = 0;
		loadReq->handle = (aadMem->nextFileHandle++ & 0x3FFF) | 0x4000;
		loadReq->directoryID = directoryID;
		loadReq->flags = flags;

		strncpy(loadReq->fileName, fileName, sizeof(loadReq->fileName) - 1);

		aadMem->nextLoadReqIn = (++aadMem->nextLoadReqIn & 0xF);
		aadMem->numLoadReqsQueued++;

		return loadReq->handle;
	}

	return 0;
}

int aadFreeDynamicSfx(int handle)//Matching - 99.73%
{
	struct AadDynamicLoadRequest* loadReq;
	int i;

	i = aadMem->nextLoadReqOut;

	while (i != aadMem->nextLoadReqIn)
	{
		loadReq = &aadMem->loadRequestQueue[i];

		if (loadReq->type == 0 && loadReq->handle == handle)
		{
			loadReq->type = 2;
			return 0;
		}

		i = (i + 1) & 0xF;
	}

	if (aadMem->numLoadReqsQueued >= 16)
	{
		return 0x100F;
	}
	else
	{
		loadReq = &aadMem->loadRequestQueue[((aadMem->nextLoadReqOut - 1) & 0xF)];

		aadMem->nextLoadReqOut = ((aadMem->nextLoadReqOut - 1) & 0xF);

		loadReq->type = 1;
		loadReq->handle = handle;

		aadMem->numLoadReqsQueued++;
	}

	return 0;
}

void aadRelocateMusicMemoryBegin()
{ 
	aadMem->flags |= 0x2;
}

void aadRelocateMusicMemoryEnd(struct MemHeader* newAddress, long offset)
{
	int bank;
	int slotNumber;
	int i;
	struct AadSoundBankHdr* bankHdr;
	struct _AadSequenceSlot* slot;
	int track;

	for (bank = 0; bank < 2; bank++)
	{
		if ((unsigned char*)newAddress == aadMem->dynamicSoundBankData[bank])
		{
			bankHdr = aadMem->dynamicSoundBankHdr[bank];

			aadMem->dynamicSoundBankData[bank] = ((unsigned char*)newAddress) + offset;

			aadMem->dynamicProgramAtr[bank] = (struct AadProgramAtr*)((unsigned char*)aadMem->dynamicProgramAtr[bank]) + offset;

			aadMem->dynamicSoundBankHdr[bank] = (struct AadSoundBankHdr*)((unsigned char*)bank) + offset;

			aadMem->dynamicToneAtr[bank] = (struct AadToneAtr*)((unsigned char*)aadMem->dynamicToneAtr[bank]) + offset;

			aadMem->dynamicWaveAddr[bank] = (unsigned long*)((unsigned char*)aadMem->dynamicWaveAddr[bank]) + offset;

			aadMem->dynamicSequenceAddressTbl[bank] = (unsigned char**)((unsigned char*)aadMem->dynamicSequenceAddressTbl[bank]) + offset;

			for (i = 0; i < bankHdr->numSequences; i++)
			{
				aadMem->dynamicSequenceAddressTbl[i][bank] += offset;
			}

			aadMem->dynamicSequenceLabelOffsetTbl[0] = (unsigned long*)((unsigned char*)aadMem->dynamicSequenceLabelOffsetTbl[bank]) + offset;

			for (slotNumber = 0; slotNumber < aadMem->numSlots; slotNumber++)
			{
				slot = aadMem->sequenceSlots[slotNumber];

				if (slot->sequenceNumberAssigned != 255 && slot->sequenceAssignedDynamicBank == bank)
				{
					for (track = 0; track < 16; track++)
					{
						if (slot->sequencePosition[track] != 0)
						{
							slot->sequencePosition[track] += offset;

							if (slot->loopCurrentNestLevel[track] != 0)
							{
								for (i = 0; i < slot->loopCurrentNestLevel[track]; i++)
								{
									slot->loopSequencePosition[i][track] += offset;
								}
							}
						}
					}
				}
			}
		}
	}

	aadMem->flags &= 0xFFFFFFFD;
}

void aadRelocateSfxMemory(void *oldAddress, int offset)
{
	struct _AadDynSfxFileHdr* snfFile;

	snfFile = aadMem->firstDynSfxFile;
	
	if ((char*)oldAddress == (char*)snfFile)
	{
		snfFile = (struct _AadDynSfxFileHdr*)((char*)snfFile + offset);
		aadMem->firstDynSfxFile = snfFile;
	}

	while (snfFile != NULL)
	{
		if ((char*)oldAddress == (char*)snfFile->prevDynSfxFile)
		{
			snfFile->prevDynSfxFile = (struct _AadDynSfxFileHdr*)((char*)oldAddress + offset);
		}

		if ((char*)oldAddress == (char*)snfFile->nextDynSfxFile)
		{
			snfFile->nextDynSfxFile = (struct _AadDynSfxFileHdr*)((char*)oldAddress + offset);
		}

		snfFile = snfFile->nextDynSfxFile;
	}
}

int aadGetNumLoadsQueued()
{
	return aadMem->numLoadReqsQueued;
}

void aadPurgeLoadQueue()
{
	aadMem->nextLoadReqIn = 0;
	aadMem->nextLoadReqOut = 0;
	aadMem->numLoadReqsQueued = 0;
}

void aadProcessLoadQueue()//Matching - 78.45%
{
	struct AadDynamicSfxLoadInfo* info;
	int i;
	char* p;
	struct AadDynamicLoadRequest* loadReq;
	char areaName[12];
	struct _AadDynSfxFileHdr* snfFile;
	unsigned short* sfxIDListPtr;

	info = &aadMem->dynamicSfxLoadInfo;

	if (!(info->flags & 0x1))
	{
		for (i = 0; i < 2; i++)
		{
			if (aadMem->dynamicBankStatus[i] == 1)
			{
				return;
			}
		}

		if (aadMem->numLoadReqsQueued != 0 && aadMem->sramDefragInfo.status == 0 && gDefragRequest == 0)
		{
			loadReq = &aadMem->loadRequestQueue[aadMem->nextLoadReqOut];

			aadMem->nextLoadReqOut = (aadMem->nextLoadReqOut + 1) & 0xF;

			aadMem->numLoadReqsQueued--;

			switch (loadReq->type)
			{
			case 0:

				strcpy(areaName, loadReq->fileName);

				p = strpbrk(areaName, "0123456789");

				if (p != NULL)
				{
					p[0] = 0;
				}

				if ((loadReq->flags & 0x1))
				{
					sprintf(info->snfFileName, "\\kain2\\area\\%s\\bin\\%s.snf", areaName, loadReq->fileName);
					sprintf(info->smfFileName, "\\kain2\\area\\%s\\bin\\%s.smf", areaName, loadReq->fileName);
				}
				else
				{
					sprintf(info->snfFileName, "\\kain2\\sfx\\object\\%s\\%s.snf", loadReq->fileName, loadReq->fileName);
					sprintf(info->smfFileName, "\\kain2\\sfx\\object\\%s\\%s.smf", loadReq->fileName, loadReq->fileName);
				}

				info->fileHandle = loadReq->handle;

				gSramFullAlarm = 0;

				info->directoryID = loadReq->directoryID;
				info->flags = 1;
				info->snfFile = NULL;
				info->error = 0;
				info->totalSramUsed = 0;
				info->loadFlags = loadReq->flags;


				if (loadReq->directoryID != 0)
				{
					LOAD_SetSearchDirectory(loadReq->directoryID);
				}

				aadMem->nonBlockLoadProc(info->snfFileName, (void*)aadLoadDynamicSfxReturn, info, NULL, (void**)&info->snfFile, 0x2F);

				if (info->directoryID != 0)
				{
					LOAD_SetSearchDirectory(0);
				}
				break;
			case 1:
				snfFile = aadMem->firstDynSfxFile;

				while (snfFile != NULL)
				{
					if (snfFile->handle == loadReq->handle)
					{
						sfxIDListPtr = (unsigned short*)(snfFile + 1);

						for (i = 0; i < snfFile->numSfxInFile; i++)
						{
							aadFreeSingleDynSfx(*sfxIDListPtr++);
						}

						if (snfFile->prevDynSfxFile != NULL)
						{
							snfFile->prevDynSfxFile->nextDynSfxFile = snfFile->nextDynSfxFile;
						}
						else
						{
							aadMem->firstDynSfxFile = snfFile->nextDynSfxFile;
						}

						if (snfFile->nextDynSfxFile != NULL)
						{
							snfFile->nextDynSfxFile->prevDynSfxFile = snfFile->prevDynSfxFile;
						}

						aadMem->memoryFreeProc((char*)snfFile);

						gSramFullAlarm = 0;

						break;
					}

					snfFile = snfFile->nextDynSfxFile;
				}

				if (aadCheckSramFragmented() != 0)
				{
					gDefragRequest = 1;
				}
				break;

			}
		}

		if (gDefragRequest != 0 && SOUND_IsMusicLoading() == 0)
		{
			aadMem->numSlots = 0;
			aadMem->sramDefragInfo.status = 1;
		}

		aadProcessSramDefrag();
	}
}

void aadLoadDynamicSfxAbort(struct AadDynamicSfxLoadInfo *info, int error)
{ 
	if (info->snfFile != NULL)
	{
		if((info->flags & 0x2))
		{
			if (info->snfFile->prevDynSfxFile != NULL)
			{
				info->snfFile->prevDynSfxFile->nextDynSfxFile = NULL;
			}
			else
			{
				aadMem->firstDynSfxFile = NULL;
			}
		}

		aadMem->memoryFreeProc((char*)info->snfFile);
	}

	info->flags = 0;
}

void aadLoadDynamicSfxDone(struct AadDynamicSfxLoadInfo *info)
{ 
	info->flags = 0;
}

void aadLoadDynamicSfxReturn(void* loadedDataPtr, void* data, void* data2)
{
	struct _AadDynSfxFileHdr* p;
	struct AadDynamicSfxLoadInfo* info;

	info = (struct AadDynamicSfxLoadInfo*)data;

	if (info->snfFile == NULL || info->snfFile != loadedDataPtr)
	{
		aadLoadDynamicSfxAbort(info, 0x100E);
	}
	else
	{
		if (info->snfFile->snfID == aadCreateFourCharID('a', 'S', 'N', 'F'))
		{
			if (info->snfFile->snfVersion != 256)
			{
				aadLoadDynamicSfxAbort(info, 0x100B);
			}
			else
			{
				info->snfFile->handle = info->fileHandle;
				
				p = aadMem->firstDynSfxFile;
				
				if (p != NULL)
				{
					if (aadMem->firstDynSfxFile->nextDynSfxFile != NULL)
					{
						do
						{
							p = p->nextDynSfxFile;

						} while (p->nextDynSfxFile != NULL);
					}

					p->nextDynSfxFile = info->snfFile;
					info->snfFile->prevDynSfxFile = p;
				}
				else
				{
					aadMem->firstDynSfxFile = info->snfFile;
					info->snfFile->prevDynSfxFile = NULL;
				}

				info->snfFile->nextDynSfxFile = NULL;
				info->smfLoadingState = 0;
				info->flags |= 0x2;

				if (info->directoryID != 0)
				{
					LOAD_SetSearchDirectory(info->directoryID);
				}

				aadMem->nonBlockBufferedLoadProc(info->smfFileName, (void*)aadLoadDynamicSfxReturn2, info, NULL);

				if (info->directoryID != 0)
				{
					LOAD_SetSearchDirectory(0);
				}
			}
		}
	}
}

int aadWaveMalloc(unsigned short waveID, unsigned long waveSize) //Matching - 98.05%
{
	struct AadNewSramBlockDesc* sramDesc;
	struct AadNewSramBlockDesc* bestFit;
	struct AadNewSramBlockDesc* next;
	struct AadNewSramBlockDesc* sramDescTbl;
	unsigned long safeWaveSize;
	int i;
	int sramDescIndex;
	int bestFitIndex;

	waveSize >>= 3;
	safeWaveSize = (waveSize & 0xFFFFFFF8);

	if ((waveSize & 0x7))
	{
		safeWaveSize += 8;
	}

	bestFit = NULL;
	bestFitIndex = 255;
	sramDescIndex = aadMem->firstSramBlockDescIndex;
	sramDescTbl = &aadMem->sramDescriptorTbl[0];
	next = sramDescTbl + sramDescIndex;

	for (i = 128; next != NULL;)
	{
		if (--i == -1)
		{
			i++;
			break;
		}

		if (!(next->waveID & 0x4000) && next->size >= safeWaveSize)
		{
			if (bestFit == NULL || next->size < bestFit->size)
			{
				bestFitIndex = sramDescIndex;
				bestFit = next;
			}
		}

		if ((char)next->nextIndex >= 0)
		{
			if (next->nextIndex != sramDescIndex)
			{
				sramDescIndex = next->nextIndex;

				next = sramDescTbl + sramDescIndex;
			}
			else
			{
				next = NULL;
			}
		}
		else
		{
			next = NULL;
		}
	}

	if (bestFit != NULL)
	{
		bestFit->waveID = waveID | 0xC000;

		if (waveSize < bestFit->size)
		{
			if ((char)bestFit->nextIndex >= 0)
			{
				next = sramDescTbl + bestFit->nextIndex;

				if (!(next->waveID & 0x4000))
				{
					next->address -= bestFit->size - waveSize;
					next->size += (bestFit->size - waveSize);

					bestFit->size = waveSize;

					return bestFitIndex;
				}
			}

			sramDescIndex = aadMem->nextSramDescIndex;

			next = sramDescTbl + sramDescIndex;


			if ((next->waveID & 0x8000))
			{
				for (i = sramDescIndex; (next->waveID & 0x8000) != 0; i++)
				{
					i = i + 1 & 0x7F;

					if (i == sramDescIndex)
					{
						return 255;
					}
					else
					{
						sramDescIndex = i;
					}

					next = sramDescTbl + sramDescIndex;
				}
			}

			aadMem->nextSramDescIndex = (aadMem->nextSramDescIndex + 8) & 0x7F;

			next->waveID = 0x8000;
			next->address = bestFit->address + waveSize;
			next->size = bestFit->size - waveSize;
			next->prevIndex = bestFitIndex;
			next->nextIndex = bestFit->nextIndex;

			if ((char)next->nextIndex >= 0)
			{
				(sramDescTbl + next->nextIndex)->prevIndex = sramDescIndex;
			}

			bestFit->size = waveSize;
			bestFit->nextIndex = sramDescIndex;
		}
		return bestFitIndex;
	}

	return 255;
}

unsigned long aadGetSramBlockAddr(int handle)
{
	struct AadNewSramBlockDesc* sramDesc;

	if (handle < 0x80)
	{
		return aadMem->sramDescriptorTbl[handle].address << 3;
	}

	return 0;
}

void aadWaveFree(int handle)//Matching - 99.93%
{
	struct AadNewSramBlockDesc* sramDesc;
	struct AadNewSramBlockDesc* sramDescTbl;
	struct AadNewSramBlockDesc* next;
	struct AadNewSramBlockDesc* prev;

	if (handle < 128)
	{
		sramDescTbl = &aadMem->sramDescriptorTbl[0];
		sramDesc = sramDescTbl + handle;
		sramDesc->waveID = 0x8000;

		if ((char)sramDesc->nextIndex >= 0)
		{
			next = sramDescTbl + sramDesc->nextIndex;

			if (!(next->waveID & 0x4000))
			{
				sramDesc->size += next->size;

				next->waveID = 0;

				sramDesc->nextIndex = next->nextIndex;

				if ((char)sramDesc->nextIndex << 24 >= 0)
				{
					(sramDescTbl + sramDesc->nextIndex)->prevIndex = handle;
				}
			}
		}

		if ((char)sramDesc->prevIndex >= 0)
		{
			prev = sramDescTbl + sramDesc->prevIndex;

			if (!(prev->waveID & 0x4000))
			{
				prev->size += sramDesc->size;

				sramDesc->waveID = 0;

				prev->nextIndex = sramDesc->nextIndex;

				if ((char)prev->nextIndex << 24 >= 0)
				{
					(sramDescTbl + sramDesc->nextIndex)->prevIndex = sramDesc->prevIndex;
				}
			}
		}
	}
}

void aadFreeSingleDynSfx(int sfxID)//Matching - 99.70%
{ 
	int ti;
	int wi;
	struct AadLoadedSfxToneAttr* toneAttr;
	struct AadLoadedSfxWaveAttr* waveAttr;
	
	ti = aadMem->sfxToneMasterList[sfxID];

	if (ti < 0xFE)
	{
		toneAttr = &aadMem->sfxToneAttrTbl[ti];

		if (--toneAttr->referenceCount == 0)
		{
			aadMem->sfxToneMasterList[sfxID] = 255;

			wi = aadMem->sfxWaveMasterList[toneAttr->waveID];

			if (wi < 0xFE)
			{
				waveAttr = &aadMem->sfxWaveAttrTbl[wi];
				if (--waveAttr->referenceCount == 0)
				{
					aadMem->sfxWaveMasterList[toneAttr->waveID] = 255;
					aadWaveFree(waveAttr->sramHandle);
				}
			}
		}
	}
}

void setSramFullAlarm()
{ 
	struct AadNewSramBlockDesc* sramDescTbl;
	struct AadNewSramBlockDesc* sramDesc;
	long totalUsed;
	long totalFree;
	long largestFree;
	long numFreeBlocks;
	long numUsedBlocks;
	int i;

	totalUsed = 0;
	totalFree = 0;
	largestFree = 0;
	numFreeBlocks = 0;
	numUsedBlocks = 0;

	sramDescTbl = aadMem->sramDescriptorTbl;

	sramDesc = sramDescTbl + aadMem->firstSramBlockDescIndex;

	i = 128;
	while (sramDesc != NULL)
	{
		if ((sramDesc->waveID & 0x4000))
		{
			numUsedBlocks++;
			totalUsed = sramDesc->size;
		}
		else
		{
			numFreeBlocks++;
			totalFree += sramDesc->size;
			
			if (largestFree < sramDesc->size)
			{
				largestFree = sramDesc->size;
			}
		}

		if (sramDesc->nextIndex >= 0)
		{
			sramDesc = sramDescTbl + sramDesc->nextIndex;
		}
		else
		{
			sramDesc = NULL;
		}

		if (--i == 0)
			break;
	}

	gSramFullAlarm = 1;
	gSramTotalUsed = totalUsed << 3;
	gSramTotalFree = totalFree << 3;
	gSramUsedBlocks = numUsedBlocks;
	gSramLargestFree = largestFree << 3;
	gSramFreeBlocks = numFreeBlocks;
}

void aadLoadSingleDynSfx(struct AadDynamicSfxLoadInfo* info)//Matching - 83.42%
{
	int i; // $a0
	struct AadLoadedSfxToneAttr* toneAttr; // $a1
	struct AadLoadedSfxWaveAttr* waveAttr; // $s0
	struct AadDynSfxAttr* attr; // $s1

	attr = &info->attr;
	info->waveTransferAddr = NULL;

	if (aadMem->sfxToneMasterList[info->attr.sfxID] == 0xFE)
	{
		aadMem->sfxToneMasterList[info->attr.sfxID] = 0xFF;
	}

	if (aadMem->sfxToneMasterList[info->attr.sfxID] != 0xFF)
	{
		toneAttr = &aadMem->sfxToneAttrTbl[aadMem->sfxToneMasterList[info->attr.sfxID]];

		toneAttr->referenceCount++;

		info->smfLoadingState = 2;

		return;
	}
	else
	{
		toneAttr = &aadMem->sfxToneAttrTbl[aadMem->nextToneIndex];
		
		for (i = aadMem->nextToneIndex; toneAttr->referenceCount != 0; toneAttr = &aadMem->sfxToneAttrTbl[(++i & 0x7F)])
		{
			if (((i + 1) & 0x7F) == aadMem->nextToneIndex)
			{
				info->smfLoadingState = 2;

				aadMem->sfxToneMasterList[i] = 0xFE;

				return;
			}
		}

		aadMem->nextToneIndex = (aadMem->nextToneIndex + 8) & 0x7F;

		toneAttr->referenceCount = 1;

		toneAttr->waveID = attr->waveID;

		toneAttr->toneAttr = attr->toneAttr;

		aadMem->sfxToneMasterList[attr->sfxID] = i;

		i = aadMem->sfxWaveMasterList[attr->waveID];

		if (i != 0xFF)
		{
			waveAttr = &aadMem->sfxWaveAttrTbl[i];

			waveAttr->referenceCount++;

			info->smfLoadingState = 2;

			return;
		}
		else
		{
			i = aadMem->nextWaveIndex;
			waveAttr = &aadMem->sfxWaveAttrTbl[i];
		}
	}

	goto waveAttr;

	for (; waveAttr->referenceCount != 0; )
	{
		if (i >= 0x78)
		{
			i = 0;
		}

		if (i == aadMem->nextWaveIndex)
		{
			aadFreeSingleDynSfx(attr->sfxID);

			setSramFullAlarm();

			info->smfLoadingState = 2;

			aadMem->sfxToneMasterList[attr->sfxID] = 0xFE;

			return;
		}

	waveAttr:
		waveAttr = &aadMem->sfxWaveAttrTbl[i++];
	}

	i--;

	aadMem->nextWaveIndex += 8;

	if (aadMem->nextWaveIndex >= 0x78)
	{
		aadMem->nextWaveIndex -= 0x78;
	}

	waveAttr->referenceCount = 1;

	aadMem->sfxWaveMasterList[attr->waveID] = i;

	waveAttr->sramHandle = aadWaveMalloc(attr->waveID, attr->waveSize);

	if (waveAttr->sramHandle << 24 >= 0)
	{
		info->waveTransferAddr = aadGetSramBlockAddr(waveAttr->sramHandle);

		info->smfLoadingState = 3;
	}
	else
	{
		aadFreeSingleDynSfx(attr->sfxID);

		setSramFullAlarm();

		info->smfLoadingState = 2;

		aadMem->sfxToneMasterList[attr->sfxID] = 0xFE;

	}
}

#if defined(PSXPC_VERSION)
int inCallback = FALSE;
#endif

void HackCallback()
{ 
	SpuSetTransferCallback(NULL);

	inCallback = TRUE;

	aadLoadDynamicSfxReturn2(smfDataPtr, smfBytesLeft, NULL, smfInfo, NULL);

	inCallback = FALSE;
}

void aadLoadDynamicSfxReturn2(void* loadedDataPtr, long loadedDataSize, short status, void* data1, void* data2)//Matching - 98.51%
{
	unsigned char* dataPtr;
	unsigned long dataOffset;
	unsigned long bytesRemaining;
	struct AadDynamicSfxLoadInfo* info;
	unsigned long n;

	info = (struct AadDynamicSfxLoadInfo*)data1;
	dataOffset = 0;
	bytesRemaining = loadedDataSize;
	dataPtr = (unsigned char*)loadedDataPtr;

	while (bytesRemaining != 0)
	{
		switch (info->smfLoadingState)
		{
		case 0:
			if (((struct _AadDynSfxFileHdr*)dataPtr)->snfID != aadCreateFourCharID('a', 'S', 'M', 'F'))
			{
				aadLoadDynamicSfxAbort(info, 0x100B);

				return;
			}
			else
			{
				if (((struct _AadDynSfxFileHdr*)dataPtr)->snfVersion != 0x100)
				{
					aadLoadDynamicSfxAbort(info, 0x100C);

					return;
				}
				else
				{
					if (((struct _AadDynSfxFileHdr*)dataPtr)->uniqueID != info->snfFile->uniqueID || ((struct _AadDynSfxFileHdr*)dataPtr)->handle != info->snfFile->numSfxInFile)
					{
						aadLoadDynamicSfxAbort(info, 0x100D);

						return;
					}
					else
					{
						dataOffset += 16;
						bytesRemaining -= 16;
						info->numSfxToLoad = info->snfFile->numSfxInFile;
						info->smfLoadingState = 1;
						info->bytesToLoad = 24;
					}
				}
			}
			break;
		case 1:
			n = info->bytesToLoad;

			if (bytesRemaining < n)
			{
				n = bytesRemaining;
			}

#if defined(_WIN64)
			//char* test = ((char*)info - ((uintptr_t)info->bytesToLoad + 148));
			memcpy(((char*)info - ((uintptr_t)info->bytesToLoad + 148)), &dataPtr[dataOffset], n);
#else
			//char* test = ((char*)info - (unsigned)(info->bytesToLoad + 148));
			memcpy((char*)info - (info->bytesToLoad - 148), &dataPtr[dataOffset], n);
#endif
			dataOffset += n;

			bytesRemaining -= n;

			info->bytesToLoad -= n;

			if (info->bytesToLoad == 0)
			{
				aadLoadSingleDynSfx(info);
				info->bytesToLoad = info->attr.waveSize;
			}
			break;

		case 2:
			n = info->bytesToLoad;

			if (bytesRemaining < n)
			{
				n = bytesRemaining;
			}

			dataOffset += n;
			bytesRemaining -= n;
			info->bytesToLoad -= n;

			if (info->bytesToLoad == 0)
			{
				if (--info->numSfxToLoad != 0)
				{
					info->smfLoadingState = 1;
					info->bytesToLoad = 24;
				}
				else
				{
					aadLoadDynamicSfxDone(info);
					return;
				}
			}
			break;

		case 3:
			n = info->bytesToLoad;

			if (bytesRemaining < n)
			{
				n = bytesRemaining;
			}

			bytesRemaining -= n;

			aadWaitForSramTransferComplete();

#if !defined(PSXPC_VERSION)
			SpuSetTransferCallback(HackCallback);
#endif

			SpuSetTransferStartAddr(info->waveTransferAddr);
			SpuWrite(&dataPtr[dataOffset], n);

			dataOffset += n;

			info->waveTransferAddr += n;

			info->bytesToLoad -= n;

			if (info->bytesToLoad == 0)
			{
				info->totalSramUsed += info->attr.waveSize;

				if (--info->numSfxToLoad == 0)
				{
					SpuSetTransferCallback(NULL);
					aadLoadDynamicSfxDone(info);
					return;
				}
				else
				{
					info->smfLoadingState = 1;
					info->bytesToLoad = 24;

					if (bytesRemaining == 0)
					{
						SpuSetTransferCallback(NULL);
					}
					else
					{
						smfDataPtr = &dataPtr[dataOffset];
						smfBytesLeft = bytesRemaining;
						smfInfo = info;

#if !defined(PSXPC_VERSION)
						return;
#endif
					}
				}
			}
			else
			{
				if (bytesRemaining != 0)
				{
					smfDataPtr = &dataPtr[dataOffset];
					smfBytesLeft = bytesRemaining;
					smfInfo = info;

#if !defined(PSXPC_VERSION)
					return;
#endif
				}
				else
				{
					SpuSetTransferCallback(NULL);
				}
			}
			break;
		}
	}
}

int aadCheckSramFragmented()
{
	struct AadNewSramBlockDesc* sramDescTbl;
	struct AadNewSramBlockDesc* sramDesc;
	long totalFree;
	long smallestFree;
	long numFreeBlocks;
	int i;
	int defragNeeded;

	totalFree = 0;
	smallestFree = 999999;
	numFreeBlocks = 0;

	sramDescTbl = &aadMem->sramDescriptorTbl[0];
	sramDesc = sramDescTbl + aadMem->firstSramBlockDescIndex;

	for (i = 0x80; i != 0 && sramDesc != NULL; i--)
	{
		if (!(sramDesc->waveID & 0x4000))
		{
			numFreeBlocks++;

			totalFree += sramDesc->size;

			if (sramDesc->size < smallestFree)
			{
				smallestFree = sramDesc->size;
			}
		}

		if ((char)sramDesc->nextIndex >= 0)
		{
			sramDesc = sramDescTbl + sramDesc->nextIndex;
		}
		else
		{
			sramDesc = NULL;
		}
	}
	
	defragNeeded = 0;

	if (numFreeBlocks >= 3)
	{
		defragNeeded = smallestFree < (totalFree >> 2);
	}

	return defragNeeded;
}

void aadProcessSramDefrag()//Matching - 93.14%
{
	struct AadSramDefragInfo* info;
	struct AadNewSramBlockDesc* sramDescTbl;
	struct AadNewSramBlockDesc* firstBlock;
	struct AadNewSramBlockDesc* secondBlock;
	int n;
	int waveID;
	int firstBlockIndex;
	int secondBlockIndex;
	struct AadNewSramBlockDesc* next;

	info = &aadMem->sramDefragInfo;
	firstBlockIndex = 1;

	switch (aadMem->sramDefragInfo.status)
	{
	case 0:
		break;
	case 1:
		firstBlockIndex = aadMem->firstSramBlockDescIndex;
		sramDescTbl = &aadMem->sramDescriptorTbl[0];
		firstBlock = sramDescTbl + firstBlockIndex;

		n = 128;
		if (firstBlock != NULL)
		{
			while (firstBlock != NULL)
			{
				if ((firstBlock->waveID & 0x4000))
				{
					firstBlockIndex = firstBlock->nextIndex;

					if (--n != 0 && firstBlockIndex < 128)
					{
						firstBlock = sramDescTbl + firstBlockIndex;
						continue;
					}
					else
					{
						firstBlock = NULL;
					}
				}
				else
				{
					break;
				}
			}

			if (firstBlock != NULL)
			{
				if ((char)firstBlock->nextIndex >= 0)
				{
					secondBlockIndex = firstBlock->nextIndex;

					secondBlock = sramDescTbl + secondBlockIndex;

					if ((secondBlock->waveID & 0x4000))
					{
						info->fragBuffer = (unsigned char*)aadMem->memoryMallocProc(0x1000, 0x30);

						if (info->fragBuffer != NULL)
						{
							waveID = secondBlock->waveID & 0x3FFF;

							info->masterListEntry = aadMem->sfxWaveMasterList[waveID];

							if (info->masterListEntry >= 0xFE)
							{
								aadMem->memoryFreeProc((char*)info->fragBuffer);
							}
							else
							{
								aadMem->sfxWaveMasterList[waveID] = 0xFF;

								info->waveID = waveID;

								info->destSramAddr = firstBlock->address << 3;

								info->srcSramAddr = secondBlock->address << 3;

								info->moveSize = secondBlock->size << 3;

								n = firstBlock->size;
								firstBlock->size = secondBlock->size;
								secondBlock->size = n;
								n = firstBlock->waveID;
								firstBlock->waveID = secondBlock->waveID;
								secondBlock->waveID = n;

								secondBlock->address = firstBlock->address + firstBlock->size;

								if ((char)secondBlock->nextIndex >= 0)
								{
									next = sramDescTbl + secondBlock->nextIndex;

									if (!(next->waveID & 0x4000))
									{
										secondBlock->size += next->size;

										next->waveID = 0;

										secondBlock->nextIndex = next->nextIndex;

										if (next->nextIndex << 24 >= 0)
										{
											(sramDescTbl + secondBlock->nextIndex)->prevIndex = secondBlockIndex;
										}
									}
								}

								aadMem->sfxWaveAttrTbl[info->masterListEntry].sramHandle = firstBlockIndex;

								info->status = 2;
								return;
							}
						}
					}
				}
			}
		}
		break;
	case 2:
		if (SpuIsTransferCompleted(0) != 0)
		{
			if (info->moveSize < 4097)
			{
				n = info->moveSize;
			}
			else
			{
				n = 4096;
			}

			SpuSetTransferStartAddr(info->srcSramAddr);

			SpuRead(info->fragBuffer, n);

			info->readSize = n;

			aadMem->sramDefragInfo.status = 3;
		}
		break;
	case 3:

		if (SpuIsTransferCompleted(0) != 0)
		{
			SpuSetTransferStartAddr(info->destSramAddr);

			n = info->readSize;

			SpuWrite(info->fragBuffer, n);

			info->srcSramAddr += n;
			info->moveSize -= n;
			info->destSramAddr += n;

			if (info->moveSize != 0)
			{
				aadMem->sramDefragInfo.status = 2;
			}
			else
			{
				aadMem->memoryFreeProc((char*)info->fragBuffer);

				aadMem->sfxWaveMasterList[info->waveID] = info->masterListEntry;

				if (aadCheckSramFragmented() != 0)
				{
					aadMem->sramDefragInfo.status = 1;
				}
				else
				{
					aadMem->sramDefragInfo.status = 0;
				}
			}
		}
		break;
	}
}

int aadIsSfxLoaded(unsigned int toneID)
{
	if (aadMem->sfxToneMasterList[toneID] < 254)
	{
		return 1;
	}
	else if (toneID == 254)
	{
		return -1;
	}

	return 0;
}

void aadInitSequenceSlot(struct _AadSequenceSlot* slot)//Matching - 99.70%
{
	struct AadSequenceHdr* seqHdr;
	unsigned long trackOffset;
	int i;
	int bank;

	slot->slotFlags &= 0x1;

	bank = slot->sequenceAssignedDynamicBank;

	slot->status = 0;

	slot->selectedDynamicBank = bank;

	seqHdr = (struct AadSequenceHdr*)aadMem->dynamicSequenceAddressTbl[bank][slot->sequenceNumberAssigned];

	for (i = 0; i < 16; i++)
	{
#if defined(AKUJI)
		if (i < 1)
#else
		if (i < seqHdr->numTracks)
#endif
		{
#if defined(AKUJI)
			slot->sequencePosition[i] = (unsigned char*)(seqHdr + 1);
#else
			slot->sequencePosition[i] = (unsigned char*)(char*)seqHdr + ((int*)seqHdr)[4 + i];
#endif
		}
		else
		{
			slot->sequencePosition[i] = NULL;
		}

		slot->trackFlags[i] = 0;
		slot->loopCurrentNestLevel[i] = 0;
		slot->eventsInQueue[i] = 0;
		slot->eventIn[i] = 0;
		slot->eventOut[i] = 0;
		slot->trackFlags[i] |= 0x20;
	}

	for (i = 0; i < 16; i++)
	{
		slot->currentDynamicBank[i] = slot->sequenceAssignedDynamicBank;
		slot->currentProgram[i] = 255;
		slot->volume[i] = 127;
		slot->panPosition[i] = 63;
		slot->pitchWheel[i] = 8192;
	}

	slot->selectedSlotPtr = slot;
	slot->delayedMuteMode = 0;
	slot->delayedMuteCmds = 0;
	slot->delayedUnMuteCmds = 0;
	slot->selectedSlotNum = slot->thisSlotNumber;
}

int aadWaitForSramTransferComplete()
{ 
	int n;
	
	for (n = 100000; n != 0; n--)
	{
		if (SpuIsTransferCompleted(0) != 0)
		{
			return 1;
		}
	}

	return 0;
}

void aadInitReverb()
{
	SpuSetReverbModeType(aadGetReverbMode());

	SpuSetReverbVoice(0, 0xFFFFFFFu);

	if (aadWaitForSramTransferComplete() != 0)
	{
		SpuClearReverbWorkArea(aadGetReverbMode());
	}

	SpuSetReverbModeDepth(aadGetReverbDepth(), aadGetReverbDepth());

	SpuSetReverb(1);
}

void aadShutdownReverb()
{
	if (aadWaitForSramTransferComplete() != 0)
	{
		SpuClearReverbWorkArea(aadGetReverbMode());
	}
}

int aadGetReverbMode()
{ 
	return 3;
}

unsigned long aadGetReverbSize()
{
	return aadReverbModeSize[aadGetReverbMode()] + 64;
}

int aadGetReverbDepth()
{ 
	return 10000;
}

int aadGetNumDynamicSequences(int bank)
{
	if (aadMem->dynamicBankStatus[bank] == 2)
	{
		return aadMem->dynamicSoundBankHdr[bank]->numSequences;
	}

	return 0;
}

int aadAssignDynamicSequence(int bank, int sequenceNumber, int slotNumber)//Matching - 99.27%
{ 
	struct AadTempo tempo;
	struct _AadSequenceSlot* slot;
	int i;

	if (aadMem->dynamicBankStatus[bank] != 2)
	{
		return 0x1007;
	}

	slot = aadMem->sequenceSlots[slotNumber];
	slot->sequenceNumberAssigned = sequenceNumber;
	slot->sequenceAssignedDynamicBank = bank;

	aadInitSequenceSlot(slot);
	aadAllNotesOff(slotNumber);

	if (slot->tempo.ticksPerUpdate == 0)
	{
		aadSetSlotTempo(slotNumber, aadGetTempoFromDynamicSequence(bank, sequenceNumber, &tempo));
	}
	
	slot->channelMute = 0;
	slot->enableSustainUpdate = 0;
	slot->ignoreTranspose = 0;

	for (i = 0; i < 16; i++)
	{
		slot->transpose[i] = 0;
	}

	return 0;
}

struct AadTempo* aadGetTempoFromDynamicSequence(int bank, int sequenceNumber, struct AadTempo* tempo)//Matching - 99.75%
{
	struct AadSequenceHdr *seqHdr;
	
	if (aadMem->dynamicBankStatus[bank] == 2)
	{
		seqHdr = (struct AadSequenceHdr*)aadMem->dynamicSequenceAddressTbl[bank][sequenceNumber];
		
		tempo->quarterNoteTime = seqHdr->quarterNoteTime;
		tempo->ppqn = seqHdr->ppqn;
	}

	return tempo;
}

void aadSetSlotTempo(int slotNumber, struct AadTempo *tempo)
{
	struct _AadSequenceSlot *slot;
	unsigned long tickTime;
	unsigned long tickTimeRemainder;

	slot = aadMem->sequenceSlots[slotNumber];
	tickTime = ((tempo->quarterNoteTime / tempo->ppqn) << 16) + ((unsigned int)((tempo->quarterNoteTime % tempo->ppqn) << 16) / tempo->ppqn);
	slot->tempo.tickTimeFixed = tickTime;
	slot->tempo.ticksPerUpdate = (aadUpdateRate[aadMem->updateMode & 3]) / tickTime;
	slot->tempo.errorPerUpdate = (aadUpdateRate[aadMem->updateMode & 3]) % slot->tempo.tickTimeFixed;
	slot->tempo.quarterNoteTime = tempo->quarterNoteTime;
	slot->tempo.ppqn = tempo->ppqn;
}

void aadStartSlot(int slotNumber)//Matching - 99.35%
{
	struct _AadSequenceSlot *slot;

	if (slotNumber < aadMem->numSlots)
	{
		slot = aadMem->sequenceSlots[slotNumber];

		if (!(slot->status & 0x1))
		{
			if (slot->sequenceNumberAssigned != 255)
			{
				aadInitSequenceSlot(slot);
				slot->status |= 0x1;
			}
		}
	}
}

void aadStopSlot(int slotNumber)
{
	struct _AadSequenceSlot *slot;

	if (slotNumber < aadMem->numSlots)
	{
		slot = aadMem->sequenceSlots[slotNumber];

		if (slot->sequenceNumberAssigned != 255)
		{
			slot->status &= -2;
			aadInitSequenceSlot(slot);
			aadAllNotesOff(slotNumber);
		}
	}
}

void aadStopAllSlots()
{ 
#ifdef PSX_VERSION
	struct _AadSequenceSlot* slot;
	int slotNumber;

	slotNumber = 0;
	
	if (aadMem->numSlots > 0)
	{
		do
		{
			slot = aadMem->sequenceSlots[slotNumber];

			if ((slot->status & 0x1))
			{
				aadStopSlot(slotNumber);
			}

			slot->sequenceNumberAssigned = 255;

		} while (++slotNumber < aadMem->numSlots);
	}
#else
	AadMemoryStruct* v0; // ecx
	int v1; // edx
	int numSlots; // eax
	int v3; // edi
	int v4; // esi
	AadMemoryStruct* v5; // ebp
	int v6; // ecx
	int v7; // edx
	int i; // edi
	char* v9; // eax
	int v10; // ebp
	int v11; // [esp+0h] [ebp-8h]
	int v12; // [esp+4h] [ebp-4h]

	v0 = aadMem;
	v1 = 0;
	v11 = 0;
	numSlots = aadMem->numSlots;
	if (numSlots > 0)
	{
		v3 = 52;
		v12 = 52;
		do
		{
			v4 = *(unsigned int*)((char*)&v0->updateCounter + v3);
			if ((*(BYTE*)(v4 + 1344) & 1) != 0 && v1 < numSlots && *(BYTE*)(v4 + 1342) != 0xFF)
			{
				*(WORD*)(v4 + 1344) &= ~1u;
				aadInitSequenceSlot((struct _AadSequenceSlot* )v4);
				v5 = aadMem;
				v6 = 0;
				v7 = *(unsigned int*)((char*)&aadMem->updateCounter + v3);
				for (i = 476; i < 1148; i += 28)
				{
					v9 = (char*)v5 + i;
					if ((*((BYTE*)&v5->updateMode + i) & 0xF0) == *(BYTE*)(v7 + 1361))
					{
						v10 = *(DWORD*)v9;
						v9[8] = -1;
						v6 |= v10;
						v9[18] |= 2u;
						v5 = aadMem;
					}
				}
				if (v6)
				{
					v5->voiceKeyOffRequest |= v6;
					aadMem->voiceKeyOnRequest &= ~v6;
				}
			}
			v1 = v11 + 1;
			*(BYTE*)(v4 + 1342) = -1;
			v0 = aadMem;
			v3 = v12 + 4;
			v11 = v1;
			numSlots = aadMem->numSlots;
			v12 += 4;
		} while (v1 < numSlots);
	}
#endif
}

void aadDisableSlot(int slotNumber)
{
#if defined(PSX_VERSION)

	if (slotNumber < aadMem->numSlots)
	{
		aadMem->sequenceSlots[slotNumber]->slotFlags |= 0x1;

		aadAllNotesOff(slotNumber);
	}

#elif defined(PC_VERSION)
	int v1; // esi
	AadMemoryStruct* v2; // edi
	int v3; // ecx
	struct _AadSequenceSlot* v4; // ebp
	char* v5; // eax
	int v6; // ebx

	if (slotNumber < aadMem->numSlots)
	{
		v1 = 476;
		aadMem->sequenceSlots[slotNumber]->slotFlags |= 1u;
		v2 = aadMem;
		v3 = 0;
		v4 = aadMem->sequenceSlots[slotNumber];
		do
		{
			v5 = (char*)v2 + v1;
			if ((*((BYTE*)&v2->updateMode + v1) & 0xF0) == v4->slotID)
			{
				v6 = *(DWORD*)v5;
				v5[8] = -1;
				v3 |= v6;
				*((WORD*)v5 + 9) |= 2u;
				v2 = aadMem;
			}
			v1 += 28;
		} while (v1 < 1148);
		if (v3)
		{
			v2->voiceKeyOffRequest |= v3;
			aadMem->voiceKeyOnRequest &= ~v3;
		}
	}
#endif
}

void aadEnableSlot(int slotNumber)
{
#if defined(PSX_VERSION)
	if (slotNumber < aadMem->numSlots)
	{
		aadMem->sequenceSlots[slotNumber]->slotFlags &= 0xFE;
	}
#elif defined(PC_VERSION)
	if (slotNumber < aadMem->numSlots)
		aadMem->sequenceSlots[slotNumber]->slotFlags &= ~1u;
#endif
}

void aadPauseSlot(int slotNumber)
{
#if defined(PSX_VERSION)

	if (slotNumber < aadMem->numSlots)
	{
		aadMem->sequenceSlots[slotNumber]->status &= 0xFFFE;

		aadAllNotesOff(slotNumber);
	}

#elif defined(PC_VERSION)
	int v1; // esi
	AadMemoryStruct* v2; // edi
	int v3; // ecx
	struct _AadSequenceSlot* v4; // ebp
	char* v5; // eax
	int v6; // ebx

	if (slotNumber < aadMem->numSlots)
	{
		v1 = 476;
		aadMem->sequenceSlots[slotNumber]->status &= ~1u;
		v2 = aadMem;
		v3 = 0;
		v4 = aadMem->sequenceSlots[slotNumber];
		do
		{
			v5 = (char*)v2 + v1;
			if ((*((BYTE*)&v2->updateMode + v1) & 0xF0) == v4->slotID)
			{
				v6 = *(DWORD*)v5;
				v5[8] = -1;
				v3 |= v6;
				*((WORD*)v5 + 9) |= 2u;
				v2 = aadMem;
			}
			v1 += 28;
		} while (v1 < 1148);
		if (v3)
		{
			v2->voiceKeyOffRequest |= v3;
			aadMem->voiceKeyOnRequest &= ~v3;
		}
	}
#endif
}

void aadResumeSlot(int slotNumber)
{
#if defined(PSX_VERSION)
	struct _AadSequenceSlot* slot;
	int track;

	if (slotNumber < aadMem->numSlots)
	{
		slot = aadMem->sequenceSlots[slotNumber];

		if (slot->sequenceNumberAssigned != 255)
		{
			track = 0;

			for (track = 0; track < 16; track++)
			{
				slot->trackFlags[track] |= 0x20;
			}

			slot->status |= 0x1;
		}
	}

#elif defined(PC_VERSION)
	// line 3165, offset 0x800544f8
	/* begin block 1 */
		// Start line: 3167
		// Start offset: 0x800544F8
		// Variables:
	struct _AadSequenceSlot *slot; // $a1
	int track; // $a0

	unsigned __int8* trackFlags; // eax

	if (slotNumber < aadMem->numSlots)
	{
		slot = aadMem->sequenceSlots[slotNumber];
		if (slot->sequenceNumberAssigned != 0xFF)
		{
			trackFlags = slot->trackFlags;
			do
				*trackFlags++ |= 0x20u;
			while ((int)&trackFlags[-984 - (DWORD)slot] < 16);
			slot->status |= 1u;
		}
	}
#endif
}

int aadGetSlotStatus(int slotNumber)
{
#if defined(PSX_VERSION)
	return aadMem->sequenceSlots[slotNumber]->status;
#else
	return aadMem->sequenceSlots[slotNumber]->status;
#endif
}

void aadAllNotesOff(int slotNumber)
{ 
#ifdef PSX_VERSION
	struct AadSynthVoice* voice;
	unsigned long vmask;
	int i;
	struct _AadSequenceSlot* slot;

	vmask = 0;
	slot = aadMem->sequenceSlots[slotNumber];

	for (i = 0; i < 24; i++)
	{
		voice = &aadMem->synthVoice[i];

		if ((voice->voiceID & 0xF0) == slot->slotID)
		{
			voice->voiceID = 255;
			vmask |= voice->voiceMask;
			voice->flags |= 0x2;
		}
	}

	if (vmask != 0)
	{
		aadMem->voiceKeyOffRequest |= vmask;
		aadMem->voiceKeyOnRequest &= ~vmask;
	}
#else
	AadMemoryStruct* v1; // edi
	int v2; // ecx
	int v3; // esi
	struct _AadSequenceSlot* v4; // ebp
	char* v5; // eax
	int v6; // ebx

	v1 = aadMem;
	v2 = 0;
	v3 = 476;
	v4 = aadMem->sequenceSlots[slotNumber];
	do
	{
		v5 = (char*)v1 + v3;
		if ((*((BYTE*)&v1->updateMode + v3) & 0xF0) == v4->slotID)
		{
			v6 = *(DWORD*)v5;
			v5[8] = -1;
			v2 |= v6;
			*((WORD*)v5 + 9) |= 2u;
			v1 = aadMem;
		}
		v3 += 28;
	} while (v3 < 1148);
	if (v2)
	{
		v1->voiceKeyOffRequest |= v2;
		aadMem->voiceKeyOnRequest &= ~v2;
	}

#endif
}

void aadMuteChannels(struct _AadSequenceSlot *slot, unsigned long channelList)
{ 
#if defined(PSX_VERSION)
	struct AadSynthVoice* voice;
	unsigned long vmask;
	unsigned long delayedMute;
	int channel;
	int i;

	delayedMute = slot->delayedMuteMode & channelList;

	if (delayedMute != 0)
	{
		slot->delayedMuteCmds |= delayedMute;
		channelList &= ~delayedMute;
	}

	vmask = 0;
	
	slot->channelMute |= channelList;

	for(channel = 0; channel < 16; channel++)
	{
		if ((channelList & (1 << channel)) != 0)
		{
			for (i = 0; i < 24; i++)
			{
				voice = &aadMem->synthVoice[i];

				if (voice->voiceID == (slot->slotID | channel))
				{
					voice->voiceID = 255;
					vmask |= voice->voiceMask;
				}
			}
		}
	}

	if (vmask != 0)
	{
		aadMem->voiceKeyOffRequest |= vmask;
		aadMem->voiceKeyOnRequest &= ~vmask;
	}

#elif defined(PC_VERSION)
	// line 3252, offset 0x80054628
	/* begin block 1 */
		// Start line: 3254
		// Start offset: 0x80054628
		// Variables:
			struct AadSynthVoice *voice; // $a2
			unsigned long vmask; // $t2
			unsigned long delayedMute; // $a2
			int channel; // $t1
			//int i; // $t0
	unsigned int v2; // edx
	unsigned int v3; // eax
	AadMemoryStruct* v4; // esi
	int v5; // edi
	int i; // ebp
	int j; // eax
	BYTE* v8; // edx
	int v9; // ecx

	v2 = channelList;
	v3 = channelList & slot->delayedMuteMode;
	if (((unsigned __int16)channelList & slot->delayedMuteMode) != 0)
	{
		slot->delayedMuteCmds |= v3;
		v2 = ~v3 & channelList;
		channelList = v2;
	}
	slot->channelMute |= v2;
	v4 = aadMem;
	v5 = 0;
	for (i = 0; i < 16; ++i)
	{
		if (((1 << i) & v2) != 0)
		{
			for (j = 476; j < 1148; j += 28)
			{
				v8 = (BYTE*)&v4->updateMode + j;
				if ((unsigned __int8)*v8 == (slot->slotID | i))
				{
					v9 = *(unsigned int*)((char*)&v4->updateCounter + j);
					*v8 = -1;
					v4 = aadMem;
					v5 |= v9;
				}
			}
			v2 = channelList;
		}
	}
	if (v5)
	{
		v4->voiceKeyOffRequest |= v5;
		aadMem->voiceKeyOnRequest &= ~v5;
	}
#endif
}

void aadUnMuteChannels(struct _AadSequenceSlot* slot, unsigned long channelList)//Matching - 100%
{
#if defined(PSX_VERSION)
	unsigned long delayedUnMute;

	delayedUnMute = slot->delayedMuteMode & channelList;

	if (delayedUnMute != 0)
	{
		slot->delayedUnMuteCmds |= delayedUnMute;
		channelList &= ~delayedUnMute;
	}

	slot->channelMute &= ~channelList;

#elif defined(PC_VERSION)
	__int16 v2; // dx
	__int16 v3; // ax

	v2 = channelList;
	v3 = channelList & slot->delayedMuteMode;
	if (v3)
	{
		slot->delayedUnMuteCmds |= v3;
		v2 = ~v3 & channelList;
	}
	slot->channelMute &= ~v2;
#endif
}

void* aadInstallEndSequenceCallback(void (*func)(long, int, int), long data)
{
#if defined(PSX_VERSION)
	void* previousCallbackProc;
	previousCallbackProc = (void*)aadMem->endSequenceCallback;
	aadMem->endSequenceCallback = func;
	aadMem->endSequenceCallbackData = data;
	return previousCallbackProc;
#elif 0
	TDRFuncPtr_aadInstallEndSequenceCallback result; // eax

	result = (TDRFuncPtr_aadInstallEndSequenceCallback)aadMem[3].loadRequestQueue[13].type;
	aadMem[3].loadRequestQueue[13].type = (int)callbackProc;
	aadMem[3].loadRequestQueue[13].directoryID = data;
	return result;
#endif
}

void aadSetUserVariable(int variableNumber, int value)
{
#if defined(PSX_VERSION)
	aadMem->userVariables[variableNumber] = value;
#elif defined(PC_VERSION)
	*((BYTE*)&aadMem[3].loadRequestQueue[13].flags + variableNumber) = value;
#endif
}

void aadSetNoUpdateMode(int noUpdate)
{ 
#if defined(PSX_VERSION)
	if (noUpdate != 0)
	{
		aadMem->flags |= 2;
	}
	else
	{
		aadMem->flags &= -3;
	}
#else
	int flags; // ecx

	flags = aadMem->flags;
	if (noUpdate)
		aadMem->flags = flags | 2;
	else
		aadMem->flags = flags & ~2u;
#endif
}


// autogenerated function stub: 
// void /*$ra*/ aadPauseSound()
void aadPauseSound()
{ // line 3467, offset 0x800547a8
#if defined(PC_VERSION)
	int flags; // eax
	int v1; // esi
	int v2; // ebx
	int i; // edi

	flags = aadMem->flags;
	if ((flags & 8) == 0)
	{
		v1 = 0;
		aadMem->flags |= 0xC;
		v2 = 0;
		for (i = 1172; i < 1220; i += 2)
		{
			aadMem->synthVoice[v2].flags &= ~2u;
			SpuGetVoicePitch(v1, (unsigned __int16*)((char*)aadMem + i));
			SpuSetVoicePitch(v1++, 0);
			++v2;
		}
	}
#else
	UNIMPLEMENTED();
#endif
}

void aadCancelPauseSound()
{
#if defined(PSX_VERSION)
	aadMem->flags &= 0xFFFFFFF3;
#elif defined(PC_VERSION)
	aadMem->flags &= ~0xCu;
#endif
}


// autogenerated function stub: 
// void /*$ra*/ aadResumeSound()
void aadResumeSound()
{ // line 3493, offset 0x8005485c

#if defined(PC_VERSION)
	int flags; // eax
	AadMemoryStruct* v1; // eax
	int v2; // edi
	int v3; // ebp
	int i; // esi

	flags = aadMem->flags;
	if ((flags & 8) != 0)
	{
		aadMem->flags &= ~0xC;
		v1 = aadMem;
		v2 = 0;
		v3 = 0;
		for (i = 1172; i < 1220; i += 2)
		{
			if ((v1->synthVoice[v3].flags & 2) == 0)
			{
				SpuSetVoicePitch(v2, *(WORD*)((char*)&v1->updateCounter + i));
				v1 = aadMem;
			}
			++v2;
			++v3;
		}
	}
#else
	UNIMPLEMENTED();
#endif
}