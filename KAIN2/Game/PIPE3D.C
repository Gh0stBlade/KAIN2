#include "CORE.H"
#include "CAMERA.H"
#include "PIPE3D.H"
#include "GAMELOOP.H"
#include "MATH3D.H"
#include "STREAM.H"
#include "LIGHT3D.H"
#include "DRAW.H"
#include "PSX/COLLIDES.H"
#include "G2/MAING2.H"
#include "G2/QUATG2.H"

#if defined(EDITOR)
struct _Rotation overrideEditorRotation;
struct _Position overrideEditorPosition;
#endif

void PIPE3D_AspectAdjustMatrix(MATRIX* matrix) // Matching - 100%
{
	int temp, temp2, temp3;  // not from SYMDUMP

	temp = matrix->m[1][0];
	temp2 = matrix->m[1][1];
	temp3 = matrix->m[1][2];

	matrix->m[1][0] = temp;
	matrix->m[1][1] = temp2;
	matrix->m[1][2] = temp3;

	matrix->m[0][0] = (matrix->m[0][0] * 512) / 320;
	matrix->m[0][1] = (matrix->m[0][1] * 512) / 320;
	matrix->m[0][2] = (matrix->m[0][2] * 512) / 320;
}

void PIPE3D_CalculateWCTransform(struct _CameraCore_Type* cameraCore) // Matching - 97.74%
{
	MATRIX user_rotation;
	MATRIX first;
	MATRIX* cam_wcTrans;
	SVECTOR v0;
	VECTOR v1;

#if defined(EDITOR)
	cameraCore->rotation.x = overrideEditorRotation.x;
	cameraCore->rotation.y = overrideEditorRotation.y;
	cameraCore->rotation.z = overrideEditorRotation.z;

	cameraCore->position.x = overrideEditorPosition.x;
	cameraCore->position.y = overrideEditorPosition.y;
	cameraCore->position.z = overrideEditorPosition.z;
#endif

	cam_wcTrans = cameraCore->wcTransform;

	v0.vx = -cameraCore->position.x;
	v0.vy = -cameraCore->position.y;
	v0.vz = -cameraCore->position.z;

	if (!(gameTrackerX.debugFlags & 0x8))
	{
		MATH3D_SetUnityMatrix(&user_rotation);

		RotMatrixZ(-cameraCore->rotation.z, &user_rotation);
		RotMatrixX(-cameraCore->rotation.x, &user_rotation);
		RotMatrixY(-cameraCore->rotation.y, &user_rotation);

		v0.vx = -cameraCore->position.x;
		v0.vy = -cameraCore->position.y;
		v0.vz = -cameraCore->position.z;
	}
	else
	{
		MATH3D_SetUnityMatrix(&user_rotation);

		RotMatrixZ(-cameraCore->debugRot.z, &user_rotation);
		RotMatrixY(-cameraCore->debugRot.y, &user_rotation);
		RotMatrixX(-cameraCore->debugRot.x, &user_rotation);

		v0.vx = -cameraCore->debugPos.x;
		v0.vy = -cameraCore->debugPos.y;
		v0.vz = -cameraCore->debugPos.z;

		cameraCore->nondebugPos.x = cameraCore->position.x;
		cameraCore->nondebugPos.y = cameraCore->position.y;
		cameraCore->nondebugPos.z = cameraCore->position.z;

		cameraCore->position.x = cameraCore->debugPos.x;
		cameraCore->position.y = cameraCore->debugPos.y;
		cameraCore->position.z = cameraCore->debugPos.z;
	}

	first.m[0][0] = ONE;
	first.m[0][1] = 0;
	first.m[0][2] = 0;

	first.m[1][0] = 0;
	first.m[1][1] = 0;
	first.m[1][2] = -ONE;

	first.m[2][0] = 0;
	first.m[2][1] = ONE;
	first.m[2][2] = 0;

	MulMatrix0(&first, &user_rotation, cam_wcTrans);
	MulMatrix0(&first, &user_rotation, cameraCore->wcTransform2);

	PIPE3D_AspectAdjustMatrix(cam_wcTrans);

	cam_wcTrans->m[0][0] = (cam_wcTrans->m[0][0] * cameraCore->screenScale.x) >> 12;
	cam_wcTrans->m[0][1] = (cam_wcTrans->m[0][1] * cameraCore->screenScale.x) >> 12;
	cam_wcTrans->m[0][2] = (cam_wcTrans->m[0][2] * cameraCore->screenScale.x) >> 12;

	cam_wcTrans->m[1][0] = (cam_wcTrans->m[1][0] * cameraCore->screenScale.y) >> 12;
	cam_wcTrans->m[1][1] = (cam_wcTrans->m[1][1] * cameraCore->screenScale.y) >> 12;
	cam_wcTrans->m[1][2] = (cam_wcTrans->m[1][2] * cameraCore->screenScale.y) >> 12;

	cam_wcTrans->m[2][0] = (cam_wcTrans->m[2][0] * cameraCore->screenScale.z) >> 12;
	cam_wcTrans->m[2][1] = (cam_wcTrans->m[2][1] * cameraCore->screenScale.z) >> 12;
	cam_wcTrans->m[2][2] = (cam_wcTrans->m[2][2] * cameraCore->screenScale.z) >> 12;

	gte_SetRotMatrix(cam_wcTrans);
	gte_ldv0(&v0);
	gte_rtv0();
	gte_stlvnl(&v1);

	TransMatrix(cam_wcTrans, &v1);

	gte_SetRotMatrix(cameraCore->wcTransform2);
	gte_ldv0(&v0);
	gte_rtv0();
	gte_stlvnl(&v1);

	TransMatrix(cameraCore->wcTransform2, &v1);
}

void PIPE3D_InvertTransform(MATRIX *target, MATRIX *source)
{
	VECTOR sourceTrans;
	MATRIX normMat;

	if (source->m[2][0] == 1)
	{
		PIPE3D_NormalizeMatrix(target, &normMat);
		TransposeMatrix(&normMat, target);
	}
	else
	{
		TransposeMatrix(source, target);
	}

	sourceTrans.vx = -source->t[0];
	sourceTrans.vy = -source->t[1];
	sourceTrans.vz = -source->t[2];

	ApplyMatrixLV(target, &sourceTrans, (VECTOR*)target->t);
}

long PIPE3D_MatrixColumnLength(MATRIX* transform, long column) // Matching - 100%
{
	return MATH3D_FastSqrt0(MATH3D_SquareLength(transform->m[0][column], transform->m[1][column], transform->m[2][column]));
}

void PIPE3D_NormalizeMatrix(MATRIX* target, MATRIX* source) // Matching - 96.44%
{
	VECTOR scalevec;
	long scale;
	typedef struct
	{
		long m[3];
	} tmm;
	typedef struct
	{
		long m[5];
	} cmm;

	scale = PIPE3D_MatrixColumnLength(source, 0);

	if (scale != 0)
	{
		scale = 0x1000000 / scale;
	}

	scalevec.vx = scale;

	scale = PIPE3D_MatrixColumnLength(source, 1);

	if (scale != 0)
	{
		scale = 0x1000000 / scale;
	}

	scalevec.vy = scale;

	scale = PIPE3D_MatrixColumnLength(source, 2);

	if (scale != 0)
	{
		scale = 0x1000000 / scale;
	}

	scalevec.vz = scale;

	*(tmm*)(target->t) = *(tmm*)(source->t);
	*(cmm*)(target->m) = *(cmm*)(source->m);

	ScaleMatrix(target, &scalevec);
}

// autogenerated function stub: 
// void /*$ra*/ PIPE3D_TransformVerticesToWorld(struct _Instance *instance /*stack 0*/, struct _SVector *poolVertex /*$s2*/, long *vtxSegment /*$s5*/, struct _Vector *Average /*$fp*/)
void PIPE3D_TransformVerticesToWorld(struct _Instance *instance, struct _SVector *poolVertex, long *vtxSegment, struct _Vector *Average)
{ // line 753, offset 0x8003a7b0
	/* begin block 1 */
		// Start line: 754
		// Start offset: 0x8003A7B0
		// Variables:
			MATRIX *segMatrix; // $s1
			struct _Model *model; // $s7
			struct _MVertex *vertexList; // stack offset -48
			long i; // $s3
			struct _Segment *segment; // $v1
			struct _SVector *orgPoolVertex; // stack offset -44
			struct _SVector minV; // stack offset -64
			struct _SVector maxV; // stack offset -56

		/* begin block 1.1 */
			// Start line: 773
			// Start offset: 0x8003A890
			// Variables:
				struct _MVertex *firstVertex; // $s0
				struct _MVertex *lastVertex; // $s6
				struct _MVertex *modelVertex; // $s0
		/* end block 1.1 */
		// End offset: 0x8003A9DC
		// End Line: 815
	/* end block 1 */
	// End offset: 0x8003AAAC
	// End Line: 840

	/* begin block 2 */
		// Start line: 1506
	/* end block 2 */
	// End Line: 1507
				UNIMPLEMENTED();
}

void PIPE3D_InstanceTransformAndDraw(struct _Instance* instance, struct _CameraCore_Type* cameraCore, struct _VertexPool* vertexPool, struct _PrimPool* primPool, unsigned int** ot, struct _Mirror* mirror)
{
	struct Object* object;
	struct _Model* model;
	MATRIX* matrixPool;
	MATRIX lm;
	long flags;
	_MVertex* vertexList;
	struct _PVertex* poolVertex;
	CVECTOR* vertexColor;
	long spadOffset;
	long spadFree;
	long allocSize;
	long BackColorSave;
	long BlendStartSave;
	int pval;

	object = instance->object;
	
	matrixPool = instance->matrix;

	model = object->modelList[instance->currentModel];

	if (matrixPool != NULL)
	{
		LIGHT_PresetInstanceLight(instance, 2048, &lm);

		poolVertex = (struct _PVertex*)&vertexPool->vertex[0];
		
		vertexColor = (CVECTOR*)&vertexPool->color[0];

		spadFree = 224;
		
		vertexList = model->vertexList;

		spadOffset = 32;
		
		if (spadFree >= model->numVertices * 4)
		{
			poolVertex = (struct _PVertex*)getScratchAddr(32);
			spadOffset += model->numVertices * 4 + 32;
			spadFree -= model->numVertices * 4 + 32;
		}

		if (spadFree >= model->numVertices)
		{
			vertexColor = (CVECTOR*)getScratchAddr(spadOffset);
		}

		modelFadeValue = INSTANCE_GetFadeValue(instance);

		flags = PIPE3D_TransformAnimatedInstanceVertices_S((_VertexPool*)vertexList, poolVertex, model, cameraCore->wcTransform, matrixPool, &lm, vertexColor, instance->perVertexColor);

		LIGHT_PresetInstanceLight(instance, 4096, &lm);

		MulMatrix0(&lm, matrixPool + instance->lightMatrix, &lm);

		SetLightMatrix(&lm);

		if ((flags & 0x8000))
		{
			flags &= 0x7FFF6FFF;
		}
		
		object = instance->object;

		if (!(flags & 0xFFFFEFFF) || !(object->oflags2 & 0x2000))
		{
			BlendStartSave = 0;

			if (primPool->nextPrim + (model->numFaces * 12) < primPool->lastPrim)
			{
				object = instance->object;

				BackColorSave = 0;

				if (!(object->oflags2 & 0x1000))
				{
					SetRotMatrix(theCamera.core.wcTransform);
					
					SetTransMatrix(theCamera.core.wcTransform);

					gte_ldv0(&instance->position);

					gte_rtps();
				}

				gte_stdp(pval);

				if (instance->petrifyValue != 0)
				{
					BlendStartSave = depthQBlendStart;

					depthQBackColor = 0x707070;
					
					depthQBlendStart = depthQFogStart;
					
					BackColorSave = depthQBackColor;

					LoadAverageCol((unsigned char*)&BackColorSave, (unsigned char*)&depthQBackColor, pval, ONE - pval, (unsigned char*)&depthQBackColor);
				
					if (instance->petrifyValue < pval)
					{
						gte_lddp(pval);
					}
					else
					{
						gte_lddp(pval);
					}
				}

				if (modelFadeValue < 4094)
				{
					object = instance->object;

					if ((object->oflags2 & 0x1000) || pval < 4090)
					{
						primPool->nextPrim = (unsigned int*)gameTrackerX.drawAnimatedModelFunc(model, poolVertex, primPool, ot, vertexColor);
					}
				}

				if (instance->petrifyValue != 0)
				{
					depthQBlendStart = BlendStartSave;

					depthQBackColor = BackColorSave;
				}
			}
		}
	}
}

void PIPE3D_InstanceListTransformAndDrawFunc(struct _StreamUnit* unit, unsigned int** ot, struct _CameraCore_Type* cameraCore, struct _Instance* instance)
{
	struct _VertexPool* vertexPool;
	struct _PrimPool* primPool;
	VECTOR dpv[2];
	long maxRad;
	struct Level* level;
	SVECTOR bsPos;

	level = unit->level;

	vertexPool = gameTrackerX.vertexPool;

	primPool = gameTrackerX.primPool;

	bsPos.vx = instance->position.x;
	bsPos.vy = instance->position.y;
	bsPos.vz = instance->position.z;

	if (unit == NULL || !(unit->flags & 0x1) || unit->StreamUnitID != gameTrackerX.StreamUnitID || WARPGATE_IsObjectOnWarpSide(instance))
	{
		maxRad = instance->object->modelList[instance->currentModel]->maxRad;

		gte_SetRotMatrix(&cameraCore->vvNormalWorVecMat[0]);
		gte_ldv0(&bsPos);
		gte_rtv0();
		gte_stlvnl(&dpv[0]);

		dpv[0].vx -= cameraCore->vvPlaneConsts[0];

		if (-maxRad < dpv[0].vx && 
			dpv[0].vx < cameraCore->farPlane + maxRad &&
			-maxRad < dpv[0].vy - cameraCore->vvPlaneConsts[1] &&
			-maxRad < dpv[0].vz - cameraCore->vvPlaneConsts[2])
		{
			gte_SetRotMatrix(&cameraCore->vvNormalWorVecMat[1]);

			gte_ldv0(&bsPos);

			gte_rtv0();

			gte_stlvnl(&dpv[1]);

			if (-maxRad < dpv[1].vx - cameraCore->vvPlaneConsts[3] &&
				-maxRad < dpv[1].vy - cameraCore->vvPlaneConsts[4])
			{
				if ((instance->flags & 0x80))
				{
					PIPE3D_AnimateTextures(instance->object->modelList[instance->currentModel]->aniTextures, instance->currentTextureAnimFrame);
				}
				else
				{
					PIPE3D_AnimateTextures(instance->object->modelList[instance->currentModel]->aniTextures, gameTrackerX.frameCount);
					instance->currentTextureAnimFrame++;
				}

				LIGHT_SetMatrixForLightGroupInstance(instance, level);

				if (!(instance->halvePlane.flags & 0xB) || (instance->flags2 & 0x800000))
				{
					PIPE3D_InstanceTransformAndDraw(instance, cameraCore, vertexPool, primPool, ot, NULL);
				}
				else
				{
					PIPE3D_HalvePlaneInstanceTransformAndDraw(instance, cameraCore->wcTransform, vertexPool, primPool, ot, NULL);
				}

				if ((instance->flags2 & 0x40))
				{
					LIGHT_DrawShadow(cameraCore->wcTransform, instance, primPool, ot);
				}

				if (instance->additionalDrawFunc != NULL)
				{
					instance->additionalDrawFunc(cameraCore->wcTransform, instance, vertexPool, primPool, ot);
				}

				gameTrackerX.visibleInstances++;

				instance->flags |= 0x200;
			}
			else
			{
				instance->flags &= 0xFFFFFDFF;

			}
		}
		else
		{
			instance->flags &= 0xFFFFFDFF;
		}
	}
	else
	{
		instance->flags &= 0xFFFFFDFF;
	}
}

void PIPE3D_InstanceListTransformAndDraw(struct _StreamUnit* unit, struct GameTracker* gameTracker, unsigned int** ot, struct _CameraCore_Type* cameraCore)
{
	struct _Instance* instance;
	int id;
	struct _Instance* player;

	player = gameTracker->playerInstance;
	id = unit->StreamUnitID;
	instance = gameTracker->instanceList->first;

	if (player->currentStreamUnitID == id)
	{
		if (!(player->flags & 0x800))
		{
			PIPE3D_InstanceListTransformAndDrawFunc(unit, ot, cameraCore, player);
		}
	}

	while (instance != NULL)
	{
		if (!(instance->flags & 0x800) && !(instance->flags2 & 0x4000000))
		{
			if (instance->currentStreamUnitID == id && instance != player)
			{
				PIPE3D_InstanceListTransformAndDrawFunc(unit, ot, cameraCore, instance);
			}
		}

		instance = instance->next;
	}
}

void PIPE3D_TransformFromZAxis(MATRIX* transform, struct _SVector* normal) // Matching - 100%
{
	struct _G2EulerAngles_Type ea1;
	struct _SVector xprod;
	struct _SVector yprod;

	if (abs(normal->x) < 5) {
		if (abs(normal->y) < 5) {
			if (normal->z >= 0) {
				MATH3D_SetUnityMatrix(transform);
			}
			else
			{
				transform->m[0][0] = 4096;
				transform->m[0][1] = 0;
				transform->m[0][2] = 0;
				transform->m[1][0] = 0;
				transform->m[1][1] = -4096;
				transform->m[1][2] = 0;
				transform->m[2][0] = 0;
				transform->m[2][1] = 0;
				transform->m[2][2] = -4096;
			}
			return;
		}
	}

	//xprod.x = (short*)-normal->y;
	//xprod.y = (short*)normal->x;
	xprod.x = -normal->y;
	xprod.y = normal->x;
	xprod.z = 0;
	MATH3D_Normalize((struct _Normal*)&xprod);

	yprod.x = (normal->y * xprod.z - normal->z * xprod.y) >> 12;
	yprod.y = -((normal->x * xprod.z - normal->z * xprod.x) >> 12);
	yprod.z = (normal->x * xprod.y - normal->y * xprod.x) >> 12;
	MATH3D_Normalize((struct _Normal*)&yprod);

	transform->m[0][0] = xprod.x;
	transform->m[0][1] = xprod.y;
	transform->m[0][2] = xprod.z;
	transform->m[1][0] = yprod.x;
	transform->m[1][1] = yprod.y;
	transform->m[1][2] = yprod.z;
	transform->m[2][0] = normal->x;
	transform->m[2][1] = normal->y;
	transform->m[2][2] = normal->z;
	G2EulerAngles_FromMatrix(&ea1, (struct _G2Matrix_Type*)transform, 21);
	RotMatrix((SVECTOR*)&ea1, transform);
}

void PIPE3D_CalcWorldToSplitPlaneTransform(MATRIX* wpTransform, struct _SVector* normal, struct _SVector* translation)  // Matching - 100%
{
	struct _SVector svector;
	struct _Vector vector;

	PIPE3D_TransformFromZAxis(wpTransform, normal);
	svector.x = -translation->x;
	svector.y = -translation->y;
	svector.z = -translation->z;
	gte_SetRotMatrix(&wpTransform->m[0][0]);
	gte_ldv0(&svector.x);
	gte_rtv0();
	gte_stlvnl(&vector);
	wpTransform->t[0] = vector.x;
	wpTransform->t[1] = vector.y;
	wpTransform->t[2] = vector.z;
}


// autogenerated function stub: 
// long /*$ra*/ PIPE3D_TransformAnimatedSplitInstanceVertices(struct _MVertex *vertexList /*stack 0*/, struct _PVertex *poolVertex /*$s4*/, struct _Model *model /*stack 8*/, MATRIX *wcTransform /*stack 12*/, MATRIX *matrixPool /*stack 16*/, struct _Mirror *mirror /*stack 20*/, MATRIX *lm /*stack 24*/, struct CVECTOR *vertexColor /*stack 28*/, struct CVECTOR *vertexSrcCol /*stack 32*/)
long PIPE3D_TransformAnimatedSplitInstanceVertices(_MVertex *vertexList, struct _PVertex *poolVertex, struct _Model *model, MATRIX *wcTransform, MATRIX *matrixPool, struct _Mirror *mirror, MATRIX *lm, CVECTOR *vertexColor, CVECTOR *vertexSrcCol)
{ // line 1688, offset 0x8003b4dc
	/* begin block 1 */
		// Start line: 1689
		// Start offset: 0x8003B4DC
		// Variables:
			struct TransformAnimatedInstanceVerticesWork_t *w; // $s7
			MATRIX *segMatrix; // $s2
			long i; // stack offset -64
			struct _Segment *segment; // stack offset -60
			//struct CVECTOR defaultRGBCD; // stack offset -80

		/* begin block 1.1 */
			// Start line: 1710
			// Start offset: 0x8003B56C
		/* end block 1.1 */
		// End offset: 0x8003B570
		// End Line: 1712

		/* begin block 1.2 */
			// Start line: 1719
			// Start offset: 0x8003B5A8
			// Variables:
				struct _MVertex *firstVertex; // stack offset -56
				struct _MVertex *lastVertex; // $fp
				struct _MVertex *modelVertex; // $a2
				struct _Normal *n0; // $t1
				struct _Normal *n1; // $t2
				struct _Normal *n2; // $t3
				//struct CVECTOR white; // stack offset -72
				//struct CVECTOR *c0; // $s1
				//struct CVECTOR *c1; // $s6
				//struct CVECTOR *c2; // $s5
				//long vtxcolflgs; // stack offset -52
		/* end block 1.2 */
		// End offset: 0x8003BA44
		// End Line: 1879
	/* end block 1 */
	// End offset: 0x8003BA6C
	// End Line: 1883

	/* begin block 2 */
		// Start line: 3376
	/* end block 2 */
	// End Line: 3377
				UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ PIPE3D_TransformSplitInstanceVertices(struct _MVertex *vertexList /*stack 0*/, struct _PVertex *pvertex /*$s2*/, struct _Model *model /*stack 8*/, MATRIX *wpTransform /*stack 12*/, MATRIX *matrixPool /*stack 16*/, struct _Mirror *mirror /*stack 20*/)
void PIPE3D_TransformSplitInstanceVertices(_MVertex *vertexList, struct _PVertex *pvertex, struct _Model *model, MATRIX *wpTransform, MATRIX *matrixPool, struct _Mirror *mirror)
{ // line 1891, offset 0x8003baa8
	/* begin block 1 */
		// Start line: 1892
		// Start offset: 0x8003BAA8
		// Variables:
			MATRIX *spTransform; // $fp
			struct _Vector *vector; // $s1
			long i; // $s5
			struct _Segment *segment; // $v1

		/* begin block 1.1 */
			// Start line: 1904
			// Start offset: 0x8003BB30
			// Variables:
				struct _MVertex *firstVertex; // $s0
				struct _MVertex *lastVertex; // $s4
				struct _MVertex *modelVertex; // $s0
		/* end block 1.1 */
		// End offset: 0x8003BBC8
		// End Line: 1922
	/* end block 1 */
	// End offset: 0x8003BBEC
	// End Line: 1925

	/* begin block 2 */
		// Start line: 3663
	/* end block 2 */
	// End Line: 3664
				UNIMPLEMENTED();
}

void PIPE3D_AnimateTextures(struct AniTex* aniTextures, long req_frame)
{
	struct AniTexInfo* ani_tex_info; // $t0
	struct TextureMT3* dest; // $a0
	struct TextureMT3* src; // $v1
	long i; // $a3

	//t1 = aniTextures
	
	if (aniTextures != NULL && req_frame != -1)
	{
		ani_tex_info = &aniTextures->aniTexInfo;
		
		i = 0;
		if (aniTextures->numAniTextues > 0)
		{
			//a2 = &aniTextures->aniTexInfo.speed

		}
		//locret_8003C2C0
	}
	//locret_8003C2C0
#if 0
		addiu   $a2, $t1, 8

		loc_8003C250:
	lw      $v0, 4($a2)
		nop
		divu    $a1, $v0
		mflo    $v0
		lw      $v1, 0($a2)
		nop
		divu    $v0, $v1
		mfhi    $v1
		lw      $a0, 0($t0)
		sll     $v1, 4
		addu    $v1, $a0, $v1
		lw      $v0, 0x10($v1)
		addiu   $v1, 0x10
		sw      $v0, 0($a0)
		lw      $v0, 4($v1)
		nop
		sw      $v0, 4($a0)
		lw      $v0, 8($v1)
		addiu   $t0, 0xC
		sw      $v0, 8($a0)
		lw      $v0, 0xC($v1)
		addiu   $a3, 1
		sw      $v0, 0xC($a0)
		lw      $v0, 0($t1)
		nop
		slt     $v0, $a3, $v0
		bnez    $v0, loc_8003C250
		addiu   $a2, 0xC

		locret_8003C2C0:
	jr      $ra
		nop
#endif
}

void PIPE3D_AnimateTerrainTextures(struct DrMoveAniTex* aniTextures, long req_frame, struct _PrimPool* primPool, unsigned int** drawot)
{
	unsigned int* prim;
	struct DrMoveAniTexDestInfo* dest;
	struct DrMoveAniTexSrcInfo* src;
	long i;
	unsigned int** otl;

	prim = primPool->nextPrim;

#if defined(PSXPC_VERSION)
	otl = &drawot[3071 * 2];
#else
	otl = &drawot[3071];
#endif

	if (aniTextures != NULL)
	{
		if ((char*)prim < (char*)&primPool->lastPrim[-(aniTextures->numAniTextues * 40)])//Maybe add 4 to 36 for 32-bit addr
		{
			for (i = 0; i < aniTextures->numAniTextues; i++)
			{
				dest = ((struct DrMoveAniTex*)(((unsigned long*)aniTextures) + i))->aniTexInfo;

				src = &dest->frame + ((req_frame / dest->speed) % dest->numFrames);

				if (dest->pixCurrentX != src->pixSrcX || dest->pixCurrentY != src->pixSrcY)
				{
					prim[2] = 0x1000000;
					prim[3] = 0x80000000;
					prim[4] = ((int*)src)[0];
					prim[5] = ((int*)dest)[0];

					prim[6] = ((int*)dest)[1];

					setlen(prim, 5);
					addPrim(otl[0], prim);

					((int*)&dest->pixCurrentX)[0] = ((int*)&src->pixSrcX)[0];

					prim += (0x18 / 4) + 1;

					primPool->numPrims++;
				}

				if (src->clutSrcX != dest->clutCurrentX || src->clutSrcY != dest->clutCurrentY)
				{
					prim[2] = 0x1000000;
					prim[3] = 0x80000000;
					prim[4] = ((int*)&src->clutSrcX)[0];
					prim[5] = ((int*)&dest->clutDstX)[0];
					prim[6] = ((int*)&dest->clutW)[0];

					setlen(prim, 5);
					addPrim(otl[0], prim);

					prim += (0x18 / 4) + 1;

					((int*)&dest->clutCurrentX)[0] = ((int*)&src->clutSrcX)[0];

					primPool->numPrims++;
				}
			}
		}

		primPool->nextPrim = prim;
	}
}

void PIPE3D_HalvePlaneInstanceTransformAndDraw(struct _Instance* instance, MATRIX* wcTransform, struct _VertexPool* vertexPool, struct _PrimPool* primPool, unsigned int** ot, struct _Mirror* mirror) // Matching - 99.88%
{
	struct Object* object;
	struct _Model* model;
	MATRIX* matrixPool;
	MATRIX wpTransform;
	MATRIX pwTransform;
	MATRIX pcTransform;
	MATRIX lm;
	struct _MVertex* vertexList;
	struct _SVector normalX;
	struct _SVector* normal;
	struct _SVector translation;

	normal = &normalX;
	matrixPool = instance->matrix;
	model = instance->object->modelList[instance->currentModel];
	vertexList = model->vertexList;

	normal->x = instance->halvePlane.a;
	normal->y = instance->halvePlane.b;
	normal->z = instance->halvePlane.c;
	translation.x = (short)-((normal->x * instance->halvePlane.d) >> 12);
	translation.y = (short)-((normal->y * instance->halvePlane.d) >> 12);
	translation.z = (short)-((normal->z * instance->halvePlane.d) >> 12);

	PIPE3D_CalcWorldToSplitPlaneTransform(&wpTransform, normal, &translation);
	PIPE3D_InvertTransform(&pwTransform, &wpTransform);
	CompMatrix(wcTransform, &pwTransform, &pcTransform);

	if (matrixPool != 0)
	{
		LIGHT_PresetInstanceLight(instance, 2048, &lm);
		modelFadeValue = INSTANCE_GetFadeValue(instance);
		PIPE3D_TransformAnimatedSplitInstanceVertices(vertexList, vertexPool->vertex, model, &wpTransform, matrixPool, mirror, &lm, vertexPool->color, instance->perVertexColor);
		if ((&primPool->nextPrim[model->numFaces * 12]) < primPool->lastPrim)
		{
			if (instance->halvePlane.flags & 2)
			{
				draw_belowSplit = 1;
			}
			else
			{
				draw_belowSplit = 0;
			}
			primPool->nextPrim = DRAW_SplitModel_S(model, instance, vertexPool, &pcTransform, primPool, ot);
		}
	}
}

void PIPE3D_HalvePlaneGetRingPoints(struct _Instance* instance, MATRIX* wcTransform, struct _VertexPool* vertexPool, _PrimPool* primPool, unsigned long** ot, struct _FXHalvePlane* ring) // Matching - 99.96%
{
	struct Object* object;
	struct _Model* model;
	MATRIX* matrixPool;
	MATRIX wpTransform;
	MATRIX pwTransform;
	MATRIX pcTransform;
	struct _MVertex* vertexList;
	struct _PVertex* poolVertex;
	_SVector normalX;
	_SVector* normal;
	_SVector translation;
	struct _PlaneConstants* halvePlane;

	poolVertex = (struct _PVertex*)vertexPool;
	object = instance->object;
	normal = &normalX;

	matrixPool = instance->matrix;
	model = object->modelList[instance->currentModel];
	vertexList = model->vertexList;

	if (ring->type == 0)
	{
		halvePlane = &ring->ringPlane;
	}
	else
	{
		halvePlane = &instance->halvePlane;
	}

	normal->x = halvePlane->a;
	normal->y = halvePlane->b;
	normal->z = halvePlane->c;
	translation.x = (short)-((normal->x * halvePlane->d) >> 12);
	translation.y = (short)-((normal->y * halvePlane->d) >> 12);
	translation.z = (short)-((normal->z * halvePlane->d) >> 12);

	PIPE3D_CalcWorldToSplitPlaneTransform(&wpTransform, normal, &translation);
	PIPE3D_InvertTransform(&pwTransform, &wpTransform);

	if (ring->type == 2)
	{
		PIPE3D_InvertTransform(&pcTransform, &wpTransform);
	}
	else
	{
		CompMatrix(wcTransform, &pwTransform, &pcTransform);
	}

	if (matrixPool != 0)
	{
		PIPE3D_TransformSplitInstanceVertices(vertexList, poolVertex, model, &wpTransform, matrixPool, 0);
		draw_belowSplit = 0;
		primPool->nextPrim = (unsigned int*)DRAW_DrawRingPoints(model, vertexPool, &pcTransform, primPool, ot, ring->currentColor, ring->type);
	}
}

// autogenerated function stub: 
// void /*$ra*/ PIPE3D_DoGlow(struct _Instance *instance /*$s2*/, MATRIX *wcTransform /*$a1*/, struct _VertexPool *vertexPool /*$a2*/, struct _PrimPool *primPool /*$s3*/, unsigned long **ot /*stack 16*/, struct _FXGlowEffect *glow /*stack 20*/)
void PIPE3D_DoGlow(struct _Instance *instance, MATRIX *wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _FXGlowEffect *glow)
{ // line 2294, offset 0x8003c218
	/* begin block 1 */
		// Start line: 2295
		// Start offset: 0x8003C218
		// Variables:
			long currentColorID; // $a2
			long previousColorID; // $a1
			long value; // $a0
			long fade; // $a3
			long fadeflag; // $s0

		/* begin block 1.1 */
			// Start line: 2325
			// Start offset: 0x8003C400
			// Variables:
				long color; // stack offset -32

			/* begin block 1.1.1 */
				// Start line: 2370
				// Start offset: 0x8003C578
				// Variables:
					long i; // $s0
			/* end block 1.1.1 */
			// End offset: 0x8003C5D4
			// End Line: 2380
		/* end block 1.1 */
		// End offset: 0x8003C5D4
		// End Line: 2381
	/* end block 1 */
	// End offset: 0x8003C5D4
	// End Line: 2382

	/* begin block 2 */
		// Start line: 4588
	/* end block 2 */
	// End Line: 4589
					UNIMPLEMENTED();
}

long PIPE3D_Segment2ScreenPt(struct _Instance* instance, MATRIX* wcTransform, int segIndex, _Position* pos)
{
	MATRIX scTransform;
	struct _Position vOrigin;
	long face_z;

	CompMatrix(wcTransform, wcTransform + segIndex, &scTransform);

	SetRotMatrix(&scTransform);

	SetTransMatrix(&scTransform);

	vOrigin.x = 0;
	vOrigin.y = 0;
	vOrigin.z = 0;

	gte_ldv0(&vOrigin);

	gte_rtps();

	gte_stsxy(pos);
	gte_stsz(&face_z);

	return (face_z >> 2) - 20;
}
