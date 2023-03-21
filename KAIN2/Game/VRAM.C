#include "CORE.H"
#include "VRAM.H"
#include "MEMPACK.H"
#include "GAMELOOP.H"

#include <stddef.h>

#if defined(PSXPC_VERSION)
#include <assert.h>//TEMP
#endif

short M_TrackClutUpdate;
struct _BlockVramEntry* openVramBlocks; // offset 0x800D482C
struct _BlockVramEntry* usedVramBlocks; // offset 0x800D4828
struct _BlockVramEntry vramBlockList[90]; // offset 0x800D3E2C
long numOfBlocksUsed; // offset 0x800D4808
long VRAM_NeedToUpdateMorph;

void VRAM_PrintVramBlock(struct _BlockVramEntry* vblock)
{
}

void VRAM_PrintInfo()
{
	struct _BlockVramEntry* vblock;

	vblock = usedVramBlocks;
	while (vblock != NULL)
	{
		vblock = vblock->next;
	}

	vblock = openVramBlocks;
	while (vblock != NULL)
	{
		vblock = vblock->next;
	}
}

void VRAM_InitVramBlockCache()//Matching - 94.38%
{
	int i;

	openVramBlocks = NULL;
	usedVramBlocks = NULL;
	numOfBlocksUsed = 0;

#if defined(DEMO)
	for (i = 74; i >= 0; i--)
#else
	for (i = 89; i >= 0; i--)
#endif
	{
		vramBlockList[i].flags = 0;
	}

	VRAM_InsertFreeVram(SCREEN_WIDTH, SCREEN_HEIGHT + 16, SCREEN_WIDTH, SCREEN_HEIGHT + 16, 1);
	VRAM_InitMorphPalettes();
}

void VRAM_EnableTerrainArea()
{ 
	VRAM_InsertFreeVram(SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT + 16, 0);
}

void VRAM_DisableTerrainArea()
{
	VRAM_DeleteFreeVram(SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT + 16);
}

int VRAM_ConcatanateMemory(struct _BlockVramEntry* curBlock)
{
	struct _BlockVramEntry* nextBlock;

	while (curBlock != NULL)
	{
		nextBlock = curBlock->next;

		while (nextBlock != NULL)
		{
			if (curBlock->x == nextBlock->x && curBlock->w == nextBlock->w &&
				(curBlock->y >> 8) == (nextBlock->y >> 8))
			{
				if ((curBlock->y + curBlock->h) == nextBlock->y)
				{
					curBlock->h += nextBlock->h;
					VRAM_DeleteFreeBlock(nextBlock);
					nextBlock->flags = 0;
					return 1;
				}
				else
				{
					if ((nextBlock->y + nextBlock->h) == nextBlock->y)
					{
						nextBlock->h += curBlock->h;
					}

					VRAM_DeleteFreeBlock(curBlock);
					curBlock->flags = 0;
					return 1;
				}
			}
			else
			{
				if (curBlock->y == nextBlock->y && curBlock->h == nextBlock->h)
				{
					if ((curBlock->x + curBlock->w) == nextBlock->x &&
						!(curBlock->x & 0x3F) || curBlock->w + nextBlock->w < 65)
					{
						curBlock->w += nextBlock->w;
						VRAM_DeleteFreeBlock(nextBlock);
						nextBlock->flags = 0;
						return 1;
					}

					if ((nextBlock->x + nextBlock->w) == curBlock->x)
					{
						if (!(nextBlock->x & 0x3F) || curBlock->w + nextBlock->w < 65)
						{
							curBlock->w += nextBlock->w;
							VRAM_DeleteFreeBlock(curBlock);
							curBlock->flags = 0;
							return 1;
						}
					}
				}
			}
			nextBlock = nextBlock->next;
		}
		curBlock = curBlock->next;
	}

	return 0;
}

void VRAM_GarbageCollect()
{ 
	while (VRAM_ConcatanateMemory(openVramBlocks) == 1)
	{

	}
}

int VRAM_InsertFreeBlock(struct _BlockVramEntry* block)//Matching - 99.44%
{
	struct _BlockVramEntry* next;
	struct _BlockVramEntry* prev;

	prev = NULL;
	if (block == NULL)
	{
		return 0;
	}

	next = openVramBlocks;
	while (next != NULL)
	{
		if (next->area >= block->area)
		{
			break;
		}

		prev = next;
		next = prev->next;
	}

	if (prev == NULL)
	{
		block->next = openVramBlocks;
		openVramBlocks = block;
	}
	else
	{
		block->next = next;
		prev->next = block;
	}

	VRAM_GarbageCollect();

	return 1;
}

void VRAM_DeleteFreeBlock(struct _BlockVramEntry *block)
{
	struct _BlockVramEntry *next;
	struct _BlockVramEntry *prev;
	
	next = openVramBlocks;
	prev = NULL;

	if (block != NULL)
	{
		if (block != next)
		{
			do
			{
				if (next == NULL)
				{
					break;
				}

				prev = next;
				next = prev->next;

			} while (block != next);
		
			if (block != next)
			{
				return;
			}
		}

		if (prev == NULL)
		{
			openVramBlocks = block->next;
		}
		else
		{
			prev->next = block->next;
		}
	}

	return;
}

void VRAM_InsertUsedBlock(struct _BlockVramEntry *block)
{ 
	if (block != NULL)
	{
		if (usedVramBlocks == NULL)
		{
			usedVramBlocks = block;
		}
		else
		{
			block->next = usedVramBlocks;
			usedVramBlocks = block;
		}
	}
}

void VRAM_DeleteUsedBlock(struct _BlockVramEntry *block)
{
	struct _BlockVramEntry *next;
	struct _BlockVramEntry *prev;
	
	next = usedVramBlocks;
	prev = NULL;

	while (block != next)
	{
		if (next != NULL)
		{
			prev = next;

			next = prev->next;
		}

		if (block != next)
		{
			return;
		}
	}

	if (prev == NULL)
	{
		usedVramBlocks = block->next;
		return;
	}

	prev->next = block->next;
}

struct _BlockVramEntry * VRAM_GetOpenBlock()
{ 
	int i;

	for (i = 0; i < 90; i++)
	{
		if (vramBlockList[i].flags == 0)
		{
			return &vramBlockList[i];
		}
	}

	return NULL;
}

int VRAM_DeleteFreeVram(short x, short y, short w, short h)//Matching - 87.76%
{
	struct _BlockVramEntry* prev;
	struct _BlockVramEntry* vblock;
	struct _BlockVramEntry* nextVBlock;
	struct _BlockVramEntry* blockLists[2];
	int i;
	int delCount;

	delCount = 0;

	blockLists[0] = openVramBlocks;
	blockLists[1] = usedVramBlocks;

	for (i = 0; i < 2; i++)
	{
		vblock = blockLists[i];
		prev = NULL;

		while (vblock != NULL)
		{
			nextVBlock = vblock->next;

			if (vblock->x >= x && (x + w) >= (vblock->x + vblock->w) &&
				vblock->y >= y && (y + h) >= (vblock->y + vblock->h))
			{
				vblock->flags = 0;

				if (prev == NULL)
				{
					if (i == 0)
					{
						openVramBlocks = nextVBlock;

						delCount++;
					}
					else
					{
						usedVramBlocks = nextVBlock;

						delCount++;
					}
				}
				else
				{
					prev->next = nextVBlock;

					delCount++;
				}
			}
			else
			{
				prev = vblock;
			}

			vblock = nextVBlock;
		}
	}

	return delCount;
}

int VRAM_InsertFreeVram(short x, short y, short w, short h, int flags)
{
	struct _BlockVramEntry* useBlock;

	if ((x & 0x3F) != 0 && (64 - (x & 0x3F)) < w)
	{
		useBlock = VRAM_GetOpenBlock();

		useBlock->w = 64 - (x & 0x3F);
		useBlock->next = NULL;
		useBlock->flags = flags;
		useBlock->time = 0;
		useBlock->ID = 0;
		useBlock->x = x;
		useBlock->y = y;
		useBlock->h = h;
		useBlock->area = useBlock->w * h;

		VRAM_InsertFreeBlock(useBlock);

		useBlock = VRAM_GetOpenBlock();

		useBlock->w = (w - 64) + (x & 0x3F);
		useBlock->next = NULL;
		useBlock->flags = flags;
		useBlock->time = 0;
		useBlock->ID = 0;
		useBlock->y = y;
		useBlock->h = h;
		useBlock->x = (x + 64) - (x & 0x3F);
		useBlock->area = useBlock->w * h;

		VRAM_InsertFreeBlock(useBlock);
	}
	else
	{
		useBlock = VRAM_GetOpenBlock();

		useBlock->next = NULL;
		useBlock->flags = flags;
		useBlock->time = 0;
		useBlock->ID = 0;
		useBlock->x = x;
		useBlock->y = y;
		useBlock->w = w;
		useBlock->h = h;
		useBlock->area = w * h;

		VRAM_InsertFreeBlock(useBlock);
	}

	return 1;
}

struct _BlockVramEntry* VRAM_CheckVramSlot(short* x, short* y, short w, short h, int type, int startY)
{
	struct _BlockVramEntry* vblock;
	short hldx;
	short hldy;
	short hldw;
	short hldh;
	long fits;
	long offset;
	struct _BlockVramEntry* vblockright;
	long offsetright;
	int newx;
	int xval;

	vblock = openVramBlocks;
	fits = 0;
	vblockright = NULL;
	offsetright = 0;

	while (vblock != NULL)
	{
		if ((((vblock->w >= w) && (vblock->h >= h))) && ((startY == -1 || ((vblock->y >= startY) && (vblock->y < startY + 256)))))
		{
			if ((vblock->x & 0x3F))
			{
				if ((64 - (vblock->x & 0x3F)) >= w)
				{
					if (((vblock->x & 0x3F) & 0xF))
					{
						hldw = (16 - (64 - (vblock->x & 0x3F)));
						newx = 64 - ((vblock->x + hldw) & 0x3F);

						if (newx >= w || (vblock->w - hldw) >= w)
						{
							VRAM_InsertFreeVram(vblock->x, vblock->y, hldw, vblock->h, vblock->flags);

							fits = 0;

							vblock->x += hldw;
							vblock->w -= hldw;
							break;
						}
					}
					else
					{
						fits = 0;
						break;
					}
				}
				else
				{
					hldw = 64 - (vblock->x & 0x3F);
					newx = 64 - ((vblock->x + hldw) & 0x3F);

					if (newx >= w || (vblock->w - hldw) >= w)
					{
						fits = 1;
						vblockright = vblock;
						offsetright = hldw;
					}
				}
			}
			else
			{
				fits = 0;
				break;
			}
		}

		vblock = vblock->next;
	}

	if (vblock == NULL)
	{
		if (vblockright != NULL && fits == 1)
		{
			vblock = vblockright;
	
			VRAM_InsertFreeVram(vblock->x, vblock->y, offsetright, vblock->h, vblock->flags);
			
			fits = 0;
			
			vblock->x += offsetright;
			vblock->w -= offsetright;
		}
		
		if (vblock == NULL)
		{
			return vblock;
		}

	}

	if (fits != 0)
	{
		return vblock;
	}

	hldx = vblock->x;
	hldy = vblock->y;
	hldw = vblock->w;
	hldh = vblock->h;

	VRAM_DeleteFreeBlock(vblock);

	vblock->next = NULL;
	vblock->flags = 1;
	vblock->w = w;
	vblock->h = h;
	vblock->type = type;

	VRAM_InsertUsedBlock(vblock);

	x[0] = vblock->x;
	y[0] = vblock->y;

	offsetright = hldw;

	if (offsetright != vblock->w)
	{
		if (hldh != h)
		{
			if (ABS(((hldw - w) * hldh) - (w * (hldh - h))) < ABS(((hldw - w) * h) - (hldw * (hldh - h))))
			{
				VRAM_InsertFreeVram(hldx + w, hldy, hldw - w, h, 1);
				VRAM_InsertFreeVram(hldx, hldy + h, hldw, hldh - h, 1);
			}
			else
			{
				VRAM_InsertFreeVram(hldx + w, hldy, hldw - w, hldh, 1);
				VRAM_InsertFreeVram(hldx, hldy + h, w, hldh - h, 1);
			}
		}
		else
		{
			VRAM_InsertFreeVram(hldx + w, hldy, hldw - w, hldh, 16);
		}
	}
	else
	{
		if (hldh != h)
		{
			VRAM_InsertFreeVram(hldx, hldy + h, vblock->w, hldh - h, 16);
		}
	}

	return vblock;
}

void VRAM_ClearVramBlock(struct _BlockVramEntry* block)
{
	if (block != NULL)
	{
		VRAM_DeleteUsedBlock(block);
		VRAM_InsertFreeBlock(block);
	}
}

void AdjustVramCoordsObject(int oldx, int oldy, int newx, int newy, struct Object *object)
{
	struct TextureMT3* texture;
	int oldclutxoffset;
	int oldclutyoffset;
	int newclut;
	int d;
	int oldtpagexoffset;
	int oldtpageyoffset;
	int newtpage;
	struct _Model* model;
	short diffy;
	short diffx;

	diffx = (newx & 0x3F) - (oldx & 0x3F);
	diffy = (newy & 0xFF) - (oldy & 0xFF);

	for (d = 0; d < object->numModels; d++)
	{
		model = object->modelList[d];

		if (model->startTextures != NULL)
		{
			texture = model->startTextures;

			while (texture < model->endTextures)
			{
				oldtpagexoffset = (newx & 0xFFFFFFC0) + (((texture->tpage & 0xF) << 6) - (oldx & 0xFFFFFFC0));
				oldtpageyoffset = newy + (((texture->tpage & 0x10) << 4) - (oldy & 0xFFFFFF00));

				newtpage = (texture->tpage & 0x1E0) | getTPage(0, 0, oldtpagexoffset, oldtpageyoffset);

				texture->v0 += diffy;
				texture->v2 += diffy;

				texture->v1 += diffy;
				texture->u0 += diffx << (2 - ((texture->tpage >> 7) & 0x3));

				oldclutxoffset = (newx & 0xFFFFFFF0) + (((texture->clut & 0x3F) << 4) - (oldx & 0xFFFFFFF0));
				oldclutyoffset = newy + ((texture->clut >> 6) - oldy);

				newclut = getClut(oldclutxoffset, oldclutyoffset);

				texture->u1 += diffx << (2 - ((texture->tpage >> 7) & 0x3));
				texture->u2 += diffx << (2 - ((texture->tpage >> 7) & 0x3));

				texture->tpage = newtpage;
				texture->clut = newclut;

				texture++;
			}
		}
	}
}

struct _BlockVramEntry* VRAM_InsertionSort(struct _BlockVramEntry* rootNode, struct _BlockVramEntry* newBlock, int pack_type)//Matching - 100%
{
	struct _BlockVramEntry* prev;
	struct _BlockVramEntry* next;

	prev = NULL;
	next = rootNode;

	while (next != NULL)
	{
		if (newBlock->area >= next->area)
		{
			break;
		}

		prev = next;
		next = next->next;
	}

	if (prev == NULL)
	{
		newBlock->next = rootNode;

		rootNode = newBlock;
	}
	else
	{
		newBlock->next = next;
		
		prev->next = newBlock;
	}

	return rootNode;
}


// autogenerated function stub: 
// void /*$ra*/ VRAM_ClearVram()
void VRAM_ClearVram()
{ // line 970, offset 0x80073704
	/* begin block 1 */
		// Start line: 971
		// Start offset: 0x80073704
		// Variables:
			struct _BlockVramEntry *vblock; // $a0
			struct _BlockVramEntry *next; // $s0
	/* end block 1 */
	// End offset: 0x80073730
	// End Line: 981

	/* begin block 2 */
		// Start line: 2226
	/* end block 2 */
	// End Line: 2227

	/* begin block 3 */
		// Start line: 2229
	/* end block 3 */
	// End Line: 2230
    UNIMPLEMENTED();
}


struct _BlockVramEntry* VRAM_RearrangeVramsLayer(long whichLayer)
{ // line 991, offset 0x80073740
	UNIMPLEMENTED();
	return NULL;
}

void VRAM_TransferBufferToVram(void* dataPtr, long dataSize, short status, void *data1, void *data2)
{ 
	struct VramBuffer* vramControl;
	PSX_RECT rect;
	long* nextOTag;
#if defined(PSXPC_VERSION)
	unsigned long long* drawTimerReturn;
#else
	long *drawTimerReturn;
#endif

	nextOTag = (long*)BreakDraw();
	
	DrawSync(0);
	
	drawTimerReturn = gameTrackerX.drawTimerReturn;
	
	vramControl = (struct VramBuffer*)data1;
	
	gameTrackerX.drawTimerReturn = NULL;

	if (vramControl != NULL)
	{
		if (!(vramControl->flags & 0x1))
		{
			vramControl->flags |= 0x1;
		
			dataPtr = ((char*)dataPtr + 0x24);
			
			dataSize -= 0x24;;
		}

		if (vramControl->lengthOfLeftOverData != 0)
		{
			if (dataSize < ((vramControl->w << 1) - vramControl->lengthOfLeftOverData))
			{
				memcpy(vramControl->lineOverFlow, dataPtr, dataSize);
				vramControl->lengthOfLeftOverData += dataSize;
				dataSize = 0;
			}
			else
			{
				memcpy(&vramControl->lineOverFlow[vramControl->lengthOfLeftOverData], dataPtr, ((vramControl->w << 1) - vramControl->lengthOfLeftOverData));
			
				dataPtr = ((char*)dataPtr + ((vramControl->w << 1) - vramControl->lengthOfLeftOverData));
				dataSize -= ((vramControl->w << 1) - vramControl->lengthOfLeftOverData);

				vramControl->lengthOfLeftOverData += ((vramControl->w << 1) - vramControl->lengthOfLeftOverData);
		
				rect.x = vramControl->x;
				rect.y = vramControl->y + vramControl->yOffset;
				rect.h = 1;
				rect.w = vramControl->w;
				
				vramControl->yOffset++;
				
				LoadImage(&rect, (unsigned int*)vramControl->lineOverFlow);
				vramControl->lengthOfLeftOverData = 0;
			}
		}

		if (dataSize > 0)
		{
			rect.x = vramControl->x;
			rect.y = vramControl->y + vramControl->yOffset;
			rect.h = dataSize / (vramControl->w << 1);
			rect.w = vramControl->w;
	
			LoadImage(&rect, (unsigned int*)((char*)dataPtr));

			vramControl->yOffset += rect.h;
			dataSize -= vramControl->w * (rect.h << 1);
			dataPtr = ((char*)dataPtr + (vramControl->w * (rect.h << 1)));

			if (dataSize > 0)
			{
				memcpy(((char*)vramControl->lineOverFlow + vramControl->lengthOfLeftOverData), dataPtr, dataSize);
				vramControl->lengthOfLeftOverData += dataSize;
			}
		}

		if (status == 1 && data2 != NULL && ((struct _ObjectTracker*)data2)->objectStatus == 4)
		{
			((struct _ObjectTracker*)data2)->objectStatus = 2;
		}
	}

	DrawSync(0);
	gameTrackerX.drawTimerReturn = drawTimerReturn;
	
	if ((unsigned long)nextOTag != 0xFFFFFFFF)
	{
		DrawOTag((unsigned int*)nextOTag);
	}
}

void VRAM_LoadReturn(void *dataPtr, void *data1, void *data2)
{
	MEMPACK_Free((char*)data1);
}

long VRAM_GetObjectVramSpace(struct VramSize *vramSize, struct _ObjectTracker *objectTracker)
{
	PSX_RECT rect;
	long result;
	struct _BlockVramEntry* lastVramBlockUsed;

	result = 1;
	
	rect.x = SCREEN_WIDTH;
	rect.y = vramSize->y;
	rect.w = vramSize->w;
	rect.h = vramSize->h;

	lastVramBlockUsed = VRAM_CheckVramSlot(&rect.x, &rect.y, vramSize->w, vramSize->h, 2, 256);

	if (lastVramBlockUsed == NULL)
	{
		VRAM_RearrangeVramsLayer(result);
		lastVramBlockUsed = VRAM_CheckVramSlot(&rect.x, &rect.y, rect.w, rect.h, 2, 256);

		if (lastVramBlockUsed == 0)
		{
			result = 0;
			VRAM_PrintInfo();
		}
	}
	
	if (lastVramBlockUsed != NULL)
	{
		objectTracker->vramBlock = lastVramBlockUsed;
		lastVramBlockUsed->udata.streamObject = objectTracker;
	}

	return result;
}

void VRAM_InitMorphPalettes()
{
}

void VRAM_UpdateMorphPalettes()
{
}

void MORPH_ChangeAreaPalettes(long time)
{
}
