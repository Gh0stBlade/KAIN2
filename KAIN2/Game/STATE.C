#include "CORE.H"
#include "STATE.H"
#include "PLAYER.H"
#include "G2/ANMG2ILF.H"
#include "G2/INSTNCG2.H"
#include "G2/ANIMG2.H"
#include "MEMPACK.H"
#include "Game/GENERIC.H"
#include "G2/ANMCTRLR.H"

//static struct _G2AnimAlphaTable_Type* G2AlphaTables[7]; // offset 0x800D1A50
struct _G2AnimAlphaTable_Type* G2AlphaTables[7] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static char circBuf[4096]; // offset 0x800D47E0

//static void *circWhere; // offset 0x800D1A6C
void* circWhere = &circBuf[92];

static inline void STATE_CheckIfObjectSpins(struct evObjectThrowData* Ptr, struct _SVector* angularVel, long spinType)
{
	if (spinType < 0)
	{
		return;
	}
	if (spinType < 2)
	{
		return;
	}
	if (spinType != 2)
	{
		return;
	}
	if (angularVel == NULL)
	{
		Ptr->spinType = 0;
	}
	else
	{
		Ptr->angularVel = *angularVel;
	}
}

void InitMessageQueue(struct __MessageQueue* In)  // Matching - 100%
{
	In->Head = 0;
	In->Tail = 0;
}

struct __Event* DeMessageQueue(struct __MessageQueue* In)//Matching - 100%
{
	int head = In->Head;

	if (In->Head == In->Tail)
	{
		return NULL;
	}
	else if (++In->Head == 16)
	{
		In->Head = 0;
	}

	return &In->Queue[head];
}

void PurgeMessageQueue(struct __MessageQueue* In)  // Matching - 100%
{
	In->Tail = 0;
	In->Head = 0;
}

struct __Event* PeekMessageQueue(struct __MessageQueue* In)//Matching - 100%
{
	if (In->Head == In->Tail)
	{
		return NULL;
	}

	return &In->Queue[In->Head];
}

void EnMessageQueue(struct __MessageQueue* In, struct __Event* Element)  // Matching - 100%
{ 
	EnMessageQueueData(In, Element->ID, Element->Data);
}

void EnMessageQueueData(struct __MessageQueue* In, int ID, int Data)//Matching - 100%
{
	int i;

	In->Queue[In->Tail].ID = ID;
	In->Queue[In->Tail].Data = Data;

	if (++In->Tail == 16)
	{
		In->Tail = 0;
	}

	i = In->Head;

	if (i == In->Tail)
	{
		do
		{
			if (++i == 0x10)
			{
				i = 0;
			}

		} while (i != In->Tail);
	}
}


void* CIRC_Alloc(int size)
{
	void* ret;
	
	size = (size + 3) & -4;

	if (&circBuf[sizeof(circBuf)] < (char*)circWhere + size)
	{
		ret = &circBuf[0];

		circWhere = (char*)circWhere + size;

		return ret;
	}
	else
	{
		ret = circWhere;

		circWhere = (char*)circWhere + size;

		return circWhere;
	}
}

uintptr_t SetMonsterHitData(struct _Instance* Sender, struct _Instance* lastHit, int Power, int knockBackDistance, int knockBackFrames)  // Matching - 100%
{
	struct evMonsterHitData* Ptr;

	Ptr = (struct evMonsterHitData*)CIRC_Alloc(sizeof(struct evMonsterHitData));

	Ptr->sender = Sender;
	Ptr->lastHit = lastHit;
	Ptr->power = Power;
	Ptr->knockBackDistance = knockBackDistance;
	Ptr->knockBackDuration = knockBackFrames;

	return (uintptr_t)Ptr;
}

uintptr_t SetMonsterThrownData(struct _Instance* Sender, struct _Rotation* Direction, int Power)  // Matching - 100%
{
	struct evMonsterThrownData* Ptr;

	Ptr = (struct evMonsterThrownData*)CIRC_Alloc(sizeof(struct evMonsterThrownData));
	Ptr->sender = Sender;
	Ptr->direction.x = Direction->x;
	Ptr->direction.y = Direction->y;
	Ptr->direction.z = Direction->z;
	Ptr->power = Power;
	return (uintptr_t)Ptr;
}

uintptr_t SetMonsterAlarmData(struct _Instance* sender, _Position* position, int type)  // Matching - 100%
{
	struct evMonsterAlarmData* Ptr;

	Ptr = (struct evMonsterAlarmData*)CIRC_Alloc(sizeof(struct evMonsterAlarmData));
	Ptr->sender = sender;
	Ptr->position.x = position->x;
	Ptr->position.y = position->y;
	Ptr->position.z = position->z;
	Ptr->type = type;
	return (uintptr_t)Ptr;
}

uintptr_t SetMonsterSoulSuckData(struct _Instance* Sender, int x, int y, int z)  // Matching - 100%
{
	struct evMonsterSoulSuckData* Ptr;

	Ptr = (struct evMonsterSoulSuckData*)CIRC_Alloc(sizeof(struct evMonsterSoulSuckData));

	Ptr->sender = Sender;

	Ptr->Destination.x = x;
	Ptr->Destination.y = y;
	Ptr->Destination.z = z;

	return (uintptr_t)Ptr;
}

uintptr_t SetMonsterImpaleData(struct _Instance* weapon, struct _Rotation* direction, _Position* position, int distance)  // Matching - 100%
{
	struct evMonsterImpaleData* Ptr;

	Ptr = (struct evMonsterImpaleData*)CIRC_Alloc(sizeof(struct evMonsterImpaleData));
	Ptr->weapon = weapon;
	Ptr->direction.x = direction->x;
	Ptr->direction.y = direction->y;
	Ptr->direction.z = direction->z;
	Ptr->position.x = position->x;
	Ptr->position.y = position->y;
	Ptr->position.z = position->z;
	Ptr->distance = distance;
	return (uintptr_t)Ptr;
}

uintptr_t SetObjectData(int x, int y, int PathNumber, struct _Instance* Force, int node)  // Matching - 100%
{
	struct evObjectData* Ptr;

	Ptr = (struct evObjectData*)CIRC_Alloc(sizeof(struct evObjectData));
	Ptr->x = x;
	Ptr->y = y;
	Ptr->PathNumber = PathNumber;
	Ptr->LinkNode = node;
	Ptr->Force = Force;
	return (uintptr_t)Ptr;
}

uintptr_t SetPositionData(int x, int y, int z)  // Matching - 100%
{
	struct evPositionData* Ptr;
	
	Ptr = (struct evPositionData*)CIRC_Alloc(sizeof(struct evPositionData));

	Ptr->x = x;
	Ptr->y = y;
	Ptr->z = z;

	return (uintptr_t)Ptr;
}

uintptr_t SetPhysicsGravityData(int UpperOffset, int LowerOffset, int x, int y, int z, int slope)  // Matching - 100%
{
	struct evPhysicsGravityData* Ptr;
	
	Ptr = (struct evPhysicsGravityData*)CIRC_Alloc(sizeof(struct evPhysicsGravityData));
	Ptr->UpperOffset = UpperOffset;
	Ptr->LowerOffset = LowerOffset;
	Ptr->x = x;
	Ptr->y = y;
	Ptr->z = z;
	Ptr->slipSlope = slope;
	
	return (uintptr_t)Ptr;
}

uintptr_t SetPhysicsEdgeData(int UpperOffset, int ForwardOffset, int AboveOffset, int x, int y, int z, struct _SVector* Normal1, struct _SVector* Normal2, struct _SVector* Delta)  // Matching - 100%
{
	struct evPhysicsEdgeData* Ptr;

	Ptr = (struct evPhysicsEdgeData*)CIRC_Alloc(sizeof(struct evPhysicsEdgeData));

	Ptr->UpperOffset = UpperOffset;

	Ptr->ForwardOffset = ForwardOffset;

	Ptr->AboveOffset = AboveOffset;

	Ptr->XDistance = x;

	Ptr->YDistance = y;

	Ptr->ZDistance = z;

	Ptr->Normal1 = Normal1;

	Ptr->Normal2 = Normal2;

	Ptr->Delta = Delta;

	return (uintptr_t)Ptr;
}

uintptr_t SetPhysicsSwimData(int CheckDepth, struct _SVector* iVelocity, int SwimDepth, int WadeDepth, int TreadDepth)  // Matching - 100%
{
	struct evPhysicsSwimData* Ptr;

	Ptr = (struct evPhysicsSwimData*)CIRC_Alloc(sizeof(struct evPhysicsSwimData));

	Ptr->CheckDepth = CheckDepth;
	Ptr->iVelocity = iVelocity;
	Ptr->SwimDepth = SwimDepth;
	Ptr->WadeDepth = WadeDepth;
	Ptr->TreadDepth = TreadDepth;
	
	return (uintptr_t)Ptr;
}

uintptr_t SetPhysicsDropOffData(int xOffset, int yOffset, int DropOffset, int slipSlope, int UpperOffset)
{
	struct evPhysicsDropOffData* Ptr;

	Ptr = (struct evPhysicsDropOffData*)CIRC_Alloc(sizeof(struct evPhysicsDropOffData));

	Ptr->xOffset = xOffset;

	Ptr->yOffset = yOffset;

	Ptr->DropOffset = DropOffset;

	Ptr->slipSlope = slipSlope;

	Ptr->UpperOffset = UpperOffset;

	return (uintptr_t)Ptr;
}

uintptr_t SetPhysicsSlideData(int Segment, int ForwardVectorX, int ForwardVectorY, int ForwardVectorZ, int DropOffset, int UpperOffset, int Height) { // Matching 100%
	struct evPhysicsSlideData* Ptr;
	Ptr = (struct evPhysicsSlideData*)CIRC_Alloc(sizeof(struct evPhysicsSlideData));
	Ptr->Segment = Segment;
	Ptr->ForwardVector.x = ForwardVectorX;
	Ptr->ForwardVector.y = ForwardVectorY;
	Ptr->ForwardVector.z = ForwardVectorZ;
	Ptr->DropOffset = DropOffset;
	Ptr->UpperOffset = UpperOffset;
	Ptr->Height = Height;
	return (uintptr_t)Ptr;
}

uintptr_t SetPhysicsWallCrawlData(int Segment, int Length, int ForwardOffset, int NormalDistance)//Matching - 99.78%
{
	struct evPhysicsWallCrawlData* Ptr;

	Ptr = (struct evPhysicsWallCrawlData*)CIRC_Alloc(sizeof(struct evPhysicsWallCrawlData));

	Ptr->Segment = Segment;
	Ptr->Length = Length;
	Ptr->ForwardOffset = ForwardOffset;
	Ptr->NormalDistance = NormalDistance;

	return (uintptr_t)Ptr;
}

uintptr_t SetPhysicsLinkedMoveData(struct _Instance* instance, int segment, struct _SVector* posDelta, struct _SVector* rotDelta) // Matching - 100%
{
	struct evPhysicsLinkedMoveData* Ptr;
	Ptr = (struct evPhysicsLinkedMoveData*)CIRC_Alloc(sizeof(struct evPhysicsLinkedMoveData));
	Ptr->segment = segment;
	Ptr->instance = instance;
	if (posDelta != NULL)
	{
		Ptr->posDelta = *posDelta;
	}
	if (rotDelta != NULL)
	{
		Ptr->rotDelta = *rotDelta;
	}
	return (uintptr_t)Ptr;
}

uintptr_t SetPhysicsDropHeightData(_Position* offset, int dropOffset, int mode) { // Matching 100%
	struct evPhysicsDropHeightData* ptr;
	ptr = (struct evPhysicsDropHeightData*)CIRC_Alloc(sizeof(struct evPhysicsDropHeightData));
	ptr->DropOffset = dropOffset;
	ptr->mode = mode;
	ptr->origin.x = offset->x;
	ptr->origin.y = offset->y;
	ptr->origin.z = offset->z + 25;
	return (uintptr_t)ptr;
}

uintptr_t SetAnimationControllerDoneData(struct _Instance* instance, long segment, long type, int data)//Matching - 99.78%
{
	struct evAnimationControllerDoneData* Ptr;

	Ptr = (struct evAnimationControllerDoneData*)CIRC_Alloc(sizeof(struct evAnimationControllerDoneData));

	Ptr->instance = instance;
	Ptr->segment = segment;
	Ptr->type = type;
	Ptr->data = data;

	return (uintptr_t)Ptr;
}

uintptr_t SetAnimationInstanceSwitchData(struct _Instance* instance, int anim, int frame, int frames, int mode) { // Matching 100%
	struct evAnimationInstanceSwitchData* Ptr;
	Ptr = (struct evAnimationInstanceSwitchData*)CIRC_Alloc(sizeof(struct evAnimationInstanceSwitchData));
	Ptr->instance = instance;
	Ptr->anim = anim;
	Ptr->frame = frame;
	Ptr->frames = frames;
	Ptr->mode = mode;
	return (uintptr_t)Ptr;
}

uintptr_t SetFXHitData(struct _Instance* hitter, int segment, int amount, int type)  // Matching - 100%
{
	struct evFXHitData* Ptr;

	Ptr = (struct evFXHitData*)CIRC_Alloc(sizeof(struct evFXHitData));
	Ptr->amount = amount;
	Ptr->type = type;
	if (hitter != NULL)
	{
		Ptr->location.x = (short)hitter->matrix[segment].t[0];
		Ptr->velocity.x = Ptr->location.x - (short)hitter->oldMatrix[segment].t[0];
		Ptr->location.y = (short)hitter->matrix[segment].t[1];
		Ptr->velocity.y = Ptr->location.y - (short)hitter->oldMatrix[segment].t[1];
		Ptr->location.z = (short)hitter->matrix[segment].t[2];
		Ptr->velocity.z = Ptr->location.z - (short)hitter->oldMatrix[segment].t[2];
	}
	return (uintptr_t)Ptr;
}

uintptr_t SetObjectThrowData(void* target, struct _SVector* angularVel, unsigned short type, unsigned short spinType, int speed, int gravity, int zVel, int initialXRot)  // Matching - 100%
{
	struct evObjectThrowData* Ptr;
	short temp, temp2, temp3, temp4;  // not from SYMDUMP

	temp = speed;
	temp2 = gravity;
	temp3 = zVel;
	temp4 = initialXRot;
	Ptr = (struct evObjectThrowData*)CIRC_Alloc(28);
	Ptr->type = type;
	Ptr->spinType = spinType;
	if (target == NULL)
	{
		Ptr->type = 0;
	}
	else
	{
		switch (type)
		{
		case 0:
			break;
		case 1:
			Ptr->data.target = (struct _Instance*)target;
			break;
		case 3:
			Ptr->data.direction = *(struct _Rotation*)target;
			break;
		case 2:
		case 4:
			Ptr->data.throwVector = *(struct _Position*)target;
			break;
		default:
			break;
		}
	}
	STATE_CheckIfObjectSpins(Ptr, angularVel, spinType);
	Ptr->speed = temp;
	Ptr->gravity = temp2;
	Ptr->zVel = temp3;
	Ptr->initialXRot = temp4;
	return (uintptr_t)Ptr;
}

uintptr_t SetObjectBreakOffData(struct _Instance* force, short node, short distance, short animation, int frame, int type, int action) // Matching - 100%
{
	struct evObjectBreakOffData* Ptr;

	Ptr = (struct evObjectBreakOffData*)CIRC_Alloc(sizeof(struct evObjectBreakOffData));
	Ptr->force = force;
	Ptr->node = node;
	Ptr->distance = distance;
	Ptr->animation = animation;
	Ptr->frame = frame;
	Ptr->type = type;
	Ptr->action = action;
	return (uintptr_t)Ptr;
}

uintptr_t SetControlInitHangData(struct _Instance* instance, long frame, long frames)
{
	struct evControlInitHangData* Ptr;
	
	Ptr = (struct evControlInitHangData*)CIRC_Alloc(sizeof(struct evControlInitHangData));

	Ptr->instance = instance;
	Ptr->frame = frame;
	Ptr->frames = frames;
	
	return (uintptr_t)Ptr;
}

uintptr_t SetControlInitIdleData(int mode, long frame, long frames)
{
	struct evControlInitIdleData* Ptr;

	Ptr = (struct evControlInitIdleData*)CIRC_Alloc(sizeof(struct evControlInitIdleData));
	
	Ptr->mode = mode;
	Ptr->frame = frame;
	Ptr->frames = frames;

	return (uintptr_t)Ptr;
}

uintptr_t SetObjectDraftData(short force, unsigned short radius, unsigned short radiusCoef, unsigned short height, int maxVelocity) // Matching - 100%
{
	struct evObjectDraftData* Ptr;

	Ptr = (struct evObjectDraftData*)CIRC_Alloc(sizeof(struct evObjectDraftData));
	Ptr->force = force;
	Ptr->radius = radius;
	Ptr->radiusCoef = radiusCoef;
	Ptr->height = height;
	Ptr->maxVelocity = maxVelocity;
	return (uintptr_t)Ptr;
}

uintptr_t SetObjectAbsorbData(struct _Instance* force, unsigned short node, unsigned short steps)
{
	struct evObjectAbsorbData* Ptr;

	Ptr = (struct evObjectAbsorbData*)CIRC_Alloc(sizeof(struct evObjectAbsorbData));

	Ptr->force = force;
	Ptr->node = node;
	Ptr->steps = steps;

	return (uintptr_t)Ptr;
}

uintptr_t SetControlSaveDataData(long length, void* data)
{
	struct evControlSaveDataData* Ptr;
	
	Ptr = (struct evControlSaveDataData*)CIRC_Alloc(sizeof(struct evControlSaveDataData));

	Ptr->length = length;
	Ptr->data = data;

	return (uintptr_t)Ptr;
}

uintptr_t SetObjectIdleData(long mode, struct _Instance* instance)
{
	struct evObjectIdleData* Ptr;
	
	Ptr = (struct evObjectIdleData*)CIRC_Alloc(sizeof(struct evObjectIdleData));

	Ptr->mode = mode;
	Ptr->instance = instance;
	
	return (uintptr_t)Ptr;
}

uintptr_t SetActionPlayHostAnimationData(struct _Instance* instance, struct _Instance* host, int newAnim, int newFrame, int frames, int mode)  // Matching - 100%
{
	struct evActionPlayHostAnimationData* Ptr;

	Ptr = (struct evActionPlayHostAnimationData*)CIRC_Alloc(sizeof(struct evActionPlayHostAnimationData));
	Ptr->instance = instance;
	Ptr->host = host;
	Ptr->newAnim = newAnim;
	Ptr->newFrame = newFrame;
	Ptr->frames = frames;
	Ptr->mode = mode;
	return (uintptr_t)Ptr;
}

uintptr_t SetObjectBirthProjectileData(struct _Instance* instance, int joint, int type)
{
	struct evObjectBirthProjectileData* Ptr;

	Ptr = (struct evObjectBirthProjectileData*)CIRC_Alloc(sizeof(struct evObjectBirthProjectileData));

	Ptr->instance = instance;

	Ptr->joint = joint;

	Ptr->type = type;

	Ptr->birthInstance = 0;

	return (uintptr_t)Ptr;
}

uintptr_t SetShadowSegmentData(unsigned long total)
{
	struct evShadowSegmentData* Ptr;
	
	Ptr = (struct evShadowSegmentData*)CIRC_Alloc(sizeof(struct evShadowSegmentData));

	Ptr->totalShadowSegments = total;

	return (uintptr_t)Ptr;
}

void G2EmulationInit()//Matching - 98.37%
{
	G2AlphaTables[0] = NULL;

	if (G2AlphaTables[1] == NULL)
	{
		G2AlphaTables[1] = (struct _G2AnimAlphaTable_Type*)MEMPACK_Malloc(0x12, 0x19);
		G2AlphaTables[1]->size = 4;
		G2AlphaTables[1]->data[0] = 0;
		G2AlphaTables[1]->data[1] = 256;
		G2AlphaTables[1]->data[2] = 1024;
		G2AlphaTables[1]->data[3] = 4096;
	}

	if (G2AlphaTables[2] == NULL)
	{
		G2AlphaTables[2] = (struct _G2AnimAlphaTable_Type*)MEMPACK_Malloc(0x12, 0x19);
		G2AlphaTables[2]->size = 4;
		G2AlphaTables[2]->data[0] = 0;
		G2AlphaTables[2]->data[1] = 3072;
		G2AlphaTables[2]->data[2] = 3840;
		G2AlphaTables[2]->data[3] = 4096;
	}

	if (G2AlphaTables[3] == NULL)
	{
		G2AlphaTables[3] = (struct _G2AnimAlphaTable_Type*)MEMPACK_Malloc(0x6, 0x19);
		G2AlphaTables[3]->size = 1;
		G2AlphaTables[3]->data[0] = 4096;
	}

	if (G2AlphaTables[4] == NULL)
	{
		G2AlphaTables[4] = (struct _G2AnimAlphaTable_Type*)MEMPACK_Malloc(0x12, 0x19);
		G2AlphaTables[4]->size = 4;
		G2AlphaTables[4]->data[0] = 0;
		G2AlphaTables[4]->data[1] = 256;
		G2AlphaTables[4]->data[2] = 3840;
		G2AlphaTables[4]->data[3] = 4096;
	}

	if (G2AlphaTables[5] == NULL)
	{
		G2AlphaTables[5] = (struct _G2AnimAlphaTable_Type*)MEMPACK_Malloc(0x12, 0x19);
		G2AlphaTables[5]->size = 4;
		G2AlphaTables[5]->data[0] = 0;
		G2AlphaTables[5]->data[1] = 128;
		G2AlphaTables[5]->data[2] = 512;
		G2AlphaTables[5]->data[3] = 4096;
	}

	if (G2AlphaTables[6] == NULL)
	{
		G2AlphaTables[6] = (struct _G2AnimAlphaTable_Type*)MEMPACK_Malloc(0x12, 0x19);
		G2AlphaTables[6]->size = 4;
		G2AlphaTables[6]->data[0] = 0;
		G2AlphaTables[6]->data[1] = 3584;
		G2AlphaTables[6]->data[2] = 3968;
		G2AlphaTables[6]->data[3] = 4096;
	}
}

void G2EmulationInstancePlayAnimation(struct _Instance* instance)
{
	int i;
	struct _G2Anim_Type* anim;
	
	anim = &instance->anim;

	for (i = 0; i < anim->sectionCount; i++)
	{
		G2AnimSection_NextKeyframe(&anim->section[i]);
	}
}

void G2EmulatePlayAnimation(struct __CharacterState* In)
{
	G2EmulationInstancePlayAnimation(In->CharacterInstance);
}

void G2EmulationInstanceToInstanceSwitchAnimation(struct _Instance* instance, struct _Instance* host, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode) // Matching - 100%
{
	struct _G2AnimSection_Type* animSection;
	struct _G2AnimKeylist_Type* keylist;

	animSection = &instance->anim.section[CurrentSection];
	keylist = G2Instance_GetKeylist(host, NewAnim);
	G2AnimSection_SetAlphaTable(animSection, NULL);
	G2AnimSection_InterpToKeylistFrame(animSection, keylist, NewAnim, NewFrame, (short)(Frames * 100));
	if (Mode == 0)
	{
		G2AnimSection_SetPaused(animSection);
	}
	else
	{
		G2AnimSection_SetUnpaused(animSection);
		if (Mode == 2)
		{
			G2AnimSection_SetLooping(animSection);
		}
		else
		{
			G2AnimSection_SetNoLooping(animSection);
		}
	}
}

void G2EmulationInstanceSwitchAnimation(struct _Instance* instance, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode)//Matching - 99.43%
{
	struct _G2AnimSection_Type* animSection;
	struct _G2AnimKeylist_Type* keylist;

	animSection = &instance->anim.section[CurrentSection];

	keylist = G2Instance_GetKeylist(instance, NewAnim);

	G2AnimSection_SetAlphaTable(animSection, NULL);

	G2AnimSection_InterpToKeylistFrame(animSection, keylist, NewAnim, NewFrame, (short)(Frames * 100));

	if (Mode == 0)
	{
		G2AnimSection_SetPaused(animSection);
	}
	else
	{
		G2AnimSection_SetUnpaused(animSection);

		if (Mode == 2)
		{
			G2AnimSection_SetLooping(animSection);
		}
		else
		{
			G2AnimSection_SetNoLooping(animSection);
		}
	}
}

void G2EmulationInstanceSwitchAnimationAlpha(struct _Instance* instance, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode, int AlphaTable)
{
	struct _G2AnimSection_Type* animSection;

	animSection = &instance->anim.section[CurrentSection];

	G2EmulationInstanceSwitchAnimation(instance, CurrentSection, NewAnim, NewFrame, Frames, Mode);

	G2AnimSection_SetAlphaTable(animSection, G2AlphaTables[AlphaTable]);
}

void G2EmulationSwitchAnimation(struct __CharacterState* In, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode)
{
	if (NewAnim < 0)
	{
		NewAnim = 0;
	}

	G2EmulationInstanceSwitchAnimation(In->CharacterInstance, CurrentSection, NewAnim, NewFrame, Frames, Mode);
}

void G2EmulationSwitchAnimationAlpha(struct __CharacterState* In, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode, int AlphaTable)//Matching - 93.79%
{
	struct _G2AnimSection_Type* animSection;

	animSection = &In->CharacterInstance->anim.section[(char)CurrentSection];

	G2EmulationInstanceSwitchAnimation(In->CharacterInstance, CurrentSection, NewAnim, NewFrame, Frames, Mode);

	G2AnimSection_SetAlphaTable(animSection, G2AlphaTables[AlphaTable]);
}


// autogenerated function stub: 
// void /*$ra*/ G2EmulationSwitchAnimationSync(struct __CharacterState *In /*stack 0*/, int SlaveSectionID /*$fp*/, int MasterSectionID /*$a2*/, int Frames /*stack 12*/)
void G2EmulationSwitchAnimationSync(struct __CharacterState *In, int SlaveSectionID, int MasterSectionID, int Frames)
{ // line 1093, offset 0x80071bc4
	/* begin block 1 */
		// Start line: 1094
		// Start offset: 0x80071BC4
		// Variables:
			struct _Instance *instance; // $a1
			struct _G2AnimSection_Type *masterAnimSection; // $s6
			struct _G2AnimSection_Type *slaveAnimSection; // $s5
			struct _G2AnimKeylist_Type *keylist; // $s1
			int keylistID; // $s3
			struct __State *masterSection; // $s7
			struct __State *slaveSection; // $s2
	/* end block 1 */
	// End offset: 0x80071D34
	// End Line: 1130

	/* begin block 2 */
		// Start line: 2097
	/* end block 2 */
	// End Line: 2098
			UNIMPLEMENTED();
}

void G2EmulationInstanceToInstanceSwitchAnimationCharacter(struct _Instance* instance, struct _Instance* host, int NewAnim, int NewFrame, int Frames, int Mode) // Matching - 100%
{
	struct _G2AnimKeylist_Type* keylist1;

	keylist1 = G2Instance_GetKeylist(host, NewAnim);
	G2Anim_SetAlphaTable(&instance->anim, 0);
	G2Anim_InterpToKeylistFrame(&instance->anim, keylist1, NewAnim, NewFrame, (short)(Frames * 100));
	if (Mode == 0)
	{
		G2Anim_SetPaused(&instance->anim);
	}
	else
	{
		G2Anim_SetUnpaused(&instance->anim);
		if (Mode == 2)
		{
			G2Anim_SetLooping(&instance->anim);
		}
		else
		{
			G2Anim_SetNoLooping(&instance->anim);
		}
	}
}

void G2EmulationSwitchAnimationCharacter(struct __CharacterState* In, int NewAnim, int NewFrame, int Frames, int Mode)//Matching - 99.43%
{
	struct _Instance* instance;
	struct _G2AnimKeylist_Type* keylist;

	if (NewAnim < 0)
	{
		NewAnim = 0;
	}

	instance = In->CharacterInstance;
	keylist = G2Instance_GetKeylist(instance, NewAnim);
	
	G2Anim_SetAlphaTable(&instance->anim, 0);
	G2Anim_InterpToKeylistFrame(&instance->anim, keylist, NewAnim, NewFrame, (short)(100 * Frames));
	
	if (Mode == 0)
	{
		G2Anim_SetPaused(&instance->anim);
	}
	else
	{
		G2Anim_SetUnpaused(&instance->anim);
		if (Mode == 2)
		{
			G2Anim_SetLooping(&instance->anim);
		}
		else
		{
			G2Anim_SetNoLooping(&instance->anim);
		}
	}
}

void G2EmulationInstanceSetAnimation(struct _Instance *instance, int CurrentSection, int NewAnim, int NewFrame, int Frames)
{
	struct _G2AnimSection_Type *animSection;
	struct _G2AnimKeylist_Type *keylist;

	animSection = &instance->anim.section[CurrentSection];
	
	keylist = G2Instance_GetKeylist(instance, NewAnim);
	
	G2AnimSection_SetAlphaTable(animSection, NULL);
	
	G2AnimSection_InterpToKeylistFrame(animSection, keylist, NewAnim, NewFrame, Frames * 100);
}

void G2EmulationSetAnimation(struct __CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames)
{
#if defined(PSX_VERSION)

	if (NewAnim < 0)
	{
		NewAnim = 0;
	}
	
	G2EmulationInstanceSetAnimation(In->CharacterInstance, CurrentSection, NewAnim, NewFrame, Frames);
#endif
}

void G2EmulationInstanceSetMode(struct _Instance* instance, int CurrentSection, int Mode)//Matching - 99.38%
{
	struct _G2AnimSection_Type* animSection;

	animSection = &instance->anim.section[(char)CurrentSection];

	if (Mode == 0)
	{
		G2AnimSection_SetPaused(animSection);
	}
	else
	{
		G2AnimSection_SetUnpaused(animSection);

		if (Mode == 1)
		{
			G2AnimSection_SetNoLooping(animSection);
		}
		else
		{
			G2AnimSection_SetLooping(animSection);
		}
	}
}

void G2EmulationSetMode(struct __CharacterState *In, int CurrentSection, int Mode)
{ 
	G2EmulationInstanceSetMode(In->CharacterInstance, CurrentSection, Mode);
}

void G2EmulationInstanceSetAnimSpeed(struct _Instance* instance, int CurrentSection, int speed)
{
	struct _G2AnimSection_Type* animSection;

	animSection = &instance->anim.section[CurrentSection];

	animSection->speedAdjustment = speed;
}

int G2EmulationInstanceQueryAnimation(struct _Instance* instance, int CurrentSection)
{
	struct _G2AnimSection_Type* animSection;
	
	animSection = &instance->anim.section[CurrentSection];

	return animSection->keylistID;
}

int G2EmulationQueryAnimation(struct __CharacterState* In, int CurrentSection)
{
	return G2EmulationInstanceQueryAnimation(In->CharacterInstance, CurrentSection);
}

int G2EmulationInstanceQueryFrame(struct _Instance* instance, int CurrentSection)
{
	return G2AnimSection_GetKeyframeNumber(&instance->anim.section[CurrentSection]);
}

int G2EmulationInstanceQueryLastFrame(struct _Instance* instance, int CurrentSection)
{
	return G2AnimSection_GetStoredKeyframeNumber(&instance->anim.section[CurrentSection]);
}

int G2EmulationInstanceQueryPassedFrame(struct _Instance* instance, int CurrentSection, int frame) // Matching - 100%
{
	struct _G2AnimSection_Type* animSection;
	short sectionCount; // not from SYMDUMP

	animSection = &instance->anim.section[CurrentSection & 0xFF];
	if (G2AnimSection_IsInInterpolation(animSection) == G2FALSE)
	{
		if (frame)
		{
			sectionCount = frame * G2Anim_GetKeylist(&instance->anim)->s0TailTime;
			return (animSection->storedTime < sectionCount && animSection->elapsedTime >= sectionCount);
		}
		return 1;
	}
	return 0;
}

int G2EmulationQueryFrame(struct __CharacterState* In, int CurrentSection)
{
	return G2EmulationInstanceQueryFrame(In->CharacterInstance, CurrentSection);
}

int G2EmulationInstanceQueryMode(struct _Instance* instance, int CurrentSection)
{
	struct _G2AnimSection_Type* animSection;
	
	animSection = &instance->anim.section[CurrentSection];

	if (!(animSection->flags & 0x1) && !(animSection->flags & 0x2))
	{
		return 1;
	}
	
	return 0;
}

int G2EmulationQueryMode(struct __CharacterState* In, int CurrentSection)
{
	return G2EmulationInstanceQueryMode(In->CharacterInstance, CurrentSection);
}

void G2EmulationInstanceSetStartAndEndSegment(struct _Instance *instance, int CurrentSection, short Start, short End)
{
	struct _G2AnimSection_Type *animSection;
	
	animSection = &instance->anim.section[CurrentSection];
	animSection->firstSeg = (unsigned char)Start;
	animSection->segCount = (End - Start) + 1;
}

void G2EmulationSetStartAndEndSegment(struct __CharacterState *In, int CurrentSection, short Start, short End)
{ 
	G2EmulationInstanceSetStartAndEndSegment(In->CharacterInstance, CurrentSection, Start, End);
}

void G2EmulationInstanceSetTotalSections(struct _Instance *instance, short Total)
{
	struct _G2Anim_Type *anim;
	
	anim = &instance->anim;
	
	while (anim->sectionCount < Total)
	{
		G2Anim_AddSection(anim, 0, 0);
	}

	G2Anim_SetCallback(anim, INSTANCE_DefaultAnimCallback, instance);
}

void G2EmulationSetTotalSections(struct __CharacterState* In, short Total)
{
	In->TotalSections = Total;
	
	G2EmulationInstanceSetTotalSections(In->CharacterInstance, In->TotalSections);
}

void G2EmulationInstanceInitSection(struct _Instance* instance, int CurrentSection, long (*callback)(struct _G2Anim_Type* anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, void* data), void* data)
{
	struct _G2AnimSection_Type* animSection;

	typedef unsigned long fn(struct _G2Anim_Type*, int, enum _G2AnimCallbackMsg_Enum, long, long, struct _Instance*);

	animSection = &instance->anim.section[CurrentSection & 0xFF];

	animSection->callback = (fn*)callback;

	animSection->callbackData = data;
}

void G2EmulationSetInterpController_Vector(struct _Instance *instance, long segment, long type, struct _G2SVector3_Type *vec, int Frames, int Data)
{
    int segment0;
    int type0;
    segment0 = segment & 0xFF;
    type0 = type & 0xFF;
    G2Anim_SetInterpController_Vector(&instance->anim, segment0, type0, vec, (short)(Frames * 25 * 4));
    G2Anim_SetControllerCallbackData(&instance->anim, segment0, type0, (void*)Data);
}

void StateSwitchStateDataDefault(struct __CharacterState *In, int CurrentSection, void (*NewProcess)(struct __CharacterState* In, int CurrentSection, int Data), int Data)
{
	void(*process)(struct __CharacterState*, int, int);

	process = In->SectionList[CurrentSection].Process;

	StateSwitchStateData(In, CurrentSection, NewProcess, Data);

	process(In, CurrentSection, 0);
}

void StateSwitchStateCharacterDataDefault(struct __CharacterState* In, void (*NewProcess)(struct __CharacterState* In, int CurrentSection, int Data), int Data)
{
	int i;

	for (i = 0; i < In->TotalSections; i++)
	{
		StateSwitchStateDataDefault(In, i, NewProcess, Data);
	}
}

void StateSwitchStateData(struct __CharacterState* In, int CurrentSection, void (*NewProcess)(struct __CharacterState* In, int CurrentSection, int Data), int Data)//Matching - 99.61%
{
	PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
	EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100001, Data);

	In->SectionList[CurrentSection].Process = NewProcess;
	In->SectionList[CurrentSection].Process(In, CurrentSection, 0);

	EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100004, 0);
	EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100004, 0);
}

void StateSwitchStateCharacterData(struct __CharacterState* In, void (*NewProcess)(struct __CharacterState* In, int CurrentSection, int Data), int Data)//Matching - 99.67%
{
	int i;
	typedef void (*func)(struct __CharacterState* In, int CurrentSection, int Data);
	func callbackProc;

	for (i = 0; i < In->TotalSections; i++)
	{
		PurgeMessageQueue(&In->SectionList[i].Event);

		EnMessageQueueData(&In->SectionList[i].Event, 0x100004, 0);

		callbackProc = (func)In->SectionList[i].Process;
		callbackProc(In, i, 0);

		PurgeMessageQueue(&In->SectionList[i].Event);

		EnMessageQueueData(&In->SectionList[i].Event, 0x100001, Data);

		In->SectionList[i].Process = NewProcess;

		callbackProc = (func)In->SectionList[i].Process;
		callbackProc(In, i, 0);
	}
}

void StateGovernState(struct __CharacterState* In, int Frames)//Matching - 90.26%
{
	struct __State* pSectionA;
	struct __State* pSectionB;
	struct _G2AnimSection_Type* animSectionA;
	struct _G2AnimSection_Type* animSectionB;
	struct _G2AnimKeylist_Type* keylist;
	int keylistID;
	int i;

	for (i = 1; i < 3; i++)
	{
		pSectionA = &In->SectionList[i - 1];
		pSectionB = &In->SectionList[i];

		if (pSectionA->Process == pSectionB->Process)
		{
			animSectionA = &In->CharacterInstance->anim.section[(char)(i - 1)];
			animSectionB = &In->CharacterInstance->anim.section[(char)i];

			if (animSectionA->keylistID == animSectionB->keylistID)
			{
				if (!(G2AnimSection_IsInInterpolation(animSectionA)) && !(G2AnimSection_IsInInterpolation(animSectionB)))
				{
					if (G2AnimSection_GetKeyframeNumber(animSectionA) != G2AnimSection_GetKeyframeNumber(animSectionB))
					{
						keylist = animSectionA->keylist;
						keylistID = animSectionA->keylistID;

						G2AnimSection_InterpToKeylistFrame(animSectionB, keylist, keylistID, (G2AnimSection_GetKeyframeNumber(animSectionA) + Frames) % G2AnimKeylist_GetKeyframeCount(keylist), (short)(Frames * 100));
					}
				}
			}
		}
	}
}
