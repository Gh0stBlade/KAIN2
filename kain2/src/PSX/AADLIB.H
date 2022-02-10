#ifndef AADLIB_H
#define AADLIB_H


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

extern int aadLoadDynamicSoundBank(char *sndFileName, char *smpFileName, int dynamicBankIndex, int loadOption, TDRFuncPtr_aadLoadDynamicSoundBank4retProc retProc); // 0x80051F60

extern void aadLoadDynamicSoundBankReturn(void *loadedDataPtr, void *data, void *data2); // 0x800520B8

extern void aadLoadDynamicSoundBankReturn2(void *loadedDataPtr, long loadedDataSize, short status, void *data1, void *data2); // 0x80052188

extern int aadFreeDynamicSoundBank(int dynamicBankIndex); // 0x800523B4

extern int aadOpenDynamicSoundBank(unsigned char *soundBank, int dynamicBankIndex); // 0x80052430

extern int aadLoadDynamicSfx(char *fileName, long directoryID, long flags); // 0x80052550

extern int aadFreeDynamicSfx(int handle); // 0x80052604

extern void aadRelocateMusicMemoryBegin(); // 0x800526E0

extern void aadRelocateMusicMemoryEnd(struct MemHeader *newAddress, long offset, void *oldAddress); // 0x800526FC

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

extern TDRFuncPtr_aadInstallEndSequenceCallback aadInstallEndSequenceCallback(TDRFuncPtr_aadInstallEndSequenceCallback0callbackProc callbackProc, long data); // 0x8005473C

extern void aadSetUserVariable(int variableNumber, int value); // 0x80054754

extern void aadSetNoUpdateMode(int noUpdate); // 0x80054768

extern void aadPauseSound(); // 0x800547A8

extern void aadCancelPauseSound(); // 0x80054840

extern void aadResumeSound(); // 0x8005485C


typedef long (*TDRFuncPtr_aadInstallUpdateFunc0updateFuncPtr)();

typedef void (*TDRFuncPtr_aadInstallEndSequenceCallback)();

typedef void (*TDRFuncPtr_aadInstallEndSequenceCallback0callbackProc)();

typedef void (*TDRFuncPtr_aadLoadDynamicSoundBank4retProc)();

typedef void (*TDRFuncPtr_aadStartMasterVolumeFade2fadeCompleteCallback)();

typedef void (*TDRFuncPtr_aadStartMusicMasterVolFade2fadeCompleteCallback)();




#endif
