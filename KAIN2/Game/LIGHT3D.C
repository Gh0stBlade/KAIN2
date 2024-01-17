#include "CORE.H"
#include "LIGHT3D.H"
#include "STREAM.H"
#include "PSX/DRAWS.H"
#include "Game/COLLIDE.H"
#include "Game/MATH3D.H"
#include "Game/DRAW.H"

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
			colorM->m[i][j] = (short)(start->m[i][j] + ((end->m[i][j] - start->m[i][j]) * ratio));
		}
	}

	return;
}

void LIGHT_PresetInstanceLight(struct _Instance* instance, short attenuate, MATRIX* lm)
{
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
}

void LIGHT_GetAmbient(struct _ColorType* color, struct _Instance* instance)  // Matching - 100%
{
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

void LIGHT_SetMatrixForLightGroupInstance(struct _Instance* instance, struct Level* level)//Matching - 64.62%
{
	int lightGrp; // $s1
	struct LightList* lightList; // $s0
	int numLightGroups; // $v0
	MATRIX* lgt; // $s0
	int lightMatrix; // $v0
	int x; // $t0
	int y; // $t1
	int z; // $t2
	int w;
	MATRIX* matrix; // $v0
	MATRIX lgt_cat; // [sp+10h] [-70h] BYREF
	MATRIX lm; // [sp+30h] [-50h] BYREF
	MATRIX cm; // [sp+50h] [-30h] BYREF
	VECTOR half; // [sp+70h] [-10h] BYREF
	typedef struct {
		long m[5]; // size=20, offset=0
	} cmm;
	static cmm cmmVec = { 2048, 2048, 2048, 0 };

	half.vx = cmmVec.m[0];
	half.vy = cmmVec.m[1];
	half.vz = cmmVec.m[2];
	half.pad = cmmVec.m[3];

	lightGrp = instance->lightGroup;
	if (instance->matrix)
	{
		if (gameTrackerX.gameData.asmData.MorphType != 0 && (lightList = level->spectrallightList) != 0 && (numLightGroups = lightList->numLightGroups) != 0)
		{
			lightGrp = instance->spectralLightGroup;
			if (numLightGroups < lightGrp)
			{
				instance->spectralLightGroup = 0;
				lightGrp = 0;
			}
		}
		else
		{
			lightList = level->lightList;
			if (lightList && lightList->numLightGroups < lightGrp)
			{
				instance->lightGroup = 0;
				lightGrp = 0;
			}
		}
		LIGHT_SetAmbientInstance(instance, level);
		if (lightList->numLightGroups == 0)
		{
			lgt = (MATRIX*)&default_lightgroup;
		}
		else
		{
			lgt = &lightList->lightGroupList[lightGrp].lightMatrix;
		}

		lightMatrix = instance->lightMatrix;
		if (lightMatrix != 0)
		{
			((cmm*)&lgt_cat.m[0][0])->m[0] = ((cmm*)&instance->matrix[lightMatrix].m[0][0])->m[0];
			((cmm*)&lgt_cat.m[0][0])->m[1] = ((cmm*)&instance->matrix[lightMatrix].m[0][0])->m[1];
			((cmm*)&lgt_cat.m[0][0])->m[2] = ((cmm*)&instance->matrix[lightMatrix].m[0][0])->m[2];

			lgt_cat.t[0] = instance->matrix[lightMatrix].t[0];
			lgt_cat.t[1] = instance->matrix[lightMatrix].t[1];
			lgt_cat.t[2] = instance->matrix[lightMatrix].t[2];
		}
		else
		{
			if ((instance->flags & 0x1) != 0)
			{
				matrix = instance->matrix;
				((cmm*)&lgt_cat.m[0][0])->m[0] = ((cmm*)&matrix->m[0][0])->m[0];
				((cmm*)&lgt_cat.m[0][0])->m[1] = ((cmm*)&matrix->m[0][0])->m[1];
				((cmm*)&lgt_cat.m[0][0])->m[2] = ((cmm*)&matrix->m[0][0])->m[2];
				((cmm*)&lgt_cat.m[0][0])->m[3] = ((cmm*)&matrix->m[0][0])->m[3];

				lgt_cat.t[0] = matrix->t[0];
				lgt_cat.t[1] = matrix->t[1];
				lgt_cat.t[2] = matrix->t[2];
			}
			else
			{
				RotMatrix((SVECTOR*)&instance->rotation, &lgt_cat);
			}
		}
		if (instance->extraLight)
		{
			lm.m[0][0] = lgt->m[0][0];
			lm.m[0][1] = lgt->m[0][1];
			lm.m[0][2] = lgt->m[0][2];
			lm.m[1][0] = lgt->m[1][0];
			lm.m[1][1] = lgt->m[1][1];
			lm.m[1][2] = lgt->m[1][2];
			lm.m[2][0] = (instance->extraLightDir.x * instance->extraLightScale) >> 12;
			lm.m[2][1] = (instance->extraLightDir.y * instance->extraLightScale) >> 12;
			lm.m[2][2] = (instance->extraLightDir.z * instance->extraLightScale) >> 12;
			cm.m[0][0] = lgt[1].m[0][0];
			cm.m[0][1] = lgt[1].m[1][0];
			cm.m[0][2] = lgt[1].m[2][0];
			cm.m[1][0] = lgt[1].m[0][1];
			cm.m[1][1] = lgt[1].m[1][1];
			cm.m[1][2] = lgt[1].m[2][1];
			cm.m[2][0] = ((struct CDLight*)instance->extraLight)->r * 16;
			cm.m[2][1] = ((struct CDLight*)instance->extraLight)->g * 16;
			cm.m[2][2] = ((struct CDLight*)instance->extraLight)->b * 16;
			MulMatrix0(&lm, &lgt_cat, &lgt_cat);
			SetLightMatrix(&lgt_cat);
			if ((instance->flags & 0x200000))
			{
				ScaleMatrix(&cm, &half);
			}
		}
		else
		{
			MulMatrix0(lgt, &lgt_cat, &lgt_cat);
			SetLightMatrix(&lgt_cat);
			if ((instance->flags & 0x200000) != 0)
			{
				((cmm*)&cm.m[0][0])->m[0] = ((cmm*)&lgt[1].m[0][0])->m[0];
				((cmm*)&cm.m[0][0])->m[1] = ((cmm*)&lgt[1].m[0][0])->m[1];
				((cmm*)&cm.m[0][0])->m[2] = ((cmm*)&lgt[1].m[0][0])->m[2];
				((cmm*)&cm.m[0][0])->m[3] = ((cmm*)&lgt[1].m[0][0])->m[3];
				((cmm*)&cm.m[0][0])->m[4] = ((cmm*)&lgt[1].m[0][0])->m[4];

				ScaleMatrix(&cm, &half);
				SetColorMatrix(&cm);
			}
			else
			{
				SetColorMatrix(lgt + 1);
			}
		}
	}
}

void LIGHT_DrawShadow(MATRIX* wcTransform, struct _Instance* instance, struct _PrimPool* primPool, unsigned int** ot)//Matching - 85.65%
{
	SVECTOR face_orient;
	MATRIX rot;
	MATRIX scTransform;
	struct _Vector scale;
	struct _Instance* playerInstance;

	playerInstance = gameTrackerX.playerInstance;

	if (playerInstance->position.z - 1280 < playerInstance->shadowPosition.z)
	{
		face_orient.vx = -MATH3D_FastAtan2(playerInstance->wNormal.y, playerInstance->wNormal.z);
		face_orient.vy = MATH3D_FastAtan2(playerInstance->wNormal.x, MATH3D_FastSqrt0(0x1000000 - playerInstance->wNormal.x * playerInstance->wNormal.x));
		face_orient.vz = playerInstance->rotation.z;

		RotMatrix(&face_orient, &rot);

		rot.t[0] = playerInstance->shadowPosition.x;
		rot.t[1] = playerInstance->shadowPosition.y;
		rot.t[2] = playerInstance->shadowPosition.z;

		gte_SetRotMatrix(&wcTransform->m[0][0]);

		gte_ldclmv(&rot.m[0][0]);
		gte_rtir();
		gte_stclmv(&scTransform);

		gte_ldclmv(&rot.m[0][1]);
		gte_rtir();
		gte_stclmv(&scTransform.m[0][1]);

		gte_ldclmv(&rot.m[0][2]);
		gte_rtir();
		gte_stclmv(&scTransform.m[0][2]);

		gte_SetTransVector(wcTransform->t);
		gte_ldlv0(&rot);
		gte_rt();
		gte_stlvnl(&rot);

		scale.z = (((playerInstance->object->modelList[playerInstance->currentModel]->maxRad) << 12) / 480 * (4096 - ((playerInstance->position.z - playerInstance->shadowPosition.z) << 12) / 1280)) >> 12;
		scale.y = scale.z;
		scale.x = scale.z;
		ScaleMatrix(&scTransform, (VECTOR*)&scale);
		SetRotMatrix(&scTransform);
		SetTransMatrix(&scTransform);
		primPool->nextPrim = (unsigned int*)DRAW_DrawShadow(primPool, 0, (unsigned long**)ot, playerInstance->fadeValue);
	}
}

void LIGHT_CalcShadowPositions(struct GameTracker* gameTracker)//Matching - 97.15%
{
	struct _InstanceList* instanceList;
	struct _Instance* instance;
	struct _PCollideInfo pcollideInfo;
	struct _Position newPos;
	struct _Position oldPos;
	struct Level* level;

	instanceList = gameTracker->instanceList;

	for (instance = instanceList->first; instance; instance = instance->next)
	{
		if ((instance->flags2 & 0x40) != 0)
		{
			if ((instance->flags & 0xA00) == 0x200 && !(instance->flags2 & 0x4000000))
			{
				if ((instance->flags & 0x18000000) != 0x8000000)
				{
					if ((instance->flags & 0x10000000))
					{
						newPos.x = (short)instance->matrix[1].t[0];
						oldPos.x = newPos.x;
						
						newPos.y = (short)instance->matrix[1].t[1];
						oldPos.y = newPos.y;
					
						newPos.z = (short)instance->matrix[1].t[2];
						oldPos.z = newPos.z;
					}
					else
					{
						newPos = instance->position;
						oldPos = instance->position;
					}
					
					pcollideInfo.collideType = 55;
					pcollideInfo.newPoint = (SVECTOR*)&newPos;
					pcollideInfo.oldPoint = (SVECTOR*)&oldPos;
					pcollideInfo.instance = instance;
					
					newPos.z -= 1280;
					oldPos.z += 256;
					
					instance->flags |= 0x40u;
					
					level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

					if (level)
					{
						COLLIDE_PointAndWorld(&pcollideInfo, level);
					}
					else
					{
						pcollideInfo.type = 0;
					}
					instance->flags &= ~0x40;
					if (pcollideInfo.type == 3)
					{
						LIGHT_CalcLightValue((struct _TFace*)pcollideInfo.prim, instance, (struct _Terrain*)pcollideInfo.inst->node.prev);
					}
					else if (pcollideInfo.type != 5)
					{
						LIGHT_CalcLightValue(NULL, instance, (struct _Terrain*)NULL);
					}

					if (pcollideInfo.type)
					{
						if (pcollideInfo.type != 1)
						{
							if (pcollideInfo.type == 3
								&& (((struct _TFace*)pcollideInfo.prim)->textoff != 0xFFFF)
								&& (*(unsigned short*)((char*)&pcollideInfo.inst->node.prev[6].next[1].prev + ((struct _TFace*)pcollideInfo.prim)->textoff + 2) & 0x4000))
							{
								instance->flags |= 0x200000;
							}
							else
							{
								instance->flags &= ~0x200000;
							}
							instance->wNormal.x = pcollideInfo.wNormal.vx;
							instance->wNormal.y = pcollideInfo.wNormal.vy;
							instance->wNormal.z = pcollideInfo.wNormal.vz;
						}
						else
						{
							instance->wNormal.x = 0;
							instance->wNormal.y = 0;
							instance->wNormal.z = 4096;
						}
					}
					instance->shadowPosition = newPos;
				}
				else
				{
					if (instance->waterFace != NULL)
					{
						LIGHT_CalcLightValue(instance->waterFace, instance, instance->waterFaceTerrain);
					}
					else if (instance->tfaceLevel != NULL)
					{
						LIGHT_CalcLightValue(instance->tface, instance, ((struct Level*)instance->tfaceLevel)->terrain);
					}
				}
			}
			else if (!(instance->flags2 & 0x40) && instance->tfaceLevel != NULL)
			{
				LIGHT_CalcLightValue(instance->tface, instance, ((struct Level*)instance->tfaceLevel)->terrain);
			}
			instance->flags &= ~0x8000000;

		}
	}
}

void LIGHT_Restore(struct LightInfo* lightInfo)
{
}

void LIGHT_CalcDQPTable(struct Level* level)
{
	long dqa;
	long limit;

	if (level->fogFar != level->fogNear)
	{
		dqa = -((level->fogFar * level->fogNear) / (level->fogFar - level->fogNear));
		limit = 40958;

		if (dqa > limit)
		{
			dqa = limit;

			if (level->holdFogNear == level->fogNear)
			{
				level->holdFogNear = (unsigned short)(limit * level->fogFar / (limit - level->fogFar));
			}

			level->fogNear = (unsigned short)(limit * level->fogFar / (limit - level->fogFar));
		}
		if (dqa < -limit)
		{
			dqa = -limit;

			if (level->holdFogNear == level->fogNear)
			{
				level->holdFogNear = (unsigned short)(-limit * level->fogFar / (-limit - level->fogFar));
			}

			level->fogNear = (unsigned short)(-limit * level->fogFar / (-limit - level->fogFar));
		}
		
		depthQFogStart = -4096 * dqa / ((level->fogFar << 12) / (level->fogFar - level->fogNear));
		
		if (level->backColorR && level->backColorG && level->backColorB)
		{
			depthQBlendStart = depthQFogStart;
		}
		else
		{
			depthQBlendStart = 0xFFFF;
		}

		level->depthQFogStart = depthQFogStart;
		level->depthQBlendStart = depthQBlendStart;
	}
}