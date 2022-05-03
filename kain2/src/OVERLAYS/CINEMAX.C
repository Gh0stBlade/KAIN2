#include "CORE.H"

#include "CINEMAX.H"

#include "PSX/SUPPORT.H"
#include "MEMPACK.H"

unsigned int unk_E21BC = 0x40000001;//0x800E21BC

int buffer_count = 0;//800F3360
BufferInfo buffer_details;//800F330C
int unk_F3350 = 0;//800F3350
int unk_F3354 = 0;//800F3354
int unk_F3358 = 0;//800F3358
int unk_F335C = 0;//800F335C
int unk_F334C = 0;//800F334C

//0x800E0DBC
int CINEMAX_E0DBC()
{
	unsigned char result[8];
	CdControlB(CdlGetlocP, NULL, result);

	return CdPosToInt((CdlLOC*)&result[5]) < unk_F335C ^ 1;
}

//0x800E0DF8
int CINEMAX_Play(char* strfile, unsigned short mask, int buffers)
{
#if 0
	return CINEMAX_ActuallyPlay(strfile, mask, buffers);
#else
	return 0;
#endif
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
	unsigned short m;//0xA0
	//s0 = strfile
	//s5 = 0
	//s7 = 0
	//fp = 0
	//s1 = 1

	buffer_count = buffers;
	unk_F334C = 1;
	m = mask;

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
		///temp disabled return 0;
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
	
	if (CINEMAX_E1658(&buffer_details) == -1)
	{
		
	}
	else
	{
		//s1 = &buffer_details
		//s6 = 1
		//s2 = 1
	}

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
	DecDCTReset(0);
	DecDCToutCallback((void(*)())func);
	StSetRing((u_long*)buffer, 48);
	StSetStream(1, 1, 0xFFFFFFFF, NULL, NULL);
	CINEMAX_E190C(fp);
	CINEMAX_E1658(&buffer_details);
}

//0x800E151C
void CINEMAX_Unknown()
{

}

//0x800E1658
int CINEMAX_E1658(BufferInfo* buffer_info)
{
	//s2 = buffer_info
	//s0 = 0x96
	//s3 = 1
	
	for (int s0 = 150; s0 != 0; s0--)
	{
		int s1 = CINEMAX_E1708(buffer_info);

		if (s1 == 0)
		{
			if (unk_F3358 == 1)
			{
				break;
			}
		}
	}

	return 0;
}

int CINEMAX_E1708(BufferInfo* buffer_info)
{
	unsigned long* nextstream;
	StHEADER* header;
	int s0;

	//s1 = buffer_info
	for(s0 = 0x36B0; s0 != 0; s0--)
	{
		if (StGetNext(&nextstream, (unsigned long**)&header) == 0)
		{
			//0xE1754
		}
	} 

	if (CINEMAX_E0DBC() == 0)
	{
		return 0;
	}
	else
	{
		unk_F3358 = 1;
		return 1;
	}
	//0xE1780
}

//0x800E190C
void CINEMAX_E190C(CdlFILE* fp)
{
	unsigned char param;
	
	param = 128;

	do
	{
		while (CdControl(CdlSetloc, (u_char*)fp, NULL) == 0)
		{

		}

		while (CdControl(CdlSetmode, &param, NULL) == 0)
		{

		}

		VSync(3);

	} while(CdRead2(0x1E0) == 0);
}

//0x800E1FCC
void CINEMAX_E1FCC()
{
	//((unsigned int*)scratch3[0])[0] = 0x100000;

}