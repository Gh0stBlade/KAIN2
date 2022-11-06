#include "Game/CORE.H"
#include "INSTNCG2.H"
#include "Game/G2/ANIMG2.H"
#include "Game/MATH3D.H"
#include "Game/CAMERA.H"

void G2Instance_BuildTransformsForList(struct _Instance* listHead)//Matching - 99.74%
{
	struct _Instance* instance;

	instance = listHead;

	while (instance != NULL)
	{
		if (instance->LinkParent == NULL)
		{
			if ((instance->flags2 & 0x1) || ((instance->flags & 0x100000) &&
				(instance->oldPos.x == instance->position.x) &&
				(instance->oldPos.y == instance->position.y) &&
				(instance->oldPos.z == instance->position.z) &&
				(instance->oldRotation.x == instance->rotation.x) &&
				(instance->oldRotation.y == instance->rotation.y) &&
				(instance->oldRotation.z == instance->rotation.z) &&
				(instance->matrix != NULL) && (((instance->object->animList == NULL)) ||
				(instance->object->oflags2 & 0x40000000) || !(instance->anim.flags & 0x1))))
			{

				_G2Instance_BuildDeactivatedTransforms(instance);
			}
			else
			{
				G2Instance_BuildTransforms(instance);
			}
		}

		instance = instance->next;
	}

	instance = listHead;

	while (instance != NULL)
	{
		if (instance->rebuildCallback != NULL)
		{
			if (instance->rebuildCallback(instance) != FALSE)
			{
				G2Anim_UpdateStoredFrame(&instance->anim);

				G2Instance_RebuildTransforms(instance);
			}
		}

		instance = instance->next;
	}
}

void G2Instance_BuildTransforms(struct _Instance* instance)
{
	if (instance->object->animList != NULL && !(instance->object->oflags2 & 0x40000000))
	{
		_G2Instance_BuildAnimatedTransforms(instance);
	}
	else
	{
		_G2Instance_BuildNonAnimatedTransforms(instance);
	}
}

void G2Instance_RebuildTransforms(struct _Instance *instance)
{
#if defined(PSX_VERSION)

	if (instance->object->animList != NULL && !(instance->object->oflags2 & 0x40000000))
	{
		_G2Instance_RebuildAnimatedTransforms(instance);
	}
	else
	{
		_G2Instance_RebuildNonAnimatedTransforms(instance);
	}

#elif defined(PC_VERSION)
	struct Object* object; // eax
  object = instance->object;
  if ( !object->animList || (object->oflags2 & 0x40000000) != 0 )
    G2Instance_RebuildNonAnimatedTransforms(instance);
  else
    G2Instance_RebuildAnimatedTransforms(instance);
#endif
}

struct _G2AnimKeylist_Type* G2Instance_GetKeylist(struct _Instance* instance, int id)
{
	return instance->object->animList[id];
}

void _G2Instance_RebuildAnimatedTransforms(struct _Instance* instance)
{
	struct _Model* model; // $s7
	struct _G2Matrix_Type* rootMatrix; // $s4
	struct _Rotation pre_facade_rot; // stack offset -120
	struct _G2Matrix_Type* segMatrix; // $a2
	struct _G2Matrix_Type seg1RotMatrix; // stack offset -112
	struct _G2Matrix_Type seg2RotMatrix; // stack offset -80
	struct _G2SVector3_Type rotVector; // stack offset -48
	long otx; // $s6
	long oty; // $s5
	long otz; // $s2
	long segIndex; // $s1
	VECTOR* ins_scale; // $v1
	
	//s3 = instance
	rootMatrix = (struct _G2Matrix_Type*)instance->matrix;

	if (rootMatrix != NULL)
	{
		//v1 = instance->currentModel
		//v0 = instance->object
		//a0 = instance->object->modelList
		//v0 = instance->object->oflags
		model = instance->object->modelList[instance->currentModel];
		rootMatrix--;

		if (instance->object->oflags & 0x1)
		{
			pre_facade_rot = instance->rotation;
			
			instance->rotation.x = 0;
			instance->rotation.y = 0;
			instance->rotation.z = MATH3D_FastAtan2(theCamera.core.position.y - instance->position.y, theCamera.core.position.x - instance->position.x) + 3072;
		}
		//loc_800951D4
	}
	//loc_8009552C
#if 0
		loc_800951D4:
		lw      $v0, 0x14($s3)
		nop
		andi    $v0, 1
		beqz    $v0, loc_80095248
		nop
		lw      $v0, 0x20($s3)
		nop
		beqz    $v0, loc_80095248
		lui     $v1, 0x1000
		lw      $v0, 0x38($v0)
		nop
		lw      $t0, 0x30($v0)
		lw      $t1, 0x34($v0)
		lw      $t2, 0x38($v0)
		lw      $t3, 0x3C($v0)
		sw      $t0, 0($s4)
		sw      $t1, 4($s4)
		sw      $t2, 8($s4)
		sw      $t3, 0xC($s4)
		lw      $t0, 0x40($v0)
		lw      $t1, 0x44($v0)
		lw      $t2, 0x48($v0)
		lw      $t3, 0x4C($v0)
		sw      $t0, 0x10($s4)
		sw      $t1, 0x14($s4)
		sw      $t2, 0x18($s4)
		sw      $t3, 0x1C($s4)
		j       loc_80095364
		nop

		loc_80095248 :
	lw      $v0, 0x148($s3)
		nop
		beqz    $v0, loc_80095354
		addiu   $s1, $s3, 0x1C8
		jal     sub_80092EDC
		move    $a0, $s1
		move    $a0, $s1
		li      $a1, 1
		addiu   $s0, $sp, 0x60 + var_8
		move    $a2, $s0
		jal     sub_80093204
		li      $a3, 7
		move    $a0, $s0
		addiu   $s2, $sp, 0x60 + var_48
		jal     sub_80078B60
		move    $a1, $s2
		move    $a0, $s1
		li      $a1, 2
		move    $a2, $s0
		jal     sub_80093204
		li      $a3, 7
		move    $a0, $s0
		addiu   $s0, $sp, 0x60 + var_28
		jal     sub_80078B60
		move    $a1, $s0
		move    $a0, $s2
		jal     MulMatrix2
		move    $a1, $s0
		move    $a0, $s0
		jal     TransposeMatrix
		move    $a1, $s4
		lw      $v0, 0x154($s3)
		lw      $v1, 0x148($s3)
		lw      $a0, 0x40($s3)
		lw      $v1, 0x40($v1)
		sll     $v0, 5
		addu    $v0, $v1
		lw      $t0, 0($v0)
		lw      $t1, 4($v0)
		lw      $t2, 8($v0)
		lw      $t3, 0xC($v0)
		sw      $t0, 0($a0)
		sw      $t1, 4($a0)
		sw      $t2, 8($a0)
		sw      $t3, 0xC($a0)
		lw      $t0, 0x10($v0)
		lw      $t1, 0x14($v0)
		lw      $t2, 0x18($v0)
		lw      $t3, 0x1C($v0)
		sw      $t0, 0x10($a0)
		sw      $t1, 0x14($a0)
		sw      $t2, 0x18($a0)
		sw      $t3, 0x1C($a0)
		lw      $a0, 0x40($s3)
		jal     MulMatrix2
		move    $a1, $s4
		lw      $v0, 0x40($s3)
		lw      $v1, 0x40($s3)
		lhu     $v0, 0x14($v0)
		nop
		sh      $v0, 0x5C($s3)
		lhu     $v0, 0x18($v1)
		nop
		sh      $v0, 0x5E($s3)
		lhu     $v0, 0x1C($v1)
		j       loc_80095360
		sh      $v0, 0x60($s3)

		loc_80095354:
	addiu   $a0, $s3, 0x74  # 't'
		jal     sub_80078CF4
		move    $a1, $s4

		loc_80095360 :
	lui     $v1, 0x1000

		loc_80095364 :
		lw      $v0, 0x84($s3)
		ori     $v1, 0x1000
		bne     $v0, $v1, loc_80095384
		lui     $v1, 0x1F80
		lh      $v1, 0x88($s3)
		li      $v0, 0x1000
		beq     $v1, $v0, loc_800953B8
		lui     $v1, 0x1F80

		loc_80095384:
	li      $v1, 0x1F800020
		lh      $v0, 0x84($s3)
		move    $a0, $s4
		sw      $v0, 0($v1)
		lh      $v0, 0x86($s3)
		lui     $a1, 0x1F80
		sw      $v0, 4($v1)
		lh      $v0, 0x88($s3)
		li      $a1, 0x1F800020
		jal     sub_80079258
		sw      $v0, 8($v1)
		li      $v0, 1
		sh      $v0, 0x12($s4)

		loc_800953B8:
	lh      $v0, 0x5C($s3)
		nop
		sw      $v0, 0x14($s4)
		lh      $v0, 0x5E($s3)
		nop
		sw      $v0, 0x18($s4)
		lh      $v0, 0x60($s3)
		nop
		sw      $v0, 0x1C($s4)
		lw      $v0, 0x40($s3)
		addiu   $a0, $s3, 0x1C8
		jal     sub_80092E10
		sw      $v0, 0x1DC($s3)
		lw      $v0, 0x148($s3)
		nop
		beqz    $v0, loc_800954AC
		nop
		lw      $a2, 0x40($s3)
		move    $s1, $zero
		lw      $a1, 0x14($a2)
		lw      $v1, 0x74($a2)
		lw      $a0, 0x18($a2)
		lw      $v0, 0x78($a2)
		subu    $s6, $a1, $v1
		subu    $s5, $a0, $v0
		lw      $a0, 0x1C($a2)
		lw      $v0, 0x7C($a2)
		lw      $v1, 0x18($s7)
		nop
		blez    $v1, loc_8009548C
		subu    $s2, $a0, $v0
		addiu   $s0, $a2, 0x1C

		loc_80095438:
	addiu   $a0, $s3, 0x1C8
		move    $a1, $s1
		jal     sub_80090794
		li      $a2, 0x20  # ' '
		bnez    $v0, loc_8009548C
		nop
		addiu   $s1, 1
		lw      $v0, -8($s0)
		lw      $v1, 0($s0)
		addu    $v0, $s6
		sw      $v0, -8($s0)
		lw      $v0, -4($s0)
		addu    $v1, $s2
		sw      $v1, 0($s0)
		addu    $v0, $s5
		sw      $v0, -4($s0)
		lw      $v0, 0x18($s7)
		nop
		slt     $v0, $s1, $v0
		bnez    $v0, loc_80095438
		addiu   $s0, 0x20  # ' '

		loc_8009548C:
	lw      $v0, 0x40($s3)
		nop
		lw      $t0, 0x14($v0)
		lw      $t1, 0x18($v0)
		lw      $t2, 0x1C($v0)
		sw      $t0, 0x14($s4)
		sw      $t1, 0x18($s4)
		sw      $t2, 0x1C($s4)

		loc_800954AC:
	lhu     $v0, 0x14($s4)
		lw      $v1, 0x1C($s3)
		sh      $v0, 0x5C($s3)
		lhu     $v0, 0x18($s4)
		nop
		sh      $v0, 0x5E($s3)
		lhu     $v0, 0x1C($s4)
		nop
		sh      $v0, 0x60($s3)
		lw      $v0, 0($v1)
		nop
		andi    $v0, 4
		beqz    $v0, loc_80095504
		nop
		ulw     $t0, 0x60 + var_50($sp)
		ulw     $t1, 0x60 + var_4C($sp)
		usw     $t0, 0x74($s3)
		usw     $t1, 0x78($s3)

		loc_80095504:
	lw      $s3, 0x14C($s3)
		nop
		beqz    $s3, loc_8009552C
		nop

		loc_80095514 :
	jal     sub_8009504C
		move    $a0, $s3
		lw      $s3, 0x150($s3)
		nop
		bnez    $s3, loc_80095514
		nop

		loc_8009552C :
	lw      $ra, 0x60 + var_s20($sp)
		lw      $s7, 0x60 + var_s1C($sp)
		lw      $s6, 0x60 + var_s18($sp)
		lw      $s5, 0x60 + var_s14($sp)
		lw      $s4, 0x60 + var_s10($sp)
		lw      $s3, 0x60 + var_sC($sp)
		lw      $s2, 0x60 + var_s8($sp)
		lw      $s1, 0x60 + var_s4($sp)
		lw      $s0, 0x60 + var_s0($sp)
		jr      $ra
		addiu   $sp, 0x88
#endif

}

void G2Instance_ClearMatrices(struct _Instance* instance)
{ 
	instance->oldMatrix = instance->matrix;

	instance->matrix = NULL;

	instance = instance->LinkChild;

	while (instance != NULL)
	{
		G2Instance_ClearMatrices(instance);

		instance = instance->LinkSibling;
	}
}

void _G2Instance_BuildAnimatedTransforms(struct _Instance* instance)
{
	MATRIX* rootMatrix;
	struct _Model* model;

	if ((instance->flags2 & 0x10000000) && (instance->flags2 & 0x4000000) || (instance->flags2 & 0x20000000) && (instance->flags & 0x800))
	{
		G2Instance_ClearMatrices(instance);
		
		return;
	}

	model = instance->object->modelList[instance->currentModel];

	rootMatrix = (MATRIX*)GAMELOOP_GetMatrices(model->numSegments + 1);

	if (rootMatrix == NULL)
	{
		instance->matrix = NULL;
		return;
	}

	instance->oldMatrix = instance->matrix;

	instance->matrix = rootMatrix + 1;

	_G2Instance_RebuildAnimatedTransforms(instance);
}

void _G2Instance_RebuildNonAnimatedTransforms(struct _Instance* instance)
{
	VECTOR* scale;
	MATRIX* introTransform;
	MATRIX* segMatrix;
	struct _Model* model;
	struct _Segment* segment;
	short scale_flag;
	long i;

	scale = (VECTOR*)getScratchAddr(10);
	segMatrix = instance->matrix;
	introTransform = (MATRIX*)getScratchAddr(14);
	scale_flag = 0;

	if (segMatrix != NULL)
	{
		model = instance->object->modelList[instance->currentModel];
		
		segment = model->segmentList;

		if (instance->scale.x != 4096 || instance->scale.y != 4096 || instance->scale.z != 4096)
		{
			scale->vx = instance->scale.x;
			scale->vy = instance->scale.y;
			scale->vz = instance->scale.z;

			scale_flag = 1;
		}
		
		for (i = 0; i < model->numSegments; i++, segMatrix++, segment++)
		{
			if (segment->lastTri != -1)
			{
				if ((segment->flags & 0x3))
				{
					RotMatrix((SVECTOR*)&instance->rotation, introTransform);

					if (scale_flag != 0)
					{
						ScaleMatrix(introTransform, scale);
					}

					_G2Instance_BuildFacadeTransforms(instance, segment, segMatrix, introTransform, scale_flag);
				}
				else
				{
					if ((instance->flags & 0x1) && instance->intro != NULL)
					{
						((long*)segMatrix)[0] = (long)instance->intro->data;
						((long*)segMatrix)[1] = (long)instance->intro->instance;
						((long*)segMatrix)[2] = (long)instance->intro->multiSpline;
						((long*)segMatrix)[3] = (long)instance->intro->dsignal;

						((long*)segMatrix)[4] = (long)((long*)&instance->intro->multiSpline->curRotMatrix)[4];
						((long*)segMatrix)[5] = (long)instance->intro->multiSpline->curRotMatrix.t[0];
						((long*)segMatrix)[6] = (long)instance->intro->multiSpline->curRotMatrix.t[1];
						((long*)segMatrix)[7] = (long)instance->intro->multiSpline->curRotMatrix.t[2];
					}
					else
					{
						if (instance->rotation.z != 0 || instance->rotation.x != 0 || instance->rotation.y != 0)
						{
							RotMatrix((SVECTOR*)&instance->rotation, segMatrix);
						}
						else
						{
							MATH3D_SetUnityMatrix(segMatrix);
						}
					}

					if (scale_flag != 0)
					{
						ScaleMatrix(segMatrix, scale);
					}
					
					segMatrix->t[0] = instance->position.x;
					segMatrix->t[1] = instance->position.y;
					segMatrix->t[2] = instance->position.z;
				}
			}

			((short*)segMatrix)[9] = scale_flag;
		}

		instance = instance->LinkChild;

		while (instance != NULL)
		{
			G2Instance_BuildTransforms(instance);
			instance = instance->LinkSibling;
		}
	}
}

void _G2Instance_BuildDeactivatedTransforms(struct _Instance* instance)
{ 
	MATRIX* segMatrix;
	MATRIX* startOldMatrix;
	int numMatrices;
	struct _Model* model;

	if ((instance->flags2 & 0x10000000) && ((instance->flags2 & 0x04000000) || ((instance->flags2 & 0x20000000) && (instance->flags & 0x800))))
	{
		G2Instance_ClearMatrices(instance);
		return;
	}

	if (instance->matrix == NULL)
	{
		G2Instance_BuildTransforms(instance);
		return;
	}
	
	model = instance->object->modelList[instance->currentModel];
	
	if (instance->object->animList != NULL && !(instance->object->oflags2 & 0x40000000))
	{
		numMatrices = model->numSegments + 1;
	}
	else
	{
		numMatrices = model->numSegments;
	}

	segMatrix = GAMELOOP_GetMatrices(numMatrices);

	if (segMatrix == NULL)
	{
		instance->matrix = NULL;
		return;
	}

	startOldMatrix = instance->matrix;

	instance->oldMatrix = startOldMatrix;

	if (instance->object->animList != NULL)
	{
		startOldMatrix--;
		
		if (!(instance->object->oflags2 & 0x40000000))
		{
			instance->matrix = segMatrix + 1;
		}
		else
		{
			startOldMatrix = instance->oldMatrix;

			instance->matrix = segMatrix;
		}
	}
	else
	{
		startOldMatrix = instance->oldMatrix;

		instance->matrix = segMatrix;
	}

	if (instance->oldMatrix != NULL)
	{
		memcpy(segMatrix, startOldMatrix, numMatrices + 5);///@FIXME bug/size error?
	}
	
	instance = instance->LinkChild;

	while (instance != NULL)
	{
		G2Instance_BuildTransforms(instance);

		instance = instance->LinkSibling;
	}
}

void _G2Instance_BuildNonAnimatedTransforms(struct _Instance* instance)
{
	MATRIX* segMatrix;
	struct _Model* model;

	if ((instance->flags2 & 0x10000000) && (instance->flags2 & 0x4000000) && (instance->flags2 & 0x20000000) && (instance->flags & 0x800))
	{
		G2Instance_ClearMatrices(instance);
	}
	else
	{
		model = instance->object->modelList[instance->currentModel];

		segMatrix = GAMELOOP_GetMatrices(model->numSegments);

		if (segMatrix == NULL)
		{
			instance->matrix = NULL;
		}
		else
		{
			instance->oldMatrix = instance->matrix;
			instance->matrix = segMatrix;

			_G2Instance_RebuildNonAnimatedTransforms(instance);
		}
	}
}


// autogenerated function stub: 
// void /*$ra*/ _G2Instance_BuildFacadeTransforms(struct _Instance *instance /*$s0*/, struct _Segment *segment /*$s1*/, MATRIX *segMatrix /*$s5*/, MATRIX *matrixPool /*$s6*/, long scale_flag /*stack 16*/)
void _G2Instance_BuildFacadeTransforms(struct _Instance *instance, struct _Segment *segment, MATRIX *segMatrix, MATRIX *matrixPool, long scale_flag)
{ // line 619, offset 0x800959f0
	/* begin block 1 */
		// Start line: 620
		// Start offset: 0x800959F0
		// Variables:
			_Position *cameraPos; // $s7
			SVECTOR*segmentPos; // $s3
			SVECTOR*segmentRot; // $s4
			//struct VECTOR *scale; // $fp

		/* begin block 1.1 */
			// Start line: 644
			// Start offset: 0x80095B1C
			// Variables:
				SVECTOR*zvec; // $s1
				SVECTOR*camWorldPos; // $s2
				SVECTOR*camLocPos; // $s6
				long sqrt; // $s0
		/* end block 1.1 */
		// End offset: 0x80095C38
		// End Line: 676

		/* begin block 1.2 */
			// Start line: 683
			// Start offset: 0x80095C98
			// Variables:
				//struct VECTOR *xy; // $s0
		/* end block 1.2 */
		// End offset: 0x80095D5C
		// End Line: 702
	/* end block 1 */
	// End offset: 0x80095D5C
	// End Line: 703

	/* begin block 2 */
		// Start line: 1488
	/* end block 2 */
	// End Line: 1489
				UNIMPLEMENTED();
}




