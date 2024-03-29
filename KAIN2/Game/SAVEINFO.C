#include "CORE.H"
#include "SAVEINFO.H"
#include "MEMPACK.H"
#include "PSX/MAIN.H"
#include "GAMELOOP.H"
#include "MCARD/MEMCARD.H"
#include "DEBUG.H"
#include "SAVEINFO.H"
#include "STREAM.H"
#include "STRMLOAD.H"
#include "EVENT.H"
#include "SCRIPT.H"
#include "SOUND.H"
#include "GENERIC.H"
#include "GAMEPAD.H"

struct _GlobalSaveTracker* GlobalSave;
struct SavedBasic* bufferSavedIntroArray[64];
long numbufferedIntros; // offset 0x800CF8AC
long SaveArraySize[10] = { 0, 40, 16, 8, 834, 4, 120, 32, 12, 10 };

void SAVE_GetInstanceRotation(struct _Instance* instance, struct _SmallRotation* vector)  // Matching - 100%
{
	struct evPositionData* rotation;
	rotation = (struct evPositionData*)INSTANCE_Query(instance, 7);
	if (rotation == NULL)
	{
		vector->x = instance->rotation.x;
		vector->y = instance->rotation.y;
		vector->z = instance->rotation.z;
	}
	else
	{
		vector->x = rotation->x;
		vector->y = rotation->y;
		vector->z = rotation->z;
	}
}

void SAVE_ClearMemory(struct GameTracker *gameTracker)
{
	char *buffer;

	buffer = savedInfoTracker.MemoryCardBuffer;

	savedInfoTracker.InfoStart = &buffer[the_header_size];
	savedInfoTracker.InfoEnd = &buffer[the_header_size];
	savedInfoTracker.EndOfMemory = &buffer[24576];

	memset(&savedInfoTracker.MemoryCardBuffer[the_header_size], 0, the_header_size);

	numbufferedIntros = 0;

	memset(bufferSavedIntroArray, 0, sizeof(bufferSavedIntroArray));

	GlobalSave = (struct _GlobalSaveTracker*)SAVE_GetSavedBlock(6, 0);
	GlobalSave->CurrentBirthID = 8192;
	GlobalSave->humanOpinionOfRaziel = 0;


	SAVE_GetSavedBlock(4, 0);
}

void SAVE_Init(struct GameTracker *gt)
{
	void *buffer;

	buffer = MEMPACK_Malloc(24576, 18);

	if (DoMainMenu != 0)
	{
		gt->memcard = &gMemcard;
		the_header_size = memcard_initialize(&gMemcard, gt, 3, buffer, 24576);
	}
	else
	{
		gt->memcard = NULL;
	}

	savedInfoTracker.MemoryCardBuffer = (char*)buffer;
	SAVE_ClearMemory(gt);
}

void* SAVE_GetSavedBlock(long saveType, long extraSize)
{	
	struct SavedBasic *savedInfo;
	long sizeOfSave;
	long done;

	savedInfo = NULL;

	if(saveType >= 10)
	{
		DEBUG_FatalError("illegal save type %d\n", saveType);
	}

	sizeOfSave = ((((SaveArraySize[saveType] + extraSize) + 3) >> 2) << 2);
	done = 0;
	
	if (sizeOfSave >= 1021)
	{
		DEBUG_FatalError("save %d is too big! (type %d)\n", sizeOfSave, saveType);
	}

	do
	{
		if (savedInfoTracker.InfoEnd + sizeOfSave < savedInfoTracker.EndOfMemory)
		{
			savedInfo = (struct SavedBasic*)savedInfoTracker.InfoEnd;

			savedInfoTracker.InfoEnd[0] = (char)saveType;
			savedInfo->shiftedSaveSize = (unsigned char)(sizeOfSave >> 2);
			savedInfoTracker.InfoEnd += sizeOfSave;
			done = 1;
			break;
		}
		else
		{
			if (SAVE_PurgeAMemoryBlock() == 0)
			{
				done = 1;
				DEBUG_FatalError("ran out of saved memory. needed %d, used %d. increase from % d\n", sizeOfSave, savedInfoTracker.EndOfMemory - savedInfoTracker.InfoEnd, 24576);
			}
		}
	} while (done == 0);

	return savedInfo;
}

long SAVE_PurgeAMemoryBlock(void) // Matching - 99.63%
{
	struct SavedBasic* curSave;
	long result;

	curSave = (struct SavedBasic*)savedInfoTracker.InfoStart;
	result = 0;
	while (curSave < (struct SavedBasic*)savedInfoTracker.InfoEnd)
	{
		if ((curSave->savedID == 1 && ((struct _SavedIntro*)curSave)->flags2 & 0x100) || (curSave->savedID == 7 && ((struct _SavedIntroWithIntro*)curSave)->flags2 & 0x100))
		{
			SAVE_DeleteBlock(curSave);
			result = 1;
			break;
		}
		curSave += curSave->shiftedSaveSize * 2;
	}
	return result;
}

long SAVE_SaveableInstance(struct _Instance* instance)  // Matching - 100%
{
	long result;
	int temp, temp2;  // not from SYMDUMP

	result = 0;
	temp2 = instance->flags2;
	if ((temp2 & 0x20000) == 0)
	{
		if (instance->object != NULL)
		{
			if (((instance->object->oflags2 & 0x80000) != 0) || (instance == gameTrackerX.playerInstance))
			{
				result = 1;
			}
			else if ((instance->object->oflags2 & 0x40000) != 0)
			{
				result = 1;
				if ((instance->object->oflags & result) != 0)
				{
					temp = (temp2 & 8) > 0;
					result = temp > 0;
				}
			}
			else if ((instance->flags & 0x100000) == 0)
			{
				result = 3;
			}
		}
	}
	if ((instance->object->oflags2 & 0x100000) != 0)
	{
		result = 0;
	}
	if (((result == 1) && (instance->currentStreamUnitID == instance->birthStreamUnitID)) && ((instance->introUniqueID < 0x2000) != 0))
	{
		result = 2;
	}
	return result;
}

struct _SavedIntro* SAVE_UpdateSavedIntro(struct _Instance* instance, struct Level* level, struct _SavedIntro* savedIntro, struct evControlSaveDataData* extraData) // Matching - 100%
{
	struct _Position* levelOffset;
	short _x0;
	short _y0;
	short _z0;
	short _x1;
	short _y1;
	short _z1;
	struct _Position* _v;
	struct _Position* _v0;

	levelOffset = &level->terrain->BSPTreeArray->globalOffset;
	if (savedIntro)
	{
		memcpy(savedIntro->name, instance->introName, 8);
		savedIntro->savedID = 1;
		savedIntro->introUniqueID = (short)instance->introUniqueID;
		savedIntro->streamUnitID = (short)instance->currentStreamUnitID;
		savedIntro->birthUnitID = (short)instance->birthStreamUnitID;
		_v = &savedIntro->position;
		_v0 = &instance->position;
		_x0 = _v0->x;
		_y0 = _v0->y;
		_z0 = _v0->z;
		_x1 = levelOffset->x;
		_y1 = levelOffset->y;
		_z1 = levelOffset->z;
		_v->x = _x0 - _x1;
		_v->y = _y0 - _y1;
		_v->z = _z0 - _z1;
		SAVE_GetInstanceRotation(instance, &savedIntro->smallRotation);
		savedIntro->flags = instance->flags;
		savedIntro->flags2 = instance->flags2;
		savedIntro->specturalLightGroup = instance->spectralLightGroup;
		savedIntro->lightGroup = instance->lightGroup;
		savedIntro->attachedUniqueID = (short)instance->attachedID;
		if (extraData)
		{
			memcpy(savedIntro + 1, extraData->data, extraData->length);
		}
	}
	return savedIntro;
}

struct _SavedIntroWithIntro* SAVE_UpdateSavedIntroWithIntro(struct _Instance* instance, struct Level* level, struct _SavedIntroWithIntro* savedIntro, struct evControlSaveDataData* extraData) // Matching - 100%
{
	struct _Position* levelOffset;
	short _x0;
	short _y0;
	short _z0;
	short _x1;
	short _y1;
	short _z1;
	struct _Position* _v;
	struct _Position* _v0;

	levelOffset = &level->terrain->BSPTreeArray->globalOffset;
	if (savedIntro && instance->intro)
	{
		savedIntro->savedID = 7;
		savedIntro->introOffset = ((int)instance->intro - (int)level->introList) * 0x286bca1b >> 2;
		savedIntro->introUniqueID = (short)instance->introUniqueID;
		savedIntro->birthUnitID = (short)instance->birthStreamUnitID;
		_v = &savedIntro->position;
		_v0 = &instance->position;
		_x0 = _v0->x;
		_y0 = _v0->y;
		_z0 = _v0->z;
		_x1 = levelOffset->x;
		_y1 = levelOffset->y;
		_z1 = levelOffset->z;
		_v->x = _x0 - _x1;
		_v->y = _y0 - _y1;
		_v->z = _z0 - _z1;
		SAVE_GetInstanceRotation(instance, &savedIntro->smallRotation);
		savedIntro->flags = instance->flags;
		savedIntro->flags2 = instance->flags2;
		savedIntro->specturalLightGroup = instance->spectralLightGroup;
		savedIntro->lightGroup = instance->lightGroup;
		savedIntro->attachedUniqueID = (unsigned short)instance->attachedID;
		if (extraData)
		{
			memcpy(savedIntro + 1, extraData->data, extraData->length);
		}
	}
	return savedIntro;
}

struct SavedBasic* SAVE_GetSavedEvent(long areaID, long eventNumber) // Matching - 99.76%
{
	struct SavedBasic* curSave;

	curSave = (struct SavedBasic*)savedInfoTracker.InfoStart;
	while (curSave < (struct SavedBasic*)savedInfoTracker.InfoEnd)
	{
		if (curSave->savedID == 2 && ((struct SavedEvent*)curSave)->areaID == areaID && ((struct SavedEvent*)curSave)->eventNumber == eventNumber ||
			curSave->savedID == 9 && ((struct SavedEventSmallVars*)curSave)->areaID == areaID && ((struct SavedEventSmallVars*)curSave)->eventNumber == eventNumber)
		{
			return curSave;
		}
		curSave += curSave->shiftedSaveSize * 2;
	}
	return NULL;
}

void SAVE_DeleteSavedEvent(long areaID, long eventNumber)
{
	struct SavedBasic* savedEvent;

	savedEvent = SAVE_GetSavedEvent(areaID, eventNumber);

	if (savedEvent != NULL)
	{
		SAVE_DeleteBlock(savedEvent);
	}
}

struct SavedBasic* SAVE_GetSavedNextEvent(long areaID, struct SavedBasic* curSave)//Matching - 96.59%
{
	if (curSave == NULL)
	{
		curSave = (struct SavedBasic*)savedInfoTracker.InfoStart;
	}
	else
	{
		curSave = &curSave[curSave->shiftedSaveSize * 2];
	}

	while ((uintptr_t)curSave < (uintptr_t)savedInfoTracker.InfoEnd)
	{
		if (curSave->savedID == 2 && ((struct SavedEvent*)curSave)->areaID == areaID)
		{
			return curSave;
		}

		if (curSave->savedID == 9 && ((struct SavedEvent*)curSave)->areaID != areaID)
		{
			return curSave;
		}

		curSave += curSave->shiftedSaveSize * 2;
	}
	return 0;
}

void SAVE_BufferIntro(struct SavedBasic* savedIntro) // Matching - 89.53%
{
	long i;

	i = 0;
	if (numbufferedIntros < 64)
	{
		while (i < 64)
		{
			if (bufferSavedIntroArray[i] == savedIntro)
			{
				break;
			}
			i++;
		}
		if (i == 64)
		{
			i = 0;
			numbufferedIntros = numbufferedIntros + 1;
			while (i < 64)
			{
				if (bufferSavedIntroArray[i] == NULL)
				{
					bufferSavedIntroArray[i] = savedIntro;
					return;
				}
				i++;
			}
		}
	}
}

void SAVE_IntroduceBufferIntros()
{
	long i;
	struct _StreamUnit* streamUnit;
	int deleted;

	for (i = 0; numbufferedIntros != 0; i++)
	{
		if (i < 64)
		{
			if (bufferSavedIntroArray[i] != NULL)
			{
				if (bufferSavedIntroArray[i]->savedID == 1)
				{
					streamUnit = STREAM_GetStreamUnitWithID(((struct _SavedIntro*)bufferSavedIntroArray[i])->streamUnitID);

					if (streamUnit != NULL)
					{
						if (INSTANCE_IntroduceSavedInstance((struct _SavedIntro*)bufferSavedIntroArray[i], streamUnit, &deleted) != NULL)
						{
							bufferSavedIntroArray[i] = NULL;
							numbufferedIntros--;
						}
					}
					else
					{
						bufferSavedIntroArray[i] = NULL;
						numbufferedIntros--;
					}
				}
				else
				{
					streamUnit = STREAM_GetStreamUnitWithID(((struct _SavedIntroWithIntro*)bufferSavedIntroArray[i])->birthUnitID);

					if (streamUnit != NULL)
					{
						if (INSTANCE_IntroduceSavedInstanceWithIntro(((struct _SavedIntroWithIntro*)bufferSavedIntroArray[i]), streamUnit, &deleted) != NULL)
						{
							bufferSavedIntroArray[i] = NULL;
							numbufferedIntros--;
						}
					}
					else
					{
						bufferSavedIntroArray[i] = NULL;
						numbufferedIntros--;
					}
				}
			}
		}
	}
}

void SAVE_IntroForStreamID(struct _StreamUnit* streamUnit)
{
	struct SavedBasic* saveIntro;
	long streamID;
	int deleted;
	
	saveIntro = (struct SavedBasic*)savedInfoTracker.InfoStart;

	streamID = streamUnit->StreamUnitID;

	while ((char*)saveIntro < savedInfoTracker.InfoEnd)
	{
		deleted = 0;

		if (saveIntro->savedID == 1 && ((struct _SavedIntro*)saveIntro)->streamUnitID == streamID)
		{
			INSTANCE_IntroduceSavedInstance((struct _SavedIntro*)saveIntro, streamUnit, &deleted);
		}
		else if(saveIntro->savedID == 7 && ((struct _SavedIntroWithIntro*)saveIntro)->birthUnitID == streamID)
		{
			INSTANCE_IntroduceSavedInstanceWithIntro((struct _SavedIntroWithIntro*)saveIntro, streamUnit, &deleted);
		}

		if (deleted == 0)
		{
			saveIntro = (struct SavedBasic*)((char*)saveIntro + (saveIntro->shiftedSaveSize << 2));
		}
	}
}

long SAVE_HasSavedIntro(struct Intro* intro, long currentStreamID)//Matching - 99.72%
{
	struct _SavedIntro* saveIntro;
	long result;

	saveIntro = (struct _SavedIntro*)savedInfoTracker.InfoStart;
	result = 0;

	while ((uintptr_t)saveIntro < (uintptr_t)savedInfoTracker.InfoEnd)
	{
		if (saveIntro->savedID == 1 && saveIntro->introUniqueID == intro->UniqueID ||
			saveIntro->savedID == 7 && *(short*)&saveIntro->name[4] == intro->UniqueID)
		{
			result = 1;
			break;
		}

		saveIntro = (struct _SavedIntro*)(((char*)saveIntro) + saveIntro->shiftedSaveSize * 4);
	}

	return result;
}

struct SavedLevel* SAVE_HasSavedLevel(long areaID)//Matching - 91.20%
{
	struct SavedLevel* savedLevel;

	savedLevel = (struct SavedLevel*)savedInfoTracker.InfoStart;

	while ((char*)savedLevel < savedInfoTracker.InfoEnd)
	{
		if (savedLevel->savedID == 3)
		{
			if (savedLevel->areaID == areaID)
			{
				return savedLevel;
			}
		}
		savedLevel = (struct SavedLevel*)((char*)savedLevel + 4 * savedLevel->shiftedSaveSize);
	}

	return 0;
}

void SAVE_UpdateLevelWithSave(struct _StreamUnit* streamUnit)//Matching - 99.70%
{
	long Zoffset;
	struct ActualSavedLevel* savedLevel;
	struct _Terrain* terrain;
	long i;
	struct BSPTree* bspTree;

	Zoffset = streamUnit->level->terrain->BSPTreeArray->globalOffset.z;

	savedLevel = (struct ActualSavedLevel*)SAVE_HasSavedLevel(streamUnit->StreamUnitID);

	if (savedLevel != NULL)
	{
		terrain = streamUnit->level->terrain;

		for (i = 0; i < savedLevel->numberBSPTreesSaved; i++)
		{
			bspTree = &terrain->BSPTreeArray[savedLevel->bspTreeArray[i].bspIndex];

			bspTree->localOffset = savedLevel->bspTreeArray[i].localOffset;

			bspTree->flags = ((unsigned short)bspTree->flags << 16 >> 24 << 8);

			bspTree->flags |= savedLevel->bspTreeArray[i].importantFlagsSaved;

			bspTree->globalOffset.x += bspTree->localOffset.x;
			bspTree->globalOffset.y += bspTree->localOffset.y;
			bspTree->globalOffset.z += bspTree->localOffset.z;
		}

		if (savedLevel->waterZ != -32767 && savedLevel->waterZ != 32767)
		{
			streamUnit->level->waterZLevel = savedLevel->waterZ + Zoffset;
		}
		else
		{
			streamUnit->level->waterZLevel = savedLevel->waterZ;
		}

		terrain->UnitChangeFlags |= 0x3;
	}
}

struct SavedLevel* SAVE_CreatedSavedLevel(long areaID, struct Level* level) // Matching 99.28%
{
	struct SavedLevel* savedLevel;
	struct ActualSavedLevel* slevel;
	long doSave;
	long i;
	struct BSPTree* bspTree;
	long Zoffset;
	long numBSPTrees;

	Zoffset = level->terrain->BSPTreeArray->globalOffset.z;
	savedLevel = NULL;
	doSave = FALSE;
	if (level->terrain->UnitChangeFlags & 1 || level->terrain->UnitChangeFlags & 2)
	{
		doSave = TRUE;
	}
	if (doSave)
	{
		numBSPTrees = level->terrain->numBSPTrees - 2;
		if (numBSPTrees > 0)
		{
			if ((savedLevel = SAVE_HasSavedLevel(areaID)) ||
				(savedLevel = (struct SavedLevel*)SAVE_GetSavedBlock(3, numBSPTrees * 8)))
			{
				slevel = (struct ActualSavedLevel*)savedLevel;
				slevel->areaID = (short)areaID;
				if (level->waterZLevel != -0x7fff && level->waterZLevel != 0x7fff)
				{
					slevel->waterZ = (short)(level->waterZLevel - Zoffset);
				}
				else
				{
					slevel->waterZ = (short)level->waterZLevel;
				}
				slevel->numberBSPTreesSaved = (short)numBSPTrees;
				for (i = 0; i < level->terrain->numBSPTrees; i++)
				{
					bspTree = &level->terrain->BSPTreeArray[i];
					if ((unsigned short)(bspTree->ID + 1) > 1)
					{
						slevel->bspTreeArray->bspIndex = (unsigned char)i;
						memcpy(&slevel->bspTreeArray->localOffset, &bspTree->localOffset, sizeof(_Position));
						slevel->bspTreeArray->importantFlagsSaved = (unsigned char)bspTree->flags;
						slevel = (struct ActualSavedLevel*)&slevel->bspTreeArray;
					}
				}
			}
		}
	}
	else
	{
		savedLevel = SAVE_HasSavedLevel(areaID);
		if (savedLevel)
		{
			SAVE_DeleteBlock((struct SavedBasic*)savedLevel);
		}
	}
	return savedLevel;
}

void SAVE_DeleteBlock(struct SavedBasic* savedBlock) // Matching - 96.29%
{
	long size;
	char* nextBlock;
	int i;

	size = savedBlock->shiftedSaveSize * 4;
	nextBlock = ((char*)savedBlock + size);
	if (numbufferedIntros != 0)
	{
		for (i = 0; i < 64; i++)
		{
			if (bufferSavedIntroArray[i] == savedBlock)
			{
				bufferSavedIntroArray[i] = NULL;
			}
			else if (bufferSavedIntroArray[i] > savedBlock)
			{
				bufferSavedIntroArray[i] = (struct SavedBasic*)((char*)bufferSavedIntroArray[i] - size);
			}
		}
	}
	memmove(savedBlock, nextBlock, savedInfoTracker.InfoEnd - nextBlock);
	savedInfoTracker.InfoEnd -= size;
}

void SAVE_Instance(struct _Instance* instance, struct Level* level) // Matching - 100%
{
	struct _SavedIntro* savedIntro;
	struct evControlSaveDataData* extraData;
	long extraSize;
	long saveType;
	struct SavedIntroSmall* savedSmallIntro;
	struct _SavedIntroWithIntro* savedIntroWithIntro;
	struct SavedIntroSpline* savedIntroSpline;
	struct MultiSpline* multi;
	extraSize = 0;
	saveType = SAVE_SaveableInstance(instance);
	if (saveType != 0)
	{
		if ((instance->flags2 & 4) != 0)
		{
			SAVE_DeleteInstance(instance);
			extraData = (struct evControlSaveDataData*)INSTANCE_Query(instance, 24);
			if (extraData != 0)
			{
				savedSmallIntro = (struct SavedIntroSmall*)SAVE_GetSavedBlock(5, extraData->length);
				if (savedSmallIntro != 0)
				{
					savedSmallIntro->introUniqueID = (short)instance->introUniqueID;
					memcpy((void*)((int)&savedSmallIntro->introUniqueID + 2), extraData->data, extraData->length);
				}
			}
		}
		else if (saveType == 1)
		{
			SAVE_DeleteInstance(instance);
			extraData = (struct evControlSaveDataData*)INSTANCE_Query(instance, 24);
			if (extraData != 0)
			{
				extraSize = extraData->length;
			}
			savedIntro = (struct _SavedIntro*)SAVE_GetSavedBlock(1, extraSize);
			if (savedIntro != 0)
			{
				SAVE_UpdateSavedIntro(instance, level, savedIntro, extraData);
			}
		}
		else if (saveType == 2)
		{
			SAVE_DeleteInstance(instance);
			extraData = (struct evControlSaveDataData*)INSTANCE_Query(instance, 24);
			if (extraData != 0)
			{
				extraSize = extraData->length;
			}
			savedIntroWithIntro = (struct _SavedIntroWithIntro*)SAVE_GetSavedBlock(7, extraSize);
			if (savedIntroWithIntro != 0)
			{
				SAVE_UpdateSavedIntroWithIntro(instance, level, savedIntroWithIntro, extraData);
			}
		}
		else if (saveType == 3)
		{
			SAVE_DeleteInstance(instance);
			savedIntroSpline = (struct SavedIntroSpline*)SAVE_GetSavedBlock(8, 0);
			if (savedIntroSpline != 0)
			{
				multi = SCRIPT_GetMultiSpline(instance, NULL, NULL);
				if (multi != 0)
				{
					instance->splineFlags &= 0xFE7F;
					if ((instance->flags & 0x1000000) != 0)
					{
						instance->splineFlags |= 0x80;
					}
					if ((instance->flags & 0x2000000) != 0)
					{
						instance->splineFlags |= 0x100;
					}
					savedIntroSpline->savedID = 8;
					savedIntroSpline->introUniqueID = (short)instance->introUniqueID;
					savedIntroSpline->splineFlags = instance->splineFlags;
					savedIntroSpline->splineKeyFrame = (short)INSTANCE_GetSplineFrameNumber(instance, multi);
					savedIntroSpline->splineClipBeg = instance->clipBeg;
					savedIntroSpline->splineClipEnd = instance->clipEnd;
				}
			}
		}
	}
}

void SAVE_DeleteInstance(struct _Instance* instance) // Matching - 99.71%
{
	struct SavedBasic* saveIntro;

	saveIntro = (struct SavedBasic*)savedInfoTracker.InfoStart;
	while (saveIntro < (struct SavedBasic*)savedInfoTracker.InfoEnd)
	{
		if ((saveIntro->savedID == 1 && ((struct _SavedIntro*)saveIntro)->introUniqueID == instance->introUniqueID) ||
			(saveIntro->savedID == 7 && ((struct _SavedIntroWithIntro*)saveIntro)->introUniqueID == instance->introUniqueID) ||
			(saveIntro->savedID == 5 && ((struct SavedIntroSmall*)saveIntro)->introUniqueID == instance->introUniqueID) ||
			(saveIntro->savedID == 8 && ((struct SavedIntroSpline*)saveIntro)->introUniqueID == instance->introUniqueID))
		{
			SAVE_DeleteBlock(saveIntro);
			return;
		}
		saveIntro += saveIntro->shiftedSaveSize * 2;
	}
}

void SAVE_SetDeadDeadBit(int uniqueID, long set) // Matching - 99.80%
{
	struct _SavedIntro* saveIntro;
	struct SavedDeadDeadBits* deadDeadBits;
	int deadByte;
	int deadBit;

	deadDeadBits = NULL;
	if (uniqueID < 8192)
	{
		saveIntro = (struct _SavedIntro*)savedInfoTracker.InfoStart;
		while (saveIntro < (struct _SavedIntro*)savedInfoTracker.InfoEnd)
		{
			if (saveIntro->savedID == 4)
			{
				deadDeadBits = (struct SavedDeadDeadBits*)saveIntro;
				break;
			}
			saveIntro = (struct _SavedIntro*)((char*)saveIntro + saveIntro->shiftedSaveSize * 4);
		}
		if (deadDeadBits != NULL)
		{
			deadByte = uniqueID / 8;
			deadBit = 1 << (uniqueID & 7);
			if (deadByte < 832)
			{
				if (set == 1)
				{
					deadDeadBits->deadDeadBits[deadByte] |= (char)deadBit;
				}
				else
				{
					deadDeadBits->deadDeadBits[deadByte] &= (char)~deadBit;
				}
			}
		}
	}
}

void SAVE_RestoreGlobalSavePointer()//Matching - 99.09%
{
	struct SavedBasic* saveIntro;

	saveIntro = (struct SavedBasic*)savedInfoTracker.InfoStart;

	GlobalSave = NULL;

	while ((uintptr_t)saveIntro < (uintptr_t)savedInfoTracker.InfoEnd)
	{
		if (saveIntro->savedID == 6)
		{
			GlobalSave = (struct _GlobalSaveTracker*)saveIntro;

			return;
		}

		saveIntro = (struct SavedBasic*)(char*)(saveIntro + (2 * (unsigned char)saveIntro->shiftedSaveSize));
	}
}

long SAVE_IsUniqueIDDeadDead(long uniqueID)
{
	struct _SavedIntro* saveIntro;
	struct SavedDeadDeadBits* deadDeadBits;
	long result;
	int deadByte;
	int deadBit;

	deadDeadBits = NULL;

	result = 0;

	if (uniqueID >= 0x2000)
	{
		saveIntro = (struct _SavedIntro*)savedInfoTracker.InfoStart;

		while ((char*)saveIntro < savedInfoTracker.InfoEnd)
		{
			if (saveIntro->savedID == 4)
			{
				deadDeadBits = (struct SavedDeadDeadBits*)saveIntro;
				break;
			}

			saveIntro = (struct _SavedIntro*)((char*)saveIntro + (saveIntro->shiftedSaveSize << 2));
		}

		if (deadDeadBits != NULL)
		{
			deadByte = ((uniqueID < 0 ? uniqueID + 7 : uniqueID) >> 3);

			deadBit = (1 << (uniqueID & 0x7));

			if (deadByte < 832)
			{
				result = (unsigned)((deadDeadBits->deadDeadBits[deadByte] & deadBit) ^ deadBit) < 1;
			}
		}
	}

	return result;
}

long SAVE_IsIntroDeadDead(struct Intro* intro)
{
	return SAVE_IsUniqueIDDeadDead(intro->UniqueID);
}

void SAVE_DoInstanceDeadDead(struct _Instance* instance)
{
	SAVE_DeleteInstance(instance);

	SAVE_SetDeadDeadBit(instance->introUniqueID, 1);
}

void SAVE_MarkDeadDead(struct _Instance* instance)
{
	instance->flags |= 0x800000;
}

void SAVE_UndestroyInstance(struct _Instance* instance)
{
	SAVE_SetDeadDeadBit(instance->introUniqueID, 0);
}

struct SavedIntroSmall * SAVE_GetSavedSmallIntro(struct _Instance *instance)
{
	struct SavedBasic* curSave;

	curSave = (struct SavedBasic*)savedInfoTracker.InfoStart;
	
	while ((char*)curSave < savedInfoTracker.InfoEnd)
	{
		if (curSave->savedID == 2)
		{
			if (((struct SavedIntroSmall*)curSave)->introUniqueID == instance->introUniqueID)
			{
				return (struct SavedIntroSmall*)curSave;
			}
		}
		curSave = (struct SavedBasic*)((char*)curSave + (curSave->shiftedSaveSize << 2));
	}

	return NULL;
}

struct SavedIntroSpline* SAVE_GetIntroSpline(struct _Instance* instance)//Matching - 91.85%
{
	struct SavedBasic* curSave;

	curSave = (struct SavedBasic*)savedInfoTracker.InfoStart;
	while ((unsigned int)curSave < (unsigned int)savedInfoTracker.InfoEnd)
	{
		if (curSave->savedID == 8)
		{
			if (((struct SavedIntroSpline*)curSave)->introUniqueID == instance->introUniqueID)
				return (struct SavedIntroSpline*)curSave;
		}
		curSave = (struct SavedBasic*)(char*)curSave + 2 * (unsigned char)curSave->shiftedSaveSize;
	}

	return NULL;
}

void SAVE_UpdateGlobalSaveTracker(void) // Matching - 96.35%
{
	GlobalSave->currentTime = (unsigned long)gameTrackerX.currentTime;
	memcpy(&GlobalSave->sound, &gameTrackerX.sound, sizeof(struct gSoundData));
	GlobalSave->saveVersion = 21793;
	if (GAMEPAD_DualShockEnabled())
	{
		GlobalSave->flags = GlobalSave->flags | 2;
	}
	else
	{
		GlobalSave->flags = GlobalSave->flags & 0xfffd;
	}
}

void SAVE_RestoreGlobalSaveTracker(void) // Matching - 97.21%
{
	if (GlobalSave->saveVersion != 21793)
	{
		DEBUG_FatalError("error: old save game\n");
	}
	else
	{
		gameTrackerX.currentTime = GlobalSave->currentTime;
		memcpy(&gameTrackerX.sound, &GlobalSave->sound, sizeof(struct gSoundData));
		SOUND_SetSfxVolume(gameTrackerX.sound.gSfxVol);
		SOUND_SetMusicVolume(gameTrackerX.sound.gMusicVol);
		SOUND_SetVoiceVolume(gameTrackerX.sound.gVoiceVol);
		if ((GlobalSave->flags & 2U) != 0)
		{
			GAMEPAD_EnableDualShock();
		}
		else
		{
			GAMEPAD_DisableDualShock();
		}
	}
}

void SAVE_SaveEverythingInMemory()
{ 
	struct _Instance* instance;
	long i;
	struct _Instance* next;
	struct Level* level;
	
	instance = gameTrackerX.instanceList->first;
	next = NULL;

	while (instance != NULL)
	{
		next = instance->next;
		level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

		if (level != NULL)
		{
			SAVE_Instance(instance, level);
		}

		instance = next;
		next = NULL;
	}

	for (i = 0; i < 16; i++)
	{
		if (StreamTracker.StreamList[i].used == 2)
		{
			EVENT_SaveEventsFromLevel(StreamTracker.StreamList[i].StreamUnitID, StreamTracker.StreamList[i].level);
			
			SAVE_CreatedSavedLevel(StreamTracker.StreamList[i].StreamUnitID, StreamTracker.StreamList[i].level);
		}
	}
}

void SAVE_SaveGame()
{
	while (STREAM_PollLoadQueue() != 0)
	{
	}

	SAVE_SaveEverythingInMemory();

	SAVE_UpdateGlobalSaveTracker();

	GlobalSave->sizeUsedInBlock = ((unsigned short*)&savedInfoTracker.InfoEnd)[0] - ((unsigned short*)&savedInfoTracker.InfoStart)[0];
}

void SAVE_RestoreGame(void) // Matching - 94.40%
{
	gameTrackerX.streamFlags |= 0x200000;
	SAVE_RestoreGlobalSavePointer();
	SAVE_RestoreGlobalSaveTracker();
	savedInfoTracker.InfoEnd = savedInfoTracker.InfoStart + GlobalSave->sizeUsedInBlock;
	GAMELOOP_RequestLevelChange("under", 1, &gameTrackerX);
}

void SAVE_DebugSaveGame()
{
}

void SAVE_LoadSaveGame()
{
	gameTrackerX.streamFlags |= 0x200000;

	GAMELOOP_RequestLevelChange("under", 1, &gameTrackerX);

	gameTrackerX.gameMode = 0;
}

long SAVE_SizeOfFreeSpace()
{
	return savedInfoTracker.EndOfMemory - savedInfoTracker.InfoEnd;
}
