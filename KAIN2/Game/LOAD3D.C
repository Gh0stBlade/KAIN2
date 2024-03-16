#include "CORE.H"
#include "LOAD3D.H"
#include "FONT.H"
#include "MEMPACK.H"
#include "TIMER.H"
#include "VOICEXA.H"
#include "DEBUG.H"
#include "RESOLVE.H"
#include "GAMELOOP.H"
#include "STRMLOAD.H"

#include <stdlib.h>

#define false	0
#define true	1

struct _LoadStatus loadStatus; // offset 0x800D0D84
long crap1;
long crap35[4];

char HashExtensions[7][4] = { "drm", "crm", "tim", "smp", "snd", "smf", "snf"};

void LOAD_InitCd()//Matching - 99.17%
{
#if defined(PSX_VERSION)
	CdInit();
	ResetCallback();
	CdSetDebug(0);
#endif
}

void LOAD_CdSeekCallback(unsigned char intr, unsigned char *result)//Matching - 88.16%
{ 
	if (loadStatus.state == 1)
	{
		loadStatus.state = 2;
#if defined(PSXPC_VERSION)
		crap1 = (long)(Emulator_GetPerformanceCounter() / (Emulator_GetPerformanceFrequency() / 1000000));
#else
		crap1 = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);
#endif
	}
}

void LOAD_CdDataReady()//Matching - 91.46%
{
	struct _ReadQueueEntry* currentQueueFile;
	long actualReadSize;
	int status;

	if (loadStatus.state == 3)
	{
		loadStatus.state = 4;
	}
	else if (loadStatus.state == 4)
	{
		currentQueueFile = &loadStatus.currentQueueFile;
		actualReadSize = loadStatus.bytesTransferred;
		currentQueueFile->readCurSize += actualReadSize;

		if (currentQueueFile->readStatus == 3)
		{
			currentQueueFile->readCurDest = (void*)((char*)currentQueueFile->readCurDest + actualReadSize);

			if (currentQueueFile->readCurSize == currentQueueFile->readSize)
			{
				loadStatus.state = 5;
			}
			else
			{
				loadStatus.state = 2;
			}
		}
		else
		{
			if (currentQueueFile->readStatus == 6)
			{
				if (currentQueueFile->readCurSize == currentQueueFile->readSize)
				{
					status = 5;
				}
				else
				{
					status = 2;
				}

				if (currentQueueFile->retFunc != NULL)
				{
					typedef void (*retFunc)(void*, long, long, void*, void*);
					retFunc func = (retFunc)currentQueueFile->retFunc;
					func(currentQueueFile->readCurDest, actualReadSize, (status ^ 5) < 1, currentQueueFile->retData, currentQueueFile->retData2);
				}

				if (currentQueueFile->readCurDest == loadStatus.buffer1)
				{
					currentQueueFile->readCurDest = loadStatus.buffer2;
				}
				else
				{
					currentQueueFile->readCurDest = loadStatus.buffer1;
				}

				loadStatus.state = status;
			}
		}
	}
}

void LOAD_CdReadReady(unsigned char intr, unsigned char *result)
{
#if defined(PSX_VERSION) || (PSXPC_VERSION)
	static long crap[3];
	int bytes;
	CdlLOC loc;

	if (loadStatus.state == 2)
	{
		if (intr == CdlDataReady)
		{
			bytes = loadStatus.currentQueueFile.readSize - loadStatus.currentQueueFile.readCurSize;
			if (bytes >= 2048)
			{
				bytes = 2048;
			}

			loadStatus.bytesTransferred = bytes;
			loadStatus.state = 4;

			CdGetSector(&crap, 3);

			if (loadStatus.currentSector == CdPosToInt((CdlLOC*)&crap))
			{
#if !defined(PSXPC_VERSION)
				loadStatus.currentSector++;
				CdGetSector(loadStatus.currentQueueFile.readCurDest, bytes >> 2);
				LOAD_CdDataReady();
#else
				//Because the emu is mostly single threaded we can't continuously call the callback.
				int numSectorsToRead = (loadStatus.currentQueueFile.readSize - loadStatus.currentQueueFile.readCurSize) / 2048;
				unsigned int numRemainingDataToRead = (loadStatus.currentQueueFile.readSize - loadStatus.currentQueueFile.readCurSize) % 2048;
				char* dest = (char*)loadStatus.currentQueueFile.readCurDest;

				for (int i = 0; i < numSectorsToRead; i++)
				{
					loadStatus.currentSector++;
					if (i > 0)
					{
						CdGetSector(&crap, 3);
					}
					CdGetSector(dest, 2048 >> 2);

					CdIntToPos(loadStatus.currentSector, &loc);
					CdControl(CdlReadN, &loc.minute, NULL);

					dest += 2048;
				}

				if (numRemainingDataToRead != 0)
				{
					loadStatus.currentSector++;
					if (numSectorsToRead != 0)
					{
						CdGetSector(&crap, 3);
					}
					CdGetSector(dest, numRemainingDataToRead >> 2);

					CdIntToPos(loadStatus.currentSector, &loc);
					CdControl(CdlReadN, &loc.minute, NULL);

					dest += numRemainingDataToRead;
				}

				loadStatus.bytesTransferred = (loadStatus.currentQueueFile.readSize - loadStatus.currentQueueFile.readCurSize);

				static int fallThrough = 0;

				if (loadStatus.currentQueueFile.readStatus == 6 && fallThrough == 0)
				{
					fallThrough ^= 1;
					loadStatus.currentQueueFile.readStatus = 0;
				}
				else
				{
					fallThrough = 0;
					LOAD_CdDataReady();
				}
#endif
			}
			else
			{
				loadStatus.state = 1;
				CdIntToPos(loadStatus.currentSector, &loc);
				CdControl(CdlReadN, &loc.minute, NULL);
			}
		}
		else
		{
			if (intr == 5)
			{
				loadStatus.state = 6;
				loadStatus.currentQueueFile.readStatus = 4;
			}
			else
			{
				printf("something %x\n", intr);
			}
		}
	}
	
	if (crap1 != 0)
	{
		loadStatus.seekTime = (long)TIMER_TimeDiff(crap1);
		crap1 = 0;
	}

	loadStatus.sectorTime = (long)TIMER_TimeDiff(crap35[0]);
#if defined(PSXPC_VERSION)
	crap35[0] = (long)(Emulator_GetPerformanceCounter() / (Emulator_GetPerformanceFrequency() / 1000000));
#else
	crap35[0] = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);
#endif
#endif
}

void LOAD_UpdateCheckSum(long bytes)//Matching - 95.59%
{
	if (loadStatus.currentQueueFile.checksumType == 1 && bytes != 0)
	{
		do
		{
			loadStatus.checksum += *loadStatus.checkAddr++;
		} while(bytes -= sizeof(int));
	}
}

void LOAD_DoCDReading()//Matching - 83.60%
{
	long bytesLoaded;
	long readSoFar;
	long state;
	long lastCheck;

	state = loadStatus.state;
	readSoFar = loadStatus.currentQueueFile.readCurSize;
	lastCheck = loadStatus.lastCheckPos;
	bytesLoaded = readSoFar - lastCheck;

	loadStatus.lastCheckPos = readSoFar;

	if (bytesLoaded != 0 && loadStatus.currentQueueFile.checksumType != 0)
	{
#if !defined(__EMSCRIPTEN__) && !(defined(_WINDOWS) && defined(_DEBUG)) && !defined(__APPLE__) && !defined(__linux__)///@FIXME crash!
		LOAD_UpdateCheckSum(bytesLoaded);
#endif
	}

	if (state == 5)
	{
#if defined(_DEBUG) || defined(__EMSCRIPTEN__)//Disable checksum
		if (loadStatus.currentQueueFile.checksumType != 0 && 0)
#else
		if (loadStatus.currentQueueFile.checksumType != 0 && loadStatus.checksum != loadStatus.currentQueueFile.checksum)
#endif
		{
			loadStatus.currentQueueFile.readStatus = 7;
		}
		else
		{
			loadStatus.currentQueueFile.readStatus = 0;

			if (loadStatus.currentDirLoading != 0)
			{
				loadStatus.currentDirLoading = 0;
				MEMPACK_SetMemoryDoneStreamed((char*)loadStatus.bigFile.currentDir);
			}
		}
	}
}

void LOAD_DoCDBufferedReading()
{ 
	if (loadStatus.state == 5)
	{
		loadStatus.currentQueueFile.readStatus = 0;
	}
}

void LOAD_SetupFileToDoCDReading()
{
#if defined(PSX_VERSION) || (PSXPC_VERSION)
#define CD_SECTOR_LENGTH 2048

	CdlLOC loc;

	loadStatus.currentQueueFile.readStatus = 3;
	loadStatus.checksum = 0;
	loadStatus.lastCheckPos = 0;
	loadStatus.state = 1;
	loadStatus.currentQueueFile.readCurDest = loadStatus.currentQueueFile.readStartDest;
	loadStatus.checkAddr = (long*)loadStatus.currentQueueFile.readStartDest;

	if (loadStatus.currentQueueFile.readStartPos < 0)
	{
		loadStatus.currentSector = loadStatus.bigFile.bigfileBaseOffset + ((loadStatus.currentQueueFile.readStartPos + (CD_SECTOR_LENGTH - 1)) >> 11);
	}
	else
	{
		loadStatus.currentSector = loadStatus.bigFile.bigfileBaseOffset + (loadStatus.currentQueueFile.readStartPos >> 11);
	}

#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
	extern void Emulator_OpenRead(char* fileName, void* buff, int size);
	Emulator_OpenRead(loadHead ? &loadHead->loadEntry.fileName[1] : &loadTail->loadEntry.fileName[1], loadStatus.currentQueueFile.readStartDest, loadStatus.currentQueueFile.readSize);
	loadStatus.bytesTransferred = (loadStatus.currentQueueFile.readSize - loadStatus.currentQueueFile.readCurSize);
	
	loadStatus.state = 4;
	LOAD_CdDataReady();
#elif defined(PSXPC_VERSION) && defined(NO_CD)
	PClseek(loadStatus.bigFile.bigfileFileHandle, loadStatus.currentQueueFile.readStartPos, 0);
	PCread(loadStatus.bigFile.bigfileFileHandle, (char*)loadStatus.currentQueueFile.readStartDest, loadStatus.currentQueueFile.readSize);
	loadStatus.bytesTransferred = (loadStatus.currentQueueFile.readSize - loadStatus.currentQueueFile.readCurSize);
	loadStatus.state = 4;
	LOAD_CdDataReady();
#else
	CdIntToPos(loadStatus.currentSector, &loc);
	CdControl(CdlReadN, &loc.minute, NULL);
#endif
	loadStatus.cdWaitTime = (long)TIMER_GetTimeMS();
#endif
}


void LOAD_SetupFileToDoBufferedCDReading()
{
#if defined(PSX_VERSION) || (PSXPC_VERSION)
	CdlLOC loc;

	loadStatus.currentQueueFile.readStatus = 6;
	loadStatus.checksum = 0;
	loadStatus.state = 1;
	loadStatus.checkAddr = (long*)loadStatus.currentQueueFile.readStartDest;

	if (loadStatus.currentQueueFile.readStartPos < 0)
	{
		loadStatus.currentSector = loadStatus.bigFile.bigfileBaseOffset + ((loadStatus.currentQueueFile.readStartPos + (CD_SECTOR_LENGTH - 1)) >> 11);
	}
	else
	{
		loadStatus.currentSector = loadStatus.bigFile.bigfileBaseOffset + (loadStatus.currentQueueFile.readStartPos >> 11);
	}

#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
	extern void Emulator_OpenRead(char* fileName, void* buff, int size);
	Emulator_OpenRead(loadHead ? &loadHead->loadEntry.fileName[1] : &loadTail->loadEntry.fileName[1], loadStatus.currentQueueFile.readStartDest, loadStatus.currentQueueFile.readSize);
	loadStatus.bytesTransferred = (loadStatus.currentQueueFile.readSize - loadStatus.currentQueueFile.readCurSize);

	loadStatus.state = 4;
	LOAD_CdDataReady();

#elif defined(PSXPC_VERSION) && defined(NO_CD)
	PClseek(loadStatus.bigFile.bigfileFileHandle, loadStatus.currentQueueFile.readStartPos, 0);
	PCread(loadStatus.bigFile.bigfileFileHandle, (char*)loadStatus.currentQueueFile.readStartDest, loadStatus.currentQueueFile.readSize);
	loadStatus.bytesTransferred = (loadStatus.currentQueueFile.readSize - loadStatus.currentQueueFile.readCurSize);
	loadStatus.state = 4;
	LOAD_CdDataReady();
#else
	CdIntToPos(loadStatus.currentSector, &loc);
	CdControl(CdlReadN, &loc.minute, NULL);
#endif
	loadStatus.cdWaitTime = (long)TIMER_GetTimeMS();
#endif
}

void LOAD_ProcessReadQueue()
{
#if defined(PSX_VERSION) || (PSXPC_VERSION)
	long cdWaitTimeDiff;
	CdlLOC loc;

	if (gameTrackerX.debugFlags < 0)
	{
		FONT_Print("CD St %d LS %d sk %d tm %d rd %d cs %d\n", loadStatus.currentQueueFile.readStatus, loadStatus.state, loadStatus.seekTime, loadStatus.sectorTime, loadStatus.currentQueueFile.readCurSize);
	}

	if (loadStatus.currentQueueFile.readStatus == 3)
	{
		LOAD_DoCDReading();
	}
	else if (loadStatus.currentQueueFile.readStatus == 1)
	{
		LOAD_SetupFileToDoCDReading();
	}
	else if (loadStatus.currentQueueFile.readStatus == 5)
	{
		LOAD_SetupFileToDoBufferedCDReading();
	}
	else if (loadStatus.currentQueueFile.readStatus == 6)
	{
		LOAD_DoCDBufferedReading();
	}

	if (loadStatus.currentQueueFile.readStatus == 7)
	{
		loadStatus.currentQueueFile.readStatus = 1;
	}
	else if(loadStatus.cdWaitTime != 0)
	{
		cdWaitTimeDiff = (long)TIMER_GetTimeMS() - loadStatus.cdWaitTime;

		if(cdWaitTimeDiff >= 8400)
		{
			if (loadStatus.currentQueueFile.readStatus == 3)
			{
				loadStatus.state = 0;
				CdReset(0);
				LOAD_InitCdStreamMode();
#if defined(PSXPC_VERSION)
				loadStatus.state = 5;
#else
				loadStatus.state = 1;
#endif
				CdIntToPos(loadStatus.currentSector, &loc);
				CdControl(CdlReadN, &loc.minute, NULL);
				loadStatus.cdWaitTime = (long)TIMER_GetTimeMS();
			}
			else if (loadStatus.currentQueueFile.readStatus == 6)
			{
				if (VOICEXA_IsPlaying() == 0)
				{
					CdControlF(CdlPause, NULL);
				}

				loadStatus.cdWaitTime = 0;
			}
		}
	}
#endif
}


#ifndef PC_VERSION
char* LOAD_ReadFileFromCD(char* filename, int memType)
{ 
#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
	char* readBuffer;
	extern void Emulator_GetFileSize(const char* filePath, int* outSize);
	extern void Emulator_OpenReadFP(const char* filePath, void* buff, int size);

	int fileSize = 0;
	
	Emulator_GetFileSize(filename, &fileSize);	
	readBuffer = MEMPACK_Malloc(fileSize, memType);
	if (readBuffer != NULL)
	{
		Emulator_OpenReadFP(filename, readBuffer, fileSize);
		return readBuffer;
	}

    return NULL;
#else
#if defined(PSXPC_VERSION) && defined(NO_CD)
#if defined(_WIN64) || defined(_WIN32) || defined(__EMSCRIPTEN__) || defined(PLATFORM_NX_ARM) || defined(__ANDROID__) || defined(SN_TARGET_PSP2) || defined(__APPLE__) || defined(__linux__)
	FILE* fp;
#else
	long fp;
#endif

	int i;
	char* readBuffer;
	long fileSize;

	fileSize = 0;
	i = 0;
	
	do
	{
		fp = PCopen(filename, 0, 0);

#if defined(_WIN64) || defined(_WIN32) || defined(__EMSCRIPTEN__) || defined(PLATFORM_NX_ARM) || defined(__ANDROID__) || defined(SN_TARGET_PSP2) || defined(__APPLE__) || defined(__linux__)
		if (fp != (FILE*)-1)
#else
		if (fp != -1)
#endif
		{
			PCinit();
			break;
		}

	} while (++i < 10);


	if (i != 10)
	{
		fileSize = PClseek(fp, 0, 2);
		PClseek(fp, 0, 0);

		readBuffer = MEMPACK_Malloc(fileSize, memType);
		if (readBuffer != NULL)
		{
			PCread(fp, readBuffer, fileSize);
			PCclose(fp);
			return readBuffer;
		}
	}

	return NULL;
#else
	CdlFILE fp;
	int i;
	char *readBuffer;

	i = 0;
	do
	{
		if (CdSearchFile(&fp, filename) != 0)
		{
			CdReset(0);
			break;
		}
	
	} while (++i < 10);
	
	if (i != 10)
	{
		readBuffer = MEMPACK_Malloc(fp.size, memType);
		if (readBuffer != NULL)
		{	
			loadStatus.currentQueueFile.readCurSize = 0;
			loadStatus.currentQueueFile.readStartDest = readBuffer;
			loadStatus.currentQueueFile.readStatus = 1;
			loadStatus.currentQueueFile.checksumType = 0;
			loadStatus.currentQueueFile.checksum = 0;
			loadStatus.currentQueueFile.readSize = fp.size;
			loadStatus.currentQueueFile.readStartPos = (CdPosToInt(&fp.pos) - loadStatus.bigFile.bigfileBaseOffset) << 11;

			do
			{
				LOAD_ProcessReadQueue();

			} while (LOAD_IsFileLoading() != 0);

			CdControlF(CdlPause, NULL);
			return readBuffer;
		}
	}

	return NULL;
#endif
#endif
}
#endif

#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
void LOAD_CdReadFromBigFile(long fileOffset, unsigned int* loadAddr, long bytes, long chksumLevel, long checksum, long fileHash)
#else
void LOAD_CdReadFromBigFile(long fileOffset, unsigned int* loadAddr, long bytes, long chksumLevel, long checksum)
#endif
{
	loadStatus.currentQueueFile.readSize = bytes;
	loadStatus.currentQueueFile.readCurSize = 0;
	loadStatus.currentQueueFile.readStartDest = loadAddr;
	loadStatus.currentQueueFile.readStartPos = fileOffset;
	loadStatus.currentQueueFile.readStatus = 1;
	loadStatus.currentQueueFile.checksumType = chksumLevel;
	loadStatus.currentQueueFile.checksum = checksum;
#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
	loadStatus.currentQueueFile.fileHash = fileHash;
#endif
}

struct _BigFileDir * LOAD_ReadDirectory(struct _BigFileDirEntry *dirEntry)
{
#if defined(PSX_VERSION) || (PSXPC_VERSION)
	struct _BigFileDir* dir;
	long sizeOfDir;
	
	sizeOfDir = (dirEntry->numFiles * sizeof(struct _BigFileEntry)) + sizeof(long);
	dir = (struct _BigFileDir*)MEMPACK_Malloc(sizeOfDir, 0x2C);
	
#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
	LOAD_CdReadFromBigFile(dirEntry->subDirOffset, (unsigned int*)dir, sizeOfDir, 0, 0, 0);
#else
	LOAD_CdReadFromBigFile(dirEntry->subDirOffset, (unsigned int*)dir, sizeOfDir, 0, 0);
#endif

	return dir;
#else
	return NULL;
#endif
}

#ifndef PC_VERSION
void LOAD_InitCdLoader(char *bigFileName, char *voiceFileName)
{
#if !defined(_DEBUG)  && !defined(__EMSCRIPTEN__) || defined(NO_FILESYSTEM)
#if defined(PSXPC_VERSION) && defined(NO_CD)
	CdlFILE fp;
	long i;
	char* ptr;

	loadStatus.state = 0;

	for (i = 0; i < 10; i++)
	{
		loadStatus.bigFile.bigfileFileHandle = PCopen(bigFileName, 0, 0);
#if defined(_WIN64) || defined(_WIN32) || defined(__EMSCRIPTEN__) || defined(PLATFORM_NX_ARM) || defined(__ANDROID__) || defined(SN_TARGET_PSP2) || defined(__APPLE__) || defined(__linux__)
		if (loadStatus.bigFile.bigfileFileHandle != (FILE*) -1)
#else
		if (loadStatus.bigFile.bigfileFileHandle != -1)
#endif
		{
			break;
		}

		PCinit();
	}

	if (i != 10)
	{
		loadStatus.bigFile.bigfileBaseOffset = 0;
		loadStatus.cdWaitTime = 0;
		loadStatus.currentQueueFile.readStatus = 0;
		loadStatus.bigFile.currentDir = NULL;
		loadStatus.bigFile.currentDirID = 0;
		loadStatus.bigFile.cachedDir = NULL;
		loadStatus.bigFile.cachedDirID = 0;
		loadStatus.bigFile.searchDirID = 0;

#if defined(_DEBUG) && !defined(NO_FILESYSTEM)  || defined(__EMSCRIPTEN__)
		LOAD_CdReadFromBigFile(0, (unsigned long*)&loadStatus.bigFile.numSubDirs, sizeof(loadStatus.bigFile.numSubDirs), 0, 0, 0);
#else
		LOAD_CdReadFromBigFile(0, (unsigned int*)&loadStatus.bigFile.numSubDirs, sizeof(long), 0, 0);
#endif

		do
		{
			LOAD_ProcessReadQueue();

		} while (LOAD_IsFileLoading() != 0);


		ptr = MEMPACK_Malloc((loadStatus.bigFile.numSubDirs << 3) + 4, 8);
#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
		LOAD_CdReadFromBigFile(0, (unsigned long*)ptr, (loadStatus.bigFile.numSubDirs << 3) + 4, 0, 0, 0);
#else
		LOAD_CdReadFromBigFile(0, (unsigned int*)ptr, (loadStatus.bigFile.numSubDirs << 3) + 4, 0, 0);
#endif

		ptr += 4;
		loadStatus.bigFile.subDirList = (struct _BigFileDirEntry*)ptr;

		do
		{
			LOAD_ProcessReadQueue();

		} while (LOAD_IsFileLoading() != 0);

		loadStatus.bigFile.rootDir = LOAD_ReadDirectory(loadStatus.bigFile.subDirList);

		do
		{
			LOAD_ProcessReadQueue();

		} while (LOAD_IsFileLoading() != 0);
	}
#else
	CdlFILE fp;
	long i;
	char *ptr;

	loadStatus.state = 0;

	for(i = 0; i < 10; i++)
	{
		if (CdSearchFile(&fp, bigFileName) != NULL)
		{
			break;
		}

		CdReset(0);
	}

	if (i != 10)
	{
		LOAD_InitCdStreamMode();
		loadStatus.bigFile.bigfileBaseOffset = CdPosToInt(&fp.pos);
		loadStatus.cdWaitTime = 0;
		loadStatus.currentQueueFile.readStatus = 0;
		loadStatus.bigFile.currentDir = NULL;
		loadStatus.bigFile.currentDirID = 0;
		loadStatus.bigFile.cachedDir = NULL;
		loadStatus.bigFile.cachedDirID = 0;
		loadStatus.bigFile.searchDirID = 0;

		LOAD_CdReadFromBigFile(0, (unsigned long*)&loadStatus.bigFile.numSubDirs, sizeof(loadStatus.bigFile.numSubDirs), 0, 0);
		
		do
		{
			LOAD_ProcessReadQueue();
		
		} while (LOAD_IsFileLoading() != 0);
		

		CdControlF(CdlPause, NULL);
		ptr = MEMPACK_Malloc((loadStatus.bigFile.numSubDirs << 3) + 4, 8);

		CdSync(0, NULL);

		LOAD_CdReadFromBigFile(0, (unsigned long*)ptr, (loadStatus.bigFile.numSubDirs << 3) + 4, 0, 0);
		ptr += 4;
		loadStatus.bigFile.subDirList = (struct _BigFileDirEntry*)ptr;
		
		do
		{
			LOAD_ProcessReadQueue();

		} while (LOAD_IsFileLoading() != 0);

		loadStatus.bigFile.rootDir = LOAD_ReadDirectory(loadStatus.bigFile.subDirList);

		do
		{
			LOAD_ProcessReadQueue();

		} while (LOAD_IsFileLoading() != 0);
	}
#endif
#endif
}

#endif

int LOAD_SetupFileInfo(struct _NonBlockLoadEntry* loadEntry)
{ 
#if defined(PSX_VERSION)
	struct _BigFileEntry *fileInfo;

#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
	fileInfo = LOAD_GetBigFileEntry(loadEntry->fileName);
#else
	fileInfo = LOAD_GetBigFileEntryByHash(loadEntry->fileHash);
#endif
	if (fileInfo == NULL)
	{
		if (loadEntry->dirHash == loadStatus.bigFile.currentDirID)
		{
			DEBUG_FatalError("CD ERROR: File %s does not exist\n", &loadEntry->fileName[0]);
		}
		
		return 0;
	}

	loadEntry->filePos = fileInfo->filePos;
	loadEntry->loadSize = fileInfo->fileLen;
	loadEntry->checksum = fileInfo->checkSumFull;
#endif

	return 1;
}

void LOAD_CD_ReadPartOfFile(struct _NonBlockLoadEntry *loadEntry)
{ 
	struct _ReadQueueEntry *currentQueueReq;

	if (LOAD_SetupFileInfo(loadEntry) != 0)
	{
		currentQueueReq = &loadStatus.currentQueueFile;
		currentQueueReq->readCurSize = 0;
		currentQueueReq->readSize = loadEntry->loadSize;
		currentQueueReq->readStartDest = loadEntry->loadAddr;
		currentQueueReq->readCurDest = loadEntry->loadAddr;
		currentQueueReq->readStatus = 5;
		currentQueueReq->checksumType = 0;
		loadStatus.currentQueueFile.readStartPos = loadEntry->filePos;
		currentQueueReq->checksum = loadEntry->checksum;
		currentQueueReq->retFunc = loadEntry->retFunc;
		currentQueueReq->retData = loadEntry->retData;
		loadStatus.changeDir = 0;
		currentQueueReq->retData2 = loadEntry->retData2;
#if defined(_DEBUG) || defined(__EMSCRIPTEN__)
		currentQueueReq->fileHash = loadEntry->fileHash;
#endif
	}
	else
	{
		loadStatus.changeDir = 1;
	}
}

long LOAD_HashName(char *string)
{ 
	long sum;
	long _xor;	// visual studio doesn't like 'xor' for whatever reason
	long length;
	long ext;
	char c;
	long strl;
	long endPos;
	long i;
	char* pos;

	sum = 0;
	_xor = 0;
	length = 0;
	ext = 0;

	strl = strlen(string) - 1;
	pos = strchr(string, '.');
	endPos = 0;

	if (pos != NULL)
	{
		pos++;
		
		for (i = 0; i < 7; i++)
		{
#if defined(PSXPC_VERSION)
			if (_strcmpi(pos, &HashExtensions[i][0]) == 0)
#else
			if (strcmpi(pos, &HashExtensions[i][0]) == 0)
#endif
			{
				ext = i;
				break;
			}
		}

		if (i < 7)
		{
			strl -= 4;
		}

		if (strl >= endPos)
		{
			for(; strl >= endPos; strl--)
			{
				c = string[strl];
				
				if (c != '\\')
				{
					if ((unsigned)(c - 0x61) < 0x1A)
					{
						c &= 0xDF;
					}

					c = (c - 0x1A) & 0xFF;
					sum = sum + c;
					_xor = _xor ^ (c * length++);
				}
			}
		}
	}

	return (length << 27) | (sum << 15) | (_xor << 3) | ext;
}

long LOAD_HashUnit(char *name)
{
	int val;
	int last;
	int hash;
	int num;
	int flag;
	char* c;

	last = 0;
	hash = 0;
	num = 0;
	flag = 0;

	if (name[0] != 0)
	{
		for (c = name; *c != 0; c++)
		{
			val = *c;

			if (val - 0x30 < 10)
			{
				num = ((num * 2) - 0x30) + val;
			}
			else
			{
				if (val - 0x41 < 0x1A)
				{
					val -= 0x41;
				}
				else
				{
					val -= 0x61;
				}

				if (flag != 0)
				{
					hash = (hash << 2) + ((val - last) * 32);
					flag ^= 1;
					last = val;
				}
				else
				{
					hash = (hash << 2) + (val - last);
					flag ^= 1;
					last = val;
				}
			}
		}
	}

	hash += num;
	return (hash << 16) >> 16;
}

struct _BigFileEntry* LOAD_GetBigFileEntryByHash(long hash) // Matching - 100%
{
	int i;
	struct _BigFileEntry* entry;

	if ((loadStatus.bigFile.currentDir != NULL) && (loadStatus.currentDirLoading == 0))
	{
		for (i = loadStatus.bigFile.currentDir->numFiles, entry = &loadStatus.bigFile.currentDir->fileList[0]; i != 0; i--, entry++)
		{
			if (entry->fileHash == hash)
			{
				return entry;
			}
		}
	}

	for (i = loadStatus.bigFile.rootDir->numFiles, entry = &loadStatus.bigFile.rootDir->fileList[0]; i != 0; i--, entry++)
	{
		if (entry->fileHash == hash)
		{
			return entry;
		}
	}

	return NULL;
}

struct _BigFileEntry* LOAD_GetBigFileEntry(char* fileName)
{
#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
	static struct _BigFileEntry bigFileEntry;

	int fileSize = 0;
	Emulator_GetFileSize(&fileName[1], &fileSize);

	if (fileSize == 0)
	{
		return NULL;
	}

	bigFileEntry.checkSumFull = 0;
	bigFileEntry.fileHash = LOAD_HashName(fileName);
	bigFileEntry.fileLen = fileSize;
	bigFileEntry.filePos = 0;

	return &bigFileEntry;
#else
	return LOAD_GetBigFileEntryByHash(LOAD_HashName(fileName));
#endif
}

#ifndef PC_VERSION
long LOAD_DoesFileExist(char* fileName) // Matching - 100%
{
	struct _BigFileEntry* entry;
	int temp;  // not from SYMDUMP

	entry = LOAD_GetBigFileEntry(fileName);

	temp = 0;

	if (entry != NULL)
	{
		temp = entry->fileLen != 0;
	}

	return temp;
}
#endif

void LOAD_NonBlockingReadFile(struct _NonBlockLoadEntry* loadEntry)
{ 
	if (LOAD_SetupFileInfo(loadEntry) != 0)
	{
		if (loadEntry->loadAddr == NULL)
		{
			loadEntry->loadAddr = (long*)MEMPACK_Malloc(loadEntry->loadSize, (unsigned char)loadEntry->memType);
		}

#if defined(_DEBUG) && !defined(NO_FILESYSTEM) || defined(__EMSCRIPTEN__)
		LOAD_CdReadFromBigFile(loadEntry->filePos, (unsigned int*)loadEntry->loadAddr, loadEntry->loadSize, loadEntry->checksumType, loadEntry->checksum, loadEntry->fileHash);
#else
		LOAD_CdReadFromBigFile(loadEntry->filePos, (unsigned int*)loadEntry->loadAddr, loadEntry->loadSize, loadEntry->checksumType, loadEntry->checksum);
#endif
		loadStatus.changeDir = 0;
	}
	else
	{
		loadStatus.changeDir = 1;
	}
}

#ifndef PC_VERSION
void LOAD_LoadTIM(int *addr, long x_pos, long y_pos, long clut_x, long clut_y)
{ 
	PSX_RECT rect;
	int *clutAddr;

	addr += 2;
	clutAddr = NULL;
	
	if (addr[-1] == 8)
	{
		clutAddr = addr + 3;
		addr += 11;
	}
    
	rect.x = (short)x_pos;
	rect.y = (short)y_pos;
	rect.w = ((unsigned short*)addr)[4];
	rect.h = ((unsigned short*)addr)[5];

	LoadImage(&rect, (unsigned int*)(addr + 3));

	if (clutAddr != NULL)
	{
		rect.x = (short)clut_x;
		rect.y = (short)clut_y;
		rect.w = 16;
		rect.h = 1;
		DrawSync(0);
		LoadImage(&rect, (unsigned int*)clutAddr);
	}
}
#endif

#ifndef PC_VERSION
void LOAD_LoadTIM2(int* addr, long x_pos, long y_pos, long width, long height) // Matching - 100%
{
	PSX_RECT rect;

	rect.x = (short)x_pos;
	rect.y = (short)y_pos;

	addr += 2;

	rect.w = ((unsigned short*)addr)[4];
	rect.h = ((unsigned short*)addr)[5];

	LoadImage(&rect, (unsigned int*)addr + 3);

	DrawSync(0);
}
#endif

long LOAD_RelocBinaryData(long* data, long fileSize) // Matching - 100%
{
	long* lastMoveDest;
	long tableSize;
	struct RedirectList redirectListX;
	struct RedirectList* redirectList;

	fileSize = ((fileSize + 3) >> 2);

	redirectList = &redirectListX;

	redirectList->data = (long*)data + 1;

	redirectList->numPointers = data[0];

	tableSize = ((redirectList->numPointers + 512) / 512) * 512;

	RESOLVE_Pointers(redirectList, (long*)&data[tableSize], (long*)data);

	lastMoveDest = &data[fileSize - (tableSize)];

	while (data < lastMoveDest)
	{
		*data++ = data[tableSize];
	}

	return (tableSize * 4);
}

void LOAD_CleanUpBuffers()
{ 
	if (loadStatus.buffer1 != NULL)
	{
		MEMPACK_Free((char*)loadStatus.buffer1);
		loadStatus.buffer1 = NULL;
	}

	if (loadStatus.buffer2 != NULL)
	{
		MEMPACK_Free((char*)loadStatus.buffer2);
		loadStatus.buffer2 = NULL;
	}
}

void* LOAD_InitBuffers()
{
#if defined(PSXPC_VERSION)
	loadStatus.buffer1 = MEMPACK_Malloc(2048*256, 0x23);
	loadStatus.buffer2 = MEMPACK_Malloc(2048*256, 0x23);
#else
	loadStatus.buffer1 = MEMPACK_Malloc(2048, 0x23);
	loadStatus.buffer2 = MEMPACK_Malloc(2048, 0x23);
#endif
	return loadStatus.buffer1;
}

void LOAD_InitCdStreamMode()
{ 
#if defined(PSX_VERSION) || (PSXPC_VERSION)
	unsigned char cdMode = CdlModeSize1 | CdlModeSpeed;
	CdReadyCallback(&LOAD_CdReadReady);
	CdSyncCallback(&LOAD_CdSeekCallback);
	CdControl(CdlSetmode, &cdMode, NULL);
#endif
}

void LOAD_DumpCurrentDir()
{
	if (loadStatus.bigFile.currentDir != NULL)
	{
		MEMPACK_Free((char*)loadStatus.bigFile.currentDir);
		loadStatus.bigFile.currentDir = NULL;
		loadStatus.bigFile.currentDirID = 0;
	}

	if (loadStatus.bigFile.cachedDir != NULL)
	{
		MEMPACK_Free((char*)loadStatus.bigFile.cachedDir);
		loadStatus.bigFile.cachedDir = NULL;
		loadStatus.bigFile.cachedDirID = 0;
	}
}

int LOAD_ChangeDirectoryByID(int id) // Matching - 100%
{
	int i;
	struct _BigFileDir* dir;
	struct _BigFileDir* temp;  // not from SYMDUMP

	if (id != 0)
	{
		if (loadStatus.bigFile.currentDirID == id)
		{
			return 1;
		}
		if (loadStatus.bigFile.cachedDirID == id)
		{
			temp = loadStatus.bigFile.cachedDir;
			loadStatus.bigFile.cachedDirID = loadStatus.bigFile.currentDirID;
			loadStatus.bigFile.currentDirID = id;
			loadStatus.bigFile.cachedDir = loadStatus.bigFile.currentDir;
			loadStatus.bigFile.currentDir = temp;
			return 1;
		}
		else
		{
			for (i = 0; i < loadStatus.bigFile.numSubDirs; i++)
			{
				if (id == loadStatus.bigFile.subDirList[i].streamUnitID)
				{
					if (loadStatus.bigFile.cachedDir != NULL)
					{
						MEMPACK_Free((char*)loadStatus.bigFile.cachedDir);
					}

					loadStatus.currentDirLoading = 1;
					loadStatus.bigFile.cachedDirID = loadStatus.bigFile.currentDirID;
					loadStatus.bigFile.cachedDir = loadStatus.bigFile.currentDir;

					dir = LOAD_ReadDirectory(&loadStatus.bigFile.subDirList[i]);
					loadStatus.bigFile.currentDir = dir;
					MEMPACK_SetMemoryBeingStreamed((char*)dir);
					loadStatus.bigFile.currentDirID = id;
					return 1;
				}
			}
		}
	}

	return 0;
}

void LOAD_SetSearchDirectory(long id)
{ 
	loadStatus.bigFile.searchDirID = id;
}

long LOAD_GetSearchDirectory()
{
	return loadStatus.bigFile.searchDirID;
}

int LOAD_ChangeDirectoryFlag()
{
	return loadStatus.changeDir;
}

void LOAD_UpdateBigFilePointers(struct _BigFileDir *oldDir, struct _BigFileDir *newDir)
{ 
	if (loadStatus.bigFile.currentDir == oldDir)
	{
		loadStatus.bigFile.currentDir = newDir;
	}

	if (loadStatus.bigFile.cachedDir == oldDir)
	{
		loadStatus.bigFile.cachedDir = newDir;
	}
}

int LOAD_IsFileLoading() // Matching - 100%
{
	return loadStatus.currentQueueFile.readStatus != 0;
}

void LOAD_StopLoad()
{
	loadStatus.state = 5;
	loadStatus.currentQueueFile.readStatus = 0;

	if (loadStatus.currentDirLoading != 0)
	{
		loadStatus.currentDirLoading = 0;
	}
}
