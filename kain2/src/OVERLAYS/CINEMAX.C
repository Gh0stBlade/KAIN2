#include "CORE.H"

#include "CINEMAX.H"

#include "PSX/SUPPORT.H"
#include "MEMPACK.H"

unsigned int unk_E21BC = 0x40000001;//0x800E21BC
unsigned long* scratch1 = getScratchAddr(1048);//0x800E22D4
unsigned long* scratch2 = getScratchAddr(1062);//0x800E22E0
unsigned long* scratch3 = getScratchAddr(1545);//0x800E2300

int buffer_count = 0;//800F3360
BufferInfo buffer_details;//800F330C
int unk_F3350 = 0;//800F3350
int unk_F3354 = 0;//800F3354
int unk_F3358 = 0;//800F3358
int unk_F335C = 0;//800F335C
int unk_F334C = 0;//800F334C

//0x800E0DF8
int CINEMAX_Play(char* strfile, unsigned short mask, int buffers)
{
	return CINEMAX_ActuallyPlay(strfile, mask, buffers);
}

//0x800E0E2C
void CINEMAX_ClearBuffers()
{
	PSX_RECT rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT + 16;

	ClearImage(&rect, 0, 0, 0);

	rect.x = 0;
	rect.y = SCREEN_HEIGHT + 16;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT + 16;
	ClearImage(&rect, 0, 0, 0);
}

//0x800E0E8C
void CINEMAX_VSync()
{

}

//0x800E0F18
int CINEMAX_ActuallyPlay(char* strfile, unsigned short mask, int buffers)
{
	CdlFILE fp;
	SpuCommonAttr attr;
	SpuCommonAttr attr2;
	char* buffer1;
	char* buffer2;
	char* buffer3;
	char* buffer4;
	char* buffer5;
	//s0 = strfile
	//s5 = 0
	//s7 = 0
	//fp = 0
	//s1 = 1

	buffer_count = buffers;
	unk_F334C = 1;

	if (buffer_count < 2)
	{
		buffer_count = 2;
	}

	if (buffer_count >= 3)
	{
		buffer_count = 3;
	}

	unk_F3358 = 0;
	unk_F3350 = 0;
	unk_F3354 = 0;

	VSyncCallback(NULL);
	SetDispMask(0);

	CINEMAX_ClearBuffers();

	if (CdSearchFile(&fp, strfile) == NULL)
	{
		printf("file not found\n");
		return 0;
	}

	unk_F335C = CdPosToInt(&fp.pos) + (fp.size >> 11) - 5;
	SpuGetCommonAttr(&attr);
	SpuGetCommonAttr(&attr2);

#if 0//Audio related
	///@TODO 0x800E101C-0x800E106C
#endif

	buffer1 = MEMPACK_Malloc(0x18000, 0x9);//A4
	buffer2 = MEMPACK_Malloc(0x10000, 0x9);//A8
	buffer3 = MEMPACK_Malloc(0x10000, 0x9);//AC
	buffer4 = MEMPACK_Malloc(0x2D00, 0x9);//B0
	buffer5 = MEMPACK_Malloc(0x2D00, 0x9);//B4

	//a3 = buffer5
	//a0 = buffer2
	//a1 = buffer3
	//a2 = buffer4

	CINEMAX_InitBufferDetails(buffer2, buffer3, buffer4, buffer5, &buffer_details);

	while (CdControlB(CdlSetloc, &fp.pos.minute, NULL) == 0)
	{

	}

	while (CdControlB(CdlSeekL, NULL, NULL) == 0)
	{

	}

	CdSync(0, NULL);

	VSyncCallback(CINEMAX_VSync);

	CINEMAX_InitStream(buffer1, &fp, CINEMAX_Unknown);

	return 0;
}

//0x800E1414
void CINEMAX_InitBufferDetails(char* b1, char* b2, char* b3, char* b4, BufferInfo* bd)
{
	//v1 = 0x100
	bd->unk_22 = 256;//w?
	bd->unk_28 = 480;//h?
	bd->unk_32 = -1;//start frame?
	//v1 = 0x18
	bd->buffer[0] = b1;
	bd->buffer[1] = b2;
	bd->buffer[2] = NULL;
	
	bd->buffer[3] = b3;
	bd->buffer[4] = b4;
	bd->buffer[5] = NULL;

	bd->unk_18 = 0;
	bd->unk_1A = 0;
	bd->unk_20 = 0;
	bd->unk_2A = 0;
	bd->unk_30 = 0;
	bd->unk_34 = 0;
	bd->unk_36 = 0;
	bd->unk_38 = 24;//FPS?
	bd->unk_3C = 0;
}

//0x800E1474
void CINEMAX_InitStream(char* buffer, CdlFILE* fp, void* func)
{
	//s0 = buffer
	//s2 = fp
	//s1 = func
	CINEMAX_Unknown2(0);
}

//0x800E151C
void CINEMAX_Unknown()
{

}

//0x800E1CCC
void CINEMAX_Unknown2(int unk)
{
	if (unk == 0)
	{
		ResetCallback();
		CINEMAX_Unkown3(unk);
	}
}

//0x800E1DC0
void CINEMAX_Unkown3(int unk)
{
	//a1 = unk
	if (unk != 0)
	{
		if (unk == 1)
		{
			//0x800E1E44
		}
	}
	else
	{
		((unsigned int*)scratch3[0])[0] = 0x80000000;
		((unsigned int*)scratch1[0])[0] = 0;
		((unsigned int*)scratch2[0])[0] = 0;
		((unsigned int*)scratch3[0])[0] = 0x60000000;

		CINEMAX_E1EB0(&unk_E21BC, 32);
	}

	//0x800E1E94
}

//0x800E1EB0
void CINEMAX_E1EB0(unsigned int* func, int unk2)
{
	//s0 = unk2

}

//0x800E1FCC
void CINEMAX_E1FCC()
{

}