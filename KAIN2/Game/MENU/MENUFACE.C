#include "Game/CORE.H"
#include "Game/GAMELOOP.H"
#include "MENUFACE.H"
#include "Game/DRAW.H"
#include "Game/STRMLOAD.H"
#include "Game/MEMPACK.H"

int hack_initialized;
struct menuface_t MenuFaces[8] = { 236, 49,  48, 48, 2, -1, 0, 0, 0,
								   268, 97,  48, 48, 2, -1, 0, 0, 0,
								   273, 156, 48, 48, 2, -1, 0, 0, 0,
								   326, 49,  48, 48, 2, -1, 0, 0, 0,
								   342, 120, 48, 48, 2, -1, 0, 0, 0,
								   409, 14,  64, 64, 2, -1, 0, 0, 0,
								   383, 78,  48, 48, 2, -1, 0, 0, 0,
								   400, 150, 48, 48, 2, -1, 0, 0, 0 };
struct _ButtonTexture* FaceButtons;

char* NextTimAddr(char* addr, int w, int h, enum bdepth bpp)
{
	long addtl;

	if (bpp == TIM_4BIT)
	{
		addtl = ((w * h) >> 1) + 44;
	}
	else if (bpp == TIM_8BIT)
	{
		addtl = (w * h) + 524;
	}
	else
	{
		addtl = (w * h) << 1;
	}

	addtl += 20;

	return addr + addtl;
}

void menuface_initialize()
{
	char* addr;
	char* buttonAddr;
	int i;
	int j;

	if (hack_initialized == 0)
	{
		addr = (char*)LOAD_ReadFile("\\kain2\\game\\psx\\frontend\\faces.tim", 11);
		buttonAddr = addr;

		if (addr != NULL)
		{
			FaceButtons = (struct _ButtonTexture*)MEMPACK_Malloc(sizeof(struct _ButtonTexture) * 56, 0x2D);

			if (FaceButtons == NULL)
			{
				MEMPACK_Free(addr);
			}
			else
			{
				for (i = 0; i < 8; i++)
				{
					MenuFaces[i].curFrame = -1;
					MenuFaces[i].transitionDir = 0;
					MenuFaces[i].loaded = 0;
					MenuFaces[i].delay = 0;

					for (j = 0; j < 7; j++)
					{
						DRAW_LoadButton((int*)buttonAddr, &FaceButtons[(i * 7) + j]);
						buttonAddr = NextTimAddr(buttonAddr, MenuFaces[i].w, MenuFaces[i].h, TIM_4BIT);
						MenuFaces[i].loaded |= 1 << j;
					}
				}

				MEMPACK_Free(addr);
				hack_initialized = 1;
			}
		}
	}
}

void menuface_terminate()
{
	int i;
	int j;

	if (hack_initialized != 0)
	{
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 7; j++)
			{
				if (((MenuFaces[i].loaded >> j) & 0x1))
				{
					DRAW_FreeButton(&FaceButtons[(i * 7) + j]);
				}
			}
		}
		
		MEMPACK_Free((char*)FaceButtons);
		
		hack_initialized = 0;
	}
}

void MENUFACE_ChangeStateRandomly(int index)
{
	struct menuface_t *face; // $s1
	struct menuface_t *lastFace; // $s3
	struct menuface_t* curFace; // $s0

	if (hack_initialized != 0)
	{
		lastFace = &MenuFaces[8];
		face = &MenuFaces[0];
		curFace = &lastFace[-8];

		while (face < lastFace)
		{	
			if (curFace->delay == 0)
			{
				if(curFace->transitionDir == 0)
				{
					int v0 = rand();

					if (v0 == v0 / 500 * 500)
					{
						if (curFace->curFrame == -1)
						{
							curFace->transitionDir = 1;
						}
						else
						{
							curFace->transitionDir = -1;
						}
					}
				}
				else
				{
					curFace->curFrame += curFace->transitionDir;

					if (curFace->curFrame == ((curFace->frames * 8) - (curFace->frames)) - 1 || curFace->curFrame == -1)
					{
						curFace->transitionDir = 0;
					}
				}
			}
			else
			{
				curFace->delay--;
			}

			face++;
			curFace++;
		}
	}
}

void MENUFACE_RefreshFaces()
{
	int i;
	struct menuface_t *face;

	if (hack_initialized != 0)
	{
		for (i = 0; i < 8; i++)
		{
			face = &MenuFaces[i];

			if (MenuFaces[i].curFrame >= MenuFaces[i].frames)
			{
				DRAW_DrawButton(&FaceButtons[((i * 7) + (MenuFaces[i].curFrame / MenuFaces[i].frames))], face->x, face->y, gameTrackerX.drawOT);
			}
		}
	}
}




