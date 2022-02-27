#include "../core.h"
#include "async.h"
#include "../STREAM.H"
#include "../STRMLOAD.H"
#include "d3d/d3d.h"

typedef struct BigFileEntryPC
{
	u_long hash,
		len,
		pos;
	char name[4];
} BigFileEntryPC;

HANDLE bigfileid;
DWORD bigfilecount;
BigFileEntryPC* bigfiledata;

extern void GXFilePrint(const char* fmt, ...);

//0001:00079650       _PCLOAD_RelocBinaryData    0047a650 f   pcload.obj [unused]
//0001:000796d0       _LOAD_GetBigFileFileIndex  0047a6d0 f   pcload.obj [unused]
//0001:00079730       _STREAM_IsCdBusy           0047a730 f   pcload.obj
int STREAM_IsCdBusy(long* numberInQueue)
{
	int IsBusy; // eax

	IsBusy = ASLD_IsBusy();
	if (numberInQueue)
		*numberInQueue = IsBusy;
	return IsBusy != 0;
}
//0001:00079750       _STREAM_PollLoadQueue      0047a750 f   pcload.obj
int STREAM_PollLoadQueue()
{
	int IsBusy = ASLD_IsBusy();

	return IsBusy;
}
//0001:00079890       _LOAD_NonBlockingBinaryLoad 0047a890 f   pcload.obj
void LOAD_NonBlockingBinaryLoad(char* fileName, void* retFunc, void* retData, void* retData2, void** retPointer, int memType)
{
	STREAM_QueueNonblockingLoads(fileName, memType, retFunc, retData, retData2, retPointer, 1);
}

//0001:000798c0       _STREAM_QueueNonblockingLoads 0047a8c0 f   pcload.obj
//0001:00079a00       _StreamloadCallback        0047aa00 f   pcload.obj

//0001:00079a20       _LOAD_NonBlockingFileLoad  0047aa20 f   pcload.obj+
void LOAD_NonBlockingFileLoad(char* fileName, void* retFunc, void* retData, void* retData2, void** retPointer, int memType)
{
	STREAM_QueueNonblockingLoads(fileName, memType, retFunc, retData, retData2, retPointer, 0);
}
//0001:00079a50       _LOAD_NonBlockingPartOfFileLoad 0047aa50 f   pcload.obj [unused]
//0001:00079a80       _LOAD_LoadTIM2             0047aa80 f   pcload.obj
void LOAD_LoadTIM2(long* addr, long x_pos, long y_pos, long width, long height)
{
	D3DSHL_Blit((DWORD*)&addr[5], addr[4] & 0xffff, addr[4] >> 16, x_pos, y_pos);
}
//0001:00079ab0       _LOAD_LoadTIM              0047aab0 f   pcload.obj
void LOAD_LoadTIM(void* data, int x, int y, int w, int h) {}	// does nothing on PC
//0001:00079ac0       _LOAD_ReadFile_FROM_HD     0047aac0 f   pcload.obj
void* LOAD_ReadFile_FROM_HD(const char* filename, u_char memType)
{
	int fp = PCopen(filename, 0, 0);
	if (fp == -1)
		D3D_FailAbort("Can't open file: %s", filename);

	size_t size = PClseek(fp, 0, SEEK_END);
	u_char* buffer = (u_char*)MEMPACK_Malloc(size, memType);
	if (buffer == NULL)
		GXFilePrint("Null dest. for fileload: not enough mem\n");
	PClseek(fp, 0, SEEK_SET);
	if(PCread(fp, buffer, size) != size)
		GXFilePrint("Failed to read all of file %s\n", filename);
	PCclose(fp);

	return buffer;
}
//0001:00079b60       _LOAD_ReadFile             0047ab60 f   pcload.obj
long* LOAD_ReadFile(const char* filename, u_char memType)
{
	u_long hash = LOAD_HashName(filename);
	u_long sig = LOAD_SigLetters(filename);

	int i;
	for (i = 0; i < bigfilecount; i++)
		if (bigfiledata[i].hash == hash && *(u_long*)&bigfiledata[i].name == sig)
			break;

	if (i == bigfilecount)
		D3D_FailAbort("Error");

	int fp = ASLD_GetFileDataFP(bigfileid, bigfiledata[i].pos, bigfiledata[i].len);
	void *ptr = MEMPACK_Malloc(bigfiledata[i].len, memType);
	memcpy(ptr, (void*)fp, bigfiledata[i].len);
	ASLD_Free((HGLOBAL)fp);

	return (long*)ptr;
}
//0001:00079c10       _LOAD_HashName             0047ac10 f   pcload.obj
//0001:00079ce0       _LOAD_SigLetters           0047ace0 f   pcload.obj
u_long LOAD_SigLetters(const char* filename)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (filename[i] == '.')
			break;
	}

	if (i < 4)
		return 0xCAFEDEAD;

	BYTE ch0, ch1, ch2, ch3;

	ch0 = filename[i - 4];
	if (ch0 >= 'a' && ch0 <= 'z') ch0 &= ~0x20;
	ch1 = filename[i - 3];
	if (ch1 >= 'a' && ch1 <= 'z') ch1 &= ~0x20;
	ch2 = filename[i - 2];
	if (ch2 >= 'a' && ch2 <= 'z') ch2 &= ~0x20;
	ch3 = filename[i - 1];
	if (ch3 >= 'a' && ch3 <= 'z') ch3 &= ~0x20;

	return ch0 | (ch1 << 8) | (ch2 << 16) | (ch3 << 24);
}
//0001:00079d70       _LOAD_DoesFileExist        0047ad70 f   pcload.obj
int LOAD_DoesFileExist(const char* filename)
{
	u_long hash = LOAD_HashName(filename);
	u_long sig = LOAD_SigLetters(filename);

	int i;
	for (i = 0; i < bigfilecount; i++)
		if (bigfiledata[i].hash == hash && *(u_long*)&bigfiledata[i].name == sig)
			break;

	if (i == bigfilecount)
		return 0;
	return bigfiledata[i].len;
}
//0001:00079dd0       _LOAD_NonBlockingBufferedLoad 0047add0 f   pcload.obj
void __cdecl LOAD_NonBlockingBufferedLoad(char* fileName, void* retFunc, void* retData, void* retData2)
{}
//0001:00079de0       _LOAD_InitCdLoader         0047ade0 f   pcload.obj
void LOAD_InitCdLoader(const char* filename)
{
	bigfileid = ASLD_OpenFile(filename);
	if (bigfileid == INVALID_HANDLE_VALUE)
		D3D_FailAbort("Can't load big file named \"%s\"\n", filename);
	ASLD_ReadFile(bigfileid, &bigfilecount, 4);
	GXFilePrint("Num of files in big file %d\n", bigfilecount);
	bigfiledata = (BigFileEntryPC*)MEMPACK_Malloc(sizeof(BigFileEntryPC) * bigfilecount, 8);
	ASLD_ReadFile(bigfileid, bigfiledata, sizeof(BigFileEntryPC) * bigfilecount);
}
//0001:00079e60       _LOAD_RelocateStreamPointers 0047ae60 f   pcload.obj [unused]
//0001:00079eb0       _LOAD_PlayXA               0047aeb0 f   pcload.obj
void __cdecl LOAD_PlayXA(int number)
{
	VOICEXA_Play(number, 0);
}
//0001:00079ec0       _LOAD_IsXAInQueue          0047aec0 f   pcload.obj
int LOAD_IsXAInQueue()
{
	return VOICEXA_IsPlaying();
}
