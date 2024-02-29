#ifndef LOAD3D_H
#define LOAD3D_H

struct _ReadQueueEntry 
{
	long readStartPos; // size=0, offset=0
	long readSize; // size=0, offset=4
	long readCurSize; // size=0, offset=8
	void* readStartDest; // size=0, offset=12
	void* readCurDest; // size=0, offset=16
	long readStatus; // size=0, offset=20
	long checksum; // size=0, offset=24
	long checksumType; // size=0, offset=28
	void* retFunc; // size=0, offset=32
	void* retData; // size=0, offset=36
	void* retData2; // size=0, offset=40
#if defined(_DEBUG) || defined(__EMSCRIPTEN__)
	long fileHash;
#endif
};

struct _BigFileDirEntry 
{
	short streamUnitID; // size=0, offset=0
	short numFiles; // size=0, offset=2
	long subDirOffset; // size=0, offset=4
};

struct _BigFileEntry 
{
	long fileHash; // size=0, offset=0
	long fileLen; // size=0, offset=4
	long filePos; // size=0, offset=8
	long checkSumFull; // size=0, offset=12
};

struct _BigFileDir 
{
	long numFiles; // size=0, offset=0
	struct _BigFileEntry fileList[1]; // size=0, offset=4
};

struct _BigFile 
{
#if defined(PSXPC_VERSION) && defined(NO_CD)
	FILE* bigfileFileHandle;
#endif
	long bigfileBaseOffset; // size=0, offset=0
	struct _BigFileDir* rootDir; // size=4, offset=4
	struct _BigFileDir* currentDir; // size=4, offset=8
	struct _BigFileDir* cachedDir; // size=4, offset=12
	long currentDirID; // size=0, offset=16
	long cachedDirID; // size=0, offset=20
	long searchDirID; // size=0, offset=24
	long numSubDirs; // size=0, offset=28
	struct _BigFileDirEntry* subDirList; // size=8, offset=32
};

struct _NonBlockLoadEntry 
{
	long fileHash; // size=0, offset=0
	long dirHash; // size=0, offset=4
	long filePos; // size=0, offset=8
	long* loadAddr; // size=0, offset=12
	long loadSize; // size=0, offset=16
	long checksum; // size=0, offset=20
	long checksumType; // size=0, offset=24
	long memType; // size=0, offset=28
	long posInFile; // size=0, offset=32
	void* retFunc; // size=0, offset=36
	void* retData; // size=0, offset=40
	void* retData2; // size=0, offset=44
	void** retPointer; // size=0, offset=48
#if defined(PSX_VERSION)
	char fileName[48]; // size=48, offset=52
#endif
};


struct _LoadStatus 
{
	struct _ReadQueueEntry currentQueueFile; // size=44, offset=0
	long state; // size=0, offset=44
	long bytesTransferred; // size=0, offset=48
	long lastCheckPos; // size=0, offset=52
	long cdWaitTime; // size=0, offset=56
	unsigned char* compSrcAddr; // size=0, offset=60
	unsigned char* compDstAddr; // size=0, offset=64
	long compControlbits; // size=0, offset=68
	long compControl; // size=0, offset=72
	long compEndBytes; // size=0, offset=76
	long decompressLen; // size=0, offset=80
	long checksum; // size=0, offset=84
	long* checkAddr; // size=0, offset=88
	struct _BigFile bigFile; // size=36, offset=92
	void* buffer1; // size=0, offset=128
	void* buffer2; // size=0, offset=132
	int changeDir; // size=0, offset=136
	long endLoadTime; // size=0, offset=140
	long currentDirLoading; // size=0, offset=144
	long currentSector; // size=0, offset=148
	long sectorTime; // size=0, offset=152
	long seekTime; // size=0, offset=156
};

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern long crap1;
extern long crap35[4];

extern char HashExtensions[7][4]; // offset 0x800CE4D0
extern void LOAD_InitCd(); // 0x80036D44
extern void LOAD_CdSeekCallback(unsigned char intr, unsigned char *result); // 0x80036D74
extern void LOAD_CdDataReady(); // 0x80036D90
extern void LOAD_CdReadReady(unsigned char intr, unsigned char *result); // 0x80036EB0
extern void LOAD_UpdateCheckSum(long bytes); // 0x80036F9C
extern void LOAD_DoCDReading(); // 0x80036FE0
extern void LOAD_DoCDBufferedReading(); // 0x8003708C
extern void LOAD_SetupFileToDoCDReading(); // 0x800370A8
extern void LOAD_SetupFileToDoBufferedCDReading(); // 0x80037124
extern void LOAD_ProcessReadQueue(); // 0x80037198
extern char * LOAD_ReadFileFromCD(char *filename, int memType); // 0x80037314
#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
extern void LOAD_CdReadFromBigFile(long fileOffset, unsigned int* loadAddr, long bytes, long chksumLevel, long checksum, long fileHash); // 0x800373F4
#else
extern void LOAD_CdReadFromBigFile(long fileOffset, unsigned int* loadAddr, long bytes, long chksumLevel, long checksum); // 0x800373F4
#endif
extern struct _BigFileDir * LOAD_ReadDirectory(struct _BigFileDirEntry *dirEntry); // 0x80037420
extern void LOAD_InitCdLoader(char *bigFileName, char *voiceFileName); // 0x80037488
extern int LOAD_SetupFileInfo(struct _NonBlockLoadEntry *loadEntry); // 0x800375DC
extern void LOAD_CD_ReadPartOfFile(struct _NonBlockLoadEntry *loadEntry); // 0x80037664
extern long LOAD_HashName(char *string); // 0x80037704
extern long LOAD_HashUnit(char *name); // 0x80037858
extern struct _BigFileEntry * LOAD_GetBigFileEntryByHash(long hash); // 0x800378FC
extern struct _BigFileEntry * LOAD_GetBigFileEntry(char *fileName); // 0x80037984
extern long LOAD_DoesFileExist(char *fileName); // 0x800379AC
extern void LOAD_NonBlockingReadFile(struct _NonBlockLoadEntry *loadEntry); // 0x800379E0
extern void LOAD_LoadTIM(int *addr, long x_pos, long y_pos, long clut_x, long clut_y); // 0x80037A9C
extern void LOAD_LoadTIM2(int *addr, long x_pos, long y_pos, long width, long height); // 0x80037B3C
extern long LOAD_RelocBinaryData(long* data, long fileSize); // 0x80037B84
extern void LOAD_CleanUpBuffers(); // 0x80037C3C
extern void * LOAD_InitBuffers(); // 0x80037C84
extern void LOAD_InitCdStreamMode(); // 0x80037CD4
extern void LOAD_DumpCurrentDir(); // 0x80037D1C
extern int LOAD_ChangeDirectoryByID(int id); // 0x80037D6C
extern void LOAD_SetSearchDirectory(long id); // 0x80037E74
extern long LOAD_GetSearchDirectory(); // 0x80037E80
extern int LOAD_ChangeDirectoryFlag(); // 0x80037E8C
extern void LOAD_UpdateBigFilePointers(struct _BigFileDir *oldDir, struct _BigFileDir *newDir); // 0x80037E98
extern int LOAD_IsFileLoading(); // 0x80037EC8
extern void LOAD_StopLoad(); // 0x80037ED4

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
