#include "CORE.H"
#include "LIGHT3D.H"
#include "STREAM.H"
#include "PSX/DRAWS.H"

struct LightGroup default_lightgroup;

void LIGHT_GetLightMatrix(struct _Instance* instance, struct Level* level, MATRIX* lightM, MATRIX* colorM)
{
	MATRIX* lightGroup;
	struct LightList* lightList;
	int lightGrp;
	MATRIX* tlightGroup;
	struct LightList* tlightList;
	int tlightGrp;
	MATRIX* start;
	MATRIX* end;
	int i;
	int j;
	long ratio;

	lightGrp = instance->lightGroup;
	lightGroup = NULL;

	if (gameTrackerX.gameData.asmData.MorphType != 0)
	{
		if (instance != gameTrackerX.playerInstance || level->razielSpectralLightGroup == NULL)
		{
			lightList = (struct LightList*)level->spectrallightList;

			if (lightList != NULL && lightList->numLightGroups != 0)
			{
				lightGrp = instance->spectralLightGroup;
			}
			else
			{
				lightList = level->lightList;
			}
		}
		else
		{
			lightGroup = (MATRIX*)level->razielSpectralLightGroup;
		}
	}
	else
	{
		if (instance == gameTrackerX.playerInstance && level->razielLightGroup != NULL)
		{
			lightGroup = (MATRIX*)level->razielLightGroup;
		}
		else
		{
			lightList = level->lightList;
		}
	}

	if (lightGroup == NULL)
	{
		if (lightList->numLightGroups == 0 || lightGrp >= lightList->numLightGroups)
		{
			lightGroup = (MATRIX*)&default_lightgroup;
		}
		else
		{
			lightGroup = (MATRIX*)&lightList->lightGroupList[lightGrp];
		}
	}

	if (gameTrackerX.gameData.asmData.MorphTime != 1000)
	{
		tlightGrp = instance->lightGroup;

		tlightGroup = NULL;

		if (gameTrackerX.gameData.asmData.MorphType == 0)
		{
			if (instance != gameTrackerX.playerInstance || level->razielSpectralLightGroup == NULL)
			{
				tlightList = level->spectrallightList;

				if (tlightList != NULL && tlightList->numLightGroups != 0)
				{
					tlightGrp = instance->spectralLightGroup;
				}
				else
				{
					tlightList = level->lightList;
				}
			}
			else
			{
				tlightGroup = (MATRIX*)level->razielSpectralLightGroup;
			}
		}
		else
		{
			if (instance == gameTrackerX.playerInstance && level->razielLightGroup != NULL)
			{
				tlightGroup = (MATRIX*)level->razielLightGroup;
			}
			else
			{
				tlightList = level->lightList;
			}
		}
	}
	else
	{
		lightM->m[0][0] = lightGroup[0].m[0][0];
		lightM->m[0][1] = lightGroup[0].m[0][1];
		lightM->m[0][2] = lightGroup[0].m[0][2];
		lightM->m[1][0] = lightGroup[0].m[1][0];
		lightM->m[1][1] = lightGroup[0].m[1][1];
		lightM->m[1][2] = lightGroup[0].m[1][2];
		lightM->m[2][0] = lightGroup[0].m[2][0];
		lightM->m[2][1] = lightGroup[0].m[2][1];
		lightM->m[2][2] = lightGroup[0].m[2][2];

		colorM->m[0][0] = lightGroup[1].m[0][0];
		colorM->m[0][1] = lightGroup[1].m[0][1];
		colorM->m[0][2] = lightGroup[1].m[0][2];
		colorM->m[1][0] = lightGroup[1].m[1][0];
		colorM->m[1][1] = lightGroup[1].m[1][1];
		colorM->m[1][2] = lightGroup[1].m[1][2];
		colorM->m[2][0] = lightGroup[1].m[2][0];
		colorM->m[2][1] = lightGroup[1].m[2][1];
		colorM->m[2][2] = lightGroup[1].m[2][2];

		return;
	}

	if (tlightGroup == NULL)
	{
		if (tlightList->numLightGroups == 0 || tlightGrp >= tlightList->numLightGroups)
		{
			tlightGroup = (MATRIX*)&default_lightgroup;
		}
		else
		{
			tlightGroup = (MATRIX*)&tlightList->lightGroupList[tlightGrp];
		}
	}

	lightM->m[0][0] = lightGroup->m[0][0];
	lightM->m[0][1] = lightGroup->m[0][1];
	lightM->m[0][2] = lightGroup->m[0][2];
	lightM->m[1][0] = lightGroup->m[1][0];
	lightM->m[1][1] = lightGroup->m[1][1];
	lightM->m[1][2] = lightGroup->m[1][2];
	lightM->m[2][0] = lightGroup->m[2][0];
	lightM->m[2][1] = lightGroup->m[2][1];
	lightM->m[2][2] = lightGroup->m[2][2];

	start = tlightGroup + 1;
	end = lightGroup + 1;
	
	ratio = 4096 - ((((gameTrackerX.gameData.asmData.MorphTime * 4096) / 1000) >> 6) - ((gameTrackerX.gameData.asmData.MorphTime * 4096) >> 31));

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			colorM->m[i][j] = start->m[i][j] + ((end->m[i][j] - start->m[i][j]) * ratio);
		}
	}

	return;
}

void LIGHT_PresetInstanceLight(struct _Instance* instance, short attenuate, MATRIX* lm)
{
#if defined(PSX_VERSION)
	MATRIX cm;
	long scale;
	long scaleRGB[3];
	int i;
	int j;
	struct CDLight* extraLight;
	struct Level* level;
	short tempRGB[3];
	short* todRGB;

	extraLight = (struct CDLight*)instance->extraLight;

	tempRGB[0] = 16;
	tempRGB[1] = 16;
	tempRGB[2] = 16;

	level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

	LIGHT_GetLightMatrix(instance, level, lm, &cm);

	if ((instance->flags & 0x200000))
	{
		scale = 2048;
	}
	else
	{
		scale = 4096;
	}

	if (attenuate != 4096)
	{
		scale = (scale * attenuate) >> 12;
	}

	if (instance->extraLight != NULL && !(instance->flags & 0x200000))
	{
		scale = ((4096 - instance->extraLightScale) * scale) >> 12;

		scaleRGB[0] = scale + ((instance->extraLightScale * extraLight->r) >> 6);
		scaleRGB[1] = scale + ((instance->extraLightScale * extraLight->g) >> 6);
		scaleRGB[2] = scale + ((instance->extraLightScale * extraLight->b) >> 6);
	}
	else
	{
		scaleRGB[0] = scale;
		scaleRGB[1] = scale;
		scaleRGB[2] = scale;
	}

	if (level != NULL)
	{
		todRGB = &level->TODRedScale;
	}
	else
	{
		todRGB = &tempRGB[0];
	}

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			cm.m[i][j] = MAX(MIN((cm.m[i][j] * (((scaleRGB[i] * todRGB[i]) << 4) >> 16)) >> 12, -32768), 32767);
		}
	}

	SetColorMatrix(&cm);

#elif defined(PC_VERSION)
	int currentStreamUnitID; // eax
	struct Level* LevelWithID; // edi
	int v5; // eax
	__int16* p_TODRedScale; // eax
	MATRIX* p_colorM; // ecx
	int* v8; // ebp
	__int16* v9; // ebx
	int v10; // edi
	int v11; // esi
	int v12; // eax
	int v13; // edx
	__int16 v14[4]; // [esp+10h] [ebp-34h] BYREF
	int v15[3]; // [esp+18h] [ebp-2Ch] BYREF
	MATRIX colorM; // [esp+24h] [ebp-20h] BYREF
	int attenuatea; // [esp+4Ch] [ebp+8h]

	currentStreamUnitID = instance->currentStreamUnitID;
	v14[0] = 4096;
	v14[1] = 4096;
	v14[2] = 4096;
	LevelWithID = STREAM_GetLevelWithID(currentStreamUnitID);
	LIGHT_GetLightMatrix(instance, LevelWithID, lm, &colorM);
	v5 = 4096;
	if ((instance->flags & 0x200000) != 0)
		v5 = 2048;
	if (attenuate != 4096)
		v5 = (v5 * attenuate) >> 12;
	v15[0] = v5;
	v15[1] = v5;
	v15[2] = v5;
	p_TODRedScale = &LevelWithID->TODRedScale;
	if (!LevelWithID)
		p_TODRedScale = v14;
	p_colorM = &colorM;
	v8 = v15;
	v9 = p_TODRedScale;
	attenuatea = 3;
	do
	{
		v10 = (__int16)((*v8 * *v9) >> 12);
		v11 = 3;
		do
		{
			v12 = (v10 * p_colorM->m[0][0]) >> 12;
			v13 = v12;
			if (v12 <= -32768)
				v13 = -32768;
			if (v13 >= 32767)
			{
				v12 = 32767;
			}
			else if (v12 <= -32768)
			{
				v12 = 0x8000;
			}
			p_colorM->m[0][0] = v12;
			p_colorM = (MATRIX*)((char*)p_colorM + 2);
			--v11;
		} while (v11);
		++v9;
		++v8;
		--attenuatea;
	} while (attenuatea);
	SetColorMatrix(&colorM);
#endif
}

void LIGHT_GetAmbient(struct _ColorType* color, struct _Instance* instance)
{
#if defined(PSX_VERSION)
	int lightval;
	
	if ((instance->object->oflags2 & 0x800))
	{
		lightval = 0;
	}
	else
	{
		lightval = 48;
	}

	color->b = lightval;
	color->g = lightval;
	color->r = lightval;

#elif defined(PC_VERSION)
	uchar v2; // al

	v2 = (instance->object->oflags2 & 0x800) != 0 ? 0 : 48;
	color->b = v2;
	color->g = v2;
	color->r = v2;
#endif
}


// autogenerated function stub: 
// void /*$ra*/ LIGHT_CalcLightValue(struct _TFace *tface /*$t2*/, struct _Instance *instance /*$fp*/, struct _Terrain *terrain /*$t3*/)
void LIGHT_CalcLightValue(struct _TFace *tface, struct _Instance *instance, struct _Terrain *terrain)
{ // line 336, offset 0x80035d44
	/* begin block 1 */
		// Start line: 337
		// Start offset: 0x80035D44
		// Variables:
			struct _ColorType color; // stack offset -88
			short fadespeed; // $a2

		/* begin block 1.1 */
			// Start line: 348
			// Start offset: 0x80035DA4
			// Variables:
				struct _ColorType color1; // stack offset -80
				struct _ColorType color2; // stack offset -72
				long n; // $v1
				long count; // $a0
				long edge; // $a2
				int x1; // $v1
				int x2; // $a0
				int interp1; // $s3
				int interp2; // $s2
				int interp; // $a3
				short *temp; // $v0
				short *vertex0; // $s6
				short *vertex1; // $s5
				short *vertex2; // $s4
				short position[3]; // stack offset -64
				struct _SVector normal; // stack offset -56
				struct BSPTree *bsp; // $a1
				int major; // $a3
				int minor; // $s7

			/* begin block 1.1.1 */
				// Start line: 457
				// Start offset: 0x80036194
				// Variables:
					long r; // $v0
					long g; // $a1
					long b; // $v1
					int lum; // $a0
			/* end block 1.1.1 */
			// End offset: 0x80036244
			// End Line: 478
		/* end block 1.1 */
		// End offset: 0x80036244
		// End Line: 478

		/* begin block 1.2 */
			// Start line: 493
			// Start offset: 0x80036264
			// Variables:
				int i; // $t0
				struct LightInstance *li; // $t4
				long dist; // $t3
				struct LightInstance *tli; // $t2

			/* begin block 1.2.1 */
				// Start line: 500
				// Start offset: 0x80036274
				// Variables:
					struct _Instance *inst; // $a1

				/* begin block 1.2.1.1 */
					// Start line: 503
					// Start offset: 0x8003629C
					// Variables:
						short tdist; // $a3
						_Position pos; // stack offset -64
						//MATRIX *mat; // $a1
				/* end block 1.2.1.1 */
				// End offset: 0x80036384
				// End Line: 520
			/* end block 1.2.1 */
			// End offset: 0x80036384
			// End Line: 521
		/* end block 1.2 */
		// End offset: 0x80036450
		// End Line: 545
	/* end block 1 */
	// End offset: 0x80036468
	// End Line: 553

	/* begin block 2 */
		// Start line: 740
	/* end block 2 */
	// End Line: 741
						UNIMPLEMENTED();
}

void LIGHT_SetAmbientInstance(struct _Instance* instance, struct Level* level)
{
	SetBackColor(((struct _ColorType*)&instance->light_color)->r, ((struct _ColorType*)&instance->light_color)->g, ((struct _ColorType*)&instance->light_color)->b);
}

void LIGHT_SetMatrixForLightGroupInstance(struct _Instance* instance, struct Level* level)
{
	struct LightList *lightList; // $v0
	int lightGrp; // $v1
	
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ LIGHT_DrawShadow(MATRIX *wcTransform /*$s2*/, struct _Instance *instance /*$s1*/, struct _PrimPool *primPool /*$s3*/, unsigned long **ot /*$s4*/)
void LIGHT_DrawShadow(MATRIX *wcTransform, struct _Instance *instance, struct _PrimPool *primPool, unsigned long **ot)
{ // line 730, offset 0x80036644
#if defined(PC_VERSION)
	int v4; // eax
	__int16 v5; // ax
	__int16 z; // cx
	int y; // edx
	DWORD* v8; // eax
	SVECTOR v9; // [esp+8h] [ebp-54h] BYREF
	VECTOR v; // [esp+10h] [ebp-4Ch] BYREF
	int x; // [esp+30h] [ebp-2Ch]
	int v12; // [esp+34h] [ebp-28h]
	int v13; // [esp+38h] [ebp-24h]
	MATRIX scTransform; // [esp+3Ch] [ebp-20h] BYREF

	if (instance->shadowPosition.z > instance->position.z - 1280)
	{
		v9.vx = -MATH3D_FastAtan2(instance->wNormal.y, instance->wNormal.z);
		v4 = MATH3D_FastSqrt0(0x1000000 - instance->wNormal.x * instance->wNormal.x);
		v5 = MATH3D_FastAtan2(instance->wNormal.x, v4);
		z = instance->rotation.z;
		v9.vy = v5;
		v9.vz = z;
		RotMatrix(&v9, (MATRIX*)&v.pad);
		y = instance->shadowPosition.y;
		x = instance->shadowPosition.x;
		v12 = y;
		v13 = instance->shadowPosition.z;
		TRANS_CompMatrix(wcTransform, (MATRIX*)&v.pad, &scTransform);
		v.vz = ((((instance->shadowPosition.z - instance->position.z) << 12) / 1280 + 4096)
			* ((instance->object->modelList[instance->currentModel]->maxRad << 12)
				/ 480)) >> 12;
		v.vy = v.vz;
		v.vx = v.vz;
		ScaleMatrix(&scTransform, &v);
		SetRotMatrix(&scTransform);
		SetTransMatrix(&scTransform);
#if PSX_VERSION
		DRAW_DrawShadow(primPool, model, ot, instance->fadeValue);
#else
		v8 = (DWORD*)INSTANCE_Query(instance, 38);
		if (v8 && *v8 >= 2u)
			D3DDRAW_DrawSegmentShadow(instance, v8, instance->fadeValue);
		else
			D3DDRAW_DrawShadow(0, ot, instance->fadeValue, &scTransform);
#endif
	}
#else
	UNIMPLEMENTED();
#endif
}


// autogenerated function stub: 
// void /*$ra*/ LIGHT_CalcShadowPositions(struct GameTracker *gameTracker /*$a0*/)
void LIGHT_CalcShadowPositions(struct GameTracker *gameTracker)
{ // line 806, offset 0x80036928
#if defined(PC_VERSION)
	struct _Instance* first; // esi
	int flags; // eax
	struct _TFace* waterFace; // eax
	struct _Terrain** tfaceLevel; // eax
	__int16 z; // ax
	__int16 v6; // cx
	struct Level* LevelWithID; // eax
	__int16 type; // ax
	int v9; // eax
	int v10; // eax
	int currentStreamUnitID; // [esp-Ch] [ebp-50h]
	SVECTOR v12, v13; // [esp+8h] [ebp-3Ch] BYREF
	struct _PCollideInfo pcollideInfo; // [esp+18h] [ebp-2Ch] BYREF

	first = (struct _Instance*)*((DWORD*)gameTracker->instanceList + 1);
	if (first)
	{
		while ((first->flags2 & 0x40) != 0)
		{
			flags = first->flags;
			if ((flags & 0x200) == 0 || (flags & 0x800) != 0 || (first->flags2 & 0x4000000) != 0)
				break;
			if ((flags & 0x8000000) == 0)
			{
				if ((flags & 0x10000000) != 0)
				{
				LABEL_13:
					v12.vx = first->matrix[1].t[0];
					v13.vx = v12.vx;
					v12.vy = first->matrix[1].t[1];
					v13.vy = v12.vy;
					z = first->matrix[1].t[2];
					v6 = z;
				}
				else
				{
					*(DWORD*)&v12.vx = *(DWORD*)&first->position.x;
					z = first->position.z;
					v12.vz = z;
					*(DWORD*)&v13.vx = *(DWORD*)&first->position.x;
					v6 = first->position.z;
				}
				v12.vz = z - 1280;
				v13.vz = v6 + 256;
				pcollideInfo.collideType = 55;
				pcollideInfo.instance = first;
				pcollideInfo.newPoint = (struct SVECTOR*)&v12;
				pcollideInfo.oldPoint = (struct SVECTOR*)&v13;
				currentStreamUnitID = first->currentStreamUnitID;
				first->flags |= 0x40u;
				LevelWithID = STREAM_GetLevelWithID(currentStreamUnitID);
				if (LevelWithID)
					COLLIDE_PointAndWorld(&pcollideInfo, LevelWithID);
				else
					pcollideInfo.type = 0;
				first->flags &= ~0x40u;
				type = pcollideInfo.type;
				if (pcollideInfo.type == 3)
				{
					LIGHT_CalcLightValue((struct _TFace*)pcollideInfo.prim, first, (struct _Terrain*)pcollideInfo.inst->node.prev);
					goto LABEL_22;
				}
				if (pcollideInfo.type != 5)
				{
					LIGHT_CalcLightValue(0, first, 0);
				LABEL_22:
					type = pcollideInfo.type;
				}
				if (type)
				{
					if (type == 1)
					{
						first->wNormal.x = 0;
						first->wNormal.y = 0;
						first->wNormal.z = 4096;
					}
					else
					{
						if (type == 3
							&& (*((WORD*)pcollideInfo.prim + 5) == 0xFFFF
								? (v9 &= 0xffff00ff)
								: (v9 = *(unsigned __int16*)((char*)&pcollideInfo.inst->node.prev[6].next[1].prev
									+ *((unsigned __int16*)pcollideInfo.prim + 5)
									+ 2)),
								(v9 & 0x4000) != 0))
						{
							v10 = first->flags | 0x200000;
						}
						else
						{
							v10 = first->flags & 0xFFDFFFFF;
						}
						first->flags = v10;
						first->wNormal.x = pcollideInfo.wNormal.vx;
						first->wNormal.y = pcollideInfo.wNormal.vy;
						first->wNormal.z = pcollideInfo.wNormal.vz;
					}
				}
				*(DWORD*)&first->shadowPosition.x = *(DWORD*)&v12.vx;
				first->shadowPosition.z = v12.vz;
				goto LABEL_38;
			}
			if ((flags & 0x10000000) != 0)
				goto LABEL_13;
			waterFace = first->waterFace;
			if (waterFace)
			{
				LIGHT_CalcLightValue(waterFace, first, first->waterFaceTerrain);
			}
			else
			{
				tfaceLevel = (struct _Terrain**)first->tfaceLevel;
				if (tfaceLevel)
					goto LABEL_37;
			}
		LABEL_38:
			first->flags &= ~0x8000000u;
			first = first->next;
			if (!first)
				return;
		}
		if ((first->flags2 & 0x40) != 0)
			goto LABEL_38;
		tfaceLevel = (struct _Terrain**)first->tfaceLevel;
		if (!tfaceLevel)
			goto LABEL_38;
	LABEL_37:
		LIGHT_CalcLightValue(first->tface, first, *tfaceLevel);
		goto LABEL_38;
	}
#else
UNIMPLEMENTED();
#endif
}

void LIGHT_Restore(struct LightInfo* lightInfo)
{
}

void LIGHT_CalcDQPTable(struct Level *level)
{ 
#if defined(PSX_VERSION)
	long dqa;
	long limit;

	if (level->fogFar != level->fogNear)
	{
		limit = 40958;
		dqa = -((level->fogFar / level->fogNear) / (level->fogFar - level->fogNear));
		
		if (limit < dqa)
		{
			dqa = limit;

			if (level->holdFogNear == level->fogNear)
			{
				level->holdFogNear = (level->fogFar * limit) / (limit - level->fogFar);
			}

			level->fogNear = (level->fogFar * limit) / (limit - level->fogFar);
		}

		if (dqa < -limit)
		{
			dqa = -limit;

			if (level->holdFogNear == level->fogNear)
			{
				level->holdFogNear = (-limit * level->fogFar) / (dqa - level->fogFar);
			}

			level->fogNear = (-limit * level->fogFar) / (dqa - level->fogFar);
		}
	
		depthQFogStart = (-dqa * 4096) / ((level->fogFar * 4096) / (level->fogFar - level->fogNear));

		if (level->backColorR != 0 && level->backColorG != 0)///@check maybe b too
		{
			depthQBlendStart = (-dqa * 4096) / ((level->fogFar * 4096) / (level->fogFar - level->fogNear));
		}
		else
		{
			depthQBlendStart = 0xFFFF;
		}

		level->depthQFogStart = depthQFogStart;
		level->depthQBlendStart = depthQBlendStart;
	}
#elif defined(PC_VERSION)
	unsigned __int16 fogNear; // bx
	int fogFar; // ecx
	int v3; // esi
	int v4; // eax
	int v5; // eax
	int v6; // eax
	unsigned __int8 backColorG; // cl

	fogNear = level->fogNear;
	if (level->fogFar != fogNear)
	{
		fogFar = level->fogFar;
		v3 = -(fogFar * fogNear / (fogFar - fogNear));
		if (v3 > 40958)
		{
			v3 = 40958;
			v4 = 40958 * fogFar / (40958 - fogFar);
			if (level->holdFogNear == fogNear)
				level->holdFogNear = v4;
			level->fogNear = v4;
		}
		if (v3 < -40958)
		{
			v3 = -40958;
			v5 = -40958 * fogFar / (-40958 - fogFar);
			if (level->holdFogNear == level->fogNear)
				level->holdFogNear = v5;
			level->fogNear = v5;
		}
		v6 = -4096 * v3 / ((fogFar << 12) / (fogFar - level->fogNear));
		depthQFogStart = v6;
		if (level->backColorR || (backColorG = level->backColorG, depthQBlendStart = 0xFFFF, backColorG))
			depthQBlendStart = v6;
		level->depthQFogStart = v6;
		level->depthQBlendStart = depthQBlendStart;
	}
#endif
}




