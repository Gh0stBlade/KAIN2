#ifndef STRMLOAD_H
#define STRMLOAD_H


extern void STREAM_NextLoadFromHead(); // 0x8005FAE8

extern void STREAM_NextLoadCanFail(); // 0x8005FAF8

extern void STREAM_NextLoadAsNormal(); // 0x8005FB08

extern void STREAM_InitLoader(char *bigFileName, char *voiceFileName); // 0x8005FB18

extern void STREAM_RemoveQueueHead(); // 0x8005FB6C

extern void STREAM_RemoveQueueEntry(struct _LoadQueueEntry *entry, struct _LoadQueueEntry *prev); // 0x8005FBAC

extern struct _LoadQueueEntry * STREAM_AddQueueEntryToTail(); // 0x8005FC08

extern struct _LoadQueueEntry * STREAM_AddQueueEntryToHead(); // 0x8005FC74

extern int STREAM_IsCdBusy(long *numberInQueue); // 0x8005FD28

extern int STREAM_PollLoadQueue(); // 0x8005FD48

extern struct _LoadQueueEntry * STREAM_SetUpQueueEntry(char *fileName, void *retFunc, void *retData, void *retData2, void **retPointer, int fromhead); // 0x800600C8

extern void STREAM_QueueNonblockingLoads(char *fileName, unsigned char memType, void *retFunc, void *retData, void *retData2, void **retPointer, long relocateBinary); // 0x800601B4

extern void LOAD_LoadToAddress(char *fileName, void *loadAddr, long relocateBinary); // 0x80060240

extern void LOAD_NonBlockingBinaryLoad(char *fileName, void *retFunc, void *retData, void *retData2, void **retPointer, int memType); // 0x800602A8

extern void LOAD_NonBlockingFileLoad(char *fileName, void *retFunc, void *retData, void *retData2, void **retPointer, int memType); // 0x800602EC

extern void LOAD_NonBlockingBufferedLoad(char *fileName, void *retFunc, void *retData, void *retData2); // 0x8006032C

extern int LOAD_IsXAInQueue(); // 0x80060368

extern void LOAD_PlayXA(int number); // 0x800603B0

extern long * LOAD_ReadFile(char *fileName, unsigned char memType); // 0x800603E0

extern void LOAD_ChangeDirectory(char *name); // 0x8006042C

extern void LOAD_AbortDirectoryChange(char *name); // 0x80060478

extern void LOAD_AbortFileLoad(char *fileName, void *retFunc); // 0x80060500


#endif
