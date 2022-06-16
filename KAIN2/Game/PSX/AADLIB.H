#ifndef AADLIB_H
#define AADLIB_H

typedef long (*TDRFuncPtr_aadInstallUpdateFunc0updateFuncPtr)();
typedef void (*TDRFuncPtr_aadInstallEndSequenceCallback)();
typedef void (*TDRFuncPtr_aadInstallEndSequenceCallback0callbackProc)();
typedef void (*TDRFuncPtr_aadLoadDynamicSoundBank4retProc)();
typedef void (*TDRFuncPtr_aadStartMasterVolumeFade2fadeCompleteCallback)();
typedef void (*TDRFuncPtr_aadStartMusicMasterVolFade2fadeCompleteCallback)();

struct AadTempo 
{
	unsigned long quarterNoteTime; // size=0, offset=0
	int ppqn; // size=0, offset=4
};

struct AadVolume 
{
	short left; // size=0, offset=0
	short right; // size=0, offset=2
};

struct AadSequenceHdr 
{
	unsigned long sequenceID; // size=0, offset=0
	unsigned long quarterNoteTime; // size=0, offset=4
	unsigned short ppqn; // size=0, offset=8
	unsigned short version; // size=0, offset=10
	unsigned short numTracks; // size=0, offset=12
	unsigned short unused; // size=0, offset=14
};

struct AadSoundBankHdr // hashcode: 0x453DB83F (dec: 1161672767)
{
	unsigned long bankID; // size=0, offset=0
	unsigned long headerSize; // size=0, offset=4
	int bankVersion; // size=0, offset=8
	int numPrograms; // size=0, offset=12
	int numTones; // size=0, offset=16
	int numWaves; // size=0, offset=20
	int numSequences; // size=0, offset=24
	int numLabels; // size=0, offset=28
	int reverbMode; // size=0, offset=32
	int reverbDepth; // size=0, offset=36
};

struct AadProgramAtr // hashcode: 0x13FC0BEF (dec: 335285231)
{
	unsigned short numTones; // size=0, offset=0
	unsigned short firstTone; // size=0, offset=2
	unsigned char volume; // size=0, offset=4
	unsigned char panPosition; // size=0, offset=5
	unsigned short unused; // size=0, offset=6
};

struct AadToneAtr // hashcode: 0x990CD685 (dec: -1727211899)
{
	unsigned char priority; // size=0, offset=0
	unsigned char parentProgram; // size=0, offset=1
	unsigned char volume; // size=0, offset=2
	unsigned char panPosition; // size=0, offset=3
	unsigned char centerNote; // size=0, offset=4
	char centerFine; // size=0, offset=5
	unsigned char minNote; // size=0, offset=6
	unsigned char maxNote; // size=0, offset=7
	unsigned char mode; // size=0, offset=8
	unsigned char pitchBendMax; // size=0, offset=9
	unsigned short adsr1; // size=0, offset=10
	unsigned short adsr2; // size=0, offset=12
	short waveIndex; // size=0, offset=14
};

struct _AadDynSfxFileHdr // hashcode: 0xB9609773 (dec: -1184852109)
{
	unsigned long snfID; // size=0, offset=0
	unsigned short snfVersion; // size=0, offset=4
	unsigned short uniqueID; // size=0, offset=6
	unsigned short handle; // size=0, offset=8
	unsigned short numSfxInFile; // size=0, offset=10
	struct _AadDynSfxFileHdr* prevDynSfxFile; // size=20, offset=12
	struct _AadDynSfxFileHdr* nextDynSfxFile; // size=20, offset=16
};

struct AadDynSfxAttr // hashcode: 0x20751E4B (dec: 544546379)
{
	unsigned short sfxID; // size=0, offset=0
	unsigned short waveID; // size=0, offset=2
	struct AadToneAtr toneAttr; // size=16, offset=4
	unsigned long waveSize; // size=0, offset=20
};

struct AadTempoCounter // hashcode: 0x15707CC0 (dec: 359693504)
{
	unsigned long currentTick; // size=0, offset=0
	unsigned long currentError; // size=0, offset=4
	unsigned long tickTimeFixed; // size=0, offset=8
	unsigned long errorPerUpdate; // size=0, offset=12
	unsigned short ticksPerUpdate; // size=0, offset=16
	unsigned short ppqn; // size=0, offset=18
	unsigned long quarterNoteTime; // size=0, offset=20
};

struct AadSeqEvent // hashcode: 0x3E306185 (dec: 1043358085)
{
	unsigned long deltaTime; // size=0, offset=0
	unsigned char track; // size=0, offset=4
	unsigned char statusByte; // size=0, offset=5
	char dataByte[6]; // size=6, offset=6
};

struct _AadSequenceSlot // hashcode: 0xD7421DD6 (dec: -683532842)
{
	struct AadTempoCounter tempo; // size=24, offset=0
	struct AadSeqEvent eventQueue[4][16]; // size=768, offset=24
	unsigned char eventIn[16]; // size=16, offset=792
	unsigned char eventOut[16]; // size=16, offset=808
	unsigned char eventsInQueue[16]; // size=16, offset=824
	unsigned char(*sequencePosition[16]); // size=64, offset=840
	unsigned long lastEventExecutedTime[16]; // size=64, offset=904
	unsigned char runningStatus[16]; // size=16, offset=968
	unsigned char trackFlags[16]; // size=16, offset=984
	unsigned char(*loopSequencePosition[4][16]); // size=256, offset=1000
	unsigned char loopCurrentNestLevel[16]; // size=16, offset=1256
	unsigned char loopCounter[4][16]; // size=64, offset=1272
	struct _AadSequenceSlot* selectedSlotPtr; // size=1488, offset=1336
	unsigned char selectedSlotNum; // size=0, offset=1340
	unsigned char selectedChannel; // size=0, offset=1341
	unsigned char sequenceNumberAssigned; // size=0, offset=1342
	unsigned char thisSlotNumber; // size=0, offset=1343
	unsigned short status; // size=0, offset=1344
	unsigned short channelMute; // size=0, offset=1346
	unsigned short delayedMuteMode; // size=0, offset=1348
	unsigned short delayedMuteCmds; // size=0, offset=1350
	unsigned short delayedUnMuteCmds; // size=0, offset=1352
	unsigned short unused; // size=0, offset=1354
	unsigned short enableSustainUpdate; // size=0, offset=1356
	unsigned short ignoreTranspose; // size=0, offset=1358
	unsigned char slotFlags; // size=0, offset=1360
	unsigned char slotID; // size=0, offset=1361
	unsigned char slotVolume; // size=0, offset=1362
	unsigned char slotPan; // size=0, offset=1363
	int* masterVolPtr; // size=0, offset=1364
	int selectedDynamicBank; // size=0, offset=1368
	int sequenceAssignedDynamicBank; // size=0, offset=1372
	unsigned char currentDynamicBank[16]; // size=16, offset=1376
	unsigned short pitchWheel[16]; // size=32, offset=1392
	unsigned char currentProgram[16]; // size=16, offset=1424
	unsigned char volume[16]; // size=16, offset=1440
	unsigned char panPosition[16]; // size=16, offset=1456
	char transpose[16]; // size=16, offset=1472
};

typedef struct AadSfxCommand // hashcode: 0xD8F832F6 (dec: -654822666)
{
	unsigned char statusByte; // size=0, offset=0
	unsigned char dataByte[3]; // size=3, offset=1
	unsigned long ulongParam; // size=0, offset=4
	short shortParam; // size=0, offset=8
	unsigned short unused; // size=0, offset=10
} AadSfxCommand;

typedef struct AadSfxSlot // hashcode: 0x6E655A78 (dec: 1852136056)
{
	struct AadSfxCommand commandQueue[32]; // size=384, offset=0
	unsigned char commandIn; // size=0, offset=384
	unsigned char commandOut; // size=0, offset=385
	unsigned short commandsInQueue; // size=0, offset=386
	unsigned short handleCounter; // size=0, offset=388
	unsigned char sfxVolume; // size=0, offset=390
	unsigned char sfxPan; // size=0, offset=391
} AadSfxSlot;

struct AadSynthVoice // hashcode: 0x12E02CA2 (dec: 316681378)
{
	unsigned long voiceMask; // size=0, offset=0
	unsigned long handle; // size=0, offset=4
	unsigned char voiceID; // size=0, offset=8
	unsigned char priority; // size=0, offset=9
	unsigned char note; // size=0, offset=10
	unsigned char unused; // size=0, offset=11
	unsigned char program; // size=0, offset=12
	unsigned char baseProg; // size=0, offset=13
	unsigned char volume; // size=0, offset=14
	unsigned char updateVol; // size=0, offset=15
	unsigned char pan; // size=0, offset=16
	unsigned char voiceNum; // size=0, offset=17
	unsigned short flags; // size=0, offset=18
	struct AadToneAtr* toneAtr; // size=16, offset=20
	struct AadProgramAtr* progAtr; // size=8, offset=24
};

struct AadDynamicLoadRequest // hashcode: 0xBC9D60B7 (dec: -1130536777)
{
	int type; // size=0, offset=0
	int handle; // size=0, offset=4
	long directoryID; // size=0, offset=8
	long flags; // size=0, offset=12
	char fileName[12]; // size=12, offset=16
};

struct AadDynamicBankLoadInfo // hashcode: 0x6A3AE9FB (dec: 1782245883)
{
	char sndFileName[32]; // size=32, offset=0
	char smpFileName[32]; // size=32, offset=32
	int dynamicBankIndex; // size=0, offset=64
	int loadOption; // size=0, offset=68
	void (*userCallbackProc)(); // size=0, offset=72
	unsigned long sramDataSize; // size=0, offset=76
	unsigned long nextSramAddr; // size=0, offset=80
	unsigned long flags; // size=0, offset=84
};

struct AadDynamicSfxLoadInfo // hashcode: 0xEDD41D4C (dec: -304865972)
{
	char snfFileName[40]; // size=40, offset=0
	char smfFileName[40]; // size=40, offset=40
	int fileHandle; // size=0, offset=80
	long directoryID; // size=0, offset=84
	long loadFlags; // size=0, offset=88
	long flags; // size=0, offset=92
	int smfLoadingState; // size=0, offset=96
	unsigned long bytesToLoad; // size=0, offset=100
	int numSfxToLoad; // size=0, offset=104
	int error; // size=0, offset=108
	unsigned long waveTransferAddr; // size=0, offset=112
	unsigned long totalSramUsed; // size=0, offset=116
	struct _AadDynSfxFileHdr* snfFile; // size=20, offset=120
	struct AadDynSfxAttr attr; // size=24, offset=124
};

struct AadNewSramBlockDesc // hashcode: 0x4DF7A665 (dec: 1308075621)
{
	unsigned short waveID; // size=0, offset=0
	unsigned short address; // size=0, offset=2
	unsigned short size; // size=0, offset=4
	unsigned char prevIndex; // size=0, offset=6
	unsigned char nextIndex; // size=0, offset=7
};

struct AadLoadedSfxToneAttr // hashcode: 0xC1F66431 (dec: -1040817103)
{
	unsigned short referenceCount; // size=0, offset=0
	unsigned short waveID; // size=0, offset=2
	struct AadToneAtr toneAttr; // size=16, offset=4
};

struct AadLoadedSfxWaveAttr // hashcode: 0x3EFD9321 (dec: 1056805665)
{
	unsigned short referenceCount; // size=0, offset=0
	unsigned char unused; // size=0, offset=2
	unsigned char sramHandle; // size=0, offset=3
};

struct AadSramDefragInfo // hashcode: 0xB08B77B9 (dec: -1333037127)
{
	int status; // size=0, offset=0
	unsigned char* fragBuffer; // size=0, offset=4
	int masterListEntry; // size=0, offset=8
	int waveID; // size=0, offset=12
	unsigned long destSramAddr; // size=0, offset=16
	unsigned long srcSramAddr; // size=0, offset=20
	unsigned long moveSize; // size=0, offset=24
	unsigned long readSize; // size=0, offset=28
};

struct AadVolFader // hashcode: 0x053CCA3C (dec: 87870012)
{
	int volumeStep; // size=0, offset=0
	int targetVolume; // size=0, offset=4
	void (*fadeCompleteCallback)(); // size=0, offset=8
};

struct AadInitAttr // hashcode: 0x0549E29A (dec: 88728218)
{
	int updateMode; // size=0, offset=0
	int numSlots; // size=0, offset=4
	void (*nonBlockLoadProc)(char *, void *, void *, void *, void **, int); // size=0, offset=8
	void (*nonBlockBufferedLoadProc)(char*, void*, void*, void*); // size=0, offset=12
	char* (*memoryMallocProc)(unsigned long, unsigned char); // size=0, offset=16
	void (*memoryFreeProc)(char*); // size=0, offset=20
};


typedef struct AadMemoryStruct // hashcode: 0x46CEF45B (dec: 1187968091)
{
	unsigned long updateCounter; // size=0, offset=0
	int numSlots; // size=0, offset=4
	int updateMode; // size=0, offset=8
	int masterVolume; // size=0, offset=12
	struct AadVolFader masterVolFader; // size=12, offset=16
	int sfxMasterVol; // size=0, offset=28
	int musicMasterVol; // size=0, offset=32
	int flags; // size=0, offset=36
	struct AadVolFader musicMasterVolFader; // size=12, offset=40
	struct _AadSequenceSlot(*sequenceSlots[8]); // size=32, offset=52
	struct AadSfxSlot sfxSlot; // size=392, offset=84
	struct AadSynthVoice synthVoice[24]; // size=672, offset=476
	char voiceStatus[24]; // size=24, offset=1148
	unsigned short voicePitchSave[24]; // size=48, offset=1172
	unsigned long voiceKeyOffRequest; // size=0, offset=1220
	unsigned long voiceKeyOnRequest; // size=0, offset=1224
	unsigned long voiceReverbRequest; // size=0, offset=1228
	struct AadSoundBankHdr(*dynamicSoundBankHdr[2]); // size=8, offset=1232
	struct AadProgramAtr(*dynamicProgramAtr[2]); // size=8, offset=1240
	struct AadToneAtr(*dynamicToneAtr[2]); // size=8, offset=1248
	unsigned long(*dynamicWaveAddr[2]); // size=8, offset=1256
	unsigned char(**dynamicSequenceAddressTbl[2]); // size=8, offset=1264
	unsigned long(*dynamicSequenceLabelOffsetTbl[2]); // size=8, offset=1272
	int dynamicBankStatus[2]; // size=8, offset=1280
	unsigned char(*dynamicSoundBankData[2]); // size=8, offset=1288
	unsigned long dynamicSoundBankSramData[2]; // size=8, offset=1296
	struct AadDynamicLoadRequest loadRequestQueue[16]; // size=448, offset=1304
	struct AadDynamicBankLoadInfo dynamicBankLoadInfo; // size=88, offset=1752
	void (*nonBlockLoadProc)(char*, void*, void*, void*, void**, int); // size=0, offset=1840
	void (*nonBlockBufferedLoadProc)(char*, void*, void*, void*); // size=0, offset=1844
	char* (*memoryMallocProc)(unsigned long, unsigned char); // size=0, offset=1848
	void (*memoryFreeProc)(char*); // size=0, offset=1852
	int nextLoadReqIn; // size=0, offset=1856
	int nextLoadReqOut; // size=0, offset=1860
	int numLoadReqsQueued; // size=0, offset=1864
	struct AadDynamicSfxLoadInfo dynamicSfxLoadInfo; // size=148, offset=1868
	struct _AadDynSfxFileHdr* firstDynSfxFile; // size=20, offset=2016
	int nextFileHandle; // size=0, offset=2020
	struct AadSramDefragInfo sramDefragInfo; // size=32, offset=2024
	unsigned char sfxToneMasterList[512]; // size=512, offset=2056
	unsigned char sfxWaveMasterList[512]; // size=512, offset=2568
	struct AadLoadedSfxToneAttr sfxToneAttrTbl[128]; // size=2560, offset=3080
	struct AadLoadedSfxWaveAttr sfxWaveAttrTbl[120]; // size=480, offset=5640
	int nextToneIndex; // size=0, offset=6120
	int nextWaveIndex; // size=0, offset=6124
	struct AadNewSramBlockDesc sramDescriptorTbl[128]; // size=1024, offset=6128
	int nextSramDescIndex; // size=0, offset=7152
	int firstSramBlockDescIndex; // size=0, offset=7156
	void (*controller11Callback)(); // size=0, offset=7160
	void (*endSequenceCallback)(long, int, int); // size=0, offset=7164
	long controller11CallbackData; // size=0, offset=7168
	long endSequenceCallbackData; // size=0, offset=7172
	unsigned char userVariables[128]; // size=128, offset=7176
} AadMemoryStruct;

extern unsigned short aadHblanksPerUpdate[4];
extern unsigned long aadGp;
extern struct AadMemoryStruct* aadMem; // offset 0x800CECD8

extern unsigned long aadGetMemorySize(struct AadInitAttr *attributes); // 0x800514D4
extern int aadInit(struct AadInitAttr *attributes, unsigned char *memoryPtr); // 0x800514F8
extern void aadInstallUpdateFunc(TDRFuncPtr_aadInstallUpdateFunc0updateFuncPtr updateFuncPtr, int hblanksPerUpdate); // 0x800517A0
extern void aadInitVolume(); // 0x8005181C
extern void aadSetMasterVolume(int volume); // 0x80051850
extern void aadStartMasterVolumeFade(int targetVolume, int volumeStep, TDRFuncPtr_aadStartMasterVolumeFade2fadeCompleteCallback fadeCompleteCallback); // 0x80051880
extern void aadSetSfxMasterVolume(int volume); // 0x80051898
extern void aadSetMusicMasterVolume(int volume); // 0x800518A8
extern void aadStartMusicMasterVolFade(int targetVolume, int volumeStep, TDRFuncPtr_aadStartMusicMasterVolFade2fadeCompleteCallback fadeCompleteCallback); // 0x8005190C
extern void aadShutdown(); // 0x80051924
extern long aadSlotUpdateWrapper(); // 0x800519AC
extern void aadSlotUpdate(); // 0x800519F0
extern unsigned long aadCreateFourCharID(char a, char b, char c, char d); // 0x80051F38
extern int aadLoadDynamicSoundBank(char *sndFileName, char *smpFileName, int dynamicBankIndex, int loadOption, void (*func)(int, int)); // 0x80051F60
extern void aadLoadDynamicSoundBankReturn(void *loadedDataPtr, void *data, void *data2); // 0x800520B8
extern void aadLoadDynamicSoundBankReturn2(void *loadedDataPtr, long loadedDataSize, short status, void *data1, void *data2); // 0x80052188
extern int aadFreeDynamicSoundBank(int dynamicBankIndex); // 0x800523B4
extern int aadOpenDynamicSoundBank(unsigned char *soundBank, int dynamicBankIndex); // 0x80052430
extern int aadLoadDynamicSfx(char *fileName, long directoryID, long flags); // 0x80052550
extern int aadFreeDynamicSfx(int handle); // 0x80052604
extern void aadRelocateMusicMemoryBegin(); // 0x800526E0
extern void aadRelocateMusicMemoryEnd(struct MemHeader *newAddress, long offset); // 0x800526FC
extern void aadRelocateSfxMemory(void *oldAddress, int offset); // 0x80052924
extern int aadGetNumLoadsQueued(); // 0x8005298C
extern void aadPurgeLoadQueue(); // 0x800529A0
extern void aadProcessLoadQueue(); // 0x800529B8
extern void aadLoadDynamicSfxAbort(struct AadDynamicSfxLoadInfo *info, int error); // 0x80052CF8
extern void aadLoadDynamicSfxDone(struct AadDynamicSfxLoadInfo *info); // 0x80052D7C
extern void aadLoadDynamicSfxReturn(void *loadedDataPtr, void *data, void *data2); // 0x80052D84
extern int aadWaveMalloc(unsigned short waveID, unsigned long waveSize); // 0x80052EFC
extern unsigned long aadGetSramBlockAddr(int handle); // 0x80053144
extern void aadWaveFree(int handle); // 0x80053170
extern void aadFreeSingleDynSfx(int sfxID); // 0x80053280
extern void setSramFullAlarm(); // 0x80053348
extern void aadLoadSingleDynSfx(struct AadDynamicSfxLoadInfo *info); // 0x80053424
extern void HackCallback(); // 0x800536F4
extern void aadLoadDynamicSfxReturn2(void *loadedDataPtr, long loadedDataSize, short status, void *data1, void *data2); // 0x8005372C
extern int aadCheckSramFragmented(); // 0x800539F4
extern void aadProcessSramDefrag(); // 0x80053AA8
extern int aadIsSfxLoaded(unsigned int toneID); // 0x80053E0C
extern void aadInitSequenceSlot(struct _AadSequenceSlot *slot); // 0x80053E48
extern int aadWaitForSramTransferComplete(); // 0x80053F54
extern void aadInitReverb(); // 0x80053F94
extern void aadShutdownReverb(); // 0x80054018
extern int aadGetReverbMode(); // 0x80054050
extern unsigned long aadGetReverbSize(); // 0x80054058
extern int aadGetReverbDepth(); // 0x80054088
extern int aadGetNumDynamicSequences(int bank); // 0x80054090
extern int aadAssignDynamicSequence(int bank, int sequenceNumber, int slotNumber); // 0x800540C0
extern struct AadTempo * aadGetTempoFromDynamicSequence(int bank, int sequenceNumber, struct AadTempo *tempo); // 0x8005419C
extern void aadSetSlotTempo(int slotNumber, struct AadTempo *tempo); // 0x800541EC
extern void aadStartSlot(int slotNumber); // 0x800542A0
extern void aadStopSlot(int slotNumber); // 0x8005431C
extern void aadStopAllSlots(); // 0x80054388
extern void aadDisableSlot(int slotNumber); // 0x80054418
extern void aadEnableSlot(int slotNumber); // 0x80054468
extern void aadPauseSlot(int slotNumber); // 0x800544A8
extern void aadResumeSlot(int slotNumber); // 0x800544F8
extern int aadGetSlotStatus(int slotNumber); // 0x80054568
extern void aadAllNotesOff(int slotNumber); // 0x80054588
extern void aadMuteChannels(struct _AadSequenceSlot *slot, unsigned long channelList); // 0x80054628
extern void aadUnMuteChannels(struct _AadSequenceSlot *slot, unsigned long channelList); // 0x80054704
extern void* aadInstallEndSequenceCallback(void (*func)(long, int, int), long data); // 0x8005473C
extern void aadSetUserVariable(int variableNumber, int value); // 0x80054754
extern void aadSetNoUpdateMode(int noUpdate); // 0x80054768
extern void aadPauseSound(); // 0x800547A8
extern void aadCancelPauseSound(); // 0x80054840
extern void aadResumeSound(); // 0x8005485C

#endif
