#include "CORE.H"
#include "FX.H"
#include "OBTABLE.H"
#include "MATH3D.H"
#include "PIPE3D.H"
#include "PSX/COLLIDES.H"
#include "Game/MEMPACK.H"
#include "Game/LIST.H"
#include "Game/GLYPH.H"
#include "Game/DRAW.H"
#include "Game/LIGHT3D.H"
#include "Game/COLLIDE.H"
#include "PHYSOBS.H"
#include "GAMEPAD.H"
#include "STATE.H"
#include "GENERIC.H"
#include "REAVER.H"
#include "SOUND.H"

struct _FXBlastringEffect* fx_blastring;
struct _Instance* FX_reaver_instance;
short fx_going;
struct _FXGeneralEffect* FX_GeneralEffectTracker;
short snow_amount;
short rain_amount;
short current_rain_fade;
struct _FX_PRIM* FX_LastUsedPrim;
short Spiral_Number;
short Spiral_Degrees;
long Spiral_Current;
long Spiral_Max = 100000;
DVECTOR Spiral_Array[65]; // offset 0x800d3968
DVECTOR Spiral_OffsetP[64]; // offset 0x800d3a78
DVECTOR Spiral_OffsetM[64];
int Spiral_Glow_X; // offset 0x800d3c78
int Spiral_Glow_Y; // offset 0x800d3c7c
int Spiral_Glow_Size; // offset 0x800d3c80
int Spiral_Mod;
struct _FXTracker* gFXT;
struct _FXTracker* fxTracker;
struct _FXRibbon* FX_ConstrictRibbon; // offset 0x800d1000
short FX_ConstrictStage; // offset 0x800d1004
struct _Instance* FX_ConstrictInstance; // offset 0x800d1008
struct _Position FX_ConstrictPosition; // offset 0x800d3948
struct _Position* FX_ConstrictPositionPtr;
short FX_Frames; // offset 0x800d3958
short FX_TimeCount;
long FX_ColorArray[6];
static short wind_deg; // offset 0x800D1040
static short wind_speed; // offset 0x800D1042
static short windx; // offset 0x800D103C
static short windy; // offset 0x800D103E
extern struct _Instance* inst_Reaver;

static inline long FX_GetColor(struct ObjectEffect* effect, long i)
{
	return FX_ColorArray[effect->modifierList[i]];
}

void FX_Init(struct _FXTracker* fxTracker) // Matching - 100%
{
	struct _FX_MATRIX* fxMatrix;
	struct _FX_MATRIX* endFXMatrix;
	struct _FX_PRIM* fxPrim;
	struct _FX_PRIM* endFXPrim;

	fxMatrix = fxTracker->matrixPool;

	endFXMatrix = (struct _FX_MATRIX*)&fxTracker->usedMatrixList;

	fxTracker->usedMatrixList.next = 0;
	fxTracker->usedMatrixList.prev = 0;

	fxTracker->freeMatrixList.next = 0;
	fxTracker->freeMatrixList.prev = 0;

	fxTracker->usedPrimList.next = 0;
	fxTracker->usedPrimList.prev = 0;

	fxTracker->usedPrimListSprite.next = 0;
	fxTracker->usedPrimListSprite.prev = 0;

	fxTracker->freePrimList.next = 0;
	fxTracker->freePrimList.prev = 0;

	while (fxMatrix < endFXMatrix)
	{
		LIST_InsertFunc(&fxTracker->freeMatrixList, &fxMatrix->node);

		fxMatrix++;
	}

	fxPrim = fxTracker->primPool;

	endFXPrim = (struct _FX_PRIM*)&fxTracker->usedPrimList.prev;

	while (fxPrim < endFXPrim)
	{
		LIST_InsertFunc(&fxTracker->freePrimList, &fxPrim->node);

		fxPrim++;
	}

	FX_LastUsedPrim = NULL;

	FX_ConstrictPositionPtr = &FX_ConstrictPosition;

	FX_Spiral_Init();

	snow_amount = 0;

	rain_amount = 0;

	current_rain_fade = 0;

	FX_reaver_instance = NULL;

	FX_Frames = 1;

	FX_TimeCount = 0;
}

void FX_Die(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker)  // Matching - 100%
{
	if ((FX_LastUsedPrim == fxPrim) && (FX_LastUsedPrim = (struct _FX_PRIM*)fxPrim->node.prev, FX_LastUsedPrim->node.prev == NULL))
	{
		FX_LastUsedPrim = NULL;
	}

	fxPrim->flags |= 16;

	LIST_DeleteFunc(&fxPrim->node);

	LIST_InsertFunc(&fxTracker->freePrimList, &fxPrim->node);
}

struct _FX_MATRIX* FX_GetMatrix(struct _FXTracker* fxTracker) // Matching - 100%
{
	struct _FX_MATRIX* fxMatrix;

	fxMatrix = (struct _FX_MATRIX*)LIST_GetFunc(&fxTracker->freeMatrixList);

	if (fxMatrix != NULL)
	{
		fxMatrix->flags = 0x1;

		LIST_InsertFunc(&fxTracker->usedMatrixList, &fxMatrix->node);
	}

	return fxMatrix;
}

struct _FX_PRIM* FX_GetPrim(struct _FXTracker* fxTracker)  // Matching - 100%
{
	struct _FX_PRIM* fxPrim;

	fxPrim = (struct _FX_PRIM*)LIST_GetFunc(&fxTracker->freePrimList);

	if (fxPrim == NULL)
	{
		if (FX_LastUsedPrim != NULL)
		{
			fxPrim = FX_LastUsedPrim;

			FX_LastUsedPrim = (struct _FX_PRIM*)fxPrim->node.prev;

			FX_LastUsedPrim->node.next = NULL;

			if (FX_LastUsedPrim->node.prev == NULL)
			{
				FX_LastUsedPrim = NULL;
			}
		}

		if (fxPrim != NULL)
		{
			fxPrim->process = NULL;
			fxPrim->flags = 0;
			fxPrim->matrix = NULL;
		}
	}
	else
	{
		fxPrim->process = NULL;
		fxPrim->flags = 0;
		fxPrim->matrix = NULL;
	}

	return fxPrim;
}

struct _FXParticle* FX_GetParticle(struct _Instance* instance, short startSegment)  // Matching - 100%
{
	struct _FXParticle* particle;

	particle = (struct _FXParticle*)MEMPACK_Malloc(sizeof(struct _FXParticle), 13);

	if (particle != NULL)
	{
		particle->effectType = 1;
		particle->continue_process = &FX_ContinueParticle;
		particle->type = 0;
		particle->instance = instance;
		particle->startSegment = (char)startSegment;
		particle->texture = NULL;
		particle->fxprim_process = NULL;
		particle->fxprim_modify_process = NULL;
		particle->startScale = 4096;
		particle->scaleSpeed = 0;
		particle->offset.z = 0;
		particle->offset.y = 0;
		particle->offset.x = 0;
		particle->acceleration.z = 0;
		particle->acceleration.y = 0;
		particle->acceleration.x = 0;
		particle->flag_bits = 0;
		particle->z_undulate = 0;
	}

	return particle;
}

void FX_AniTexSetup(struct _FX_PRIM* fxPrim, struct _MFace* mface, struct _Model* model, struct _FXTracker* fxTracker) // Matching - 100%
{
	if ((mface->flags & 0x2))
	{
		fxPrim->flags |= 0x1;
		fxPrim->texture = (struct TextureMT3*)mface->color;
		fxPrim->color = (fxPrim->texture->color & 0x3FFFFFF) | 0x24000000;
	}
	else
	{
		fxPrim->flags &= ~0x1;
		fxPrim->flags &= ~0x4;
		fxPrim->color = (mface->color & 0x3FFFFFF) | 0x20000000;
	}
}

void FX_StandardProcess(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker) // Matching - 100%
{
	FX_StandardFXPrimProcess(fxPrim, fxTracker);
}


// autogenerated function stub: 
// void /*$ra*/ FX_ShatterProcess(struct _FX_PRIM *fxPrim /*$s0*/, struct _FXTracker *fxTracker /*$s1*/)
void FX_ShatterProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker)
{ // line 396, offset 0x8004229c
	UNIMPLEMENTED();
}

void FX_DFacadeProcess(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker) // Matching - 100%
{
	MATRIX* swTransform;
	struct _Rotation rot;

	if (fxPrim->timeToLive > 0)
	{
		fxPrim->timeToLive--;
	}

	if (fxPrim->timeToLive == 0)
	{
		FX_Die(fxPrim, fxTracker);
	}
	else
	{
		if ((fxPrim->flags & 0x20))
		{
			swTransform = (MATRIX*)&fxPrim->duo.flame.parent->matrix[fxPrim->duo.flame.segment];

			fxPrim->position.x = (short)swTransform->t[0];
			fxPrim->position.y = (short)swTransform->t[1];
			fxPrim->position.z = (short)swTransform->t[2];
		}
		else
		{
			if (!(fxPrim->flags & 2))
			{
				fxPrim->duo.phys.xVel += fxPrim->duo.phys.xAccl;
				fxPrim->duo.phys.yVel += fxPrim->duo.phys.yAccl;
				fxPrim->duo.phys.zVel += fxPrim->duo.phys.zAccl;

				fxPrim->position.x += fxPrim->duo.phys.xVel;
				fxPrim->position.y += fxPrim->duo.phys.yVel;
				fxPrim->position.z += fxPrim->duo.phys.zVel;

				if (((fxPrim->flags & 0x100)) && (fxPrim->work0 >= fxPrim->position.z))
				{
					fxPrim->position.z = fxPrim->work0;

					fxPrim->flags |= 2;
				}
			}
		}

		if (!(fxPrim->matrix->flags & 2))
		{
			fxPrim->matrix->flags |= 2;

			if ((fxPrim->flags & 0x80))
			{
				rot.x = ((char*)&fxPrim->work2)[1] * 4;
				rot.y = ((char*)&fxPrim->work3)[0] * 4;
				rot.z = ((char*)&fxPrim->work3)[1] * 4;

				RotMatrixX(rot.x, (MATRIX*)&fxPrim->matrix->lwTransform);

				RotMatrixY(rot.y, (MATRIX*)&fxPrim->matrix->lwTransform);

				RotMatrixZ(rot.z, (MATRIX*)&fxPrim->matrix->lwTransform);
			}
		}
	}
}

// autogenerated function stub: 
// struct _FX_PRIM * /*$ra*/ _FX_BuildSingleFaceWithModel(struct _Model *model /*$a0*/, struct _MFace *mface /*$s6*/, SVECTOR*center /*$s7*/, SVECTOR*vel /*$fp*/, SVECTOR*accl /*stack 16*/, struct _FXTracker *fxTracker /*stack 20*/, TDRFuncPtr__FX_BuildSingleFaceWithModel6fxSetup fxSetup /*stack 24*/, TDRFuncPtr__FX_BuildSingleFaceWithModel7fxProcess fxProcess /*stack 28*/, struct _FX_MATRIX *fxMatrix /*stack 32*/, int timeToLive /*stack 36*/)
struct _FX_PRIM * _FX_BuildSingleFaceWithModel(struct _Model *model, struct _MFace *mface, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct _FXTracker *fxTracker, TDRFuncPtr__FX_BuildSingleFaceWithModel6fxSetup fxSetup, TDRFuncPtr__FX_BuildSingleFaceWithModel7fxProcess fxProcess, struct _FX_MATRIX *fxMatrix, int timeToLive)
{ // line 856, offset 0x80042718
	/* begin block 1 */
		// Start line: 857
		// Start offset: 0x80042718
		// Variables:
			struct _FX_PRIM *fxPrim; // $s1
			struct _MVertex *vertexList; // $s3
	/* end block 1 */
	// End offset: 0x800427E0
	// End Line: 885

	/* begin block 2 */
		// Start line: 1677
	/* end block 2 */
	// End Line: 1678
			UNIMPLEMENTED();
	return null;
}


struct _FX_PRIM* FX_BuildSingleFaceWithModel(struct _Model* model, struct _MFace* mface, SVECTOR* center, SVECTOR* vel, SVECTOR* accl, struct _FXTracker* fxTracker, void (*fxSetup)(), void (*fxProcess)(), short timeToLive) // Matching - 100%
{
	return _FX_BuildSingleFaceWithModel(model, mface, center, vel, accl, fxTracker, fxSetup, fxProcess, 0, timeToLive);
}

void _FX_SetupLighting(struct _Instance* instance) // Matching - 100%
{
	MATRIX lm;

	LIGHT_PresetInstanceLight(instance, 2048, &lm);

	MulMatrix0(&lm, &instance->matrix[instance->lightMatrix], &lm);
	
	SetLightMatrix(&lm);
}


// autogenerated function stub: 
// long /*$ra*/ _FX_DoLighting(struct _MFace *mface /*$a0*/)
long _FX_DoLighting(struct _MFace *mface)
{ // line 928, offset 0x800428b4
	/* begin block 1 */
		// Start line: 929
		// Start offset: 0x800428B4
		// Variables:
			long modelDQP; // $a2
			long color; // stack offset -8
			struct TextureMT3 *texture; // $a1
	/* end block 1 */
	// End offset: 0x800429C0
	// End Line: 961

	/* begin block 2 */
		// Start line: 1797
	/* end block 2 */
	// End Line: 1798
			UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ _FX_BuildSegmentedSplinters(struct _Instance *instance /*stack 0*/, SVECTOR*center /*stack 4*/, SVECTOR*vel /*stack 8*/, SVECTOR*accl /*stack 12*/, struct FXSplinter *splintDef /*stack 16*/, struct _FXTracker *fxTracker /*stack 20*/, TDRFuncPtr__FX_BuildSegmentedSplinters6fxSetup fxSetup /*stack 24*/, TDRFuncPtr__FX_BuildSegmentedSplinters7fxProcess fxProcess /*stack 28*/, int shardFlags /*stack 32*/)
void _FX_BuildSegmentedSplinters(struct _Instance *instance, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, TDRFuncPtr__FX_BuildSegmentedSplinters6fxSetup fxSetup, TDRFuncPtr__FX_BuildSegmentedSplinters7fxProcess fxProcess, int shardFlags)
{ // line 976, offset 0x800429cc
	/* begin block 1 */
		// Start line: 977
		// Start offset: 0x800429CC
		// Variables:
			long j; // $s4
			long maxTimeToLive; // stack offset -80
			long faceSkip; // stack offset -76
			long numFaces; // $v0
			struct _MFace *mface; // $s5
			struct _MFace *endMFace; // stack offset -72
			struct _Model *model; // stack offset -68
			struct _MVertex *vertexList; // stack offset -64
			struct _MFace *faceList; // $s0
			struct _FX_MATRIX *fxMatrix; // $s3
			struct _FX_MATRIX (*fxMatList[60]); // stack offset -960
			struct _SVector veloc[60]; // stack offset -720
			struct _SVector *curVel; // $s7
			int lastFxMat; // $fp
			unsigned char matIdx[128]; // stack offset -240
			struct _FX_PRIM *fxPrim; // $s2
			struct _SVector *vertex0; // $s0
			struct _SVector *vertex1; // $s1
			struct _SVector *vertex2; // $s4
			struct _SVector *poolOfVertices; // stack offset -60
			long *vtxSegment; // stack offset -56
			struct _Vector Center; // stack offset -112
			int seg0; // $s6
			int seg1; // $a0
			struct _SVector offset; // stack offset -96

		/* begin block 1.1 */
			// Start line: 1059
			// Start offset: 0x80042BBC
			// Variables:
				short _y0; // $v0
				short _z0; // $v1
				short _x1; // $a1
				short _y1; // $a3
				short _z1; // $a2
				struct _SVector *_v; // stack offset -48
				struct _Vector *_v1; // $a2
		/* end block 1.1 */
		// End offset: 0x80042BBC
		// End Line: 1059

		/* begin block 1.2 */
			// Start line: 1072
			// Start offset: 0x80042C50
		/* end block 1.2 */
		// End offset: 0x80042D10
		// End Line: 1083

		/* begin block 1.3 */
			// Start line: 1136
			// Start offset: 0x80043008
			// Variables:
				//short _x0; // $a1
				//short _y0; // $v1
				//short _z0; // $v0
				//short _x1; // $a0
				//short _y1; // $a3
				//short _z1; // $a2
				//_Position *_v0; // $v0
				//struct _Vector *_v1; // $a2
		/* end block 1.3 */
		// End offset: 0x80043008
		// End Line: 1136

		/* begin block 1.4 */
			// Start line: 1165
			// Start offset: 0x80043144
			// Variables:
				struct TextureMT3 *texture; // $v1
		/* end block 1.4 */
		// End offset: 0x80043144
		// End Line: 1165

		/* begin block 1.5 */
			// Start line: 1195
			// Start offset: 0x80043210
		/* end block 1.5 */
		// End offset: 0x80043270
		// End Line: 1200

		/* begin block 1.6 */
			// Start line: 1217
			// Start offset: 0x800432E4
		/* end block 1.6 */
		// End offset: 0x8004332C
		// End Line: 1223
	/* end block 1 */
	// End offset: 0x80043420
	// End Line: 1243

	/* begin block 2 */
		// Start line: 1895
	/* end block 2 */
	// End Line: 1896

	/* begin block 3 */
		// Start line: 1925
	/* end block 3 */
	// End Line: 1926
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ _FX_BuildNonSegmentedSplinters(struct _Instance *instance /*stack 0*/, SVECTOR*center /*stack 4*/, SVECTOR*vel /*stack 8*/, SVECTOR*accl /*stack 12*/, struct FXSplinter *splintDef /*stack 16*/, struct _FXTracker *fxTracker /*stack 20*/, TDRFuncPtr__FX_BuildNonSegmentedSplinters6fxSetup fxSetup /*stack 24*/, TDRFuncPtr__FX_BuildNonSegmentedSplinters7fxProcess fxProcess /*stack 28*/, int shardFlags /*stack 32*/)
void _FX_BuildNonSegmentedSplinters(struct _Instance *instance, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, TDRFuncPtr__FX_BuildNonSegmentedSplinters6fxSetup fxSetup, TDRFuncPtr__FX_BuildNonSegmentedSplinters7fxProcess fxProcess, int shardFlags)
{ // line 1254, offset 0x80043450
	/* begin block 1 */
		// Start line: 1255
		// Start offset: 0x80043450
		// Variables:
			long j; // $s2
			long maxTimeToLive; // stack offset -72
			long faceSkip; // stack offset -68
			long numFaces; // $v0
			struct _MFace *mface; // $s5
			struct _MFace *endMFace; // stack offset -64
			short whichMatrix; // $v0
			struct _Model *model; // stack offset -60
			struct _MVertex *vertexList; // stack offset -56
			struct _MFace *faceList; // $s4
			struct _FX_MATRIX *fxMatrix; // $s0
			struct _FX_MATRIX (*fxMatList[4]); // stack offset -200
			struct _SVector *curVel; // $s6
			struct _FX_PRIM *fxPrim; // $s2
			struct _SVector *vertex0; // $s1
			struct _SVector *vertex1; // $s3
			struct _SVector *vertex2; // $s4
			struct _SVector *poolOfVertices; // $fp
			//MATRIX *swTransform; // $s1
			//MATRIX ourM; // stack offset -184
			struct _Vector Center; // stack offset -152
			struct _SVector offset; // stack offset -136
			struct _Normal faceNorm; // stack offset -128
			struct _SVector sv_temp; // stack offset -120
			struct _SVector sv_vels[4]; // stack offset -112

		/* begin block 1.1 */
			// Start line: 1296
			// Start offset: 0x80043564
			// Variables:
				short start; // $v0
		/* end block 1.1 */
		// End offset: 0x8004368C
		// End Line: 1322

		/* begin block 1.2 */
			// Start line: 1352
			// Start offset: 0x800437F8
			// Variables:
				short _x1; // $v0
				short _y1; // $v1
				short _z1; // $a0
				struct _SVector *_v0; // $s3
				struct _SVector *_v1; // $a1
		/* end block 1.2 */
		// End offset: 0x800437F8
		// End Line: 1352

		/* begin block 1.3 */
			// Start line: 1414
			// Start offset: 0x80043BAC
			// Variables:
				//short _x0; // $a1
				//short _y0; // $v1
				//short _z0; // $v0
				//short _x1; // $a0
				//short _y1; // $a3
				//short _z1; // $a2
				//struct _SVector *_v; // stack offset -48
				//_Position *_v0; // $v0
				//struct _Vector *_v1; // $a2
		/* end block 1.3 */
		// End offset: 0x80043BAC
		// End Line: 1414

		/* begin block 1.4 */
			// Start line: 1443
			// Start offset: 0x80043CE8
			// Variables:
				struct TextureMT3 *texture; // $v1
		/* end block 1.4 */
		// End offset: 0x80043CE8
		// End Line: 1443

		/* begin block 1.5 */
			// Start line: 1473
			// Start offset: 0x80043DA4
		/* end block 1.5 */
		// End offset: 0x80043DA4
		// End Line: 1473

		/* begin block 1.6 */
			// Start line: 1499
			// Start offset: 0x80043EF0
		/* end block 1.6 */
		// End offset: 0x80043F38
		// End Line: 1505
	/* end block 1 */
	// End offset: 0x8004400C
	// End Line: 1530

	/* begin block 2 */
		// Start line: 2797
	/* end block 2 */
	// End Line: 2798
				UNIMPLEMENTED();
}


void _FX_BuildSplinters(struct _Instance* instance, SVECTOR* center, SVECTOR* vel, SVECTOR* accl, struct FXSplinter* splintDef, struct _FXTracker* fxTracker, void (*fxSetup)(), void (*fxProcess)(), short shardFlags) // Matching - 100%
{
	if (MEMPACK_MemoryValidFunc((char*)instance->object) != 0)
	{
		if (splintDef != NULL)
		{
			shardFlags |= splintDef->flags;

			if (splintDef->soundFx != 0)
			{
				SndPlay(splintDef->soundFx);
			}
		}

		if ((instance->object->modelList[instance->currentModel]->numSegments < 4) || ((shardFlags & 0x20)))
		{
			_FX_BuildNonSegmentedSplinters(instance, center, vel, accl, splintDef, fxTracker, fxSetup, fxProcess, shardFlags);
		}
		else
		{
			_FX_BuildSegmentedSplinters(instance, center, vel, accl, splintDef, fxTracker, fxSetup, fxProcess, shardFlags);
		}
	}
}


void _FX_Build(struct _Instance *instance, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct _FXTracker *fxTracker, TDRFuncPtr__FX_Build5fxSetup fxSetup, TDRFuncPtr__FX_Build6fxProcess fxProcess, int shardFlags)
{
	if (MEMPACK_MemoryValidFunc((char*)instance->object) != 0)
	{
		if (instance->object->modelList[instance->currentModel]->numSegments < 4)
		{
			_FX_BuildNonSegmentedSplinters(instance, center, vel, accl, NULL, fxTracker, fxSetup, fxProcess, shardFlags);
		}
		else
		{
			_FX_BuildSegmentedSplinters(instance, center, vel, accl, NULL, fxTracker, fxSetup, fxProcess, shardFlags);
		}
	}
}

void FX_Build(struct _Instance* instance, SVECTOR* center, SVECTOR* vel, SVECTOR* accl, struct _FXTracker* fxTracker, TDRFuncPtr_FX_Build5fxSetup fxSetup, TDRFuncPtr_FX_Build6fxProcess fxProcess)
{
	_FX_Build(instance, center, vel, accl, fxTracker, fxSetup, fxProcess, 0);
}

void FX_UpdatePos(struct _FX_PRIM* fxPrim, struct _SVector* offset, int spriteflag) // Matching - 100%
{
	short _x0;
	short _y0;
	short _z0;

	short _x1;
	short _y1;
	short _z1;

	struct _Position* _v;

	_v = &fxPrim->position;

	_x0 = _v->x;
	_y0 = _v->y;
	_z0 = _v->z;

	_x1 = offset->x;
	_y1 = offset->y;
	_z1 = offset->z;

	_v->x = _x0 + _x1;
	_v->y = _y0 + _y1;
	_v->z = _z0 + _z1;

	if (spriteflag == 0 && fxPrim->flags & 0x10000)
	{

		fxPrim->v0.x += offset->x;
		fxPrim->v0.y += offset->y;
		fxPrim->v0.z += offset->z;

		fxPrim->v1.x += offset->x;
		fxPrim->v1.y += offset->y;
		fxPrim->v1.z += offset->z;

		fxPrim->v2.x += offset->x;
		fxPrim->v2.y += offset->y;
		fxPrim->v2.z += offset->z;

		if (fxPrim->flags & 0x8)
		{
			fxPrim->v3.x += offset->x;
			fxPrim->v3.y += offset->y;
			fxPrim->v3.z += offset->z;
		}
	}
}

void FX_Relocate(struct _SVector* offset)//Matching - 99.72%
{
	struct _FX_PRIM* fxPrim;
	struct _FXTracker* fxTracker;
	struct _FXGeneralEffect* currentEffect;
	int i;
	int end;
	struct _FXRibbon* currentRibbon;

	fxTracker = gFXT;

	fxPrim = (struct _FX_PRIM*)fxTracker->usedPrimList.next;

	while (fxPrim != NULL)
	{
		FX_UpdatePos(fxPrim, offset, 0);

		fxPrim = (struct _FX_PRIM*)fxPrim->node.next;
	}

	fxPrim = (struct _FX_PRIM*)fxTracker->usedPrimListSprite.next;

	while (fxPrim != NULL)
	{
		FX_UpdatePos(fxPrim, offset, 1);

		if (fxPrim->process == &FX_WaterBubbleProcess)
		{
			fxPrim->timeToLive += offset->z;
		}

		fxPrim = (struct _FX_PRIM*)fxPrim->node.next;
	}

	currentEffect = FX_GeneralEffectTracker;

	while (currentEffect != NULL)
	{
		if (currentEffect->effectType == 0)
		{
			end = ((short*)currentEffect)[8];

			currentRibbon = (struct _FXRibbon*)currentEffect;

			for (i = 0; i < end; i++)
			{
				currentRibbon->vertexPool[i].vx += offset->x;
				currentRibbon->vertexPool[i].vy += offset->y;
				currentRibbon->vertexPool[i].vz += offset->z;
			}
		}
		else if (currentEffect->effectType == 0x84)
		{
			((struct _GenericLightningParams*)currentEffect)->end_offset.x += offset->x;
			((struct _GenericLightningParams*)currentEffect)->end_offset.y += offset->y;
			((struct _GenericLightningParams*)currentEffect)->end_offset.z += offset->z;
		}

		currentEffect = (struct _FXGeneralEffect*)currentEffect->next;
	}
}

void FX_UpdateTexturePointers(struct _FX_PRIM* fxPrim, struct Object* oldObject, int sizeOfObject, int offset)//Matching - 100%
{
	while (fxPrim)
	{
		if ((fxPrim->flags & 0x1))
		{
			if (fxPrim->texture >= (struct TextureMT3*)oldObject && (char*)oldObject + sizeOfObject >= (char*)fxPrim->texture)

			{
				fxPrim->texture = fxPrim->texture != NULL ? (struct TextureMT3*)((char*)fxPrim->texture + offset) : NULL;

			}
		}

		fxPrim = (struct _FX_PRIM*)fxPrim->node.next;
	}
}

void FX_RelocateFXPointers(struct Object* oldObject, struct Object* newObject, long sizeOfObject) // Matching - 100%
{
	struct _FXTracker* fxTracker;
	struct _FXGeneralEffect* currentEffect;
	long offset;
	struct _FXParticle* particle; // not from SYMDUMP

	fxTracker = gFXT;

	offset = (int)newObject - (int)oldObject;

	FX_UpdateTexturePointers((struct _FX_PRIM*)fxTracker->usedPrimList.next, oldObject, (int)sizeOfObject, (int)offset);
	FX_UpdateTexturePointers((struct _FX_PRIM*)fxTracker->usedPrimListSprite.next, oldObject, (int)sizeOfObject, (int)offset);

	currentEffect = FX_GeneralEffectTracker;

	while (currentEffect != NULL)
	{
		if (currentEffect->effectType == 1)
		{
			particle = (struct _FXParticle*)currentEffect;
			if (particle->texture != NULL && IN_BOUNDS(particle->texture, oldObject, (int)oldObject + sizeOfObject))
			{
				particle->texture = (struct TextureMT3*)OFFSET_DATA(particle->texture, offset);
			}
		}

		currentEffect = (struct _FXGeneralEffect*)currentEffect->next;
	}
}

void FX_ProcessList(struct _FXTracker* fxTracker) // Matching - 100%
{
	struct _FX_PRIM* fxPrim;
	struct _FX_PRIM* nextFXPrim;
	struct _FX_MATRIX* fxMatrix;
	struct _FX_MATRIX* nextFXMatrix;

	FX_TimeCount += (short)gameTrackerX.timeMult;

	FX_Frames = FX_TimeCount / 4096;

	FX_TimeCount -= FX_Frames << 12;

	for (fxMatrix = (struct _FX_MATRIX*)fxTracker->usedMatrixList.next; fxMatrix != NULL; fxMatrix = (struct _FX_MATRIX*)fxMatrix->node.next)
	{
		if ((fxMatrix->flags & 0x4))
		{
			fxMatrix->flags |= 0x1;
		}
		else
		{
			fxMatrix->flags &= ~0x1;
		}

		fxMatrix->flags &= ~0x2;
	}

	for (fxPrim = (struct _FX_PRIM*)fxTracker->usedPrimList.next; fxPrim != NULL; fxPrim = nextFXPrim)
	{
		nextFXPrim = (struct _FX_PRIM*)fxPrim->node.next;

		if (fxPrim->matrix != NULL)
		{
			fxPrim->matrix->flags |= 0x1;
		}

		if (fxPrim->process != NULL)
		{
			fxPrim->process(fxPrim, fxTracker);
		}
	}

	for (fxPrim = (struct _FX_PRIM*)fxTracker->usedPrimListSprite.next; fxPrim != NULL; fxPrim = nextFXPrim)
	{
		nextFXPrim = (struct _FX_PRIM*)fxPrim->node.next;

		if (fxPrim->process != NULL)
		{
			fxPrim->process(fxPrim, fxTracker);
		}
	}

	for (fxMatrix = (struct _FX_MATRIX*)fxTracker->usedMatrixList.next; fxMatrix != NULL; fxMatrix = nextFXMatrix)
	{
		nextFXMatrix = (struct _FX_MATRIX*)fxMatrix->node.next;

		if (!(fxMatrix->flags & 0x1))
		{
			LIST_DeleteFunc((struct NodeType*)fxMatrix);

			LIST_InsertFunc(&fxTracker->freeMatrixList, (struct NodeType*)fxMatrix);
		}
	}

	{
		struct _FXGeneralEffect* currentEffect;
		struct _FXGeneralEffect* nextEffect;

		for (currentEffect = FX_GeneralEffectTracker; currentEffect != NULL; currentEffect = nextEffect)
		{
			nextEffect = (struct _FXGeneralEffect*)currentEffect->next;

			if (currentEffect->continue_process != NULL)
			{
				currentEffect->continue_process(currentEffect, fxTracker);
			}
		}

		if (FX_ConstrictStage == 1)
		{
			FX_ConstrictStage = 0;
		}

		if (snow_amount != 0)
		{
			FX_ContinueSnow(fxTracker);
		}

		if (rain_amount != 0)
		{
			FX_ContinueRain(fxTracker);
		}

		FX_UpdateWind(fxTracker);
	}
}

void FX_DrawReaver(struct _PrimPool *primPool, unsigned int **ot, MATRIX *wcTransform)
{
	if (FX_reaver_instance != NULL)
	{
		FX_SoulReaverWinding(FX_reaver_instance, primPool, ot, wcTransform);

		FX_SoulReaverBlade(FX_reaver_instance, ot);
	}

	FX_reaver_instance = NULL;
}


// autogenerated function stub: 
// void /*$ra*/ FX_DrawList(struct _FXTracker *fxTracker /*stack 0*/, struct GameTracker *gameTracker /*$a1*/, unsigned long **ot /*$s5*/, MATRIX *wcTransform /*$s7*/)
void FX_DrawList(struct _FXTracker *fxTracker, struct GameTracker *gameTracker, unsigned long **ot, MATRIX *wcTransform)
{ // line 1985, offset 0x80044958
	/* begin block 1 */
		// Start line: 1986
		// Start offset: 0x80044958
		// Variables:
			struct TextureMT3 *texture; // $a1
			struct _PrimPool *primPool; // $fp
			long *prim; // $s1
			struct _FX_PRIM *fxPrim; // $s0
			struct _FX_PRIM *nextFXPrim; // $s6
			//SVECTORsv0; // stack offset -104
			//SVECTORsv1; // stack offset -96
			//SVECTORsv2; // stack offset -88
			long otz; // stack offset -52
			long sz0; // stack offset -64
			long sz1; // stack offset -60
			long sz2; // stack offset -56
			char whitec[4]; // stack offset -80
			int sizex; // stack offset -48
			int sizey; // $t4
			int matrix_wc; // $t2

		/* begin block 1.1 */
			// Start line: 2007
			// Start offset: 0x800449DC
			// Variables:
				long flags; // $s3

			/* begin block 1.1.1 */
				// Start line: 2095
				// Start offset: 0x80044CC8
				// Variables:
					struct _POLY_NG4 *ng4; // $a2

				/* begin block 1.1.1.1 */
					// Start line: 2097
					// Start offset: 0x80044CC8
					// Variables:
						int n; // $a0
						long *ptr; // $a1
				/* end block 1.1.1.1 */
				// End offset: 0x80044D88
				// End Line: 2122
			/* end block 1.1.1 */
			// End offset: 0x80044D88
			// End Line: 2144

			/* begin block 1.1.2 */
				// Start line: 2161
				// Start offset: 0x80044DC0
			/* end block 1.1.2 */
			// End offset: 0x80044DC0
			// End Line: 2163

			/* begin block 1.1.3 */
				// Start line: 2251
				// Start offset: 0x80044F40
				// Variables:
					//struct POLY_FT4 *ft4; // $a2
					unsigned short uMin; // $t1
					unsigned short uMax; // $t0
					unsigned short vMin; // $a3
					unsigned short vMax; // $v1
			/* end block 1.1.3 */
			// End offset: 0x8004518C
			// End Line: 2282

			/* begin block 1.1.4 */
				// Start line: 2289
				// Start offset: 0x800451C8
				// Variables:
					//struct POLY_FT3 *ft3; // $t0

				/* begin block 1.1.4.1 */
					// Start line: 2302
					// Start offset: 0x80045214
					// Variables:
						//short uMin; // $a3
						//short uMax; // $a2
				/* end block 1.1.4.1 */
				// End offset: 0x80045318
				// End Line: 2310
			/* end block 1.1.4 */
			// End offset: 0x80045354
			// End Line: 2316
		/* end block 1.1 */
		// End offset: 0x80045354
		// End Line: 2320

		/* begin block 1.2 */
			// Start line: 2331
			// Start offset: 0x80045394
			// Variables:
				//struct DVECTOR xy_pos; // stack offset -72
				//long flags; // $t2

			/* begin block 1.2.1 */
				// Start line: 2400
				// Start offset: 0x800455D0
				// Variables:
					struct _POLY_SG4 *sg4; // $a1

				/* begin block 1.2.1.1 */
					// Start line: 2402
					// Start offset: 0x800455D0
					// Variables:
						//int n; // $a3
						long *src; // $t2
						long *dst; // $t1
						//long *ptr; // $t3
				/* end block 1.2.1.1 */
				// End offset: 0x800456C8
				// End Line: 2433
			/* end block 1.2.1 */
			// End offset: 0x800456C8
			// End Line: 2455

			/* begin block 1.2.2 */
				// Start line: 2467
				// Start offset: 0x8004575C
			/* end block 1.2.2 */
			// End offset: 0x8004575C
			// End Line: 2471

			/* begin block 1.2.3 */
				// Start line: 2499
				// Start offset: 0x80045834
				// Variables:
					//unsigned short uMin; // $t3
					//unsigned short uMax; // $t1
					//unsigned short vMin; // $a3
					//unsigned short vMax; // $v1
					//struct POLY_FT4 *ft4; // $a2
			/* end block 1.2.3 */
			// End offset: 0x80045AA0
			// End Line: 2537
		/* end block 1.2 */
		// End offset: 0x80045B0C
		// End Line: 2556
	/* end block 1 */
	// End offset: 0x80045B18
	// End Line: 2560

	/* begin block 2 */
		// Start line: 4713
	/* end block 2 */
	// End Line: 4714
						UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_SimpleQuadSetup(struct _FX_PRIM *fxPrim /*$s5*/, TDRFuncPtr_FX_SimpleQuadSetup1fxProcess fxProcess /*stack 4*/, struct _FX_MATRIX *fxMatrix /*stack 8*/, struct _Instance *instance /*$a3*/, struct _MFace *mface /*stack 16*/, struct _MVertex *vertexList /*stack 20*/, SVECTOR*center /*stack 24*/, SVECTOR*vel /*stack 28*/, SVECTOR*accl /*stack 32*/, struct _FXTracker *fxTracker /*stack 36*/, int timeToLive /*stack 40*/)
void FX_SimpleQuadSetup(struct _FX_PRIM *fxPrim, TDRFuncPtr_FX_SimpleQuadSetup1fxProcess fxProcess, struct _FX_MATRIX *fxMatrix, struct _Instance *instance, struct _MFace *mface, struct _MVertex *vertexList, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct _FXTracker *fxTracker, int timeToLive)
{ // line 2727, offset 0x80045b60
	/* begin block 1 */
		// Start line: 2728
		// Start offset: 0x80045B60
		// Variables:
			struct _MVertex *vertex1; // $s2
			struct _MVertex *vertex2; // $s3
			struct _MVertex *vertex3; // $s4

		/* begin block 1.1 */
			// Start line: 2728
			// Start offset: 0x80045B60
			// Variables:
				short _x1; // $v0
				short _y1; // $v1
				short _z1; // $a0
				_Position *_v0; // $v0
		/* end block 1.1 */
		// End offset: 0x80045B60
		// End Line: 2728

		/* begin block 1.2 */
			// Start line: 2728
			// Start offset: 0x80045B60
			// Variables:
				//short _x1; // $v0
				//short _y1; // $v1
				//short _z1; // $a0
				//struct _SVector *_v0; // $v0
		/* end block 1.2 */
		// End offset: 0x80045B60
		// End Line: 2728

		/* begin block 1.3 */
			// Start line: 2728
			// Start offset: 0x80045B60
			// Variables:
				//short _x1; // $v0
				//short _y1; // $v1
				//short _z1; // $a0
				//struct _SVector *_v0; // $v0
		/* end block 1.3 */
		// End offset: 0x80045B60
		// End Line: 2728

		/* begin block 1.4 */
			// Start line: 2728
			// Start offset: 0x80045B60
			// Variables:
				//short _x1; // $v0
				//short _y1; // $v1
				//short _z1; // $a0
				//struct _SVector *_v0; // $v0
		/* end block 1.4 */
		// End offset: 0x80045B60
		// End Line: 2728

		/* begin block 1.5 */
			// Start line: 2728
			// Start offset: 0x80045B60
			// Variables:
				//short _x1; // $v0
				//short _y1; // $v1
				//short _z1; // $a0
				//struct _SVector *_v0; // $v0
		/* end block 1.5 */
		// End offset: 0x80045B60
		// End Line: 2728
	/* end block 1 */
	// End offset: 0x80045D58
	// End Line: 2792

	/* begin block 2 */
		// Start line: 5454
	/* end block 2 */
	// End Line: 5455
				UNIMPLEMENTED();
}

void FX_WaterRingProcess(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker) // Matching - 100%
{
	fxPrim->v0.x -= 8;
	fxPrim->v0.y -= 8;
	fxPrim->v1.x += 8;
	fxPrim->v1.y -= 8;
	fxPrim->v2.x -= 8;
	fxPrim->v2.y += 8;
	fxPrim->v3.x += 8;
	fxPrim->v3.y += 8;

	FX_StandardFXPrimProcess(fxPrim, fxTracker);
}

void FX_WaterBubbleProcess(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker) // Matching - 99.51%
{
	struct _FX_PRIM* temp;
	int temp2;  // not from SYMDUMP

	if (fxPrim->duo.phys.zVel < fxPrim->work1)
	{
		fxPrim->duo.phys.zVel += fxPrim->duo.phys.zAccl;
	}

	if (fxPrim->work0 < fxPrim->duo.phys.xAccl)
	{
		fxPrim->position.x += fxPrim->duo.phys.xVel;
		fxPrim->position.y += fxPrim->duo.phys.yVel;
	}

	fxPrim->position.z += fxPrim->duo.phys.zVel;

	fxPrim->work0 += 1;

	fxPrim->v2.y = fxPrim->work3 - (fxPrim->work0 * fxPrim->work2);
	fxPrim->v0.y = fxPrim->v2.y;

	if (fxPrim->v0.y >= fxPrim->duo.phys.yAccl)
	{
		if (fxPrim->position.z > fxPrim->timeToLive)
		{
			struct Object* waterfx;
			struct _Model* wxring;

			waterfx = (struct Object*)objectAccess[3].object;

			if (waterfx != NULL)
			{
				if (fxPrim->v1.y < fxPrim->v0.y)
				{
					wxring = *waterfx->modelList;

					temp = FX_GetPrim(gFXT);

					if (temp != NULL)
					{
						temp->position = fxPrim->position;
						temp->v2.x = temp->position.x - 8;
						temp->v0.x = temp->v2.x;
						temp->v1.y = temp->position.y - 8;
						temp->v0.y = temp->v1.y;
						temp->v3.x = temp->position.x + 8;
						temp->v1.x = temp->v3.x;
						temp->v3.z = temp->position.z;
						temp->v2.z = temp->v3.z;
						temp->v1.z = temp->v2.z;
						temp->v0.z = temp->v2.z;
						temp->v3.y = temp->position.y + 8;
						temp->v2.y = temp->v3.y;

						temp2 = wxring->faceList->color;

						temp->process = FX_WaterRingProcess;
						temp->timeToLive = 16;
						temp->flags |= 0x50009;
						temp->color = 0x2EFFFFFF;
						temp->startColor = 0xFFFFFF;
						temp->endColor = 0;
						temp->fadeValue[0] = 0;
						temp->fadeStep = 256;
						temp->texture = (struct TextureMT3*)temp2;

						LIST_InsertFunc(&fxTracker->usedPrimList, (struct NodeType*)temp);
					}
				}
			}

			FX_Die(fxPrim, fxTracker);
		}
	}
	else
	{
		FX_Die(fxPrim, fxTracker);
	}
}

void FX_Sprite_Insert(struct NodeType* list, struct _FX_PRIM* fxPrim)
{
	LIST_InsertFunc(list, &fxPrim->node);
	
	if (FX_LastUsedPrim == NULL)
	{
		FX_LastUsedPrim = fxPrim;
	}
}

struct TextureMT3* FX_GetTextureObject(struct Object *object, int modelnum, int texnum)
{
	struct _Model* model;
	struct TextureMT3* texture;
	
	object->oflags2 |= 0x20000000;

	model = object->modelList[modelnum];
	texture = (struct TextureMT3*)&((char*)((((struct _FX_PRIM*)model->faceList)->process)))[texnum * 16 + 16];

	return texture;
}


void FX_MakeWaterBubble(struct _SVector* position, struct _SVector* vel, struct _SVector* accl, long splashZ, struct __BubbleParams* BP) // Matching - 100%
{
	struct Object* waterfx;
	struct _FX_PRIM* fxPrim;

	waterfx = (struct Object*)objectAccess[3].object;

	if (waterfx != NULL)
	{
		fxPrim = FX_GetPrim(gFXT);

		if (fxPrim != NULL)
		{
			FX_DFacadeParticleSetup(fxPrim, (SVECTOR*)position, 12, 12, 0x2C000000, (SVECTOR*)vel, (SVECTOR*)accl, gFXT, (short)splashZ);

			fxPrim->texture = FX_GetTextureObject(waterfx, 2, rand() % (BP->UniqueBubbles - 1));
			fxPrim->flags |= 0x1;
			fxPrim->color = ((fxPrim->texture->color & 0x03FFFFFF) | 0x2C000000);
			fxPrim->process = &FX_WaterBubbleProcess;

			fxPrim->work0 = 0;
			fxPrim->work1 = (BP->MaxSpeed + (rand() % BP->MaxSpeedRange));
			fxPrim->work2 = (BP->ScaleRate + (rand() % BP->ScaleRateRange));
			fxPrim->work3 = (BP->StartScale + (rand() % BP->StartScaleRange));

			fxPrim->duo.phys.xAccl = BP->DisperseFrames;
			fxPrim->duo.phys.yAccl = BP->KillScale;

			fxPrim->v1.y = BP->MinSplashSize;

			FX_Sprite_Insert(&gFXT->usedPrimListSprite, fxPrim);
		}
	}
}

void FX_DrawScreenPoly(int transtype, unsigned long color, int zdepth)
{
	unsigned long** drawot;
	_POLY_TF4* poly;

	drawot = gameTrackerX.drawOT;

	poly = (_POLY_TF4*)gameTrackerX.primPool->nextPrim;

	if ((unsigned int*)(poly + 1) < gameTrackerX.primPool->lastPrim)
	{
		poly->p1.y2 = SCREEN_HEIGHT;
		poly->p1.y3 = SCREEN_HEIGHT;

		poly->drawTPage = (transtype << 5) | 0xE1000600;

		poly->p1.x0 = 0;
		poly->p1.y0 = 0;

		poly->p1.x1 = SCREEN_WIDTH;
		poly->p1.y1 = 0;

		poly->p1.x2 = 0;
		poly->p1.x3 = SCREEN_WIDTH;

		((unsigned int*)&poly->p1.r0)[0] = color;

		poly->p1.code = 0x2A;
		poly->len = 0x6;

#if defined(PSXPC_VERSION)
		addPrim(drawot[zdepth * 2], poly);
#else
		addPrim(drawot[zdepth], poly);
#endif

		gameTrackerX.primPool->nextPrim = (unsigned int*)(poly + 1);
	}
}

POLY_GT4* FX_SetupPolyGT4(int x1, int y1, int x2, int y2, int otz, struct TextureMT3 *texture, long color0, long color1, long color2, long color3)
{
	POLY_GT4* poly;
	unsigned long** drawot;

	poly = (POLY_GT4*)gameTrackerX.primPool->nextPrim;
	drawot = gameTrackerX.drawOT;
	
	if ((char*)(poly + 1) < (char*)(gameTrackerX.primPool->lastPrim))
	{
		poly->u0 = texture->u2;
		poly->v0 = texture->v2;

		poly->u1 = texture->u1;
		poly->v1 = texture->v1;

		poly->x2 = x1;
		poly->x0 = x1;

		poly->x1 = x2;
		poly->x3 = x2;

		poly->y1 = y1;
		poly->y0 = y1;

		poly->y2 = y2;
		poly->y3 = y2;

		poly->u2 = texture->u0;
		poly->v2 = texture->v0;

		*(int*)&poly->r0 = color0 | 0x3C000000;
		*(int*)&poly->r1 = color1;
		*(int*)&poly->r2 = color2;
		
		poly->u3 = poly->u1;
		poly->v3 = poly->v2;

		*(int*)&poly->r3 = color3;

		poly->tpage = texture->tpage;
		poly->clut = texture->clut;

		if (otz <= 0)
		{
			otz = 1;
		}
		
#if defined(USE_32_BIT_ADDR)
		setlen(poly, 12);
		addPrim(drawot[otz * 2], poly);
#else
		setlen(poly, 12);
		addPrim(drawot[otz], poly);
#endif

		gameTrackerX.primPool->nextPrim = (unsigned int*)(poly + 1);

		return poly;
	}

	return NULL;
}


void FX_MakeWarpArrow(int x, int y, int xsize, int ysize, int fade)
{
	struct Object* particle;
	long color;
	POLY_GT4* poly;

	particle = (struct Object*)objectAccess[10].object;

	if (particle != NULL)
	{
		color = fade >> 5;

		if (fade < 0)
		{
			fade = 0;

			color = fade >> 5;

			if (fade < 0)
			{
				fade += 31;
				color = fade >> 5;
			}
		}

		color = color | color << 8 | color << 16;

		poly = FX_SetupPolyGT4(x + xsize, y, x, y + ysize, 3, FX_GetTextureObject(particle, 0, 9), color, color, color, color);

		if (poly != NULL)
		{
			poly->code |= 0x2;
		}
	}
}

void FX_MakeMannaIcon(int x, int y, int xsize, int ysize)
{ 
	struct Object* manna;
	struct TextureMT3* texture;
	POLY_GT4* poly;
	int n;
	int newx;
	int newy;
	int sizex;
	int sizey;
	unsigned long color;

	if (objectAccess[20].object != NULL)
	{
		texture = FX_GetTextureObject((struct Object*)objectAccess[20].object, 0, 7);

		poly = FX_SetupPolyGT4(x, y, x + xsize, y + ysize, 3, texture, 0x2962828, 0x2962828, 0x2962828, 0x2962828);
	
		if (poly != NULL)
		{
			poly->tpage |= 0x20;
		}

		if (objectAccess[10].object != NULL)
		{
			texture = FX_GetTextureObject((struct Object*)objectAccess[10].object, 0, 2);

			for (n = 0; n < 5; n++)
			{
				switch (n)
				{
				default:
				case 0:
				{
					newx = x + 33;
					newy = y - 1;

					sizex = 14;
					sizey = 12;

					color = 0x24B3232;

					break;
				}
				case 1:
				{
					newx = x + 3;
					newy = y + 5;

					sizex = 14;
					sizey = 12;

					color = 0x2704B4B;
					
					break;
				}
				case 2:
				{
					newx = x + 13;
					newy = y + 4;

					sizex = 19;
					sizey = 16;

					color = 0x2966464;

					break;
				}
				case 3:
				{
					newx = x + 3;
					newy = y + 16;

					sizex = 19;
					sizey = 16;

					color = 0x24B3232;

					break;
				}
				case 4:
				{
					newx = x + 32;
					newy = y + 13;

					sizex = 19;
					sizey = 16;

					color = 0x2966464;

					break;
				}
				}

				FX_SetupPolyGT4(newx, newy, newx + sizex, newy + sizey, 3, texture, color, color, color, color);
			}
		}
	}
}

void FX_MakeGlyphIcon(struct _Position* position, struct Object* glyphObject, int size, int glyphnum, int enabled)
{
	int sizex;
	POLY_GT4* poly;
	struct TextureMT3* texture;
	DVECTOR xy_pos;
	int otz;
	long* color_array;
	struct _GlyphTuneData* glyphtunedata;
	struct Object* object;

	if (glyphObject != NULL)
	{
		if (glyphnum == 7)
		{
			object = (struct Object*)objectAccess[10].object;

			if (object != NULL)
			{
				texture = FX_GetTextureObject(object, 0, 0);

				size = (size + (size >> 31)) >> 1;

				xy_pos.vx = position->x;
				xy_pos.vy = position->y;

				sizex = ((size << 9) / 320);

				glyphtunedata = (struct _GlyphTuneData*)glyphObject->data;

				otz = (24 - size) < 0 ? (((24 - size) + 3) >> 2) + 1 : ((24 - size) >> 2) + 1;

				color_array = glyphtunedata->color_array;

				if (enabled != 0)
				{
					poly = FX_SetupPolyGT4(position->x - sizex, position->y - size, position->x + sizex + 1, position->y + size + 1, otz, texture, color_array[glyphnum], color_array[glyphnum + 1], color_array[glyphnum + 2], color_array[glyphnum + 3]);
				}
				else
				{
					poly = FX_SetupPolyGT4(position->x - sizex, position->y - size, position->x + sizex + 1, position->y + size + 1, otz, texture, 0x202020, 0x202020, 0x202020, 0x202020);
				}

				if (poly != NULL)
				{
					poly->code = (((char*)&texture->color)[3] & 0x3) | 0x3C;
				}
			}
		}
		else
		{
			texture = FX_GetTextureObject(glyphObject, 0, glyphnum);

			xy_pos.vx = position->x;
			xy_pos.vy = position->y;

			sizex = ((size << 9) / 320);

			glyphtunedata = (struct _GlyphTuneData*)glyphObject->data;

			otz = (24 - size) < 0 ? (((24 - size) + 3) >> 2) + 1: ((24 - size) >> 2) + 1;

			color_array = &glyphtunedata->color_array[glyphnum * 4];

			if (enabled != 0)
			{
				poly = FX_SetupPolyGT4(position->x - sizex, position->y - size, position->x + sizex + 1, position->y + size + 1, otz, texture, color_array[0], color_array[1], color_array[2], color_array[3]);
			}
			else
			{
				poly = FX_SetupPolyGT4(position->x - sizex, position->y - size, position->x + sizex + 1, position->y + size + 1, otz, texture, 0x202020, 0x202020, 0x202020, 0x202020);
			}

			if (poly != NULL)
			{
				poly->code = (((char*)&texture->color)[3] & 0x3) | 0x3C;
			}
		}
	}
}


void FX_SoulDustProcess(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker)  // Matching - 100%
{
	MATRIX* swTransform;
	struct _Position position;
	long color;
	long black;
	int fade;

	if ((fxPrim->work1 < 32) != 0)
	{
		FX_Die(fxPrim, fxTracker);
		return;
	}
	position = fxPrim->position;
	swTransform = fxPrim->duo.flame.segment + fxPrim->duo.flame.parent->matrix;
	fxPrim->position.x = (short)swTransform->t[0];
	fxPrim->position.y = (short)swTransform->t[1];
	fxPrim->position.z = (short)swTransform->t[2];
	fxPrim->v1.x += fxPrim->work2;
	fxPrim->position.x += (rcos(fxPrim->v1.x) * fxPrim->work1) / 4096;
	fxPrim->position.y += (rsin(fxPrim->v1.x) * fxPrim->work1) / 4096;
	fxPrim->position.z += (rcos(fxPrim->v1.y) * fxPrim->work0) / 4096;
	black = 0;
	color = 0x60FF60;
	fxPrim->work1 -= fxPrim->v2.x;
	fxPrim->v0.y -= 144;
	fxPrim->v1.y += 64;
	fade = fxPrim->v0.y;
	if ((fade < 0) != 0)
	{
		fade = 0;
	}
	LoadAverageCol((u_char*)&color, (u_char*)&black, 4096 - fade, fade, (u_char*)&fxPrim->color);
	fxPrim->color = ((fxPrim->color & 0xFFFFFF) | 0x2E000000);
}


void FX_MakeSoulDust(struct _Instance* instance, short segment) // Matching - 100%
{
	struct _FX_PRIM* fxPrim;
	SVECTOR location;
	struct Object* particle;

	if ((rand() & 0xFF) > 64)
	{
		return;
	}

	particle = (struct Object*)objectAccess[10].object;

	if (particle == NULL)
	{
		return;
	}

	fxPrim = FX_GetPrim(gFXT);

	if (fxPrim != NULL)
	{
		location.vx = 0;
		location.vy = 0;

		location.vz = (short)instance->matrix[1].t[2] + (rand() % 512) - 256;

		FX_DFacadeParticleSetup(fxPrim, &location, 25, 25, 0x2E000000, NULL, NULL, gFXT, 8);

		fxPrim->flags |= 0x2001;

		fxPrim->texture = FX_GetTextureObject(particle, 0, 0);

		fxPrim->v0.y = 4096;
		fxPrim->process = &FX_SoulDustProcess;
		fxPrim->color = 0x2E000000;
		fxPrim->v1.y = 0;
		fxPrim->duo.flame.parent = instance;
		fxPrim->duo.flame.segment = segment;

		fxPrim->work0 = (rand() % 320) - 160;

		fxPrim->work1 = (rand() & 63) + 320;

		fxPrim->v1.x = (rand() % 896) + 128;

		fxPrim->work2 = ((fxPrim->v1.x * 65536) >> 16) / 16;
		fxPrim->v2.x = fxPrim->work1 / 16;

		if ((rand() & 0x1))
		{
			fxPrim->v1.x = -fxPrim->v1.x;
			fxPrim->work2 = -fxPrim->work2;
		}

		fxPrim->v1.x = (instance->rotation.z - 1024) - fxPrim->v1.x;

		FX_SoulDustProcess(fxPrim, gFXT);

		FX_Sprite_Insert(&gFXT->usedPrimListSprite, fxPrim);
	}
}

void FX_WaterTrailProcess(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker) // Matching - 100%
{
	FX_StandardProcess(fxPrim, fxTracker);

	fxPrim->v0.x = (fxPrim->v0.x * 7) >> 3;
	fxPrim->v0.y = (fxPrim->v0.y * 7) >> 3;
	fxPrim->v0.z = (fxPrim->v0.z * 7) >> 3;

	fxPrim->v1.x = (fxPrim->v1.x * 7) >> 3;
	fxPrim->v1.y = (fxPrim->v1.y * 7) >> 3;
	fxPrim->v1.z = (fxPrim->v1.z * 7) >> 3;

	fxPrim->v2.x = (fxPrim->v2.x * 7) >> 3;
	fxPrim->v2.y = (fxPrim->v2.y * 7) >> 3;
	fxPrim->v2.z = (fxPrim->v2.z * 7) >> 3;

	fxPrim->v3.x = (fxPrim->v3.x * 7) >> 3;
	fxPrim->v3.y = (fxPrim->v3.y * 7) >> 3;
	fxPrim->v3.z = (fxPrim->v3.z * 7) >> 3;
}

// autogenerated function stub: 
// void /*$ra*/ FX_MakeWaterTrail(struct _Instance *instance /*$a0*/, int depth /*$a1*/)
void FX_MakeWaterTrail(struct _Instance *instance, int depth)
{ // line 3386, offset 0x80046d60
	/* begin block 1 */
		// Start line: 3387
		// Start offset: 0x80046D60
		// Variables:
			struct Object *waterfx; // $a1
			struct _Model *wxtrail; // $a0
			struct _SVector position; // stack offset -48
			int zvel; // $s0

		/* begin block 1.1 */
			// Start line: 3423
			// Start offset: 0x80046E6C
			// Variables:
				int n; // $s2
				int deg; // $s0
				struct _SVector vel; // stack offset -40
				struct _SVector accel; // stack offset -32
				struct _SVector startpos; // stack offset -24

			/* begin block 1.1.1 */
				// Start line: 3431
				// Start offset: 0x80046E84
				// Variables:
					int sinVal; // $s1
					int cosVal; // $s0
					int spd; // $v1
			/* end block 1.1.1 */
			// End offset: 0x80046F18
			// End Line: 3439
		/* end block 1.1 */
		// End offset: 0x80046F7C
		// End Line: 3444
	/* end block 1 */
	// End offset: 0x80046F7C
	// End Line: 3445

	/* begin block 2 */
		// Start line: 8277
	/* end block 2 */
	// End Line: 8278
					UNIMPLEMENTED();
}


// @fixme crashes the game when called (for example via pressing the attack button)
struct _FXRibbon* FX_StartRibbon(struct _Instance* instance, short startSegment, short endSegment, short type, short ribbonLifeTime, short faceLifeTime, short startFadeValue, long startColor, long endColor) // Matching - 98.11%
{
	/*MATRIX* swTransform;
	struct _FXRibbon* ribbon;
	int i;
	int number;
	MATRIX* temp; // not from SYMDUMP

	number = (endSegment - startSegment) + 1;

	if (number < 2)
	{
		return NULL;
	}

	ribbon = (struct _FXRibbon*)MEMPACK_Malloc(sizeof(struct _FXRibbon), 13);

	if (ribbon == NULL)
	{
		return NULL;
	}

	ribbon->continue_process = FX_ContinueRibbon;
	ribbon->effectType = 0;
	ribbon->endIndex = 0;

	if (type == 1)
	{
		ribbon->numberVerts = number * 2;
	}
	else
	{
		ribbon->numberVerts = 4;
	}

	ribbon->vertexPool = (SVECTOR*)MEMPACK_Malloc(ribbon->numberVerts * 8, 13);

	if (ribbon->vertexPool == NULL)
	{
		MEMPACK_Free((char*)ribbon);
		return NULL;
	}

	ribbon->faceLifeTime = faceLifeTime;
	ribbon->startSegment = startSegment;
	ribbon->endSegment = endSegment;
	ribbon->instance = instance;
	ribbon->type = (unsigned char)type;
	ribbon->lifeTime = ribbonLifeTime;
	ribbon->startColor = startColor;
	ribbon->endColor = endColor;
	ribbon->startFadeValue = startFadeValue;
	ribbon->colorStepValue = 4096 / (short)faceLifeTime;
	ribbon->fadeStep = (4096 - (short)startFadeValue) / ribbon->faceLifeTime;

	if ((type & 0xFF) == 2)
	{
		ribbon->fadeStep = (ribbon->fadeStep * 6) / 8;
	}

	swTransform = instance->matrix;

	if (swTransform != NULL)
	{
		if (ribbon->type == 1)
		{
			temp = &swTransform[startSegment];

			for (i = 0; i < number; i++)
			{
				ribbon->vertexPool[i].vx = (short)temp->t[0];
				ribbon->vertexPool[i].vy = (short)temp->t[1];
				ribbon->vertexPool[i].vz = (short)temp->t[2];
				ribbon->endIndex++;

				temp++;
			}
		}
		else
		{
			swTransform = &instance->matrix[startSegment];

			ribbon->vertexPool->vx = (short)swTransform->t[0];
			ribbon->vertexPool->vy = (short)swTransform->t[1];
			ribbon->vertexPool->vz = (short)swTransform->t[2];

			swTransform = &instance->matrix[endSegment];

			ribbon->vertexPool[1].vx = (short)swTransform->t[0];
			ribbon->vertexPool[1].vy = (short)swTransform->t[1];
			ribbon->vertexPool[1].vz = (short)swTransform->t[2];

			ribbon->endIndex = 2;
		}
	}
	else
	{
		ribbon->endIndex = -1;
	}

	FX_InsertGeneralEffect(ribbon);

	return ribbon;*/
	return NULL;
}

void FX_RibbonProcess(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker) // Matching - 100%
{
	int d;

	if (fxPrim->timeToLive > 0)
	{
		fxPrim->timeToLive--;
	}

	if (fxPrim->timeToLive == 0)
	{
		FX_Die(fxPrim, fxTracker);
		return;
	}

	for (d = 0; d < 4; d++)
	{
		fxPrim->fadeValue[d] += fxPrim->fadeStep;

		if (fxPrim->fadeValue[d] >= 4097)
		{
			fxPrim->fadeValue[d] = 4096;
		}
	}

	if (fxPrim->startColor != fxPrim->endColor)
	{
		fxPrim->colorFadeValue += fxPrim->colorFadeStep;

		if (fxPrim->colorFadeValue >= 4097)
		{
			fxPrim->colorFadeValue = 4096;
		}

		LoadAverageCol((u_char*)&fxPrim->startColor, (u_char*)&fxPrim->endColor, 4096 - fxPrim->colorFadeValue, fxPrim->colorFadeValue, (u_char*)&fxPrim->color);
	}
}

// autogenerated function stub: 
// void /*$ra*/ FX_ConstrictProcess(struct _FX_PRIM *fxPrim /*$s1*/, struct _FXTracker *fxTracker /*$s2*/)
void FX_ConstrictProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker)
{ // line 3627, offset 0x80047358
	/* begin block 1 */
		// Start line: 8805
	/* end block 1 */
	// End Line: 8806
	UNIMPLEMENTED();
}

void FX_StartConstrict(struct _Instance* instance, struct _SVector* constrict_point, short startSegment, short endSegment) // Matching - 100%
{
	if (FX_ConstrictRibbon == NULL)
	{
		if (constrict_point != NULL)
		{
			FX_ConstrictPosition.x = constrict_point->x;
			FX_ConstrictPosition.y = constrict_point->y;
			FX_ConstrictPosition.z = instance->position.z + 256;

			FX_ConstrictPositionPtr = (struct _Position*)constrict_point;
		}

		FX_ConstrictRibbon = FX_StartRibbon(instance, startSegment, endSegment, 2, -1, 40, 0, 0xE84040, 0xE84040);

		FX_ConstrictStage = 0;
	}
}

void FX_EndConstrict(int ConstrictEnemyFlag, struct _Instance* instance) // Matching - 93.70%
{
	struct _Position* _v0;
	struct _Position* _v1;
	short _x1;
	short _y1;
	short _z1;

	if (ConstrictEnemyFlag != 0)
	{
		FX_ConstrictStage = 1;
		FX_ConstrictInstance = instance;

		if (instance != NULL)
		{
			_v0 = &instance->position;
			_x1 = _v0->x;
			_y1 = _v0->y;
			_z1 = _v0->z;
		}
		else
		{
			_v0 = FX_ConstrictPositionPtr;
			_x1 = _v0->x;
			_y1 = _v0->y;
			_z1 = _v0->z;
		}

		_v1 = &FX_ConstrictPosition;

		_v1->x = _x1;
		_v1->y = _y1;
		_v1->z = _z1;
	}

	FX_DeleteGeneralEffect((struct _FXGeneralEffect*)FX_ConstrictRibbon);
	FX_ConstrictRibbon = NULL;
}


// autogenerated function stub: 
// void /*$ra*/ FX_SubDividePrim(struct _FX_PRIM *fxPrim1 /*$s0*/, struct _FX_PRIM *fxPrim2 /*$s1*/)
void FX_SubDividePrim(struct _FX_PRIM *fxPrim1, struct _FX_PRIM *fxPrim2)
{ // line 3733, offset 0x800476f8
	/* begin block 1 */
		// Start line: 3734
		// Start offset: 0x800476F8
		// Variables:
			//SVECTORmp0; // stack offset -32
			//SVECTORmp1; // stack offset -24
	/* end block 1 */
	// End offset: 0x800476F8
	// End Line: 3734

	/* begin block 2 */
		// Start line: 9067
	/* end block 2 */
	// End Line: 9068
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_ContinueRibbon(struct _FXRibbon *ribbon /*$s2*/, struct _FXTracker *fxTracker /*$fp*/)
void FX_ContinueRibbon(struct _FXRibbon *ribbon, struct _FXTracker *fxTracker)
{ // line 3781, offset 0x800477bc
	/* begin block 1 */
		// Start line: 3782
		// Start offset: 0x800477BC
		// Variables:
			//MATRIX *swTransform; // $a1
			int i; // $s3
			int i2; // $s7
			int i3; // $v1
			int startIndex; // $a2
			int period; // stack offset -48
			int d; // $s5
			struct _FX_PRIM *fxPrim; // $s0
			struct _FX_PRIM *fxPrim2; // $s1
			//SVECTORQuad[4]; // stack offset -80

		/* begin block 1.1 */
			// Start line: 3909
			// Start offset: 0x80047C38
			// Variables:
				int fade; // $v1
		/* end block 1.1 */
		// End offset: 0x80047CD4
		// End Line: 3933
	/* end block 1 */
	// End offset: 0x80047D5C
	// End Line: 3948

	/* begin block 2 */
		// Start line: 9166
	/* end block 2 */
	// End Line: 9167
				UNIMPLEMENTED();
}


void FX_StandardFXPrimProcess(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker)  // Matching - 100%
{
	long flags;
	long start;
	long end;
	int current_scale;
	MATRIX* swTransform;
	struct _Rotation rot;

	if ((fxPrim->timeToLive > 0) != 0)
	{
		fxPrim->timeToLive--;
	}
	if (fxPrim->timeToLive == 0)
	{
		FX_Die(fxPrim, fxTracker);
		return;
	}
	flags = fxPrim->flags;
	if ((flags & 0x40000) != 0)
	{
		start = fxPrim->startColor;
		end = fxPrim->endColor;
		fxPrim->fadeValue[0] += fxPrim->fadeStep;
		if ((fxPrim->fadeValue[0] < 4097) == 0)
		{
			fxPrim->fadeValue[0] = 4096;
		}
		gte_lddp(4096 - fxPrim->fadeValue[0]);
		gte_ldcv(&start);
		gte_gpf12();
		gte_lddp(fxPrim->fadeValue[0]);
		gte_ldcv(&end);
		gte_gpl12();
		gte_stcv(&fxPrim->color);
		if ((flags & 1) != 0)
		{
			fxPrim->color = (fxPrim->color & 0x3FFFFFF) | 0x2C000000;
		}
	}
	if ((flags & 0x2000) != 0)
	{
		current_scale = fxPrim->v0.y - fxPrim->work3;
		if (current_scale <= 0)
		{
			FX_Die(fxPrim, fxTracker);
			return;
		}
		fxPrim->v0.y = current_scale;
	}
	if ((flags & 32) != 0)
	{
		swTransform = fxPrim->duo.flame.segment + (MATRIX*)fxPrim->duo.flame.parent->matrix;
		fxPrim->position.x = (short)swTransform->t[0];
		fxPrim->position.y = (short)swTransform->t[1];
		fxPrim->position.z = (short)swTransform->t[2];
	}
	else if ((flags & 2) == 0)
	{
		fxPrim->duo.phys.xVel += fxPrim->duo.phys.xAccl;
		fxPrim->duo.phys.yVel += fxPrim->duo.phys.yAccl;
		fxPrim->duo.phys.zVel += fxPrim->duo.phys.zAccl;
		if ((flags & 0x1000000) != 0)
		{
			fxPrim->v0.x += fxPrim->duo.phys.xVel;
			fxPrim->v1.x += fxPrim->duo.phys.xVel;
			fxPrim->v0.y += fxPrim->duo.phys.yVel;
			fxPrim->v1.y += fxPrim->duo.phys.yVel;
			fxPrim->v0.z += fxPrim->duo.phys.zVel;
			fxPrim->v1.z += fxPrim->duo.phys.zVel;
		}
		else
		{
			fxPrim->position.x += fxPrim->duo.phys.xVel;
			fxPrim->position.y += fxPrim->duo.phys.yVel;
			fxPrim->position.z += fxPrim->duo.phys.zVel;
		}
		if (((flags & 0x100) != 0) && ((fxPrim->position.z > fxPrim->work0) == 0))
		{
			fxPrim->position.z = fxPrim->work0;
			fxPrim->flags |= 2;
		}
	}
	if ((((flags & 0x8000000) == 0) && (fxPrim->matrix != NULL) && ((fxPrim->matrix->flags & 2) == 0))
		&& (fxPrim->matrix->flags |= 2, (flags & 0x80) != 0))
	{
		rot.x = ((signed char*)&fxPrim->work2)[1] * 4;
		rot.y = ((signed char*)&fxPrim->work3)[0] * 4;
		rot.z = ((signed char*)&fxPrim->work3)[1] * 4;
		RotMatrixX(rot.x, (MATRIX*)&fxPrim->matrix->lwTransform);
		RotMatrixY(rot.y, (MATRIX*)&fxPrim->matrix->lwTransform);
		RotMatrixZ(rot.z, (MATRIX*)&fxPrim->matrix->lwTransform);
	}
}


void FX_AttachedParticlePrimProcess(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker) // Matching - 100%
{
	MATRIX* swTransform;
	MATRIX* swTransformOld;
	struct _Instance* instance;

	instance = (struct _Instance*)fxPrim->matrix;

	swTransform = &instance->matrix[fxPrim->work0];

	swTransformOld = &instance->oldMatrix[fxPrim->work0];

	if ((instance->matrix != NULL) && (instance->oldMatrix != NULL))
	{
		fxPrim->position.x += (short)(swTransform->t[0] - swTransformOld->t[0]);
		fxPrim->position.y += (short)(swTransform->t[1] - swTransformOld->t[1]);
		fxPrim->position.z += (short)(swTransform->t[2] - swTransformOld->t[2]);
	}

	FX_StandardFXPrimProcess(fxPrim, fxTracker);
}


// autogenerated function stub: 
// void /*$ra*/ FX_FlamePrimProcess(struct _FX_PRIM *fxPrim /*$a3*/, struct _FXTracker *fxTracker /*$a1*/)
void FX_FlamePrimProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker)
{ // line 4210, offset 0x80048190
	/* begin block 1 */
		// Start line: 4211
		// Start offset: 0x80048190
		// Variables:
			//MATRIX *swTransform; // $a0
			//MATRIX *swTransformOld; // $a2
			struct _Instance *instance; // $v0
			struct _SVector movement; // stack offset -16
			int total; // $a0
	/* end block 1 */
	// End offset: 0x80048354
	// End Line: 4244

	/* begin block 2 */
		// Start line: 10282
	/* end block 2 */
	// End Line: 10283
			UNIMPLEMENTED();
}

void FX_DFacadeParticleSetup(struct _FX_PRIM* fxPrim, SVECTOR* center, short halveWidth, short halveHeight, long color, SVECTOR* vel, SVECTOR* accl, struct _FXTracker* fxTracker, int timeToLive)  // Matching - 100%
{
	short _x1;
	short _y1;
	short _z1;
	struct _Position* _v0;
	short ttl;

	_x1 = center->vx;
	_y1 = center->vy;
	_z1 = center->vz;

	_v0 = &fxPrim->position;

	_v0->x = _x1;
	_v0->y = _y1;
	_v0->z = _z1;

	fxPrim->flags |= 8;

	fxPrim->v0.x = halveWidth;
	fxPrim->v0.y = 4096;
	fxPrim->v0.z = halveHeight;

	fxPrim->color = (color & 0x3FFFFFF) | 0x20000000;

	fxPrim->process = &FX_StandardFXPrimProcess;

	ttl = timeToLive;

	if (vel != NULL)
	{
		fxPrim->duo.phys.xVel = vel->vx;
		fxPrim->duo.phys.yVel = vel->vy;
		fxPrim->duo.phys.zVel = vel->vz;
	}
	else
	{
		fxPrim->duo.phys.xVel = 0;
		fxPrim->duo.phys.yVel = 0;
		fxPrim->duo.phys.zVel = 0;
	}

	if (accl != NULL)
	{
		fxPrim->duo.phys.xAccl = accl->vx;
		fxPrim->duo.phys.yAccl = accl->vy;
		fxPrim->duo.phys.zAccl = accl->vz;
	}
	else
	{
		fxPrim->duo.phys.xAccl = 0;
		fxPrim->duo.phys.yAccl = 0;
		fxPrim->duo.phys.zAccl = 0;
	}

	fxPrim->timeToLive = ttl;
}

struct _FX_PRIM* FX_Dot(struct _SVector* location, struct _SVector* vel, struct _SVector* accel, int scale_speed, long color, long size, int lifetime, int texture_num) // Matching - 100%
{
	struct _FX_PRIM* fxPrim;

	fxPrim = FX_GetPrim(gFXT);

	if (fxPrim != NULL)
	{
		if (texture_num >= 0)
		{
			FX_MakeParticleTexFX(fxPrim, location, 0, 0, texture_num, vel, accel, color, size, lifetime);
		}
		else
		{
			FX_DFacadeParticleSetup(fxPrim, (SVECTOR*)location, (short)size, (short)size, color, (SVECTOR*)vel, (SVECTOR*)accel, gFXT, (short)lifetime);

			if (color != 0)
			{
				fxPrim->flags |= 0xC0000;
			}

			fxPrim->startColor = color;
			fxPrim->endColor = 0;
			fxPrim->fadeValue[3] = 0;
			fxPrim->fadeValue[2] = 0;
			fxPrim->fadeValue[1] = 0;
			fxPrim->fadeValue[0] = 0;
			fxPrim->fadeStep = 4096 / lifetime;
		}

		if (scale_speed != 0)
		{
			fxPrim->v0.y = 4096;
			fxPrim->work3 = scale_speed;
			fxPrim->flags |= 0x2000;
		}

		FX_Sprite_Insert(&gFXT->usedPrimListSprite, fxPrim);
	}

	return fxPrim;
}

void FX_Blood(struct _SVector* location, struct _SVector* input_vel, struct _SVector* accel, int amount, long color, long size) // Matching - 100%
{
	struct _SVector vel;
	int i;

	for (i = amount; i; --i)
	{
		vel.x = input_vel->x * i / 128 + (rand() & 7) - 4;
		vel.y = input_vel->y * i / 128 + (rand() & 7) - 4;
		vel.z = input_vel->z * i / 128 + (rand() & 7) - 4;
		FX_Dot(location, &vel, accel, 0, color, size * 2, 16, 1);
	}
}

void FX_Blood2(struct _SVector* location, struct _SVector* input_vel, struct _SVector* accel, int amount, long color, long dummyCrapShouldRemove)
{
	FX_Blood(location, input_vel, accel, amount, color, 4);
}


// autogenerated function stub: 
// void /*$ra*/ FX_Blood_Impale(struct _Instance *locinst /*$a0*/, short locseg /*$a1*/, struct _Instance *instance /*$a2*/, short segment /*$a3*/)
void FX_Blood_Impale(struct _Instance *locinst, short locseg, struct _Instance *instance, short segment)
{ // line 4383, offset 0x80048720
	/* begin block 1 */
		// Start line: 4384
		// Start offset: 0x80048720
		// Variables:
			struct _SVector location; // stack offset -40
			struct _SVector accel; // stack offset -32
			struct _SVector vel; // stack offset -24
			struct _SVector input_vel; // stack offset -16
			int i; // $s0
	/* end block 1 */
	// End offset: 0x800488DC
	// End Line: 4409

	/* begin block 2 */
		// Start line: 8766
	/* end block 2 */
	// End Line: 8767
			UNIMPLEMENTED();
}


struct _FXParticle* FX_BloodCone(struct _Instance* instance, short startSegment, long time) // Matching - 100%
{
	struct _FXParticle* currentParticle;
	struct Object* particle;

	particle = (struct Object*)objectAccess[10].object;

	if (particle == 0)
	{
		return NULL;
	}

	currentParticle = FX_GetParticle(instance, startSegment);

	if (currentParticle != NULL)
	{
		currentParticle->size = 14;

		currentParticle->texture = FX_GetTextureObject(particle, 0, 1);

		currentParticle->birthRadius = 20;

		currentParticle->direction.x = 256;
		currentParticle->direction.y = 256;
		currentParticle->direction.z = 256;

		currentParticle->acceleration.x = 0;
		currentParticle->acceleration.y = 0;
		currentParticle->acceleration.z = -2;

		currentParticle->numberBirthParticles = 10;

		currentParticle->startColor = 0x021800FF;
		currentParticle->endColor = 0;

		currentParticle->lifeTime = (short)time;
		currentParticle->primLifeTime = 10;

		FX_InsertGeneralEffect(currentParticle);
	}
	return currentParticle;
}


struct _FXParticle* FX_GetTorchParticle(struct _Instance* instance, short startSegment, int tex, int birthRadius, int num) // Matching - 100%
{
	struct _FXParticle* currentParticle;

	currentParticle = FX_GetParticle(instance, startSegment);

	if (currentParticle != NULL)
	{
		currentParticle->type = 1;
		currentParticle->fxprim_process = (void*)FX_FlamePrimProcess;
		currentParticle->texture = FX_GetTextureObject(instance->object, 1, tex);
		currentParticle->startColor = 0x020040F0;
		currentParticle->primLifeTime = 16;
		currentParticle->acceleration.z = 1;
		currentParticle->lifeTime = -1;
		currentParticle->startFadeValue = 5;
		currentParticle->fadeStep = 5;
		currentParticle->size = 48;
		currentParticle->endColor = 0;
		currentParticle->scaleSpeed = 100;
		currentParticle->birthRadius = birthRadius;
		currentParticle->numberBirthParticles = num;
		currentParticle->flag_bits |= 1;

		FX_InsertGeneralEffect(currentParticle);
	}
	return currentParticle;
}

// autogenerated function stub: 
// struct _FXParticle * /*$ra*/ FX_TorchFlame(struct _Instance *instance /*$s2*/, short startSegment /*$a1*/)
struct _FXParticle * FX_TorchFlame(struct _Instance *instance, short startSegment)
{ // line 4475, offset 0x80048a9c
	/* begin block 1 */
		// Start line: 4476
		// Start offset: 0x80048A9C
		// Variables:
			struct _FXParticle *currentParticle; // $s0
			struct Object *particle; // $s3
	/* end block 1 */
	// End offset: 0x80048BD8
	// End Line: 4524

	/* begin block 2 */
		// Start line: 10917
	/* end block 2 */
	// End Line: 10918
			UNIMPLEMENTED();
	return null;
}

int FX_GetMorphFadeVal()
{
	int fade;
	
	fade = (gameTrackerX.gameData.asmData.MorphTime * 4096) / 1000;
	
	if (gameTrackerX.gameData.asmData.MorphType == 1) 
	{
		fade = 4096 - fade;
	}

	return fade;
}


// autogenerated function stub: 
// void /*$ra*/ FX_ConvertCamPersToWorld(SVECTOR*campos /*$s0*/, SVECTOR*worldpos /*$s1*/)
void FX_ConvertCamPersToWorld(SVECTOR*campos, SVECTOR*worldpos)
{ // line 4550, offset 0x80048c38
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_GetRandomScreenPt(SVECTOR*point /*$s0*/)
void FX_GetRandomScreenPt(SVECTOR*point)
{ // line 4570, offset 0x80048d50
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_ProcessSnow(struct _FX_PRIM *fxPrim /*$s0*/, struct _FXTracker *fxTracker /*$s1*/)
void FX_ProcessSnow(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker)
{ // line 4577, offset 0x80048ddc
	/* begin block 1 */
		// Start line: 4578
		// Start offset: 0x80048DDC

		/* begin block 1.1 */
			// Start line: 4581
			// Start offset: 0x80048E00
			// Variables:
				//SVECTORposition; // stack offset -24
		/* end block 1.1 */
		// End offset: 0x80048E48
		// End Line: 4597
	/* end block 1 */
	// End offset: 0x80048EFC
	// End Line: 4611

	/* begin block 2 */
		// Start line: 11162
	/* end block 2 */
	// End Line: 11163
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_ContinueSnow(struct _FXTracker *fxTracker /*$s1*/)
void FX_ContinueSnow(struct _FXTracker *fxTracker)
{ // line 4613, offset 0x80048f10
	/* begin block 1 */
		// Start line: 4614
		// Start offset: 0x80048F10
		// Variables:
			struct _FX_PRIM *fxPrim; // $s0
			//SVECTORposition; // stack offset -40
			//SVECTORvel; // stack offset -32

		/* begin block 1.1 */
			// Start line: 4631
			// Start offset: 0x80048F8C
			// Variables:
				//SVECTORcampos; // stack offset -24
		/* end block 1.1 */
		// End offset: 0x80049054
		// End Line: 4653
	/* end block 1 */
	// End offset: 0x80049054
	// End Line: 4655

	/* begin block 2 */
		// Start line: 11236
	/* end block 2 */
	// End Line: 11237
			UNIMPLEMENTED();
}


void FX_UpdateWind(struct _FXTracker* fxTracker) // Matching - 100%
{
	short change;

	change = rand() % 4;

	if ((rand() & 0x1))
	{
		change = -change;
	}

	wind_speed += change;

	if (wind_speed >= 41)
	{
		wind_speed = 40;
	}

	if (wind_speed < 15)
	{
		wind_speed = 15;
	}

	change = rand() % 8;

	if ((rand() & 0x1))
	{
		change = -change;
	}

	wind_deg += change;

	if (wind_deg >= 1281)
	{
		wind_deg = 1280;
	}

	if (wind_deg < 768)
	{
		wind_deg = 768;
	}

	windx = (rcos(wind_deg) * wind_speed) / 4096;

	windy = (rsin(wind_deg) * wind_speed) / 4096;
}

void FX_ProcessRain(struct _FX_PRIM* fxPrim, struct _FXTracker* fxTracker) // Matching - 100%
{
	int zvel;

	zvel = fxPrim->duo.phys.zVel + theCamera.focusInstanceVelVec.z;

	if (fxPrim->timeToLive > 0)
	{
		fxPrim->timeToLive--;
	}

	fxPrim->v1.z += zvel;

	if (fxPrim->work0 >= fxPrim->v1.z || fxPrim->timeToLive == 0)
	{
		FX_Die(fxPrim, fxTracker);
	}
	else
	{
		fxPrim->v0.x += fxPrim->duo.phys.xVel;
		fxPrim->v1.x += fxPrim->duo.phys.xVel;

		fxPrim->v0.y += fxPrim->duo.phys.yVel;
		fxPrim->v1.y += fxPrim->duo.phys.yVel;

		fxPrim->v0.z += zvel;
	}
}

// autogenerated function stub: 
// void /*$ra*/ FX_ContinueRain(struct _FXTracker *fxTracker /*$s7*/)
void FX_ContinueRain(struct _FXTracker *fxTracker)
{ // line 4707, offset 0x800492b0
	/* begin block 1 */
		// Start line: 4708
		// Start offset: 0x800492B0
		// Variables:
			struct _FX_PRIM *fxPrim; // $s0
			//SVECTORcampos; // stack offset -56
			int n; // $s4
			int rain_pct; // $s6
			long waterZLevel; // $s2
			int slack; // $s1
			int fade; // $v0

		/* begin block 1.1 */
			// Start line: 4753
			// Start offset: 0x800493F8
			// Variables:
				struct _SVector worldpos; // stack offset -48
				int zvel; // $s3
		/* end block 1.1 */
		// End offset: 0x800495AC
		// End Line: 4796
	/* end block 1 */
	// End offset: 0x800495BC
	// End Line: 4798

	/* begin block 2 */
		// Start line: 11452
	/* end block 2 */
	// End Line: 11453
				UNIMPLEMENTED();
}

void FX_MakeSpark(struct _Instance* instance, struct _Model* model, int segment) // Matching - 100%
{
	struct _FXParticle* currentParticle;
	struct Object* particle;

	particle = (struct Object*)objectAccess[10].object;
	if (particle != NULL)
	{
		currentParticle = FX_GetParticle(instance, segment);
		if (currentParticle != NULL)
		{
			currentParticle->size = 48;

			currentParticle->birthRadius = 50;

			currentParticle->direction.x = 128;
			currentParticle->direction.y = 128;
			currentParticle->direction.z = 160;

			currentParticle->acceleration.x = 0;
			currentParticle->acceleration.y = 0;
			currentParticle->acceleration.z = 1;

			currentParticle->texture = FX_GetTextureObject(particle, 0, 0);

			currentParticle->numberBirthParticles = 2;

			currentParticle->lifeTime = 2;

			currentParticle->startColor = 0x02004960;
			currentParticle->endColor = 0;

			currentParticle->primLifeTime = 4;

			FX_InsertGeneralEffect(currentParticle);
		}
	}
}

// autogenerated function stub: 
// void /*$ra*/ FX_ContinueParticle(struct _FXParticle *currentParticle /*$s2*/, struct _FXTracker *fxTracker /*stack 4*/)
void FX_ContinueParticle(struct _FXParticle *currentParticle, struct _FXTracker *fxTracker)
{ // line 4835, offset 0x800496a4
	/* begin block 1 */
		// Start line: 4836
		// Start offset: 0x800496A4
		// Variables:
			struct _FX_PRIM *fxPrim; // $s1
			//struct VECTOR movement; // stack offset -64
			int i; // $fp
			int num; // stack offset -44
			//MATRIX *swTransform; // $s4
			//MATRIX *swOldTransform; // $a1
			long birthRadius; // $s5
			struct _Instance *instance; // $s6
			int InstanceFade; // $s3
			unsigned long black; // stack offset -48

		/* begin block 1.1 */
			// Start line: 4956
			// Start offset: 0x80049C20
			// Variables:
				int tmp_blue; // $v1
				//struct CVECTOR *ptr; // $s0

			/* begin block 1.1.1 */
				// Start line: 4963
				// Start offset: 0x80049C44
				// Variables:
					int fade; // $v0
			/* end block 1.1.1 */
			// End offset: 0x80049C68
			// End Line: 4969
		/* end block 1.1 */
		// End offset: 0x80049C68
		// End Line: 4970
	/* end block 1 */
	// End offset: 0x80049EB4
	// End Line: 5035

	/* begin block 2 */
		// Start line: 11864
	/* end block 2 */
	// End Line: 11865
					UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_UpdraftPrimModify(struct _FX_PRIM *fxPrim /*$s3*/, struct _Instance *instance /*$a1*/, struct _FXParticle *particle /*$s1*/, struct _FXTracker *fxTracker /*$a3*/)
void FX_UpdraftPrimModify(struct _FX_PRIM *fxPrim, struct _Instance *instance, struct _FXParticle *particle, struct _FXTracker *fxTracker)
{ // line 5037, offset 0x80049ee4
	/* begin block 1 */
		// Start line: 5038
		// Start offset: 0x80049EE4
		// Variables:
			int deg; // $s2
	/* end block 1 */
	// End offset: 0x80049FF8
	// End Line: 5044

	/* begin block 2 */
		// Start line: 12443
	/* end block 2 */
	// End Line: 12444
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_MakeParticleTexFX(struct _FX_PRIM *fxPrim /*$s0*/, struct _SVector *position /*$a1*/, struct Object *object /*$s1*/, int modelnum /*$s4*/, int texnum /*stack 16*/, struct _SVector *vel /*stack 20*/, struct _SVector *accl /*stack 24*/, long color /*stack 28*/, int size /*stack 32*/, int life /*stack 36*/)
void FX_MakeParticleTexFX(struct _FX_PRIM *fxPrim, struct _SVector *position, struct Object *object, int modelnum, int texnum, struct _SVector *vel, struct _SVector *accl, long color, int size, int life)
{ // line 5047, offset 0x8004a028
	/* begin block 1 */
		// Start line: 12463
	/* end block 1 */
	// End Line: 12464
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_MakeHitFX(struct _SVector *position /*$s1*/)
void FX_MakeHitFX(struct _SVector *position)
{ // line 5070, offset 0x8004a130
	/* begin block 1 */
		// Start line: 5071
		// Start offset: 0x8004A130
		// Variables:
			struct _FX_PRIM *fxPrim; // $s0
	/* end block 1 */
	// End offset: 0x8004A1B0
	// End Line: 5082

	/* begin block 2 */
		// Start line: 12529
	/* end block 2 */
	// End Line: 12530
			UNIMPLEMENTED();
}

void FX_ContinueLightning(struct _FXLightning* zap, struct _FXTracker* fxTracker)//Matching - 99.58%
{
	zap->deg += zap->deg_inc;

	if (zap->lifeTime > 0)
	{
		zap->lifeTime = zap->lifeTime - FX_Frames;

		if (zap->lifeTime <= 0)
		{
			FX_DeleteGeneralEffect((struct _FXGeneralEffect*)zap);
		}
	}
}

void FX_SetReaverInstance(struct _Instance* instance) // Matching - 100%
{
	FX_reaver_instance = instance;
}

// autogenerated function stub: 
// void /*$ra*/ FX_SoulReaverBlade(struct _Instance *instance /*$a0*/, unsigned long **drawot /*stack 4*/)
void FX_SoulReaverBlade(struct _Instance *instance, unsigned int **drawot)
{ // line 5128, offset 0x8004a230
	/* begin block 1 */
		// Start line: 5129
		// Start offset: 0x8004A230
		// Variables:
			//SVECTORlocation; // stack offset -136
			int i; // $s3
			int size; // $s2
			int reaverScale; // $s7
			struct Object *sreaver; // $s2
			struct __ReaverData *data; // $s1
			short deg; // $s4
			long color; // $fp
			struct TextureMT3 *texture; // $v0
			//MATRIX mat; // stack offset -128
			//struct DVECTOR xy_pos; // stack offset -96
			long sizex; // stack offset -48
			long sizey; // $a0
			long otz; // stack offset -44
			//struct POLY_FT4 *poly; // $s5
			//struct POLY_FT4 poly2; // stack offset -88

		/* begin block 1.1 */
			// Start line: 5186
			// Start offset: 0x8004A398
			// Variables:
				int scale1; // $s0
		/* end block 1.1 */
		// End offset: 0x8004A5C4
		// End Line: 5252
	/* end block 1 */
	// End offset: 0x8004A5F0
	// End Line: 5256

	/* begin block 2 */
		// Start line: 12648
	/* end block 2 */
	// End Line: 12649
				UNIMPLEMENTED();
}

void FX_ReaverBladeInit()
{
}


void FX_SoulReaverWinding(struct _Instance* instance, struct _PrimPool* primPool, unsigned int** ot, MATRIX* wcTransform) // Matching - 100%
{
	MATRIX mat;
	MATRIX* swTransform;
	struct _SVector start;
	struct _SVector end;
	struct __ReaverData* data;
	long color;
	long glow_color;
	short temp;  // not from SYMDUMP

	data = (struct __ReaverData*)instance->extraData;

	if (((unsigned char)data->ReaverPickedUp != 0) && ((unsigned char)data->ReaverOn != 0))
	{
		temp = -data->ReaverDeg;

		swTransform = gameTrackerX.playerInstance->matrix;

		CompMatrix(wcTransform, &swTransform[40], &mat);

		start.z = 0;
		start.y = 0;
		start.x = 0;

		end.y = 0;
		end.x = 0;
		end.z = -128;

		color = data->ReaverBladeColor;

		glow_color = data->ReaverBladeGlowColor;

		FX_Lightning(wcTransform, (unsigned long**)ot, &mat, temp, &start, &end, 30, 10, 16, 32, 0, color, glow_color);

		swTransform = gameTrackerX.playerInstance->matrix;

		CompMatrix(wcTransform, &swTransform[39], &mat);

		end.z = -96;

		FX_Lightning(wcTransform, (unsigned long**)ot, &mat, temp, &start, &end, 30, 10, 16, 32, 0, color, glow_color);

		swTransform = &instance->matrix[1];

		end.z = -(((data->ReaverSize * data->ReaverScale) / 4096) * 380) / 4096;

		if (data->CurrentReaver == 1)
		{
			color = 0xFCFFD3;
		}

		CompMatrix(wcTransform, swTransform, &mat);

		FX_Lightning(wcTransform, (unsigned long**)ot, &mat, temp, &start, &end, 0, 25, 4, 8, 0, color, glow_color);
	}
}


void FX_UpdateInstanceWaterSplit(struct _Instance* instance)
{
	struct _TFace* waterFace;
	struct _SVector normal;

	if (instance->waterFace != NULL && instance->waterFaceTerrain != NULL)
	{
		if ((instance->halvePlane.flags & 0x8) == 0)
		{
			COLLIDE_GetNormal((short)instance->waterFace->normal, (short*)instance->waterFaceTerrain->normalList, &normal);
			instance->halvePlane.flags = 0x2;
			FX_GetPlaneEquation(&normal, &instance->splitPoint, &instance->halvePlane);
		}
		FX_MakeWaterTrail(instance, (int)instance->splitPoint.z);
	}
	else
	{
		instance->halvePlane.flags &= 0xFFFD;
	}

	waterFace = instance->waterFace;
	instance->waterFace = NULL;
	instance->waterFaceTerrain = NULL;
	instance->oldWaterFace = waterFace;
}

void FX_GetPlaneEquation(struct _SVector* normal, struct _SVector* poPlane, struct _PlaneConstants* plane)  // Matching - 100%
{
	plane->a = normal->x;
	plane->b = normal->y;
	plane->c = normal->z;
	plane->d = -((plane->a * poPlane->x + plane->b * poPlane->y + plane->c * poPlane->z) >> 12);
}


// autogenerated function stub: 
// void /*$ra*/ FX_DoInstancePowerRing(struct _Instance *instance /*$s4*/, long atuTime /*$s1*/, long *color /*$s3*/, long numColors /*$s2*/, int follow_halveplane /*stack 16*/)
void FX_DoInstancePowerRing(struct _Instance *instance, long atuTime, long *color, long numColors, int follow_halveplane)
{ // line 5436, offset 0x8004a9bc
	/* begin block 1 */
		// Start line: 5437
		// Start offset: 0x8004A9BC
		// Variables:
			struct _FXHalvePlane *ring; // $s0
			struct _SVector normal; // stack offset -40
			struct _SVector point; // stack offset -32
			long i; // $a1
	/* end block 1 */
	// End offset: 0x8004AB1C
	// End Line: 5484

	/* begin block 2 */
		// Start line: 10872
	/* end block 2 */
	// End Line: 10873
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_UpdatePowerRing(struct _FXHalvePlane *ring /*$s2*/)
void FX_UpdatePowerRing(struct _FXHalvePlane *ring)
{ // line 5488, offset 0x8004ab3c
	/* begin block 1 */
		// Start line: 5489
		// Start offset: 0x8004AB3C
		// Variables:
			struct _Instance *instance; // $v1
			struct _PlaneConstants *cPlane; // $s1
			long offset; // $s5
			long offset2; // $s4
			long cutX; // $s6
			long cutY; // $s7
			long cutZ; // $s3
			long colorIndex; // $a2
			long percent; // $a1
	/* end block 1 */
	// End offset: 0x8004AD80
	// End Line: 5548

	/* begin block 2 */
		// Start line: 13665
	/* end block 2 */
	// End Line: 13666
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_UpdateInstanceSplitRing(struct _FXHalvePlane *ring /*$s0*/, struct _FXTracker *fxTracker /*$a1*/)
void FX_UpdateInstanceSplitRing(struct _FXHalvePlane *ring, struct _FXTracker *fxTracker)
{ // line 5633, offset 0x8004adc0
	/* begin block 1 */
		// Start line: 13978
	/* end block 1 */
	// End Line: 13979
	UNIMPLEMENTED();
}

void FX_UpdateGlowEffect(struct _FXGlowEffect* effect, struct _FXTracker* fxTracker)
{
	if (effect->lifeTime == 0)
	{
		FX_DeleteGeneralEffect((struct _FXGeneralEffect*)effect);
	}
}

void FX_InsertGeneralEffect(void* ptr)
{
	FX_GeneralEffectTracker->next = FX_GeneralEffectTracker;
	FX_GeneralEffectTracker = (struct _FXGeneralEffect*)ptr;
}


void FX_DeleteGeneralEffect(struct _FXGeneralEffect* effect)  // Matching - 100%
{
	struct _FXGeneralEffect* currentEffect;
	struct _FXGeneralEffect* previousEffect;

	if (effect == NULL)
	{
		return;
	}

	currentEffect = FX_GeneralEffectTracker;

	previousEffect = NULL;

	while (currentEffect != NULL)
	{
		if (currentEffect == effect)
		{
			if (previousEffect != NULL)
			{
				previousEffect->next = effect->next;
			}
			else
			{
				FX_GeneralEffectTracker = (struct _FXGeneralEffect*)currentEffect->next;
			}

			break;
		}
		else
		{
			previousEffect = currentEffect;
		}

		currentEffect = (struct _FXGeneralEffect*)currentEffect->next;
	}

	if (effect->effectType == 0)
	{
		MEMPACK_Free((char*)effect[1].continue_process);
	}

	MEMPACK_Free((char*)effect);
}


// autogenerated function stub: 
// struct _FXGlowEffect * /*$ra*/ FX_DoInstanceOneSegmentGlow(struct _Instance *instance /*$s3*/, long segment /*$s4*/, long *color /*$s2*/, long numColors /*$s1*/, long atuColorCycleRate /*stack 16*/, long width /*stack 20*/, long height /*stack 24*/)
struct _FXGlowEffect * FX_DoInstanceOneSegmentGlow(struct _Instance *instance, long segment, long *color, long numColors, long atuColorCycleRate, long width, long height)
{ // line 5710, offset 0x8004af2c
	/* begin block 1 */
		// Start line: 5711
		// Start offset: 0x8004AF2C
		// Variables:
			struct _FXGlowEffect *glowEffect; // $a2

		/* begin block 1.1 */
			// Start line: 5743
			// Start offset: 0x8004AFF4
			// Variables:
				int i; // $a1
		/* end block 1.1 */
		// End offset: 0x8004B04C
		// End Line: 5756
	/* end block 1 */
	// End offset: 0x8004B058
	// End Line: 5762

	/* begin block 2 */
		// Start line: 14134
	/* end block 2 */
	// End Line: 14135
				UNIMPLEMENTED();
	return null;
}

void FX_SetGlowFades(struct _FXGlowEffect* glowEffect, int fadein, int fadeout) //Matching - 100%
{
	glowEffect->fadein_time = (fadein * 32) + fadein;

	glowEffect->fadeout_time = (fadeout * 32) + fadeout;
}

struct _FXGlowEffect* FX_DoInstanceTwoSegmentGlow(struct _Instance* instance, long segment, long segmentEnd, long* color, long numColors, long atuColorCycleRate, long height)//Mathing - 99.77%
{
	int inc;
	struct _FXGlowEffect* glowEffect;

	inc = segmentEnd - segment;
	
	if (segmentEnd - segment < 0)
	{
		segment = segmentEnd;
	
		inc = -inc;
	}
	
	glowEffect = FX_DoInstanceOneSegmentGlow(instance, segment, color, numColors, atuColorCycleRate, height, height);
	
	glowEffect->numSegments = 2;
	
	glowEffect->SegmentInc = inc;
	
	return glowEffect;
}

struct _FXGlowEffect* FX_DoInstanceManySegmentGlow(struct _Instance* instance, long segment, long numSegments, long *color, long numColors, long atuColorCycleRate, long height)
{
	struct _FXGlowEffect* glowEffect;
	
	glowEffect = FX_DoInstanceOneSegmentGlow(instance, segment, color, numColors, atuColorCycleRate, height, height);
	
	glowEffect->numSegments = (unsigned char)numSegments;

	return glowEffect;
}

struct _FXGlowEffect* FX_DoInstanceOneSegmentGlowWithTime(struct _Instance* instance, long segment, long* color, long numColors, long atuColorCycleRate, long width, long height, long ATULifeTime)
{
	struct _FXGlowEffect* glowEffect;
	
	glowEffect = FX_DoInstanceOneSegmentGlow(instance, segment, color, numColors, atuColorCycleRate, width, height);

	glowEffect->lifeTime = (short)(ATULifeTime * 33);

	return glowEffect;
}

void FX_StopAllGlowEffects(struct _Instance* instance, int fadeout_time) // Matching - 100%
{
	struct _FXGlowEffect* currentEffect;
	struct _FXGlowEffect* previousEffect;
	int newFadeoutTime; // not from SYMDUMP

	newFadeoutTime = fadeout_time * 33;
	currentEffect = (struct _FXGlowEffect*)FX_GeneralEffectTracker;

	while (currentEffect != NULL)
	{
		previousEffect = (struct _FXGlowEffect*)currentEffect->next;

		if (currentEffect->effectType == 131 && currentEffect->instance == instance)
		{
			if (newFadeoutTime != 0)
			{
				currentEffect->fadeout_time = newFadeoutTime;
				currentEffect->lifeTime = newFadeoutTime;
			}
			else
			{
				FX_DeleteGeneralEffect((struct _FXGeneralEffect*)currentEffect);
			}
		}

		currentEffect = previousEffect;
	}
}

void FX_StopGlowEffect(struct _FXGlowEffect* glowEffect, int fadeout_time) // Matching - 100%
{
	struct _FXGeneralEffect* currentEffect;
	struct _FXGeneralEffect* previousEffect;

	if (glowEffect != NULL)
	{
		if (fadeout_time != 0)
		{
			glowEffect->fadeout_time = fadeout_time * 33;
			glowEffect->lifeTime = fadeout_time * 33;
			return;
		}

		currentEffect = FX_GeneralEffectTracker;

		while (currentEffect != NULL)
		{
			previousEffect = (struct _FXGeneralEffect*)currentEffect->next;

			if (currentEffect->effectType == 131 && (void*)currentEffect == (void*)glowEffect)
			{
				FX_DeleteGeneralEffect(currentEffect);
			}

			currentEffect = previousEffect;
		}
	}
}

// autogenerated function stub: 
// void /*$ra*/ FX_DrawLightning(struct _FXLightning *zap /*$s1*/, MATRIX *wcTransform /*$s2*/, unsigned long **ot /*$s3*/)
void FX_DrawLightning(struct _FXLightning *zap, MATRIX *wcTransform, unsigned long **ot)
{ // line 5865, offset 0x8004b2ac
	/* begin block 1 */
		// Start line: 5866
		// Start offset: 0x8004B2AC
		// Variables:
			struct _SVector start; // stack offset -80
			struct _SVector end; // stack offset -72
			struct _SVector offset; // stack offset -64
			//MATRIX *swtransform; // $s0
			//MATRIX mat; // stack offset -56
	/* end block 1 */
	// End offset: 0x8004B4D8
	// End Line: 5928

	/* begin block 2 */
		// Start line: 14487
	/* end block 2 */
	// End Line: 14488
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_DrawAllGeneralEffects(MATRIX *wcTransform /*$s1*/, struct _VertexPool *vertexPool /*$s3*/, struct _PrimPool *primPool /*$s4*/, unsigned long **ot /*$s2*/)
void FX_DrawAllGeneralEffects(MATRIX *wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot)
{ // line 5935, offset 0x8004b560
	/* begin block 1 */
		// Start line: 5936
		// Start offset: 0x8004B560
		// Variables:
			struct _Instance *instance; // $a0
			struct _FXGeneralEffect *currentEffect; // $s0

		/* begin block 1.1 */
			// Start line: 5949
			// Start offset: 0x8004B5EC
			// Variables:
				struct _FXGlowEffect *currentGlow; // $t0
		/* end block 1.1 */
		// End offset: 0x8004B61C
		// End Line: 5957

		/* begin block 1.2 */
			// Start line: 5961
			// Start offset: 0x8004B644
		/* end block 1.2 */
		// End offset: 0x8004B644
		// End Line: 5962

		/* begin block 1.3 */
			// Start line: 5971
			// Start offset: 0x8004B688
		/* end block 1.3 */
		// End offset: 0x8004B688
		// End Line: 5973

		/* begin block 1.4 */
			// Start line: 5981
			// Start offset: 0x8004B6E0
		/* end block 1.4 */
		// End offset: 0x8004B6E0
		// End Line: 5983
	/* end block 1 */
	// End offset: 0x8004B718
	// End Line: 5993

	/* begin block 2 */
		// Start line: 14629
	/* end block 2 */
	// End Line: 14630
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_ContinueBlastRing(struct _FXBlastringEffect *blast /*$s0*/, struct _FXTracker *fxTracker /*$a1*/)
void FX_ContinueBlastRing(struct _FXBlastringEffect *blast, struct _FXTracker *fxTracker)
{ // line 6003, offset 0x8004b738
	/* begin block 1 */
		// Start line: 6004
		// Start offset: 0x8004B738
		// Variables:
			int fade; // $a2
			int tm; // $v0

		/* begin block 1.1 */
			// Start line: 6015
			// Start offset: 0x8004B7A8
			// Variables:
				int rad; // $v0
				int crad; // $a0
				int endrad; // $v1
		/* end block 1.1 */
		// End offset: 0x8004B850
		// End Line: 6047

		/* begin block 1.2 */
			// Start line: 6051
			// Start offset: 0x8004B860
			// Variables:
				unsigned long *colorPtr; // $a0
				unsigned long black; // stack offset -16
				//int fade; // $a3
		/* end block 1.2 */
		// End offset: 0x8004B8A8
		// End Line: 6059
	/* end block 1 */
	// End offset: 0x8004B93C
	// End Line: 6080

	/* begin block 2 */
		// Start line: 14770
	/* end block 2 */
	// End Line: 14771

	/* begin block 3 */
		// Start line: 14775
	/* end block 3 */
	// End Line: 14776
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// struct _FXBlastringEffect * /*$ra*/ FX_DoBlastRing(struct _Instance *instance /*$s2*/, struct _SVector *position /*$s5*/, MATRIX *mat /*$s3*/, int segment /*$s6*/, int radius /*stack 16*/, int endRadius /*stack 20*/, int colorChangeRadius /*stack 24*/, int size1 /*stack 28*/, int size2 /*stack 32*/, int vel /*stack 36*/, int accl /*stack 40*/, int height1 /*stack 44*/, int height2 /*stack 48*/, int height3 /*stack 52*/, long startColor /*stack 56*/, long endColor /*stack 60*/, int pred_offset /*stack 64*/, int lifeTime /*stack 68*/, int sortInWorld /*stack 72*/)
struct _FXBlastringEffect * FX_DoBlastRing(struct _Instance *instance, struct _SVector *position, MATRIX *mat, int segment, int radius, int endRadius, int colorChangeRadius, int size1, int size2, int vel, int accl, int height1, int height2, int height3, long startColor, long endColor, int pred_offset, int lifeTime, int sortInWorld)
{ // line 6110, offset 0x8004b94c
	/* begin block 1 */
		// Start line: 6111
		// Start offset: 0x8004B94C
		// Variables:
			struct _FXBlastringEffect *blast; // $s0
	/* end block 1 */
	// End offset: 0x8004BAF4
	// End Line: 6151

	/* begin block 2 */
		// Start line: 14998
	/* end block 2 */
	// End Line: 14999
			UNIMPLEMENTED();
	return null;
}


// autogenerated function stub: 
// void /*$ra*/ FX_DrawBlastring(MATRIX *wcTransform /*$s3*/, struct _FXBlastringEffect *blast /*$s1*/)
void FX_DrawBlastring(MATRIX *wcTransform, struct _FXBlastringEffect *blast)
{ // line 6154, offset 0x8004bb20
	/* begin block 1 */
		// Start line: 6155
		// Start offset: 0x8004BB20
		// Variables:
			int radius; // $s2
			struct _SVector position; // stack offset -64
			//MATRIX mat; // stack offset -56

		/* begin block 1.1 */
			// Start line: 6162
			// Start offset: 0x8004BB60
			// Variables:
				//MATRIX *swtransform; // $s0
		/* end block 1.1 */
		// End offset: 0x8004BB9C
		// End Line: 6175
	/* end block 1 */
	// End offset: 0x8004BCD0
	// End Line: 6197

	/* begin block 2 */
		// Start line: 15098
	/* end block 2 */
	// End Line: 15099
			UNIMPLEMENTED();
}

void FX_ContinueFlash(struct _FXFlash* flash, struct _FXTracker* fxTracker)//Matching - 99.50%
{
	flash->currentTime = flash->currentTime + gameTrackerX.timeMult;

	if (flash->currentTime / 16 >= flash->timeFromColor)
	{
		FX_DeleteGeneralEffect((struct _FXGeneralEffect*)flash);
	}
}


// autogenerated function stub: 
// void /*$ra*/ FX_DrawFlash(struct _FXFlash *flash /*$a0*/)
void FX_DrawFlash(struct _FXFlash *flash)
{ // line 6210, offset 0x8004bd3c
	/* begin block 1 */
		// Start line: 6211
		// Start offset: 0x8004BD3C
		// Variables:
			int time; // $a1
			int div; // $a2
			int transtype; // $s0
			int interp; // $a3
			unsigned long color; // stack offset -16
			unsigned long black; // stack offset -12
	/* end block 1 */
	// End offset: 0x8004BE30
	// End Line: 6241

	/* begin block 2 */
		// Start line: 15210
	/* end block 2 */
	// End Line: 15211
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_RelocateGeneric(struct Object *object /*$a0*/, long offset /*$a1*/)
void FX_RelocateGeneric(struct Object *object, long offset)
{ // line 6244, offset 0x8004be50
	/* begin block 1 */
		// Start line: 6246
		// Start offset: 0x8004BE50
		// Variables:
			struct GenericFXObject *GFXO; // $v0
	/* end block 1 */
	// End offset: 0x8004BEE4
	// End Line: 6255

	/* begin block 2 */
		// Start line: 15281
	/* end block 2 */
	// End Line: 15282

	/* begin block 3 */
		// Start line: 15282
	/* end block 3 */
	// End Line: 15283

	/* begin block 4 */
		// Start line: 15284
	/* end block 4 */
	// End Line: 15285
			UNIMPLEMENTED();
}

// @fixme crashes the game when called (for example via shifting planes) 
struct _FXParticle* FX_StartGenericParticle(struct _Instance* instance, int num, int segOverride, int lifeOverride, int InitFlag) // Matching - 100%
{
	/*struct _FXParticle* currentParticle;
	struct GenericFXObject* GFXO;
	struct _GenericParticleParams* GPP;
	struct Object* texture_obj;
	struct Object* particle;

	particle = (struct Object*)objectAccess[10].object;

	texture_obj = NULL;

	if (particle == NULL)
	{
		return NULL;
	}

	GFXO = (struct GenericFXObject*)particle->data;

	GPP = &GFXO->ParticleList[num];

	if ((InitFlag != 0) && (GPP->StartOnInit == 0))
	{
		return NULL;
	}

	if (GPP->use_child != 0)
	{
		instance = instance->LinkChild;

		if (instance == NULL)
		{
			return NULL;
		}
	}

	if (GPP->texture != -1)
	{
		if (GPP->useInstanceModel != 0)
		{
			texture_obj = instance->object;
		}
		else
		{
			texture_obj = particle;
		}

		if (texture_obj == NULL)
		{
			return NULL;
		}
	}

	currentParticle = FX_GetParticle(instance, 0);

	if (currentParticle != NULL)
	{
		currentParticle->numberBirthParticles = GPP->numberBirthParticles;
		currentParticle->size = GPP->size;
		currentParticle->type = GPP->type;
		currentParticle->birthRadius = GPP->birthRadius;

		if (segOverride == 0)
		{
			currentParticle->startSegment = GPP->startSegment;
		}
		else
		{
			currentParticle->startSegment = segOverride;
		}

		currentParticle->endSegment = GPP->endSegment;
		currentParticle->direction = GPP->direction;
		currentParticle->acceleration.x = GPP->accx;
		currentParticle->acceleration.y = GPP->accy;
		currentParticle->acceleration.z = GPP->accz;
		currentParticle->startColor = *(long*)(&GPP->startColor_red) & 0xFFFFFF;
		currentParticle->endColor = *(long*)(&GPP->endColor_red) & 0xFFFFFF;

		if (lifeOverride != 0)
		{
			currentParticle->lifeTime = lifeOverride;
		}
		else
		{
			currentParticle->lifeTime = GPP->lifeTime;
		}

		currentParticle->primLifeTime = GPP->primLifeTime;
		currentParticle->startFadeValue = GPP->startFadeValue;
		currentParticle->startScale = GPP->startScale;
		currentParticle->scaleSpeed = GPP->scaleSpeed;
		currentParticle->offset.x = GPP->offset.x;
		currentParticle->offset.y = GPP->offset.y;
		currentParticle->offset.z = GPP->offset.z;
		currentParticle->z_undulate = GPP->z_undulation_mode;

		if ((GPP->fadeStep == -1) && (GPP->primLifeTime != 0))
		{
			currentParticle->fadeStep = (4096 - currentParticle->startFadeValue) / GPP->primLifeTime;
		}
		else
		{
			currentParticle->fadeStep = GPP->fadeStep;
		}

		if (GPP->texture != -1)
		{
			struct TextureMT3* texture;

			texture = FX_GetTextureObject(texture_obj, GPP->model, GPP->texture);

			currentParticle->texture = texture;
			currentParticle->startColor |= (texture->color & 0x3000000);
		}

		if (GPP->type == 1)
		{
			currentParticle->fxprim_process = FX_AttachedParticlePrimProcess;
		}

		if (GPP->spectral_colorize == 1)
		{
			currentParticle->flag_bits |= 0x1;
		}

		if (GPP->absolute_direction != 0)
		{
			currentParticle->flag_bits |= 0x2;
		}

		if (GPP->spectral_colorize == 2)
		{
			int tmp_blue;
			CVECTOR* ptr;

			if (SoulReaverFire() != 0)
			{
				ptr = (CVECTOR*)&currentParticle->startColor;

				tmp_blue = ptr->b;

				ptr->b = (u_char)currentParticle->startColor;

				ptr = (CVECTOR*)&currentParticle->endColor;

				((char*)&currentParticle->startColor)[0] = tmp_blue;

				tmp_blue = ptr->b;

				ptr->b = (u_char)currentParticle->endColor;

				((char*)&currentParticle->endColor)[0] = tmp_blue;
			}
		}

		FX_InsertGeneralEffect(currentParticle);
	}

	return currentParticle;*/
	return NULL;
}

void FX_StartGenericRibbon(struct _Instance* instance, int num, int segOverride, int endOverride, int InitFlag) // Matching - 100%
{
	struct Object* particle;
	struct GenericFXObject* GFXO;
	struct _GenericRibbonParams* GRP;

	particle = (struct Object*)objectAccess[10].object;

	if (particle != NULL)
	{
		GFXO = (struct GenericFXObject*)particle->data;
		GRP = &GFXO->RibbonList[num];

		if (GRP->use_child == 0 || (instance = instance->LinkChild, instance != NULL))
		{
			if (InitFlag == 0 || GRP->StartOnInit != 0)
			{
				FX_StartRibbon(instance, GRP->startSegment, GRP->endSegment, GRP->type, GRP->ribbonLifeTime, GRP->faceLifeTime, GRP->startFadeValue, GRP->startColor, GRP->endColor);
			}
		}
	}
}

// autogenerated function stub: 
// void /*$ra*/ FX_StartGenericGlow(struct _Instance *instance /*$a0*/, int num /*$a1*/, int segOverride /*$t0*/, int seg2Override /*$a3*/, int InitFlag /*stack 16*/)
void FX_StartGenericGlow(struct _Instance *instance, int num, int segOverride, int seg2Override, int InitFlag)
{ // line 6392, offset 0x8004c2f8
	/* begin block 1 */
		// Start line: 6393
		// Start offset: 0x8004C2F8
		// Variables:
			struct Object *particle; // $v1
			struct GenericFXObject *GFXO; // $t1
			struct _GenericGlowParams *GGP; // $s0
			struct _FXGlowEffect *glowEffect; // $a0
			long *color; // $v1

		/* begin block 1.1 */
			// Start line: 6420
			// Start offset: 0x8004C384
			// Variables:
				int seg; // $a1
		/* end block 1.1 */
		// End offset: 0x8004C390
		// End Line: 6422

		/* begin block 1.2 */
			// Start line: 6426
			// Start offset: 0x8004C3C8
			// Variables:
				//int seg; // $a1
				int segEnd; // $a2
		/* end block 1.2 */
		// End offset: 0x8004C3E8
		// End Line: 6437

		/* begin block 1.3 */
			// Start line: 6441
			// Start offset: 0x8004C418
			// Variables:
				//int seg; // $a1
				int numSeg; // $a2
		/* end block 1.3 */
		// End offset: 0x8004C460
		// End Line: 6460
	/* end block 1 */
	// End offset: 0x8004C490
	// End Line: 6467

	/* begin block 2 */
		// Start line: 15602
	/* end block 2 */
	// End Line: 15603
				UNIMPLEMENTED();
}

struct _FXLightning* FX_CreateLightning(struct _Instance* instance, int lifeTime, int deg, int deg_inc, int width, int small_width, int segs, int sine_size, int variation, unsigned long color, unsigned long glow_color) // Matching - 100%
{
	struct _FXLightning* zap;

	zap = (struct _FXLightning*)MEMPACK_Malloc(sizeof(struct _FXLightning), 13);
	if (zap != NULL)
	{
		zap->continue_process = (void*)FX_ContinueLightning;

		zap->instance = instance;
		zap->end_instance = instance;

		zap->effectType = 135;
		zap->type = 0;
		zap->lifeTime = lifeTime;

		zap->deg = deg;
		zap->deg_inc = deg_inc;

		zap->width = width;
		zap->small_width = small_width;

		zap->segs = segs;
		zap->sine_size = sine_size;
		zap->variation = variation;

		zap->color = color;
		zap->glow_color = glow_color;

		zap->start_offset.x = 0;
		zap->start_offset.y = 0;
		zap->start_offset.z = 0;

		zap->end_offset.x = 0;
		zap->end_offset.y = 0;
		zap->end_offset.z = 0;

		zap->matrixSeg = -1;

		FX_InsertGeneralEffect(zap);
	}
	return zap;
}

// autogenerated function stub: 
// void /*$ra*/ FX_SetLightingPos(struct _FXLightning *zap /*$a0*/, struct _Instance *startInstance /*$a1*/, int startSeg /*$a2*/, _Position *startOffset /*$a3*/, struct _Instance *endInstance /*stack 16*/, int endSeg /*stack 20*/, _Position *endOffset /*stack 24*/, int matrixSeg /*stack 28*/)
void FX_SetLightingPos(struct _FXLightning *zap, struct _Instance *startInstance, int startSeg, _Position *startOffset, struct _Instance *endInstance, int endSeg, _Position *endOffset, int matrixSeg)
{ // line 6544, offset 0x8004c5a8
	/* begin block 1 */
		// Start line: 15928
	/* end block 1 */
	// End Line: 15929
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// struct _FXLightning * /*$ra*/ FX_StartGenericLightning(struct _Instance *instance /*$s2*/, int num /*$a1*/, int segOverride /*$s3*/, int endSegOverride /*$s4*/)
struct _FXLightning * FX_StartGenericLightning(struct _Instance *instance, int num, int segOverride, int endSegOverride)
{ // line 6573, offset 0x8004c658
	/* begin block 1 */
		// Start line: 6574
		// Start offset: 0x8004C658
		// Variables:
			struct _FXLightning *zap; // $s1
			struct GenericFXObject *GFXO; // $v1
			struct _GenericLightningParams *GLP; // $s0
			struct Object *particle; // $v1

		/* begin block 1.1 */
			// Start line: 6600
			// Start offset: 0x8004C74C
			// Variables:
				int startSeg; // $a2
				int endSeg; // $a3

			/* begin block 1.1.1 */
				// Start line: 6626
				// Start offset: 0x8004C7B4
				// Variables:
					int tmp_blue; // $a0
					//struct CVECTOR *ptr; // $v1
			/* end block 1.1.1 */
			// End offset: 0x8004C7D8
			// End Line: 6635
		/* end block 1.1 */
		// End offset: 0x8004C7D8
		// End Line: 6636
	/* end block 1 */
	// End offset: 0x8004C7DC
	// End Line: 6639

	/* begin block 2 */
		// Start line: 15986
	/* end block 2 */
	// End Line: 15987

	/* begin block 3 */
		// Start line: 15992
	/* end block 3 */
	// End Line: 15993
					UNIMPLEMENTED();
	return null;
}


// autogenerated function stub: 
// struct _FXBlastringEffect * /*$ra*/ FX_StartGenericBlastring(struct _Instance *instance /*$s1*/, int num /*$a1*/, int segOverride /*$a2*/, int matrixSegOverride /*$a3*/)
struct _FXBlastringEffect * FX_StartGenericBlastring(struct _Instance *instance, int num, int segOverride, int matrixSegOverride)
{ // line 6642, offset 0x8004c7fc
	/* begin block 1 */
		// Start line: 6643
		// Start offset: 0x8004C7FC
		// Variables:
			struct _FXBlastringEffect *blast; // $v0
			struct GenericFXObject *GFXO; // $v1
			struct _GenericBlastringParams *GBP; // $s0
			struct Object *particle; // $v1
			struct _SVector position; // stack offset -64
			int segment; // $s2
			int matrix_segment; // $s3
			//MATRIX mat; // stack offset -56
			//MATRIX *swTransform; // $v0
	/* end block 1 */
	// End offset: 0x8004C9E8
	// End Line: 6706

	/* begin block 2 */
		// Start line: 16135
	/* end block 2 */
	// End Line: 16136

	/* begin block 3 */
		// Start line: 16146
	/* end block 3 */
	// End Line: 16147
			UNIMPLEMENTED();
	return null;
}


struct _FXFlash* FX_StartGenericFlash(struct _Instance* instance, int num) // Matching - 100%
{
	struct GenericFXObject* GFXO;
	struct _GenericFlashParams* GFP;
	struct Object* particle;
	struct _FXFlash* flash;
	int temp;  // not from SYMDUMP
	int temp2;  // not from SYMDUMP

	particle = (struct Object*)objectAccess[10].object;

	if (particle == NULL)
	{
		return NULL;
	}

	GFXO = (struct GenericFXObject*)particle->data;

	GFP = &GFXO->FlashList[num];

	flash = (struct _FXFlash*)MEMPACK_Malloc(sizeof(struct _FXFlash), 13);

	if (flash != NULL)
	{
		flash->continue_process = (void*)FX_ContinueFlash;
		flash->effectType = 136;
		flash->instance = instance;
		flash->type = 0;
		flash->lifeTime = -1;
		flash->color = GFP->color;
		flash->currentTime = 0;

		temp = GFP->timeToColor << 8;

		flash->timeToColor = temp;

		temp2 = temp + (GFP->timeAtColor << 8);

		flash->timeAtColor = temp2;
		flash->timeFromColor = (temp2 + (GFP->timeFromColor << 8));

		FX_InsertGeneralEffect(flash);
	}

	return flash;
}


// autogenerated function stub: 
// long /*$ra*/ FX_GetHealthColor(int currentHealth /*$a0*/)
long FX_GetHealthColor(int currentHealth)
{ // line 6742, offset 0x8004cae4
	/* begin block 1 */
		// Start line: 6763
		// Start offset: 0x8004CAE4
		// Variables:
			static long HealthColors[6]; // offset 0x0
			long color; // $v1
	/* end block 1 */
	// End offset: 0x8004CB28
	// End Line: 6780

	/* begin block 2 */
		// Start line: 16362
	/* end block 2 */
	// End Line: 16363

	/* begin block 3 */
		// Start line: 16382
	/* end block 3 */
	// End Line: 16383

	/* begin block 4 */
		// Start line: 16393
	/* end block 4 */
	// End Line: 16394
			UNIMPLEMENTED();
	return 0;
}

void FX_Start_Snow(int percent)//Matching - 99.62%
{
	snow_amount = (percent * 768) / 100;
}

void FX_Start_Rain(int percent) //Matching - 99.33%
{
	rain_amount = (percent * 1024) / 100;

	if (rain_amount == 0) 
	{
		current_rain_fade = 0;
	}
}

void FX_StartLightbeam(struct _Instance* instance, int startSeg, int endSeg, int color_num) // Matching - 100%
{
	struct Object* particle;
	struct _FXLightBeam* beam;
	struct GenericFXObject* GFXO;
	long color;

	particle = (struct Object*)objectAccess[10].object;

	if (particle != NULL)
	{
		GFXO = (struct GenericFXObject*)particle->data;
		color = GFXO->ColorList[color_num];

		beam = (struct _FXLightBeam*)MEMPACK_Malloc(sizeof(struct _FXLightBeam), 13);

		if (beam != NULL)
		{
			beam->effectType = 133;
			beam->instance = instance;
			beam->continue_process = NULL;
			beam->type = 0;
			beam->lifeTime = -1;

			beam->startSeg = startSeg;
			beam->endSeg = endSeg;

			beam->color = color;

			FX_InsertGeneralEffect(beam);
		}
	}
}

void FX_StartInstanceEffect(struct _Instance* instance, struct ObjectEffect* effect, int InitFlag)  // Matching - 100%
{
	long color;
	long numberOfSegments;
	struct _FXParticle* currentParticle;
	struct evObjectDraftData* draft;
	struct FXSplinter* splinterData;
	struct GenericTune* tune;
	short shardFlags;
	struct PhysObSplinter* splintDef;
	int shock;
	int amt;
	int dist;                                // not from SYMDUMP
	int mod;                                 // not from SYMDUMP

	color = 0x4080;
	switch (effect->effectNumber)
	{
	case 0:
		break;
	case 1:
		FX_TorchFlame(instance, effect->modifierList[0]);
		break;
	case 2:
		if ((effect->modifierList[1] - 1) < 5U)
		{
			color = FX_GetColor(effect, 1);
		}
		FX_DoInstanceOneSegmentGlow(instance, effect->modifierList[0], &color, 1, 1024, 50, 100);
		break;
	case 3:
		numberOfSegments = effect->modifierList[1];
		if (numberOfSegments <= 0)
		{
			numberOfSegments = 1;
		}
		if ((effect->modifierList[2] - 1) < 5U)
		{
			color = FX_GetColor(effect, 2);
		}
		FX_DoInstanceManySegmentGlow(instance, effect->modifierList[0], numberOfSegments, &color, 1, 1024, 65);
		break;
	case 5:
		FX_StartGenericParticle(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2], InitFlag);
		break;
	case 6:
		FX_StartGenericRibbon(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2], InitFlag);
		break;
	case 7:
		FX_StartGenericGlow(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2], InitFlag);
		break;
	case 8:
		GlyphTrigger();
		break;
	case 10:
		FX_Start_Snow(effect->modifierList[0]);
		break;
	case 11:
		FX_Start_Rain(effect->modifierList[0]);
		break;
	case 12:
		draft = (struct evObjectDraftData*)INSTANCE_Query(instance, 22);
		if (draft != NULL)
		{
			currentParticle = FX_StartGenericParticle(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2], InitFlag);
			if (currentParticle != NULL)
			{
				currentParticle->direction.x = draft->radius;
				currentParticle->direction.y = draft->radiusCoef;
				currentParticle->direction.z = draft->height;
				currentParticle->primLifeTime = draft->maxVelocity >> 3;
				currentParticle->birthRadius = 0;
				currentParticle->fxprim_modify_process = FX_UpdraftPrimModify;
			}
		}
		break;
	case 13:
		FX_StartLightbeam(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2]);
		break;
	case 14:
		FX_StartGenericLightning(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2]);
		break;
	case 15:
		if (InitFlag == 0)
		{
			FX_StartGenericBlastring(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2]);
		}
		break;
	case 16:
		if (InitFlag == 0)
		{
			FX_StartGenericFlash(instance, effect->modifierList[0]);
		}
		break;
	case 17:
		splintDef = NULL;
		splinterData = NULL;
		if (InitFlag == 0)
		{
			shardFlags = 0;
			tune = (struct GenericTune*)instance->object->data;
			if (instance->object->oflags2 & 0x40000)
			{
				splintDef = PhysObGetSplinter(instance);
				if (splintDef != NULL)
				{
					splinterData = (struct FXSplinter*)splintDef->splinterData;
				}
			}
			else if (tune != NULL)
			{
				splinterData = (struct FXSplinter*)tune->shatterData;
				if (tune->flags & 2)
				{
					shardFlags = 16;
				}
			}
			_FX_BuildSplinters(instance, NULL, NULL, NULL, splinterData, gFXT, NULL, NULL, shardFlags);
		}
		break;
	case 18:
		FX_StartGenericBlastring(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2]);
		break;
	case 19:
		GAMEPAD_Shock0(effect->modifierList[0], effect->modifierList[1] << 12);
		break;
	case 20:
		shock = effect->modifierList[0];
		GAMEPAD_Shock1(shock, effect->modifierList[1] << 12);
		break;
	case 21:
		dist = MATH3D_DistanceBetweenPositions(&instance->position, &theCamera.core.position);
		mod = effect->modifierList[0];
		amt = ((8000 - dist) * mod) / 8000;
		if (amt > 0)
		{
			amt += 50;
			if (effect->modifierList[0] < amt)
			{
				amt = effect->modifierList[0];
			}
			GAMEPAD_Shock1(amt, effect->modifierList[1] << 12);
		}
		break;
	}
}

void FX_EndInstanceEffects(struct _Instance* instance)// Matching - 99.52%
{
	struct _FXGeneralEffect* currentEffect;
	struct _FXGeneralEffect* nextEffect;

	currentEffect = FX_GeneralEffectTracker;

	while (currentEffect != NULL)
	{
		nextEffect = (struct _FXGeneralEffect*)currentEffect->next;

		if (currentEffect->instance == instance)
		{
			FX_DeleteGeneralEffect(currentEffect);
		}

		currentEffect = nextEffect;
	}
}

void FX_EndInstanceParticleEffects(struct _Instance* instance) // Matching - 100%
{
	struct _FXGeneralEffect* currentEffect;
	struct _FXGeneralEffect* nextEffect;

	currentEffect = FX_GeneralEffectTracker;

	while (currentEffect != NULL)
	{
		nextEffect = (struct _FXGeneralEffect*)currentEffect->next;

		if (currentEffect->instance == instance && currentEffect->effectType == 1)
		{
			FX_DeleteGeneralEffect(currentEffect);
		}

		currentEffect = nextEffect;
	}
}

void FX_GetSpiralPoint(int radius, int deg, int* x, int* y) // Matching - 98.68%
{
	int prevx;
	int prevy;

	prevx = (((-radius * rcos(deg)) >> 12) / 240) << 9;

	if (prevx < 0)
	{
		prevx -= 2048;
	}
	else
	{
		prevx += 2048;
	}

	x[0] = (prevx >> 12) + 438;

	prevy = (radius * rsin(deg)) >> 12;

	if (prevy < 0)
	{
		prevy -= 2048;
	}
	else
	{
		prevy += 2048;
	}

	y[0] = (prevy >> 12) + 201;
}

void FX_GetLinePoint(int radius, int next_radius, int deg, int next_deg, int* pntx, int* pnty, int part)//Matching - 99.80%
{
	int x1;
	int y1;
	int x2;
	int y2;

	FX_GetSpiralPoint(radius, deg, &x1, &y1);
	FX_GetSpiralPoint(next_radius, next_deg, &x2, &y2);

	pntx[0] = x1 + (x2 - x1) * part / 4096;
	pnty[0] = y1 + (y2 - y1) * part / 4096;
}

void FX_CalcSpiral(int degchange) // Matching - 100%
{
	int radius;
	int deg;
	int n;
	int pntx;
	int pnty;
	int px;
	int py;
	int mx;
	int my;
	int mod;
	int next_deg;
	int next_radius;
	int minx;
	int maxx;
	int miny;
	int maxy;

	minx = 32767;
	maxx = -32767;
	miny = 32767;
	maxy = -32767;
	radius = 8192;
	next_radius = 8192;
	deg = 0;
	next_deg = 0;

	FX_GetSpiralPoint(8192, 0, &pntx, &pnty);

	Spiral_Array[64].vx = pntx;
	Spiral_Array[64].vy = pnty;

	for (n = 0; n < 64; n++)
	{
		if (Spiral_Number == 0)
		{
			mod = (n % Spiral_Mod);

			if (!mod)
			{
				deg = next_deg;

				radius = next_radius;

				next_radius += 1088 * Spiral_Mod;

				next_deg += degchange * Spiral_Mod;
			}

			mod = (mod << 12) / Spiral_Mod;

			FX_GetLinePoint(radius, next_radius, deg, next_deg, &pntx, &pnty, mod);

			FX_GetLinePoint(radius + 7168, next_radius + 7168, deg, next_deg, &px, &py, mod);

			FX_GetLinePoint(radius - 7168, next_radius - 7168, deg, next_deg, &mx, &my, mod);
		}
		else
		{
			radius += 1088;

			deg += degchange;

			FX_GetSpiralPoint(radius, deg, &pntx, &pnty);

			FX_GetSpiralPoint(radius + 7168, deg, &px, &py);

			FX_GetSpiralPoint(radius - 7168, deg, &mx, &my);
		}

		Spiral_Array[n].vx = pntx;
		Spiral_Array[n].vy = pnty;

		if (n == 63)
		{
			px = pntx;
			py = pnty;
			mx = pntx;
			my = pnty;
		}

		Spiral_OffsetP[n].vx = px;
		Spiral_OffsetP[n].vy = py;

		if (px < minx)
		{
			minx = px;
		}

		if (maxx < px)
		{
			maxx = px;
		}

		if (py < miny)
		{
			miny = py;
		}

		if (maxy < py)
		{
			maxy = py;
		}

		Spiral_OffsetM[n].vx = mx;
		Spiral_OffsetM[n].vy = my;
	}

	Spiral_Glow_Size = 320 * ((maxx - minx) / 2) / 512 + 2;
	Spiral_Glow_X = (minx + maxx) / 2;
	Spiral_Glow_Y = (miny + maxy) / 2;
}

void FX_Spiral(struct _PrimPool* primPool, unsigned long** ot) // Matching - 82.27%
{
	struct _POLY_2G4* poly;
	long prev;
	long offp;
	long offm;
	int n;
	DR_TPAGE* drtpage;
	int health;
	int health_mod;
	long no_color;
	long color;
	static short cnt;
	int current_cnt;
	int max64;
	long SPIRAL_COLOR;
	long SPIRAL_COLOR2;
	long SPIRAL_COLOR3;
	long SPIRAL_COLOR_END;
	long SPIRAL_NOCOLOR;

	if ((gameTrackerX.gameData.asmData.MorphTime != 1000) && (gameTrackerX.gameData.asmData.MorphType == 0))
	{
		if (Spiral_Number != 0)
		{
			FX_Health_Spiral(1, Spiral_Current, Spiral_Max);
		}
	}

	SPIRAL_COLOR = 0x3AFCFFD3;

	SPIRAL_COLOR2 = 0x3ADCE0BA;

	SPIRAL_COLOR3 = 0x3ABBC09D;

	SPIRAL_COLOR_END = 0x3A483017;

	SPIRAL_NOCOLOR = 0x3A002A15;

	if (Spiral_Number == 0)
	{
		if (Spiral_Current == Spiral_Max)
		{
			if (++cnt >= 81)
			{
				cnt = 0;
			}
		}
		else
		{
			SPIRAL_COLOR = 0x3A00FF00;
			SPIRAL_COLOR2 = 0x3A00E000;
			SPIRAL_COLOR3 = 0x3A00BF00;
			SPIRAL_COLOR_END = 0x3A004500;
		}

		SPIRAL_NOCOLOR = 0x3A00150B;
	}
	else
	{
		if (++cnt >= 81)
		{
			cnt = 0;
		}
	}

	max64 = Spiral_Max / 64;

	health = Spiral_Current / max64;

	prev = ((unsigned long*)&Spiral_Array[64])[0];

	offm = ((unsigned long*)&Spiral_Array[64])[0];

	offp = ((unsigned long*)&Spiral_Array[64])[0];

	poly = (struct _POLY_2G4*)primPool->nextPrim;

	health_mod = ((Spiral_Current - (health * max64)) << 12) / max64;

	if ((unsigned int*)(poly + 65) < primPool->lastPrim)
	{
		color = SPIRAL_NOCOLOR;
		no_color = SPIRAL_NOCOLOR;
		current_cnt = cnt;

		for (n = 0; n < 64; n++, poly++)
		{
			((unsigned long*)&poly->p1.r2)[0] = color;
			((unsigned long*)&poly->p2.r0)[0] = color;

			if (health < n)
			{
				color = no_color;
			}
			else
			{
				int tmp;

				tmp = ((n + current_cnt) & 0xF) / 4;

				switch (tmp)
				{
				default:
				case 0:
				{
					color = SPIRAL_COLOR;
					break;
				}
				case 1:
				{
					color = SPIRAL_COLOR2;
					break;
				}
				case 2:
				{
					color = SPIRAL_COLOR3;
					break;
				}
				case 3:
				{
					color = SPIRAL_COLOR2;
					break;
				}
				}
			}

			if (n == health)
			{
				LoadAverageCol((unsigned char*)&color, (unsigned char*)&no_color, health_mod, 4096 - health_mod, (unsigned char*)&color);

				color = color & 0xFFFFFF;

				color |= (SPIRAL_COLOR & 0xFF000000);
			}

			((unsigned long*)(&poly->p1.r0))[0] = SPIRAL_COLOR_END;
			((unsigned long*)(&poly->p1.r1))[0] = SPIRAL_COLOR_END;
			((unsigned long*)(&poly->p1.r3))[0] = color;
			((unsigned long*)(&poly->p2.r2))[0] = SPIRAL_COLOR_END;
			((unsigned long*)(&poly->p2.r3))[0] = SPIRAL_COLOR_END;
			((unsigned long*)(&poly->p2.r1))[0] = color;

			((unsigned long*)&poly->p2.x0)[0] = prev;
			((unsigned long*)&poly->p1.x2)[0] = prev;

			((unsigned long*)&poly->p1.x0)[0] = offp;
			((unsigned long*)&poly->p2.x2)[0] = offm;


			prev = ((unsigned long*)&Spiral_Array[n])[0];

			offp = ((unsigned long*)&Spiral_OffsetP[n])[0];

			offm = ((unsigned long*)&Spiral_OffsetM[n])[0];

			((unsigned long*)&poly->p2.x1)[0] = prev;

			((unsigned long*)&poly->p1.x3)[0] = prev;

			((unsigned long*)&poly->p1.x1)[0] = offp;

			((unsigned long*)&poly->p2.x3)[0] = offm;

			setlen(poly, 16);

#if defined(PSXPC_VERSION)
			addPrim(ot[1 * 2], poly);
#else
			addPrim(ot[1], poly);
#endif
			poly++;
		}

		setDrawTPage(poly, TRUE, TRUE, 32);

#if defined(PSXPC_VERSION)
		addPrim(ot[1 * 2], poly);
#else
		addPrim(ot[1], poly);
#endif

#if defined(PSXPC_VERSION)
		primPool->nextPrim = (unsigned int*)(struct _POLY_2G4*)(((char*)poly) + 12);
#else
		primPool->nextPrim = (unsigned int*)(struct _POLY_2G4*)(((char*)poly) + 8);
#endif

		if ((Spiral_Number != 0) || (Spiral_Current == Spiral_Max))
		{
			static short deg;
			struct _Vector f1;

			f1.x = Spiral_Glow_X;
			f1.y = Spiral_Glow_Y;

			deg = (deg - 32) & 0xFFF;

			DRAW_CreateAGlowingCircle(&f1, 320, gameTrackerX.primPool, ot, 5, 32768, Spiral_Glow_Size, Spiral_Glow_Size, deg);
		}
	}
}

void FX_Health_Spiral(int number, int current_health, int max_health) // Matching - 100%
{
	int degchange;

	if (number != 0)
	{
		degchange = Spiral_Mod;

		if (degchange >= 2)
		{
			--Spiral_Mod;

			FX_CalcSpiral(128);
			return;
		}
	}
	else if (degchange = Spiral_Mod, degchange < 6)
	{
		++Spiral_Mod;

		Spiral_Number = 0;

		FX_CalcSpiral(128);
		return;
	}

	switch (number)
	{
	case 0:
	case 1:
	default:
		degchange = 128;
		break;
	case 2:
		degchange = 160;
		break;
	case 3:
		degchange = 192;
		break;
	case 4:
		degchange = 224;
		break;
	}

	if ((degchange != Spiral_Degrees) || (Spiral_Number != number))
	{
		Spiral_Number = number;

		if (Spiral_Degrees < degchange)
		{
			Spiral_Degrees += 4;

			if (degchange < Spiral_Degrees)
			{
				Spiral_Degrees = degchange;
			}
		}
		else
		{
			if (degchange < Spiral_Degrees)
			{
				Spiral_Degrees -= 4;

				if (Spiral_Degrees < degchange)
				{
					Spiral_Degrees = degchange;
				}
			}
		}

		FX_CalcSpiral(Spiral_Degrees);
	}

	Spiral_Current = current_health;
	Spiral_Max = max_health;
}

void FX_Spiral_Init() // Matching - 100%
{
	Spiral_Max = 100000;

	Spiral_Current = 100000;

	Spiral_Number = 1;

	Spiral_Degrees = 128;

	FX_CalcSpiral(128);
}

void FX_DrawModel(struct Object* object, int model_num, struct _SVector* rotation, struct _SVector* position, struct _SVector* offset, int transflag)
{
	struct _Model* model;
	struct _MFace* mface;
	struct _MVertex* vertexList;
	struct TextureMT3* texture;
	MATRIX matrix;
	int i;
	POLY_GT3* poly;
	unsigned long** drawot;
	struct _SVector output;
	long color;
	long clip;

	drawot = gameTrackerX.drawOT;
	
	poly = (POLY_GT3*)gameTrackerX.primPool->nextPrim;

	PushMatrix();

	MATH3D_SetUnityMatrix(&matrix);

	RotMatrixX(rotation->x, &matrix);
	RotMatrixY(rotation->y, &matrix);
	RotMatrixZ(rotation->z, &matrix);

	PIPE3D_AspectAdjustMatrix(&matrix);

	ApplyMatrixSV(&matrix, (SVECTOR*)offset, (SVECTOR*)&output);

	matrix.t[0] = position->x + output.x;
	matrix.t[1] = position->y + output.y;
	matrix.t[2] = position->z + output.z;

	SetRotMatrix(&matrix);

	SetTransMatrix(&matrix);

	color = 0x34808080;

	if (transflag != 0)
	{
		color = 0x36909090;
	}

	if (object != NULL)
	{
		model = object->modelList[model_num];

		i = model->numFaces - 1;
		
		mface = model->faceList;
		
		vertexList = model->vertexList;

		if (i != -1)
		{
			for (; i != -1; i--, mface++)
			{
				if ((char*)(poly + 1) >= (char*)gameTrackerX.primPool->lastPrim)
				{
					gameTrackerX.primPool->nextPrim = (unsigned int*)poly;
				}
				else
				{
					texture = (struct TextureMT3*)mface->color;

					gte_ldv3(&vertexList[mface->face.v0], &vertexList[mface->face.v1], &vertexList[mface->face.v2]);
					gte_rtpt();

					poly->u0 = texture->u0;
					poly->v0 = texture->v0;
					poly->clut = texture->clut;

					poly->u1 = texture->u1;
					poly->v1 = texture->v1;
					poly->tpage = texture->tpage;

					poly->u2 = texture->u2;
					poly->v2 = texture->v2;
					*(unsigned int*)&poly->pad2 = *(unsigned int*)&texture->pad1;

					gte_nclip();

					*(unsigned int*)&poly->r2 = color;
					*(unsigned int*)&poly->r1 = color;
					*(unsigned int*)&poly->r0 = color;

					gte_stopz(&clip);

					if (clip < 0)
					{
						gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
						setlen(poly, 9);

#if defined(PSXPC_VERSION)
						addPrim(drawot[1 * 2], poly);
#elif defined(PSX_VERSION)
						addPrim(drawot[1], poly);
#endif
						poly = poly + 1;
					}
				}
			}
		}
	}

	gameTrackerX.primPool->nextPrim = (unsigned int*)poly;

	PopMatrix();
}

// autogenerated function stub: 
// void /*$ra*/ fx_calc_points(struct _SVector *points /*$s1*/, int degrees /*$s0*/, int radius /*$s3*/, int radius2 /*$s4*/, int radius3 /*stack 16*/)
void fx_calc_points(struct _SVector *points, int degrees, int radius, int radius2, int radius3)
{ // line 7653, offset 0x8004ddec
	/* begin block 1 */
		// Start line: 7654
		// Start offset: 0x8004DDEC
		// Variables:
			int cosval; // $s0
			int sinval; // $v0
	/* end block 1 */
	// End offset: 0x8004DDEC
	// End Line: 7654

	/* begin block 2 */
		// Start line: 18657
	/* end block 2 */
	// End Line: 18658
			UNIMPLEMENTED();
}

long fx_get_startz(struct _SVector* position)
{
	MATRIX tmpmat;
	
	gte_ldv0(position);

	gte_rt();

	gte_stlvnl(&tmpmat.t[0]);

	SetTransMatrix(&tmpmat);

	return (tmpmat.m[2][0] < 0) ? (tmpmat.m[2][0] + 3) >> 2 : (tmpmat.m[2][0]) >> 2;
}


// autogenerated function stub: 
// void /*$ra*/ FX_DrawRing(MATRIX *wcTransform /*$s0*/, struct _SVector *position /*$s3*/, MATRIX *matrix /*$s4*/, int radius /*stack 12*/, int radius2 /*stack 16*/, int radius3 /*stack 20*/, int z1 /*stack 24*/, int z2 /*stack 28*/, int z3 /*stack 32*/, long color /*stack 36*/, int sortInWorld /*stack 40*/)
void FX_DrawRing(MATRIX *wcTransform, struct _SVector *position, MATRIX *matrix, int radius, int radius2, int radius3, int z1, int z2, int z3, long color, int sortInWorld)
{ // line 7705, offset 0x8004df54
	/* begin block 1 */
		// Start line: 7706
		// Start offset: 0x8004DF54
		// Variables:
			int n; // $s3
			struct _POLY_2G4T *poly; // $s1
			unsigned long **drawot; // stack offset -44
			struct _SVector points[3]; // stack offset -96
			long sxy[3]; // stack offset -72
			long degrees; // $s6
			long color_black; // $s4
			long sz0; // stack offset -56
			long sz1; // stack offset -52
			long sz2; // stack offset -48
			long startz; // $s7
	/* end block 1 */
	// End offset: 0x8004E1D0
	// End Line: 7782

	/* begin block 2 */
		// Start line: 18769
	/* end block 2 */
	// End Line: 18770

	/* begin block 3 */
		// Start line: 18779
	/* end block 3 */
	// End Line: 18780
			UNIMPLEMENTED();
}

void fx_setTex(DVECTOR* x, unsigned char* uv, int tx, int offset)
{
	x->vx -= tx;

	if (x->vy < 256)
	{
		uv[1] = (unsigned char)x->vy;
	}
	else
	{
		uv[1] = 255;
	}

	x->vy += offset;
}


// autogenerated function stub: 
// void /*$ra*/ FX_DrawRing2(MATRIX *wcTransform /*$s0*/, struct _SVector *position /*$s1*/, MATRIX *matrix /*$s3*/, int radius /*stack 12*/, int radius2 /*stack 16*/, int radius3 /*stack 20*/, int z1 /*stack 24*/, int z2 /*stack 28*/, int z3 /*stack 32*/, long offset /*stack 36*/, int sortInWorld /*stack 40*/)
void FX_DrawRing2(MATRIX *wcTransform, struct _SVector *position, MATRIX *matrix, int radius, int radius2, int radius3, int z1, int z2, int z3, long offset, int sortInWorld)
{ // line 7847, offset 0x8004e244
	/* begin block 1 */
		// Start line: 7848
		// Start offset: 0x8004E244
		// Variables:
			int n; // $s3
			//struct POLY_FT4 *poly; // $s2
			unsigned long **drawot; // stack offset -60
			struct _SVector points[3]; // stack offset -112
			long sxy[3]; // stack offset -88
			long degrees; // $fp
			long sz0; // stack offset -72
			long sz1; // stack offset -68
			long sz2; // stack offset -64
			long startz; // stack offset -56
			short tx; // $s0
			long dispYPos; // $s0
			int num_faces; // $s4
			int deg_change; // stack offset -52
	/* end block 1 */
	// End offset: 0x8004E5EC
	// End Line: 7939

	/* begin block 2 */
		// Start line: 19122
	/* end block 2 */
	// End Line: 19123
			UNIMPLEMENTED();
}


void FX_DrawFField(MATRIX* wcTransform, struct _FXForceFieldEffect* field)  // Matching - 100%
{
	struct _Instance* instance;
	MATRIX tmpmat;
	struct _SVector position;
	int size;
	short fade;
	long color;
	long black;
	int temp, temp2;  // not from SYMDUMP

	instance = field->instance;

	position.x = instance->position.x + field->offset.x;
	position.y = instance->position.y + field->offset.y;
	position.z = instance->position.z + field->offset.z;

	black = 0;

	temp = rcos(field->deg) * field->size_change;

	field->deg = ((field->deg + field->deg_change) & 0xFFF);

	size = field->size_diff + (temp >> 12);

	if (field->start_fade != 0)
	{
		fade = field->start_fade - 128;

		field->start_fade = fade;

		if (fade < 0)
		{
			field->start_fade = 0;
		}

		temp2 = (unsigned short)field->start_fade << 16;
	}
	else
	{
		if (field->end_fade != 0)
		{
			fade = field->end_fade - 512;

			field->end_fade = fade;

			if ((fade << 16) <= 0)
			{
				FX_DeleteGeneralEffect((struct _FXGeneralEffect*)field);
				return;
			}

			fade = 4096 - fade;
		}
		else
		{
			fade = 0;
		}

		temp2 = fade << 16;
	}

	fade = temp2 >> 16;

	if (fade != 0)
	{
		LoadAverageCol((u_char*)&field->color, (u_char*)&black, 4096 - fade, fade, (u_char*)&color);
	}
	else
	{
		color = field->color;
	}

	color &= 0xFFFFFF;

	MATH3D_SetUnityMatrix(&tmpmat);

	RotMatrixZ(1024, &tmpmat);

	if (field->type == 1)
	{
		PIPE3D_AspectAdjustMatrix(&tmpmat);
	}
	else
	{
		RotMatrixX(theCamera.core.rotation.x, &tmpmat);
	}

	FX_DrawRing(wcTransform, &position, &tmpmat, field->size - size, field->size, field->size - size, 0, 0, 0, color, 1);
}


struct _FXForceFieldEffect* FX_StartFField(struct _Instance* instance, int size, _Position* offset, int size_diff, int size_change, int deg_change, long color)  // Matching - 100%
{
	struct _FXForceFieldEffect* field;
	short _x1;
	short _y1;
	short _z1;
	struct _SVector* _v0;

	field = (struct _FXForceFieldEffect*)MEMPACK_Malloc(44, 13);
	if (field != NULL)
	{
		field->effectType = 134;
		field->instance = instance;
		field->type = 0;
		field->lifeTime = -1;
		field->continue_process = NULL;
		field->size = size;
		_v0 = &field->offset;
		_x1 = offset->x;
		_y1 = offset->y;
		_z1 = offset->z;
		_v0->x = _x1;
		_v0->y = _y1;
		_v0->z = _z1;
		field->size_diff = size_diff;
		field->size_change = (short)size_change;
		field->deg = 0;
		field->deg_change = (short)deg_change;
		field->start_fade = 4096;
		field->end_fade = 0;
		field->color = color;
		FX_InsertGeneralEffect(field);
	}
	return field;
}

void FX_EndFField(struct _Instance* instance) // Matching - 100%
{
	struct _FXGeneralEffect* currentEffect;
	struct _FXForceFieldEffect* forceField; // not from SYMDUMP

	currentEffect = FX_GeneralEffectTracker;

	while (currentEffect != NULL)
	{
		if (currentEffect->instance == instance && currentEffect->effectType == 134)
		{
			forceField = (struct _FXForceFieldEffect*)currentEffect;

			if (forceField->start_fade != 0)
			{
				forceField->end_fade = 4096 - forceField->start_fade;
				forceField->start_fade = 0;

				if (forceField->end_fade == 0)
				{
					forceField->end_fade = 1;
				}
			}
			else
			{
				forceField->end_fade = 4096;
			}
		}

		currentEffect = (struct _FXGeneralEffect*)currentEffect->next;
	}
}

// autogenerated function stub: 
// void /*$ra*/ FX_Draw_Glowing_Line(unsigned long **ot /*$t6*/, long otz /*$a0*/, struct DVECTOR *sxy0 /*$a2*/, struct DVECTOR *sxy1 /*$a3*/, struct DVECTOR *xy0 /*stack 16*/, struct DVECTOR *xy1 /*stack 20*/, long color /*stack 24*/, long color2 /*stack 28*/)
void FX_Draw_Glowing_Line(unsigned long **ot, long otz, DVECTOR *sxy0, DVECTOR *sxy1, DVECTOR *xy0, DVECTOR *xy1, long color, long color2)
{ // line 8071, offset 0x8004e964
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_Lightning(MATRIX *wcTransform /*$s0*/, unsigned long **ot /*stack 4*/, MATRIX *mat /*stack 8*/, short deg /*$s1*/, struct _SVector *start /*stack 16*/, struct _SVector *end /*stack 20*/, int width /*stack 24*/, int small_width /*stack 28*/, int segs /*stack 32*/, int sine_size /*stack 36*/, int variation /*stack 40*/, long color /*stack 44*/, long glow_color /*stack 48*/)
void FX_Lightning(MATRIX *wcTransform, unsigned long **ot, MATRIX *mat, short deg, struct _SVector *start, struct _SVector *end, int width, int small_width, int segs, int sine_size, int variation, long color, long glow_color)
{ // line 8119, offset 0x8004eb00
	/* begin block 1 */
		// Start line: 8120
		// Start offset: 0x8004EB00
		// Variables:
			int sz0; // $s5
			int sz1; // stack offset -56
			long otz; // $s1
			int length; // $v1
			//struct DVECTOR sxy0; // stack offset -112
			//struct DVECTOR sxy1; // stack offset -104
			//struct DVECTOR xy0; // stack offset -96
			///truct DVECTOR xy1; // stack offset -88
			//struct DVECTOR small_xy0; // stack offset -80
			//struct DVECTOR small_xy1; // stack offset -72
			int n; // $s3
			int increment; // stack offset -52
			int rsin_nd2; // $s2

		/* begin block 1.1 */
			// Start line: 8154
			// Start offset: 0x8004EBB4
			// Variables:
				//SVECTORpoint; // stack offset -64

			/* begin block 1.1.1 */
				// Start line: 8162
				// Start offset: 0x8004EC04
				// Variables:
					int rsin_n4; // $s0
			/* end block 1.1.1 */
			// End offset: 0x8004ECF0
			// End Line: 8166

			/* begin block 1.1.2 */
				// Start line: 8170
				// Start offset: 0x8004ED00
				// Variables:
					int tmpdeg; // $s1
			/* end block 1.1.2 */
			// End offset: 0x8004EDC8
			// End Line: 8174

			/* begin block 1.1.3 */
				// Start line: 8188
				// Start offset: 0x8004EE30
				// Variables:
					int fx; // $s2
					int fy; // $s0
			/* end block 1.1.3 */
			// End offset: 0x8004EE78
			// End Line: 8196
		/* end block 1.1 */
		// End offset: 0x8004EF94
		// End Line: 8218
	/* end block 1 */
	// End offset: 0x8004EFC0
	// End Line: 8227

	/* begin block 2 */
		// Start line: 19793
	/* end block 2 */
	// End Line: 19794
					UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ FX_LightHouse(MATRIX *wcTransform /*$s0*/, unsigned long **ot /*stack 4*/, struct _Instance *instance /*$a2*/, int startSeg /*$a3*/, int endSeg /*stack 16*/, int segs /*stack 20*/, long beam_color /*stack 24*/)
void FX_LightHouse(MATRIX *wcTransform, unsigned long **ot, struct _Instance *instance, int startSeg, int endSeg, int segs, long beam_color)
{ // line 8232, offset 0x8004eff0
	/* begin block 1 */
		// Start line: 8233
		// Start offset: 0x8004EFF0
		// Variables:
			int sz0; // $s4
			int sz1; // stack offset -72
			long otz; // $a1
			int width; // $s0
			struct _SVector start; // stack offset -136
			struct _SVector end; // stack offset -128
			//struct DVECTOR sxy0; // stack offset -120
			//struct DVECTOR sxy1; // stack offset -112
			//struct DVECTOR xy0; // stack offset -104
			//struct DVECTOR xy1; // stack offset -96
			int fade; // $a3
			int n; // $s2
			int increment; // $s6
			long black; // stack offset -64
			long color; // stack offset -68
			long color1; // stack offset -56
			long color2; // stack offset -60
			int fx; // stack offset -52
			int fy; // stack offset -48
			int length; // $s7
			int segcnt; // $s3
			int end_width; // stack offset -44
			int flag; // $fp
			//MATRIX *swTransform; // $v1

		/* begin block 1.1 */
			// Start line: 8292
			// Start offset: 0x8004F138
			// Variables:
				//SVECTORpoint; // stack offset -88

			/* begin block 1.1.1 */
				// Start line: 8303
				// Start offset: 0x8004F1B0
				// Variables:
					int newlength; // $v1
					int newfx; // $s1
					int newfy; // $s0
			/* end block 1.1.1 */
			// End offset: 0x8004F1F4
			// End Line: 8317
		/* end block 1.1 */
		// End offset: 0x8004F214
		// End Line: 8324

		/* begin block 1.2 */
			// Start line: 8335
			// Start offset: 0x8004F248
			// Variables:
				//SVECTORpoint; // stack offset -80

			/* begin block 1.2.1 */
				// Start line: 8357
				// Start offset: 0x8004F330
				// Variables:
					int x; // $v0
			/* end block 1.2.1 */
			// End offset: 0x8004F3BC
			// End Line: 8369
		/* end block 1.2 */
		// End offset: 0x8004F40C
		// End Line: 8378
	/* end block 1 */
	// End offset: 0x8004F43C
	// End Line: 8388

	/* begin block 2 */
		// Start line: 20062
	/* end block 2 */
	// End Line: 20063
					UNIMPLEMENTED();
}

void FX_StartPassthruFX(struct _Instance* instance, struct _SVector* normal, struct _SVector* point) // Matching - 100%
{
	long color;

	instance->halvePlane.a = normal->x;
	instance->halvePlane.b = normal->y;
	instance->halvePlane.c = normal->z;

	color = 0x20FF40;

	instance->halvePlane.d = -(((normal->x * point->x) + (normal->y * point->y) + (normal->z * point->z)) >> 12);

	FX_DoInstancePowerRing(instance, 8400, &color, 0, 2);
	FX_DoInstancePowerRing(instance, 8400, &color, 0, 1);
}

void FX_EndPassthruFX(struct _Instance* instance) // Matching - 100%
{
	FX_EndInstanceEffects(instance);
}
