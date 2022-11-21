#include "Game/CORE.H"
#include "INSTNCG2.H"
#include "Game/G2/ANIMG2.H"
#include "Game/MATH3D.H"
#include "Game/CAMERA.H"
#include "Game/PSX/COLLIDES.H"
#include "Game/G2/ANMCTRLR.H"

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
	struct _Model* model;
	struct _G2Matrix_Type* rootMatrix;
	struct _Rotation pre_facade_rot;
	struct _G2Matrix_Type* segMatrix;
	struct _G2Matrix_Type seg1RotMatrix;
	struct _G2Matrix_Type seg2RotMatrix;
	struct _G2SVector3_Type rotVector;
	long otx;
	long oty;
	long otz;
	long segIndex;
	VECTOR* ins_scale;
	
	rootMatrix = (struct _G2Matrix_Type*)instance->matrix;

	if (rootMatrix != NULL)
	{
		model = instance->object->modelList[instance->currentModel];
		
		rootMatrix--;

		if (instance->object->oflags & 0x4)
		{
			pre_facade_rot = instance->rotation;
			
			instance->rotation.x = 0;
			instance->rotation.y = 0;
			instance->rotation.z = MATH3D_FastAtan2(theCamera.core.position.y - instance->position.y, theCamera.core.position.x - instance->position.x) + 3072;
		}

		if ((instance->flags & 0x1) && instance->intro != NULL)
		{
			rootMatrix->rotScale[0][0] = instance->intro->multiSpline->curRotMatrix.m[0][0];
			rootMatrix->rotScale[0][1] = instance->intro->multiSpline->curRotMatrix.m[0][1];
			rootMatrix->rotScale[0][2] = instance->intro->multiSpline->curRotMatrix.m[0][2];

			rootMatrix->rotScale[1][0] = instance->intro->multiSpline->curRotMatrix.m[1][0];
			rootMatrix->rotScale[1][1] = instance->intro->multiSpline->curRotMatrix.m[1][1];
			rootMatrix->rotScale[1][2] = instance->intro->multiSpline->curRotMatrix.m[1][2];

			rootMatrix->rotScale[2][0] = instance->intro->multiSpline->curRotMatrix.m[2][0];
			rootMatrix->rotScale[2][1] = instance->intro->multiSpline->curRotMatrix.m[2][1];
			rootMatrix->rotScale[2][2] = instance->intro->multiSpline->curRotMatrix.m[2][2];
			
			rootMatrix->trans.x = instance->intro->multiSpline->curRotMatrix.t[0];
			rootMatrix->trans.y = instance->intro->multiSpline->curRotMatrix.t[1];
			rootMatrix->trans.z = instance->intro->multiSpline->curRotMatrix.t[2];
		}
		else
		{
			if (instance->LinkParent != NULL)
			{
				G2Anim_UpdateStoredFrame(&instance->anim);

				G2Anim_GetSegChannelValue(&instance->anim, 1, (unsigned short*)&rotVector, 0x7);

				RotMatrixZYX((SVECTOR*)&rotVector, (MATRIX*)&seg1RotMatrix);

				G2Anim_GetSegChannelValue(&instance->anim, 2, (unsigned short*)&rotVector, 0x7);

				RotMatrixZYX((SVECTOR*)&rotVector, (MATRIX*)&seg2RotMatrix);

				MulMatrix2((MATRIX*)&seg1RotMatrix, (MATRIX*)&seg2RotMatrix);

				TransposeMatrix((MATRIX*)&seg2RotMatrix, (MATRIX*)rootMatrix);

				instance->matrix->m[0][0] = instance->LinkParent->matrix[instance->ParentLinkNode].m[0][0];
				instance->matrix->m[0][1] = instance->LinkParent->matrix[instance->ParentLinkNode].m[0][1];
				instance->matrix->m[0][2] = instance->LinkParent->matrix[instance->ParentLinkNode].m[0][2];

				instance->matrix->m[1][0] = instance->LinkParent->matrix[instance->ParentLinkNode].m[1][0];
				instance->matrix->m[1][1] = instance->LinkParent->matrix[instance->ParentLinkNode].m[1][1];
				instance->matrix->m[1][2] = instance->LinkParent->matrix[instance->ParentLinkNode].m[1][2];

				instance->matrix->m[2][0] = instance->LinkParent->matrix[instance->ParentLinkNode].m[2][0];
				instance->matrix->m[2][1] = instance->LinkParent->matrix[instance->ParentLinkNode].m[2][1];
				instance->matrix->m[2][2] = instance->LinkParent->matrix[instance->ParentLinkNode].m[2][2];

				instance->matrix->t[0] = instance->LinkParent->matrix[instance->ParentLinkNode].t[0];
				instance->matrix->t[1] = instance->LinkParent->matrix[instance->ParentLinkNode].t[1];
				instance->matrix->t[2] = instance->LinkParent->matrix[instance->ParentLinkNode].t[2];

				MulMatrix2(instance->matrix, (MATRIX*)rootMatrix);

				instance->position.x = ((unsigned short*)&instance->matrix->t[0])[0];
				instance->position.y = ((unsigned short*)&instance->matrix->t[0])[2];
				instance->position.z = ((unsigned short*)&instance->matrix->t[0])[4];
			}
			else
			{
				RotMatrix((SVECTOR*)&instance->rotation, (MATRIX*)rootMatrix);///@TODO don't use PsyQ (Valkyrie) version of this?
			}
		}

		if (instance->scale.x != 4096 || instance->scale.y != 4096 || instance->scale.z != 4096)
		{
			ins_scale = (VECTOR*)getScratchAddr(8);

			ins_scale->vx = instance->scale.x;
			ins_scale->vy = instance->scale.y;
			ins_scale->vz = instance->scale.z;

			ScaleMatrix((MATRIX*)rootMatrix, ins_scale);

			rootMatrix->scaleFlag = 1;
		}

		rootMatrix->trans.x = instance->position.x;
		rootMatrix->trans.y = instance->position.y;
		rootMatrix->trans.z = instance->position.z;

		instance->anim.segMatrices = (struct _G2Matrix_Type*)instance->matrix;

		G2Anim_BuildTransforms(&instance->anim);

		if (instance->LinkParent != NULL)
		{
			otx = instance->matrix[0].t[0] - instance->matrix[3].t[0];
			oty = instance->matrix[0].t[1] - instance->matrix[3].t[1];
			otz = instance->matrix[0].t[2] - instance->matrix[3].t[2];

			if (model->numSegments > 0)
			{
				for (segIndex = 0; segIndex < model->numSegments; segIndex++)
				{
					if ((G2Anim_IsControllerActive(&instance->anim, segIndex, 0x20)))
					{
						break;
					}

					instance->matrix[segIndex].t[0] += otx;
					instance->matrix[segIndex].t[1] += oty;
					instance->matrix[segIndex].t[2] += otz;
				}
			}

			rootMatrix->trans.x = instance->matrix[0].t[0];
			rootMatrix->trans.y = instance->matrix[0].t[1];
			rootMatrix->trans.z = instance->matrix[0].t[2];
		}

		instance->position.x = rootMatrix->trans.x;
		instance->position.y = rootMatrix->trans.y;
		instance->position.z = rootMatrix->trans.z;

		if ((instance->object->oflags & 0x4))
		{
			instance->rotation = pre_facade_rot;
		}
		
		instance = instance->LinkChild;

		while (instance != NULL)
		{
			G2Instance_BuildTransforms(instance);

			instance = instance->LinkSibling;
		}
	}
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

			segMatrix->m[2][3] = scale_flag;
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
		memcpy(segMatrix, startOldMatrix, numMatrices * sizeof(MATRIX));
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




