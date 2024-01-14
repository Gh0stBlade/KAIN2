#include "CORE.H"
#include "CAMERA.H"
#include "MATH3D.H"
#include "GAMELOOP.H"
#include "STREAM.H"
#include "LIGHT3D.H"
#include "GAMELOOP.H"
#include "COLLIDE.H"
#include "PSX/COLLIDES.H"
#include "RAZIEL/RAZLIB.H"
#include "MEMPACK.H"

short camera_still;
short shorten_flag;
short shorten_count;
short combat_cam_debounce;

struct _SVector camera_shakeOffset[16];
MATRIX wcTransformX;
MATRIX wcTransform2X;
MATRIX cwTransform2X;

long playerCameraMode = 13;
short Camera_lookHeight;
short Camera_lookDist;
int CameraCenterDelay;
short CenterFlag;
short combat_cam_distance;
long roll_target;
long current_roll_amount;
int roll_inc;
short combat_cam_angle;
short combat_cam_weight;
short panic_count;
struct _SVector camera_plane;
struct _SVector left_point;
struct _SVector right_point;
short hitline_rot;
long ACE_amount;

short CameraLookStickyFlag;

long cameraMode;

long camera_modeToIndex[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1,
	2,
	0,
};

static inline void CAMERA_Add_Vec_To_Pos(struct _Position* dest, struct _Position* pos, struct _Vector* vec)
{
	short x, y, z;

	x = pos->x;
	y = pos->y;
	z = pos->z;

	x += (short)vec->x;
	y += (short)vec->y;
	z += (short)vec->z;

	dest->x = x;
	dest->y = y;
	dest->z = z;
}

static inline void CAMERA_Sub_Vec_From_Pos(struct _Position* dest, struct _Position* pos, struct _Vector* vec)
{
	short x, y, z;

	x = pos->x;
	y = pos->y;
	z = pos->z;

	x -= (short)vec->x;
	y -= (short)vec->y;
	z -= (short)vec->z;

	dest->x = x;
	dest->y = y;
	dest->z = z;
}

static inline void CAMERA_Add_Pos_To_Vec(struct _Position* dest, struct _Vector* vec, struct _Position* pos)
{
	short x, y, z;

	x = (short)vec->x;
	y = (short)vec->y;
	z = (short)vec->z;

	x += pos->x;
	y += pos->y;
	z += pos->z;

	dest->x = x;
	dest->y = y;
	dest->z = z;
}

static inline void CAMERA_Sub_Pos_From_Vec(struct _Position* dest, struct _Vector* vec, struct _Position* pos)
{
	short x, y, z;

	x = (short)vec->x;
	y = (short)vec->y;
	z = (short)vec->z;

	x -= pos->x;
	y -= pos->y;
	z -= pos->z;

	dest->x = x;
	dest->y = y;
	dest->z = z;
}

static inline void CAMERA_Add_Pos_To_Pos(struct _Position* dest, struct _Position* pos0, struct _Position* pos1)
{
	short x, y, z;

	x = pos0->x;
	y = pos0->y;
	z = pos0->z;

	x += pos1->x;
	y += pos1->y;
	z += pos1->z;

	dest->x = x;
	dest->y = y;
	dest->z = z;
}

static inline void CAMERA_Sub_Pos_From_Pos(struct _Position* dest, struct _Position* pos0, struct _Position* pos1)
{
	short x, y, z;

	x = pos0->x;
	y = pos0->y;
	z = pos0->z;

	x -= pos1->x;
	y -= pos1->y;
	z -= pos1->z;

	dest->x = x;
	dest->y = y;
	dest->z = z;
}

static inline void CAMERA_Add_SVec_To_Pos(struct _SVector* dest, struct _Position* pos, struct _SVector* vec)
{
	short x, y, z;

	x = pos->x;
	y = pos->y;
	z = pos->z;

	x += vec->x;
	y += vec->y;
	z += vec->z;

	dest->x = x;
	dest->y = y;
	dest->z = z;
}

static inline void CAMERA_Sub_SVec_From_SVec(struct _SVector* dest, struct _SVector* vec0, struct _SVector* vec1)
{
	short x0, y0, z0;
	short x1, y1, z1;

	x0 = vec0->x;
	y0 = vec0->y;
	z0 = vec0->z;

	x1 = vec1->x;
	y1 = vec1->y;
	z1 = vec1->z;

	dest->x = x0 - x1;
	dest->y = y0 - y1;
	dest->z = z0 - z1;
}

static inline void CAMERA_Copy_Pos_To_SVec(struct _SVector* vec, struct _Position* pos)
{
	short x, y, z;

	x = pos->x;
	y = pos->y;
	z = pos->z;

	vec->x = x;
	vec->y = y;
	vec->z = z;
}

static inline void CAMERA_Copy_Vec_To_SVec(struct _SVector* SVec, struct _Vector* vec)
{
	short x, y, z;

	x = (short)vec->x;
	y = (short)vec->y;
	z = (short)vec->z;

	SVec->x = x;
	SVec->y = y;
	SVec->z = z;
}

void CAMERA_CalculateViewVolumeNormals(struct Camera *camera)
{
	short projDistance;
	struct _Normal n0;
	struct _Normal n1;
	struct _Normal n2;
	struct _Normal n3;
	int x1;
	int x2;
	int y1;
	int y2;

	x1 = ((unsigned int)camera->core.leftX - 160) << 4;
	projDistance = (short)(camera->core.projDistance << 4);
	x2 = ((unsigned int)camera->core.rightX - 160) << 4;
	y1 = ((unsigned int)camera->core.topY - 120) << 4;
	y2 = ((unsigned int)camera->core.bottomY - 120) << 4;

	n0.z = projDistance;
	n1.z = projDistance;
	n2.z = projDistance;
	n3.z = projDistance;

	n0.x = x1;
	n0.y = y1;

	n1.x = x2;
	n1.y = y1;

	n2.x = x1;
	n2.y = y2;

	n3.x = x2;
	n3.y = y2;

	camera->core.viewVolumeNormal[0].x = 0;
	camera->core.viewVolumeNormal[0].y = 0;
	camera->core.viewVolumeNormal[0].z = 4096;

	camera->core.viewVolumeNormal[1].x = (((int)(n0.y * n1.z) - (int)(n0.z * n1.y)) >> 12);
	camera->core.viewVolumeNormal[1].y = -(((int)(n0.x * n1.z) - (int)(n0.z * n1.x)) >> 12);
	camera->core.viewVolumeNormal[1].z = (((int)(n0.x * n1.y) - (int)(n0.y * n1.x)) >> 12);

	CAMERA_Normalize((_SVector*)&camera->core.viewVolumeNormal[1]);

	camera->core.viewVolumeNormal[2].x = (((int)(n2.y * n0.z) - (int)(n2.z * n0.y)) >> 12);
	camera->core.viewVolumeNormal[2].y = -(((int)(n2.x * n0.z) - (int)(n2.z * n0.x)) >> 12);
	camera->core.viewVolumeNormal[2].z = (((int)(n2.x * n0.y) - (int)(n2.y * n0.x)) >> 12);

	CAMERA_Normalize((_SVector*)&camera->core.viewVolumeNormal[2]);

	camera->core.viewVolumeNormal[3].x = (((int)(n1.y * n3.z) - (int)(n1.z * n3.y)) >> 12);
	camera->core.viewVolumeNormal[3].y = -(((int)(n1.x * n3.z) - (int)(n1.z * n3.x)) >> 12);
	camera->core.viewVolumeNormal[3].z = (((int)(n1.x * n3.y) - (int)(n1.y * n3.x)) >> 12);

	CAMERA_Normalize((_SVector*)&camera->core.viewVolumeNormal[3]);

	camera->core.viewVolumeNormal[4].x = (((int)(n3.y * n2.z) - (int)(n3.z * n2.y)) >> 12);
	camera->core.viewVolumeNormal[4].y = -(((int)(n3.x * n2.z) - (int)(n3.z * n2.x)) >> 12);
	camera->core.viewVolumeNormal[4].z = (((int)(n3.x * n2.y) - (int)(n3.y * n2.x)) >> 12);

	CAMERA_Normalize((_SVector*)&camera->core.viewVolumeNormal[4]);
}

void CAMERA_CalcVVClipInfo(struct Camera* camera)
{
	ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR*)&camera->core.viewVolumeNormal[0], (SVECTOR*)&camera->core.vvNormalWorVecMat[0].m[0][0]);
	ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR*)&camera->core.viewVolumeNormal[1], (SVECTOR*)&camera->core.vvNormalWorVecMat[0].m[1][0]);
	ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR*)&camera->core.viewVolumeNormal[2], (SVECTOR*)&camera->core.vvNormalWorVecMat[0].m[2][0]);
	ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR*)&camera->core.viewVolumeNormal[3], (SVECTOR*)&camera->core.vvNormalWorVecMat[1].m[0][0]);
	ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR*)&camera->core.viewVolumeNormal[4], (SVECTOR*)&camera->core.vvNormalWorVecMat[1].m[1][0]);

	gte_SetRotMatrix(&camera->core.vvNormalWorVecMat[0]);
	gte_ldv0(&camera->core.position);
	gte_mvmva(1, 0, 0, 3, 0);
	gte_stlvnl(&camera->core.vvPlaneConsts[0]);

	gte_SetRotMatrix(&camera->core.vvNormalWorVecMat[1]);
	gte_ldv0(&camera->core.position);
	gte_mvmva(1, 0, 0, 3, 0);
	gte_stlvnl(&camera->core.vvPlaneConsts[3]);
}

void CAMERA_SetViewVolume(struct Camera *camera)
{
	CAMERA_CalculateViewVolumeNormals(camera);
	CAMERA_CalcVVClipInfo(camera);
}

void CAMERA_SetProjDistance(struct Camera *camera, long distance)
{
	struct Level* level;
	int i;

	SetGeomScreen(distance);

	camera->core.projDistance = distance;

	CAMERA_CalculateViewVolumeNormals(camera);
	
	for (i = 0; i < 16; i++)
	{
		if (StreamTracker.StreamList[i].used == 2)
		{
			level = StreamTracker.StreamList[i].level;

			SetFogNearFar(level->fogNear, level->fogFar, camera->core.projDistance);

			LIGHT_CalcDQPTable(level);
		}
	}
}


// autogenerated function stub: 
// void /*$ra*/ CAMERA_CreateNewFocuspoint(struct Camera *camera /*$s0*/)
void CAMERA_CreateNewFocuspoint(struct Camera *camera)
{ // line 302, offset 0x80014c14
	/* begin block 1 */
		// Start line: 303
		// Start offset: 0x80014C14
		// Variables:
			struct _SVector sv; // stack offset -24

		/* begin block 1.1 */
			// Start line: 303
			// Start offset: 0x80014C14
			// Variables:
				short _x0; // $v1
				short _y0; // $a0
				short _z0; // $a1
				short _x1; // $v0
				short _y1; // $a2
				short _z1; // $a3
				struct _SVector *_v; // $v0
		/* end block 1.1 */
		// End offset: 0x80014C14
		// End Line: 303

		/* begin block 1.2 */
			// Start line: 303
			// Start offset: 0x80014C14
			// Variables:
				//short _x0; // $v0
				//short _y0; // $v0
				//short _z0; // $v1
				//_Position *_v; // $a2
		/* end block 1.2 */
		// End offset: 0x80014C14
		// End Line: 303
	/* end block 1 */
	// End offset: 0x80014C14
	// End Line: 303

	/* begin block 2 */
		// Start line: 644
	/* end block 2 */
	// End Line: 645
				UNIMPLEMENTED();
}

void CAMERA_SaveMode(struct Camera* camera, long mode)//Matching - 96.14%
{
	long i;

	if (++camera->stack >= 3)
	{
		camera->stack = 2;

		for (i = 0; i < 2; i++)
		{
			camera->savedMode[i] = camera->savedMode[i + 1];

			camera->savedCinematic[i] = camera->savedCinematic[i + 1];
		}
	}

	camera->savedMode[camera->stack] = mode;

	if ((mode == 2) || (mode == 5) || (mode == 4))
	{
		camera->savedCinematic[camera->stack].position = camera->core.position;
		camera->savedCinematic[camera->stack].focusPoint = camera->focusPoint;
		camera->savedCinematic[camera->stack].targetPos = camera->targetPos;
		camera->savedCinematic[camera->stack].targetFocusPoint = camera->targetFocusPoint;
		camera->savedCinematic[camera->stack].focusPointVel = camera->focusPointVel;
		camera->savedCinematic[camera->stack].focusPointAccl = camera->focusPointAccl;
		camera->savedCinematic[camera->stack].focusRotation = camera->focusRotation;
		camera->savedCinematic[camera->stack].targetFocusRotation = camera->targetFocusRotation;
		camera->savedCinematic[camera->stack].focusDistance = camera->focusDistance;
		camera->savedCinematic[camera->stack].posSpline = camera->data.Cinematic.posSpline;
		camera->savedCinematic[camera->stack].targetFocusDistance = camera->targetFocusDistance;
		camera->savedCinematic[camera->stack].maxVel = camera->maxVel;
		camera->savedCinematic[camera->stack].targetSpline = camera->data.Cinematic.targetSpline;
		camera->savedCinematic[camera->stack].level = gameTrackerX.level;
	}
}
void CAMERA_RestoreMode(struct Camera* camera)  // Matching - 99.64%
{
	long mode;
	short temp;  // not from SYMDUMP

	if (camera->stack >= 0)
	{
		mode = camera->savedMode[camera->stack];
		if (camera->mode == 5)
		{
			if (camera->smooth != 0)
			{
				camera->smooth = 8;
			}
			else
			{
				camera->flags |= 0x1000;
			}
		}
		switch (mode)
		{
		case 0:
		case 12:
		case 13:
		case 16:
			CAMERA_SetProjDistance(camera, 320);
			if (camera->mode == 5)
			{
				camera->focusInstance = gameTrackerX.playerInstance;
				camera->focusOffset.x = 0;
				camera->focusOffset.y = 0;
				camera->focusOffset.z = 352;
				CAMERA_Restore(camera, 7);
			}
			cameraMode = mode;
			if (mode == 12)
			{
				gameTrackerX.gameFlags &= ~64;
			}
			else
			{
				gameTrackerX.gameFlags |= 64;
			}
			camera->mode = (short)mode;
			camera->targetFocusDistance = (short)camera->focusDistanceList[camera_modeToIndex[mode << 16 >> 16]][camera->presetIndex];
			camera->data.Follow.stopTimer = 3852599296;
			camera->focusRotVel.z = 0;
			if (mode == 16)
			{
				camera->flags |= 0x2000;
			}
			else
			{
				camera->flags &= 0xFFFFDFFF;
			}
			break;
		case 2:
		case 4:
		case 5:
			CAMERA_SetProjDistance(camera, 320);
			camera->core.position = camera->savedCinematic[camera->stack].position;
			camera->focusPoint = camera->savedCinematic[camera->stack].focusPoint;
			camera->targetPos = camera->savedCinematic[camera->stack].targetPos;
			camera->targetFocusPoint = camera->savedCinematic[camera->stack].targetFocusPoint;
			camera->targetFocusDistance = camera->savedCinematic[camera->stack].targetFocusDistance;
			camera->targetFocusRotation = camera->savedCinematic[camera->stack].targetFocusRotation;
			if ((camera->smooth == 0) && (camera->mode != 6))
			{
				camera->focusDistance = camera->savedCinematic[camera->stack].focusDistance;
				camera->focusRotation = camera->savedCinematic[camera->stack].focusRotation;
			}
			else
			{
				camera->always_rotate_flag = 1;
			}
			camera->focusPointVel = camera->savedCinematic[camera->stack].focusPointVel;
			camera->focusPointAccl = camera->savedCinematic[camera->stack].focusPointAccl;
			camera->maxVel = (short)camera->savedCinematic[camera->stack].maxVel;
			camera->data.Cinematic.posSpline = camera->savedCinematic[camera->stack].posSpline;
			camera->data.Cinematic.targetSpline = camera->savedCinematic[camera->stack].targetSpline;
			camera->mode = (short)mode;
			if (INSTANCE_Query(camera->focusInstance, 9) & 80)
			{
				CAMERA_ChangeToUnderWater(camera, camera->focusInstance);
			}
			break;
		case 6:
			if ((((unsigned int)(unsigned short)camera->mode - 4) < 2) || (camera->mode == 2))
			{
				temp = (short)camera->savedTargetFocusDistance[camera->targetStack];
				camera->focusDistance = temp;
				camera->targetFocusDistance = temp;
				if (camera->targetStack >= 0)
				{
					camera->targetStack--;
				}
				camera->flags = (camera->flags | 0x800);
			}
			camera->lookTimer = 4;
			camera->mode = (short)mode;
			camera->targetFocusDistance = 2000;
			break;
		case 1:
		case 3:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 14:
		case 15:
		default:
			camera->mode = (short)mode;
		}
		camera->stack--;
	}
}


void CAMERA_Save(struct Camera* camera, long save)  // Matching - 100%
{
	int i;

	if (save & 1)
	{
		camera->targetStack++;
		if (camera->targetStack == 3)
		{
			camera->targetStack = 2;
			for (i = 0; i < 2; i++)
			{
				camera->savedTargetFocusDistance[i] = camera->savedTargetFocusDistance[i + 1];
			}
		}
		camera->savedTargetFocusDistance[camera->targetStack] = camera->targetFocusDistance;
	}
	if (save & 2)
	{
		camera->savedfocusRotation.x = camera->targetFocusRotation.x;
	}
	if (save & 4)
	{
		camera->savedfocusRotation.z = camera->focusRotation.z;
	}
	if (save & 0x100)
	{
		CAMERA_SaveMode(camera, camera->mode);
	}
}


void CAMERA_Restore(struct Camera* camera, long restore)  // Matching - 100%
{
	if (restore & 7)
	{
		if (restore & 1)
		{
			if (camera->targetStack >= 0)
			{
				camera->distanceState = 3;
				camera->signalFocusDistance = (short)camera->savedTargetFocusDistance[camera->targetStack];
				if (camera->targetStack >= 0)
				{
					camera->targetStack -= 1;
				}
			}
		}
		if (restore & 2)
		{
			camera->tiltState = 3;
			camera->signalRot.x = camera->savedfocusRotation.x;
		}
		if (restore & 4)
		{
			camera->rotState = 3;
			camera->forced_movement = 0;
			camera->always_rotate_flag = 1;
			camera->signalRot.z = camera->savedfocusRotation.z;
			camera->teleportZRot = camera->signalRot.z;
		}
	}
	if (restore & 0x100)
	{
		CAMERA_RestoreMode(camera);
	}
}

struct _SVector* SplineGetNextPointDC(struct Spline* spline, struct SplineDef* def)
{
	static struct _SVector point;

	if (SplineGetOffsetNext(spline, def, gameTrackerX.timeMult) != 0)
	{
		if (SplineGetData(spline, def, &point) != 0)
		{
			return &point;
		}
	}

	return NULL;
}

void CAMERA_SetMode(struct Camera* camera, long mode)
{
	int oldMode; // $s0
	struct SplineDef curPositional; // stack offset -56
	struct _SVector sv; // stack offset -48
	{ // line 47, offset 0x80015b10
		short _x1; // $v0
		short _y1; // $a0
		short _z1; // $a1
		struct _Rotation* _v0; // $v0
		struct _Rotation* _v1; // $v1
	} // line 47, offset 0x80015b10
	{ // line 47, offset 0x80015b10
		short _x1; // $v0
		short _y1; // $a0
		short _z1; // $v1
		struct _Rotation* _v0; // $v0
	} // line 47, offset 0x80015b10
	{ // line 47, offset 0x80015b10
		short _x0; // $v0
		short _y0; // $a1
		short _z0; // $v1
		short _x1; // $a2
		short _y1; // $a3
		short _z1; // $t0
		struct _SVector* _v; // $a0
		struct _Position* _v0; // $v1
	} // line 47, offset 0x80015b10
	{ // line 107, offset 0x80015c6c
		struct _SVector* camPos; // stack offset -32
		struct _SVector* targetPos; // $a0
		{ // line 112, offset 0x80015c80
			struct _Position pos; // stack offset -40
		} // line 114, offset 0x80015c80
		{ // line 128, offset 0x80015d2c
			short _x1; // $v0
			short _y1; // $v1
			short _z1; // $a0
			struct _Position* _v0; // $v0
		} // line 128, offset 0x80015d2c
		{ // line 138, offset 0x80015d80
			short _x1; // $v0
			short _y1; // $v1
			short _z1; // $a0
			struct _Position* _v0; // $v0
		} // line 138, offset 0x80015d9c
	} // line 142, offset 0x80015d9c

	//s2 = camera
	//s3 = mode
	oldMode = camera->mode;
	//v1 = camera->mode

	//v0 = camera->mode - 2

	if (oldMode == mode)
	{
		if (mode - 12 < 2)
		{
			return;
		}
	}
	//loc_80015A7C
	switch (camera->mode)
	{
	case 0:
	case 2:
	{
		//loc_80015AF0
		CAMERA_SaveMode(camera, camera->mode);
		//j loc_80015BE0
		break;
	}
	case 1:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	{
		//def_80015AA4
		break;
	}
	case 3:
	{
		//loc_80015B04
#if 0
		loc_80015B04:            # jumptable 80015AA4 case 3
			li      $v0, 5
			beq     $s3, $v0, loc_80015BE0
			sltiu   $v0, $s3, 0x11
			lh      $a1, 0xF0($s2)
			jal     sub_80015074
			move    $a0, $s2
			addiu   $v1, $s2, 0xB0
			lhu     $v0, 0xB0($s2)
			lhu     $a0, 2($v1)
			lhu     $a1, 4($v1)
			sh      $v0, 0x1B2($s2)
			addiu   $v0, $s2, 0x1B2
			sh      $a0, 2($v0)
			sh      $a1, 4($v0)
			lhu     $v0, 0xB0($s2)
			lhu     $a0, 2($v1)
			lhu     $v1, 4($v1)
			sh      $v0, 0x13C($s2)
			addiu   $v0, $s2, 0x13C
			sh      $a0, 2($v0)
			addiu   $a0, $sp, 0x30 + var_18
			sh      $v1, 4($v0)
			lhu     $v0, 0xB0($s2)
			lhu     $a2, 0($s2)
			lhu     $a3, 2($s2)
			lhu     $t0, 4($s2)
			addiu   $v1, $s2, 0x100
			sh      $v0, 0x4B2($s2)
			lhu     $v0, 0x100($s2)
			lhu     $a1, 2($v1)
			lhu     $v1, 4($v1)
			subu    $v0, $a2
			subu    $a1, $a3
			subu    $v1, $t0
			sh      $v0, 0x30 + var_18($sp)
			sh      $a1, 2($a0)
			jal     sub_80016270
			sh      $v1, 4($a0)
			sh      $v0, 0x106($s2)
			sw      $zero, -0x71B4($gp)
			sw      $zero, -0x71B0($gp)
			sw      $zero, -0x71AC($gp)
			j       loc_80015BE0
			sltiu   $v0, $s3, 0x11
#endif
		break;
	}
	case 4:
	{
		CAMERA_EndLook(camera);

		CAMERA_SaveMode(camera, camera->mode);

		if (mode == 5)
		{
			camera->focusRotation.z = camera->targetFocusRotation.z;

			CenterFlag = -1;

			CAMERA_Save(camera, 7);
		}

		//j loc_80015BE0
		break;
	}
	case 10:
	case 11:
	{
		//loc_80015BB4
#if 0

		loc_80015BB4:            # jumptable 80015AA4 cases 10, 11
			lh      $a1, 0xF0($s2)
			jal     sub_80015074
			move    $a0, $s2
			li      $v0, 5
			bne     $s3, $v0, loc_80015BD4
			move    $a0, $s2
			jal     sub_80015854
			li      $a1, 7

			loc_80015BD4:
		jal     sub_80014FD0
			move    $a0, $s2
#endif
		break;
	}
	}

	if (mode < 17)
	{
		switch (mode)
		{
		case 0:
		case 10:
		case 11:
		{
#if 0
			loc_80015EA4:            # jumptable 80015BFC cases 0, 10, 11
				lhu     $v0, 0x1A8($s2)
				nop
				sh      $v0, 0x1B0($s2)
				ulw     $t1, 0x1B2($s2)
				ulw     $t2, 0x1B6($s2)
				usw     $t1, 0x1BC($s2)
				usw     $t2, 0x1C0($s2)
#endif
			break;
		}
		case 12:
		case 13:
		case 16:

			CAMERA_SetProjDistance(camera, 320);

			if (mode == 16)
			{
				mode = 12;

				camera->flags |= 0x2000;
			}
			else
			{
				//loc_80015E08
				camera->flags &= 0xFFFFDFFF;
			}

			cameraMode = mode;

			gameTrackerX.gameFlags &= 0xFFFFFFBF;

			camera->mode = (short)mode;

			camera->smooth = 8;

			camera->data.Follow.stopTimer = 0xE5A20000;

			camera->focusRotVel.z = 0;

			camera->targetFocusDistance = (short)camera->focusDistanceList[camera_modeToIndex[mode]][camera->presetIndex];

			if (oldMode == 5)
			{
				if (camera->focusInstance != NULL)
				{
					CAMERA_SetFocus(camera, &camera->targetFocusPoint);
				}
			}
			//loc_80015EA4

			camera->collisionTargetFocusDistance = camera->targetFocusDistance;
			camera->collisionTargetFocusRotation = camera->targetFocusRotation;
			break;
		}
	}
	//def_80015BFC
#if 0

		loc_80015C04 : # jumptable 80015BFC cases 2, 4, 5
		move    $a0, $s2
		jal     sub_80014F2C
		li      $a1, 0x140
		lw      $v0, 0x41C($s2)
		nop
		sw      $v0, 0x424($s2)
		lw      $v0, 0x420($s2)
		lw      $v1, 0x424($s2)
		sh      $zero, 0x436($s2)
		sw      $zero, 0x41C($s2)
		sh      $zero, 0x430($s2)
		sh      $zero, 0x42E($s2)
		sh      $zero, 0x42C($s2)
		sw      $zero, 0x420($s2)
		beqz    $v1, loc_80015C48
		sw      $v0, 0x428($s2)
		sw      $zero, 0x10($v1)

		loc_80015C48:
	lw      $v0, 0x428($s2)
		nop
		beqz    $v0, loc_80015C5C
		nop
		sw      $zero, 0x10($v0)

		loc_80015C5C :
		lw      $a1, 0x424($s2)
		nop
		beqz    $a1, loc_80015D9C
		sh      $s3, 0xF0($s2)
		li      $v0, 4
		beq     $s3, $v0, loc_80015C80
		li      $v0, 2
		bne     $s3, $v0, loc_80015D0C
		nop

		loc_80015C80 :
	addiu   $s1, $sp, 0x30 + var_10
		move    $a0, $s1
		lhu     $v0, 0x13C($s2)
		addiu   $a1, $s2, 0x1AA
		sh      $v0, -0x537C($gp)
		lhu     $v0, 0x140($s2)
		addiu   $s0, $gp, -0x537C
		addiu   $v0, 0x800
		andi    $v0, 0xFFF
		sh      $v0, -0x5378($gp)
		lh      $a3, 0x1A8($s2)
		jal     sub_800169BC
		move    $a2, $s0
		lw      $v0, 0x424($s2)
		move    $a1, $s1
		lw      $a0, 0($v0)
		jal     sub_80041810
		addiu   $a2, $sp, 0x30 + var_20
		move    $a0, $s0
		lw      $a1, 0x108($s2)
		addiu   $a2, $sp, 0x30 + var_8
		sw      $v0, 0x30 + var_8($sp)
		jal     sub_80017B10
		addiu   $a1, 0x5C  # '\'
		lh      $v0, 0x30 + var_20($sp)
		lw      $v1, 0x424($s2)
		move    $a1, $s2
		sw      $v0, 0x43C($s2)
		lw      $a0, 0($v1)
		jal     sub_80041810
		addiu   $a2, $sp, 0x30 + var_20
		lh      $v1, 0x30 + var_20($sp)
		sw      $v0, 0x30 + var_8($sp)
		j       loc_80015D1C
		sw      $v1, 0x438($s2)

		loc_80015D0C:
	lw      $a0, 0($a1)
		jal     sub_80041738
		addiu   $a1, 0x10
		sw      $v0, 0x30 + var_8($sp)

		loc_80015D1C :
		lw      $a0, 0x30 + var_8($sp)
		nop
		beqz    $a0, loc_80015DA0
		li      $v0, 0xFFFFFFFF
		lhu     $v0, 0($a0)
		lhu     $v1, 2($a0)
		lhu     $a0, 4($a0)
		sh      $v0, 0x198($s2)
		addiu   $v0, $s2, 0x198
		sh      $v1, 2($v0)
		sh      $a0, 4($v0)
		li      $v0, 5
		bne     $s3, $v0, loc_80015D58
		li      $v0, 3
		sh      $v0, 0xF4($s2)

		loc_80015D58:
	lw      $a1, 0x428($s2)
		nop
		beqz    $a1, loc_80015DA0
		li      $v0, 0xFFFFFFFF
		lw      $a0, 0($a1)
		jal     sub_80041738
		addiu   $a1, 0x10
		move    $a0, $v0
		beqz    $a0, loc_80015DA0
		li      $v0, 0xFFFFFFFF
		lhu     $v0, 0($a0)
		lhu     $v1, 2($a0)
		lhu     $a0, 4($a0)
		sh      $v0, 0x1AA($s2)
		addiu   $v0, $s2, 0x1AA
		sh      $v1, 2($v0)
		sh      $a0, 4($v0)

		loc_80015D9C:
	li      $v0, 0xFFFFFFFF

		loc_80015DA0 :
		ulw     $t1, 0x13C($s2)
		ulw     $t2, 0x140($s2)
		usw     $t1, 0x1A0($s2)
		usw     $t2, 0x1A4($s2)
		sh      $v0, -0x5424($gp)
		lw      $v0, 0xE8($s2)
		li      $v1, 0xFFFFDFFF
		sh      $zero, 0xF8($s2)
		sh      $zero, 0x4BC($s2)
		and $v0, $v1
		j       loc_80015EA4     # jumptable 80015BFC cases 0, 10, 11
		sw      $v0, 0xE8($s2)

		

		def_80015BFC : # jumptable 80015BFC default case, cases 1, 3, 6 - 9, 14, 15
		sh      $s3, 0xF0($s2)

		

		loc_80015ED0:
	lw      $ra, 0x30 + var_s10($sp)
		lw      $s3, 0x30 + var_sC($sp)
		lw      $s2, 0x30 + var_s8($sp)
		lw      $s1, 0x30 + var_s4($sp)
		lw      $s0, 0x30 + var_s0($sp)
		jr      $ra
		addiu   $sp, 0x48
#endif
}

extern int rando();

void CAMERA_Initialize(struct Camera* camera)
{
	long i;
	long rand1;
	long rand2;

	memset(camera, 0, sizeof(struct Camera));
	//var_s2 = 0;

	for (i = 0; i < 16; i++)
	{
		rand1 = rand();
		rand2 = rand1;

		if (rand1 < 0)
		{
			rand2 = rand1 & 0xFF;
		}

		camera_shakeOffset[i].x = (short)((rand1 - ((rand2 >> 8) << 8)) - 128);

		rand1 = rand();
		rand2 = rand1;

		if (rand1 < 0)
		{
			rand2 = rand1 & 0xFF;
		}

		camera_shakeOffset[i].y = (short)((rand1 - ((rand2 >> 8) << 8)) - 128);

		rand1 = rand();
		rand2 = rand1;

		if (rand1 < 0)
		{
			rand2 = rand1 & 0xFF;
		}

		camera_shakeOffset[i].z = (short)((rand1 - ((rand2 >> 8) << 8)) - 128);
	}

	camera->core.rotation.x = 4039;
	camera->targetRotation.x = 4039;
	camera->focusRotation.x = 4039;
	camera->targetFocusRotation.x = 4039;

	camera->focusDistanceList[0][0] = 1500;
	camera->focusDistanceList[1][0] = 1500;
	camera->focusDistanceList[0][1] = 2250;
	camera->focusDistanceList[1][1] = 2000;
	camera->focusDistanceList[2][2] = 2000;

	camera->focusDistanceList[0][2] = 3200;
	camera->focusDistanceList[1][2] = 2600;
	camera->focusDistanceList[2][0] = 1200;
	camera->focusDistanceList[2][1] = 1600;

	camera->tiltList[0][0] = 4039;
	camera->tiltList[0][1] = 4039;
	camera->tiltList[0][2] = 4039;
	camera->tiltList[1][0] = 4039;
	camera->tiltList[1][1] = 4039;
	camera->tiltList[1][2] = 4039;
	camera->tiltList[2][0] = 4039;
	camera->tiltList[2][1] = 4039;
	camera->tiltList[2][2] = 4039;

	camera->smallBaseSphere.radiusSquared = 78400;
	camera->focusSphere.radiusSquared = 78400;
	camera->posSphere.radiusSquared = 78400;
	camera->smallBaseSphere.radius = 320;
	camera->focusSphere.radius = 320;
	camera->posSphere.radius = 320;
	camera->core.nearPlane = 50;
	camera->core.projDistance = 320;
	camera->targetFocusDistance = (short)camera->focusDistanceList[0][1];
	camera->focusDistance = (short)camera->focusDistanceList[0][1];
	camera->core.farPlane = 12000;
	camera->core.bottomY = 240;
	camera->core.wcTransform = &wcTransformX;
	camera->core.wcTransform2 = &wcTransform2X;
	camera->core.cwTransform2 = &cwTransform2X;
	camera->smooth = 16;
	camera->core.leftX = 0;
	camera->core.rightX = 320;
	camera->core.topY = 0;
	camera->maxVel = 200;
	camera->always_rotate_flag = 0;
	camera->follow_flag = 0;
	camera->real_focuspoint.x = camera->focusPoint.x;
	camera->real_focuspoint.y = camera->focusPoint.y;
	camera->real_focuspoint.z = camera->focusPoint.z;
	camera->minFocusDistance = 512;
	camera->Spline01 = NULL;
	camera->Spline00 = NULL;

	shorten_count = 0;
	shorten_flag = 0;

	camera->maxFocusDistance = 4096;

	camera->focuspoint_fallz = camera->focusPoint.z;

	camera->flags |= 0x800;

	if (camera->focusInstance != NULL)
	{
		CAMERA_EndLook(camera);
	}

	camera->presetIndex = 1;
	camera->mode = 0;

	CAMERA_SetMode(camera, playerCameraMode);

	camera->maxXYDist = 3000;
	camera->minXYDist = 0;
	camera->rotDirection = 1;

	camera->core.screenScale.z = 4096;
	camera->core.screenScale.y = 4096;
	camera->core.screenScale.x = 4096;

	camera->stack = -1;
	camera->targetStack = -1;

	camera->flags |= 0x8000;

	for (i = 0; i < 3; i++)
	{
		camera->savedMode[i] = 0;
	}

	camera->core.projDistance = 320;

	CAMERA_SetProjDistance(camera, 320);

	camera->data.Cinematic.cinema_done = 0;

	Camera_lookHeight = 512;
	Camera_lookDist = 650;
	CameraCenterDelay = 10;
	CenterFlag = -1;
	combat_cam_distance = 3000;
	roll_target = 0;
	current_roll_amount = 0;
	roll_inc = 0;
	combat_cam_angle = 0;
	combat_cam_weight = 4096;
	combat_cam_debounce = 0;
}

void CAMERA_SetInstanceFocus(struct Camera *camera, struct _Instance *instance)
{
	camera->focusInstance = instance;
	camera->flags |= 0x800;

	camera->newFocusInstancePos.x = instance->position.x;
	camera->newFocusInstancePos.y = instance->position.y;
	camera->newFocusInstancePos.z = instance->position.z;

	if (instance->object != NULL)
	{
		if (instance == gameTrackerX.playerInstance)
		{
			camera->focusOffset.z = 352;
		}
		else
		{
			camera->focusOffset.z = 512;
		}

		camera->focusOffset.x = 0;
		camera->focusOffset.y = 0;
	}
}

void CAMERA_SetZRotation(struct Camera* camera, short zrot)
{
	camera->core.rotation.z = zrot;
	camera->focusRotation.z = zrot;
	camera->targetRotation.z = zrot;
	camera->targetFocusRotation.z = zrot;
	camera->rotationAccl.z = 0;
	camera->rotationVel.z = 0;
	camera->rotationAccl.z = 0;
	camera->rotationVel.z = 0;
}

long CAMERA_LengthSVector(struct _SVector* sv)
{
	return MATH3D_FastSqrt0((sv->x * sv->x) + (sv->y * sv->y) + (sv->z * sv->z));
}

void CAMERA_SetValue(struct Camera* camera, long index, long value) // Matching - 99.91%
{
	long min;
	long max;
	long minTilt;
	long maxTilt;

	min = 0; // 0x0
	max = 16000; // 0x3E80;
	minTilt = -4096; // -0x1000;
	maxTilt = 4096; // 0x1000;

	(&camera->minFocusDistance)[index] = value;

	if (camera->cineControl != 0)
	{
		gameTrackerX.gameFlags |= 0x80;
	}
	else
	{
		gameTrackerX.gameFlags &= -0x81;
	}

	switch (camera->mode)
	{
	case 0:
		if (camera->focusDistanceList[0][0] < camera->focusDistanceList[0][1])
		{
			min = MIN(camera->focusDistanceList[0][0], camera->focusDistanceList[0][2]);
		}
		else
		{
			min = MIN(camera->focusDistanceList[0][1], camera->focusDistanceList[0][2]);
		}
		if (camera->focusDistanceList[0][0] > camera->focusDistanceList[0][1])
		{
			max = MAX(camera->focusDistanceList[0][0], camera->focusDistanceList[0][2]);
		}
		else
		{
			max = MAX(camera->focusDistanceList[0][1], camera->focusDistanceList[0][2]);
		}
		if (camera->tiltList[0][0] < camera->tiltList[0][1])
		{
			minTilt = MIN(camera->tiltList[0][0], camera->tiltList[0][2]);
		}
		else
		{
			minTilt = MIN(camera->tiltList[0][1], camera->tiltList[0][2]);
		}
		if (camera->tiltList[0][0] > camera->tiltList[0][1])
		{
			maxTilt = MAX(camera->tiltList[0][0], camera->tiltList[0][2]);
		}
		else
		{
			maxTilt = MAX(camera->tiltList[0][1], camera->tiltList[0][2]);
		}
		break;
	case 12:
		if (camera->focusDistanceList[1][0] < camera->focusDistanceList[1][1])
		{
			min = MIN(camera->focusDistanceList[1][0], camera->focusDistanceList[1][2]);
		}
		else
		{
			min = MIN(camera->focusDistanceList[1][1], camera->focusDistanceList[1][2]);
		}
		if (camera->focusDistanceList[1][0] > camera->focusDistanceList[1][1])
		{
			max = MAX(camera->focusDistanceList[1][0], camera->focusDistanceList[1][2]);
		}
		else
		{
			max = MAX(camera->focusDistanceList[1][1], camera->focusDistanceList[1][2]);
		}
		if (camera->tiltList[1][0] < camera->tiltList[1][1])
		{
			minTilt = MIN(camera->tiltList[1][0], camera->tiltList[1][2]);
		}
		else
		{
			minTilt = MIN(camera->tiltList[1][1], camera->tiltList[1][2]);
		}
		if (camera->tiltList[1][0] > camera->tiltList[1][1])
		{
			maxTilt = MAX(camera->tiltList[1][0], camera->tiltList[1][2]);
		}
		else
		{
			maxTilt = MAX(camera->tiltList[1][1], camera->tiltList[1][2]);
		}
		break;
	case 13:
		if (camera->focusDistanceList[2][0] < camera->focusDistanceList[2][1])
		{
			min = MIN(camera->focusDistanceList[2][0], camera->focusDistanceList[2][2]);
		}
		else
		{
			min = MIN(camera->focusDistanceList[2][1], camera->focusDistanceList[2][2]);
		}
		if (camera->focusDistanceList[2][0] > camera->focusDistanceList[2][1])
		{
			max = MAX(camera->focusDistanceList[2][0], camera->focusDistanceList[2][2]);
		}
		else
		{
			max = MAX(camera->focusDistanceList[2][1], camera->focusDistanceList[2][2]);
		}
		if (camera->tiltList[2][0] < camera->tiltList[2][1])
		{
			minTilt = MIN(camera->tiltList[2][0], camera->tiltList[2][2]);
		}
		else
		{
			minTilt = MIN(camera->tiltList[2][1], camera->tiltList[2][2]);
		}
		if (camera->tiltList[2][0] > camera->tiltList[2][1])
		{
			maxTilt = MAX(camera->tiltList[2][0], camera->tiltList[2][2]);
		}
		else
		{
			maxTilt = MAX(camera->tiltList[2][1], camera->tiltList[2][2]);
		}
		break;
	}

	if (camera->targetFocusDistance < min)
	{
		camera->targetFocusDistance = (short)min;
	}
	else if (camera->targetFocusDistance > max)
	{
		camera->targetFocusDistance = (short)max;
	}
	if (camera->targetFocusRotation.x < minTilt)
	{
		camera->targetFocusRotation.x = (short)minTilt;
	}
	else if (camera->targetFocusRotation.x > maxTilt)
	{
		camera->targetFocusRotation.x = (short)maxTilt;
	}
}

short CAMERA_AngleDifference(short angle0, short angle1)
{
	long temp;///@FIXME not in original, likely macro used for swap.
	angle0 &= 0xFFF;
	angle1 &= 0xFFF;

#define GET_ANGLE(x, y) ((x - y) > 2048) ? (x | 4096) : (x)
	
	temp = angle0;
	angle0 = (short)(GET_ANGLE(angle0, angle1) < angle0 ? angle0 : angle1);
	angle1 = (short)(GET_ANGLE(angle1, angle0) < angle1 ? angle1 : temp);

	return angle0 - angle1;
}

short CAMERA_SignedAngleDifference(short angle0, short angle1)
{ 
	return AngleDiff(angle1, angle0);
}

unsigned long CAMERA_QueryMode(struct Camera *camera)
{
	unsigned long mode;

	mode = INSTANCE_Query(camera->focusInstance, 10);

	if (camera->focusInstance == gameTrackerX.playerInstance)
	{
		if ((mode & 0x2000000))
		{
			combat_cam_debounce = 1;
		}
		else
		{
			if (combat_cam_debounce > 0)
			{
				combat_cam_debounce--;

				mode |= 0x2000000;
			}
		}
	}
	else
	{
		mode &= 0xFDFFFFFF;
	}

	if (WARPGATE_IsWarpgateActive() != 0)
	{
		mode |= 0x80000000;
	}

	return mode;
}

void CAMERA_SetMaxVel(struct Camera* camera)
{
	long extraVel;
	long targetMaxVel;
	struct _SVector cam_dist;
	static long maxVelAccl;
	static long maxVelVel;
	short _x0;
	short _y0;
	short _z0;
	short _x1;
	short _y1;
	short _z1;
	struct _SVector* _v;
	struct _Position* _v0;
	struct _Position* _v1;

	_v0 = &camera->focusPoint;
	_v1 = &camera->targetFocusPoint;

	_x0 = _v0->x;
	_y0 = _v0->y;
	_z0 = _v0->z;

	_x1 = _v1->x;
	_y1 = _v1->y;
	_z1 = _v1->z;

	_v = &cam_dist;

	_v->x = _x0 - _x1;
	_v->y = _y0 - _y1;
	_v->z = _z0 - _z1;

	extraVel = camera->focusDistance / 100;
	
	if (extraVel < 20)
	{
		extraVel = 20;
	}

	targetMaxVel = camera->forced_movement;

	if (targetMaxVel != 0)
	{
		extraVel += extraVel << 2;
	}

	targetMaxVel = ((short)CAMERA_LengthSVector(_v) + extraVel - camera->maxVel) >> 2;

	targetMaxVel = targetMaxVel - maxVelVel;

	maxVelVel += targetMaxVel;

	maxVelAccl = targetMaxVel;

	camera->maxVel += (short)maxVelVel;

	if (camera->maxVel <= 0)
	{
		camera->maxVel = 1;
	}
}

void CAMERA_SetTarget(struct Camera* camera, _Position* pos)//Matching - 99.68%
{
	short _x0;
	short _y0;
	short _z0;
	short _x1;
	short _y1;
	short _z1;
	struct _SVector sv;
	long len;
	struct _SVector* _v;

	CAMERA_CalcRotation(&camera->targetFocusRotation, pos, &camera->core.position);

	_v = &sv;

	_x0 = pos->x;
	_y0 = pos->y;
	_z0 = pos->z;

	_x1 = camera->core.position.x;
	_y1 = camera->core.position.y;
	_z1 = camera->core.position.z;


	_v->x = _x0 - _x1;
	_v->y = _y0 - _y1;
	_v->z = _z0 - _z1;

	len = CAMERA_LengthSVector(&sv);

	camera->focusDistance = (short)len;

	camera->targetFocusDistance = (short)len;

	camera->collisionTargetFocusDistance = (short)len;
}

void CAMERA_CalcPosition(_Position* position, _Position* base, struct _Rotation* rotation, short distance)
{
	SVECTOR sv;
	VECTOR v;
	MATRIX matrix;
	struct _Vector vectorPos;
	short _x1;
	short _y1;
	short _z1;
	struct _Vector* _v1;

	distance = -distance;

	sv.vx = 0;
	sv.vy = distance;
	sv.vz = 0;

	MATH3D_SetUnityMatrix(&matrix);

	RotMatrixX(rotation->x, &matrix);
	RotMatrixY(rotation->y, &matrix);
	RotMatrixZ(rotation->z, &matrix);

	gte_SetRotMatrix(&matrix);

	gte_ldv0(&sv);

	gte_rtv0();

	gte_stlvnl(&v);

	vectorPos.x = v.vx + base->x;
	vectorPos.y = v.vy + base->y;
	vectorPos.z = v.vz + base->z;

	_v1 = &vectorPos;

	_x1 = (short)_v1->x;
	_y1 = (short)_v1->y;
	_z1 = (short)_v1->z;

	position->x = _x1;
	position->y = _y1;
	position->z = _z1;
}

void CAMERA_SetFocus(struct Camera* camera, struct _Position* targetfocusPoint)
{
	struct _Instance* focusInstance;
	struct _Model* model;
	struct _SVector temp1;
	struct _SVector offset;
	struct _SVector* segPosVector;
	struct _Vector temp2;
	short _x1;
	short _y1;
	short _z1;
	struct _SVector* _v0;
	struct _Position* _v1;
	short _x0;
	short _y0;
	short _z0;
	struct _Instance* instance;
	struct _SVector output;

	focusInstance = camera->focusInstance;

	if ((camera->flags & 0x10000) || (camera->instance_mode & 0x4000000))
	{
		model = focusInstance->object->modelList[focusInstance->currentModel];

		_v0 = &temp1;

		segPosVector = (struct _SVector*)&model->segmentList[1].px;

		_x0 = segPosVector->x;
		_y0 = segPosVector->y;
		_z0 = segPosVector->z;

		_x1 = _v0->x;
		_y1 = _v0->y;
		_z1 = _v0->z;

		ApplyMatrix(focusInstance->matrix + 1, (SVECTOR*)_v0, (VECTOR*)&temp2);

		_v1 = &focusInstance->position;

		_x1 = _v1->x;
		_y1 = _v1->y;
		_z1 = _v1->z;

		targetfocusPoint->x = _x1;
		targetfocusPoint->y = _y1;
		targetfocusPoint->z = _z1;

		if ((camera->flags & 0x10000))
		{
			_x0 = targetfocusPoint->x;
			_y0 = targetfocusPoint->y;;
			_z0 = targetfocusPoint->z;

			_x1 = _v0->x;
			_y1 = _v0->y;
			_z1 = _v0->z;

			_x0 += _x1;
			_y0 += _y1;
			_z0 += _z1;

			targetfocusPoint->x = _x0;
			targetfocusPoint->y = _y0;
			targetfocusPoint->z = _z0;

			if ((INSTANCE_Query(focusInstance, 0x9) & 0x40))
			{
				targetfocusPoint->z += 192;
			}
		}

		camera->real_focuspoint = *targetfocusPoint;
	}
	else
	{
		if ((camera->instance_mode & 0x2000000))
		{
			_v1 = &focusInstance->position;

			if ((unsigned int)(camera->mode - 12) < 2)
			{
				if (INSTANCE_Query(focusInstance, 0x22) != 0)
				{
					_v0 = &output;

					LoadAverageShort12((SVECTOR*)&focusInstance->position, (SVECTOR*)&focusInstance->position, 4096 - combat_cam_weight, combat_cam_weight, (SVECTOR*)_v0);

					_x0 = _v0->x;
					_y0 = _v0->y;
					_z0 = _v0->z;

					targetfocusPoint->x = _x0;
					targetfocusPoint->y = _y0;
					targetfocusPoint->z = _z0;
				}
			}
		}

		_v1 = &focusInstance->position;
	}

	_x0 = _v1->x;
	_y0 = _v1->y;
	_z0 = _v1->z;

	targetfocusPoint->x = _x0;
	targetfocusPoint->y = _y0;
	targetfocusPoint->z = _z0;

	CAMERA_CalcFocusOffset(&offset, camera);

	_x0 = targetfocusPoint->x;
	_y0 = targetfocusPoint->y;
	_z0 = targetfocusPoint->z;

	_x1 = offset.x;
	_y1 = offset.y;
	_z1 = offset.z;

	targetfocusPoint->x = _x0 + _x1;
	targetfocusPoint->y = _y0 + _y1;
	targetfocusPoint->z = _z0 + _z1;

	camera->real_focuspoint = *targetfocusPoint;

	if (!(camera->instance_mode & 0x2038) || (camera->instance_mode & 0x2000) && camera->focusInstanceVelVec.z >= 71)
	{
		camera->focuspoint_fallz = targetfocusPoint->z;
	}
}

void CAMERA_Lock(struct Camera* camera, long lock)
{
	camera->lock |= lock;
}

void CAMERA_Unlock(struct Camera* camera, long unlock)
{
	camera->lock &= ~unlock;
}

void CAMERA_SetSmoothValue(struct Camera* camera, long smooth)//Matching - 99%
{
	camera->smooth = (short)smooth;
	if (smooth == 0)
	{
		if ((unsigned int)((unsigned short)camera->mode - 12) < 2 || camera->mode == 16)
		{
			camera->focusPoint = camera->targetFocusPoint;
		}
	}
}

void CAMERA_SetTimer(struct Camera* camera, long time)
{
	CAMERA_Save(camera, -1);
}

void CAMERA_Adjust_tilt(struct Camera* camera, long tilt)
{
	camera->tiltState = 3;
	
	tilt &= 0xFFF;
	
	camera->signalRot.x = (short)tilt;

	if (camera->smooth == 0)
	{
		camera->tilt = 0;

		camera->targetFocusRotation.x = (short)tilt;

		camera->targetRotation.x = (short)tilt;

		camera->tfaceTilt = (short)tilt;

		camera->focusRotation.x = (short)tilt;

		camera->actual_x_rot = (short)tilt;
	}
}

void CAMERA_Adjust_distance(struct Camera* camera, long dist)//Matching - 100%
{
	camera->signalFocusDistance = (short)dist;
	camera->distanceState = 3;

	if (camera->maxFocusDistance < (short)dist)
	{
		camera->signalFocusDistance = (short)camera->maxFocusDistance;
	}
	else
	{
		if ((short)dist < camera->minFocusDistance)
		{
			camera->signalFocusDistance = (short)camera->minFocusDistance;
		}
	}
	if (!camera->smooth)
	{
		camera->collisionTargetFocusDistance = camera->targetFocusDistance = camera->focusDistance = camera->signalFocusDistance;
	}
}

void CAMERA_Adjust_rotation(struct Camera* camera, long rotation)//Matching - 96.52%
{
	if (CAMERA_AngleDifference((short)rotation, camera->targetFocusRotation.z))
	{
		camera->rotState = 3;
		camera->forced_movement = 0;
		camera->signalRot.z = rotation & 0xFFF;

		if (!camera->smooth)
		{
			camera->collisionTargetFocusRotation.z = camera->teleportZRot = camera->focusRotation.z = camera->targetFocusRotation.z = rotation & 0xFFF;
		}

		camera->lastModTime = gameTrackerX.frameCount;
		camera->always_rotate_flag = 1;
	}
}


// autogenerated function stub: 
// void /*$ra*/ CAMERA_Adjust_roll(long roll_degrees /*$s0*/, int frames /*$s1*/)
void CAMERA_Adjust_roll(long roll_degrees, int frames)
{
	UNIMPLEMENTED();
}


void CAMERA_Adjust(struct Camera* camera, long adjust)  // Matching - 100%
{
	struct _SVector dv;
	short _x0;
	short _y0;
	short _z0;
	short _x1;
	short _y1;
	short _z1;
	struct _SVector* _v;
	struct _SVector* _v1;
	struct _CameraKey* temp;  // not from SYMDUMP

	temp = camera->cameraKey;
	if (temp != NULL)
	{
		if ((adjust & 1) != 0)
		{
			_v = (struct _SVector*)&temp->tx;
			_v1 = &dv;
			_x0 = temp->x;
			_y0 = temp->y;
			_z0 = temp->z;
			_x1 = _v->x;
			_y1 = _v->y;
			_z1 = _v->z;
			_v1->x = _x0 - _x1;
			_v1->y = _y0 - _y1;
			_v1->z = _z0 - _z1;
			CAMERA_Adjust_distance(camera, CAMERA_LengthSVector(_v1));
		}
		if ((adjust & 2) != 0)
		{
			CAMERA_Adjust_tilt(camera, temp->rx);
		}
		if ((adjust & 4) != 0)
		{
			CAMERA_Adjust_rotation(camera, temp->rz);
		}
	}
}

void CAMERA_ChangeTo(struct Camera* camera, struct _CameraKey* cameraKey)
{
	camera->cameraKey = cameraKey;
}


// autogenerated function stub: 
// void /*$ra*/ CAMERA_SetShake(struct Camera *camera /*$t0*/, long shake /*$a1*/, long scale /*$a2*/)
void CAMERA_SetShake(struct Camera *camera, long shake, long scale)
{ // line 1595, offset 0x80016cc4
	/* begin block 1 */
		// Start line: 1596
		// Start offset: 0x80016CC4
		// Variables:
			int shock; // $a0
			int duration; // $a1
	/* end block 1 */
	// End offset: 0x80016D5C
	// End Line: 1615

	/* begin block 2 */
		// Start line: 3725
	/* end block 2 */
	// End Line: 3726
			UNIMPLEMENTED();
}

void Decouple_AngleMoveToward(short* current_ptr, short destination, short step)
{
	if (gameTrackerX.timeMult != 4096)
	{
		step = (short)((step * gameTrackerX.timeMult) >> 12);
	}

	AngleMoveToward(current_ptr, destination, step);
}

void CriticalDampValue(long dampMode, short *sourceVal, short targetVal, short *vel, short *accl, int smooth)
{
	short maxVel;
	short useVel;

#if !defined(UWP)
	
	if (smooth != 0)
	{
		maxVel = smooth;

		if (smooth < 0)
		{
			maxVel = -4096 / smooth;
		}

		if (dampMode == 1)
		{
			if (dampMode == 5)
			{
				accl[0] = ((targetVal - sourceVal[0]) >> 1) - vel[0];

				useVel = vel[0] + accl[0];

				vel[0] = useVel;

				if (maxVel < useVel)
				{
					vel[0] = maxVel;

					if (gameTrackerX.timeMult != 4096)
					{
						useVel = (short)((maxVel * gameTrackerX.timeMult) >> 12);
					}
				}
				else
				{
					if (useVel < -maxVel)
					{
						useVel = -maxVel;

						vel[0] = useVel;

						if (gameTrackerX.timeMult != 4096)
						{
							useVel = -(short)((maxVel * gameTrackerX.timeMult) >> 12);
						}
					}
				}

				sourceVal[0] += useVel;
			}
			else if (dampMode == 6)
			{
				accl[0] = ((targetVal - sourceVal[0]) >> 3) - vel[0];

				useVel = vel[0] + accl[0];

				vel[0] = useVel;


				if (maxVel < useVel)
				{
					vel[0] = maxVel;

					if (gameTrackerX.timeMult != 4096)
					{
						useVel = (short)((maxVel * gameTrackerX.timeMult) >> 12);
					}
				}
				else
				{
					if (useVel < -maxVel)
					{
						useVel = -maxVel;

						vel[0] = useVel;

						if (gameTrackerX.timeMult != 4096)
						{
							useVel = -(short)((maxVel * gameTrackerX.timeMult) >> 12);
						}
					}
				}

				sourceVal[0] += useVel;
			}
			else
			{
				accl[0] = ((targetVal - sourceVal[0]) >> 2) - vel[0];

				useVel = vel[0] + accl[0];

				vel[0] = useVel;

				if (maxVel < useVel)
				{
					vel[0] = maxVel;

					if (gameTrackerX.timeMult != 4096)
					{
						useVel = (short)((maxVel * gameTrackerX.timeMult) >> 12);
					}
				}
				else
				{
					if (useVel < -maxVel)
					{
						useVel = -maxVel;

						vel[0] = useVel;

						if (gameTrackerX.timeMult != 4096)
						{
							useVel = -(short)((maxVel * gameTrackerX.timeMult) >> 12);
						}
					}
				}

				sourceVal[0] += useVel;
			}
		}
		else if (dampMode >= 2)
		{
			//loc_800171D4
			if (dampMode >= 7)
			{
				if (vel[0] == 0)
				{
					sourceVal[0] = targetVal;
				}
			}
			else if (dampMode >= 5)
			{
				if (dampMode == 5)
				{
					accl[0] = ((targetVal - sourceVal[0]) >> 1) - vel[0];
					
					useVel = vel[0] + accl[0];
					
					vel[0] = useVel;

					if (maxVel < useVel)
					{
						vel[0] = maxVel;

						if (gameTrackerX.timeMult != 4096)
						{
							useVel = (short)((maxVel * gameTrackerX.timeMult) >> 12);
						}
					}
					else
					{
						if (useVel < -maxVel)
						{
							useVel = -maxVel;

							vel[0] = useVel;

							if (gameTrackerX.timeMult != 4096)
							{
								useVel = -(short)((maxVel * gameTrackerX.timeMult) >> 12);
							}
						}
					}

					sourceVal[0] += useVel;
				}
				else if (dampMode == 6)
				{
					accl[0] = ((targetVal - sourceVal[0]) >> 3) - vel[0];
					
					useVel = vel[0] + accl[0];
					
					vel[0] = useVel;


					if (maxVel < useVel)
					{
						vel[0] = maxVel;

						if (gameTrackerX.timeMult != 4096)
						{
							useVel = (short)((maxVel * gameTrackerX.timeMult) >> 12);
						}
					}
					else
					{
						if (useVel < -maxVel)
						{
							useVel = -maxVel;

							vel[0] = useVel;

							if (gameTrackerX.timeMult != 4096)
							{
								useVel = -(short)((maxVel * gameTrackerX.timeMult) >> 12);
							}
						}
					}

					sourceVal[0] += useVel;
				}
				else
				{
					accl[0] = ((targetVal - sourceVal[0]) >> 2) - vel[0];
					
					useVel = vel[0] + accl[0];
					
					vel[0] = useVel;
					
					if (maxVel < useVel)
					{
						vel[0] = maxVel;

						if (gameTrackerX.timeMult != 4096)
						{
							useVel = (short)((maxVel * gameTrackerX.timeMult) >> 12);
						}
					}
					else
					{
						if (useVel < -maxVel)
						{
							useVel = -maxVel;

							vel[0] = useVel;

							if (gameTrackerX.timeMult != 4096)
							{
								useVel = -(short)((maxVel * gameTrackerX.timeMult) >> 12);
							}
						}
					}

					sourceVal[0] += useVel;
				}
			}
		}
		else if (dampMode == 0)
		{
			accl[0] = ((targetVal - sourceVal[0]) >> 2) - vel[0];
			
			vel[0] += accl[0];
			
			sourceVal[0] += vel[0];
		}

		if (vel[0] == 0)
		{
			sourceVal[0] = targetVal;
		}

	}
	else
	{
		sourceVal[0] = targetVal;
	}
#endif
}

void CriticalDampPosition(long dampMode, struct _Position* position, struct _Position* targetPos, struct _SVector* vel, struct _SVector* accl, int smooth)
{
	long length; // $s0
	struct _Vector vector; // stack offset -48
	struct _SVector svector; // stack offset -32
	short maxVel; // $s4
	int shift; // $a3
	long _x0; // $v0
	long _y0; // $a2
	long _z0; // $a1
	long _x1; // $v1
	long _y1; // $v1
	long _z1; // $a0
	struct _Vector* _v; // $v1

	//t0 = accl
	//v1 = (short)smooth
	//v0 = smooth
	//s1 = position
	//s3 = targetPos
	//s2 = vel

	if ((short)smooth != 0)
	{
		maxVel = smooth;

		if ((short)smooth < 0)
		{
			maxVel = -ONE / (short)smooth;
		}
		//loc_80017388

		switch (dampMode)
		{
		case 0:
		{
#if 0
			loc_800173B0 : # jumptable 800173A8 case 0
				lh      $v0, 0($s3)
				lh      $a2, 2($s3)
				lh      $v1, 0($s1)
				lh      $a1, 4($s3)
				lh      $a0, 4($s1)
				subu    $v0, $v1
				lh      $v1, 2($s1)
				subu    $a1, $a0
				sw      $v0, 0x28 + var_18($sp)
				subu    $a2, $v1
				addiu   $v1, $sp, 0x28 + var_18
				sw      $a2, 4($v1)
				sw      $a1, 8($v1)
				lhu     $v1, 0($s2)
				sra     $v0, 2
				subu    $v0, $v1
				sh      $v0, 0($t0)
				lw      $v0, 0x28 + var_14($sp)
				lhu     $v1, 2($s2)
				sra     $v0, 2
				subu    $v0, $v1
				sh      $v0, 2($t0)
				lw      $v0, 0x28 + var_10($sp)
				lhu     $v1, 4($s2)
				sra     $v0, 2
				subu    $v0, $v1
				sh      $v0, 4($t0)
				lhu     $v0, 0($s2)
				lhu     $v1, 0($t0)
				nop
				addu    $v0, $v1
				sh      $v0, 0($s2)
				lhu     $v0, 2($s2)
				lhu     $v1, 2($t0)
				nop
				addu    $v0, $v1
				sh      $v0, 2($s2)
				lhu     $v0, 4($s2)
				lhu     $v1, 4($t0)
				nop
				addu    $v0, $v1
				j       loc_80017980
				sh      $v0, 4($s2)
#endif
				break;
		}
		case 1:
		case 5:
		{
			if (dampMode == 5)
			{
				shift = 1;
			}
			else
			{
				shift = 2;
			}

			//loc_8001746C
			_x0 = targetPos->x;//v0
			_y0 = targetPos->y;//a2
			_z0 = targetPos->z;//a1

			_x1 = position->x;//v1
			_y1 = position->y;//a0
			_z1 = position->z;//v1

			_v = &vector;
			
			_v->x = _x0 - _x1;
			_v->y = _y0 - _y1;
			_v->z = _z0 - _z1;

			accl->x = (short)((_v->x >> shift) - vel->x);
			accl->y = (short)((_v->y >> shift) - vel->y);
			accl->z = (short)((_v->z >> shift) - vel->z);

			vel->x += accl->x;
			vel->y += accl->y;
			vel->z += accl->z;
			
			length = CAMERA_LengthSVector(vel);

			if (maxVel < length)
			{
				vel->x = (short)((vel->x * maxVel) / length);
				vel->y = (short)((vel->y * maxVel) / length);
				vel->z = (short)((vel->z * maxVel) / length);

				//v1 = gameTrackerX.timeMult

				if (gameTrackerX.timeMult != ONE)
				{
					position->x += (short)((vel->x * gameTrackerX.timeMult) >> 12);
					position->y += (short)((vel->y * gameTrackerX.timeMult) >> 12);
					position->z += (short)((vel->z * gameTrackerX.timeMult) >> 12);
				}
				else
				{
					//loc_80017980
					position->x += vel->x;
					position->y += vel->y;
					position->z += vel->z;
				}
			}
			else
			{
				//loc_80017980
				position->x += vel->x;
				position->y += vel->y;
				position->z += vel->z;
			}
				break;
		}
		case 2:
		{
			break;
		}
		case 3:
		{
#if 0

			loc_80017610:            # jumptable 800173A8 case 3
				addiu   $s2, $sp, 0x28 + var_8
				move    $a0, $s2
				lhu     $v0, 0($s3)
				lhu     $v1, 2($s3)
				lhu     $a1, 4($s3)
				lhu     $a2, 0($s1)
				lhu     $a3, 2($s1)
				lhu     $t0, 4($s1)
				subu    $v0, $a2
				subu    $v1, $a3
				subu    $a1, $t0
				sh      $v0, 0x28 + var_8($sp)
				sh      $v1, 2($s2)
				jal     sub_80016270
				sh      $a1, 4($s2)
				move    $s0, $v0
				sll     $v0, $s4, 16
				sra     $a0, $v0, 16
				slt     $v0, $s0, $a0
				bnez    $v0, loc_800179C0
				nop
				lh      $v0, 0x28 + var_8($sp)
				nop
				mult    $v0, $a0
				mflo    $v1
				nop
				nop
				div     $v1, $s0
				mflo    $a2
				lh      $v0, 0x28 + var_6($sp)
				nop
				mult    $v0, $a0
				mflo    $v1
				nop
				nop
				div     $v1, $s0
				mflo    $v1
				lh      $v0, 0x28 + var_4($sp)
				nop
				mult    $v0, $a0
				mflo    $t0
				nop
				nop
				div     $t0, $s0
				mflo    $v0
				sh      $a2, 0x28 + var_8($sp)
				sh      $v1, 0x28 + var_6($sp)
				sh      $v0, 0x28 + var_4($sp)
				lhu     $v0, 0($s1)
				lhu     $a0, 2($s2)
				lhu     $a1, 4($s2)
				lhu     $v1, 4($s1)
				addu    $v0, $a2
				sh      $v0, 0($s1)
				lhu     $v0, 2($s1)
				addu    $v1, $a1
				sh      $v1, 4($s1)
				addu    $v0, $a0
				j       def_800173A8     # jumptable 800173A8 default case, case 2
				sh      $v0, 2($s1)
#endif
				break;
		}
		case 4:
		{
#if 0
			loc_80017700:            # jumptable 800173A8 case 4
				lhu     $v0, 0($s3)
				lhu     $v1, 2($s3)
				lhu     $a0, 4($s3)
				lhu     $a1, 0($s1)
				lhu     $a2, 2($s1)
				lhu     $a3, 4($s1)
				subu    $v0, $a1
				addiu   $a1, $sp, 0x28 + var_8
				subu    $v1, $a2
				subu    $a0, $a3
				sh      $v0, 0x28 + var_8($sp)
				sll     $v0, 16
				sh      $v1, 2($a1)
				sh      $a0, 4($a1)
				lhu     $v1, 0($s2)
				sra     $v0, 18
				subu    $v0, $v1
				sh      $v0, 0($t0)
				lhu     $v0, 0x28 + var_6($sp)
				lhu     $v1, 2($s2)
				sll     $v0, 16
				sra     $v0, 18
				subu    $v0, $v1
				sh      $v0, 2($t0)
				lhu     $v0, 0x28 + var_4($sp)
				lhu     $v1, 4($s2)
				sll     $v0, 16
				sra     $v0, 18
				subu    $v0, $v1
				sh      $v0, 4($t0)
				lh      $v0, 0x28 + var_8($sp)
				nop
				blez    $v0, loc_800177A0
				nop
				lh      $v0, 0($t0)
				lhu     $v1, 0($t0)
				bgez    $v0, loc_800177B4
				nop
				j       loc_800177B4
				move    $v1, $zero

				loc_800177A0 :
			lh      $v0, 0($t0)
				lhu     $v1, 0($t0)
				blez    $v0, loc_800177B4
				nop
				move    $v1, $zero

				loc_800177B4 :
			sh      $v1, 0($t0)
				lh      $v0, 0x28 + var_6($sp)
				nop
				blez    $v0, loc_800177E0
				nop
				lh      $v0, 2($t0)
				lhu     $v1, 2($t0)
				bgez    $v0, loc_800177F4
				nop
				j       loc_800177F4
				move    $v1, $zero

				loc_800177E0 :
			lh      $v0, 2($t0)
				lhu     $v1, 2($t0)
				blez    $v0, loc_800177F4
				nop
				move    $v1, $zero

				loc_800177F4 :
			sh      $v1, 2($t0)
				lh      $v0, 0x28 + var_4($sp)
				nop
				blez    $v0, loc_80017820
				nop
				lh      $v0, 4($t0)
				lhu     $v1, 4($t0)
				bgez    $v0, loc_80017834
				nop
				j       loc_80017834
				move    $v1, $zero

				loc_80017820 :
			lh      $v0, 4($t0)
				lhu     $v1, 4($t0)
				blez    $v0, loc_80017834
				nop
				move    $v1, $zero

				loc_80017834 :
			sh      $v1, 4($t0)
				lhu     $v0, 0($s2)
				lhu     $v1, 0($t0)
				nop
				addu    $v0, $v1
				sh      $v0, 0($s2)
				lhu     $v0, 2($s2)
				lhu     $v1, 2($t0)
				nop
				addu    $v0, $v1
				sh      $v0, 2($s2)
				lhu     $v0, 4($s2)
				lhu     $v1, 4($t0)
				move    $a0, $s2
				addu    $v0, $v1
				jal     sub_80016270
				sh      $v0, 4($s2)
				move    $s0, $v0
				jal     sub_80016270
				addiu   $a0, $sp, 0x28 + var_8
				slt     $v0, $s0
				bnez    $v0, loc_800179C0
				sll     $v0, $s4, 16
				sra     $a1, $v0, 16
				slt     $v0, $s0, $a1
				bnez    $v0, loc_80017980
				nop
				lh      $v0, 0($s2)
				nop
				mult    $v0, $a1
				mflo    $v1
				nop
				nop
				div     $v1, $s0
				mflo    $a0
				lh      $v0, 2($s2)
				nop
				mult    $v0, $a1
				mflo    $v1
				nop
				nop
				div     $v1, $s0
				mflo    $v1
				lh      $v0, 4($s2)
				nop
				mult    $v0, $a1
				mflo    $t0
				nop
				nop
				div     $t0, $s0
				mflo    $v0
				sh      $a0, 0($s2)
				sh      $v1, 2($s2)
				sh      $v0, 4($s2)
				lw      $v1, -0x3FF8($gp)
				li      $v0, 0x1000
				beq     $v1, $v0, loc_80017980
				nop
				lh      $v0, 0($s2)
				nop
				mult    $v0, $v1
				lhu     $v0, 0($s1)
				mflo    $t1
				srl     $v1, $t1, 12
				addu    $v0, $v1
				sh      $v0, 0($s1)
				lh      $v1, 2($s2)
				lw      $v0, -0x3FF8($gp)
				nop
				mult    $v1, $v0
				lhu     $v0, 2($s1)
				mflo    $t1
				srl     $v1, $t1, 12
				addu    $v0, $v1
				sh      $v0, 2($s1)
				lh      $v1, 4($s2)
				lw      $v0, -0x3FF8($gp)
				nop
				mult    $v1, $v0
				lhu     $v0, 4($s1)
				mflo    $t1
				j       loc_800179B0
				srl     $v1, $t1, 12

				loc_80017980:
			lhu     $v0, 0($s1)
				lhu     $v1, 0($s2)
				nop
				addu    $v0, $v1
				sh      $v0, 0($s1)
				lhu     $v0, 2($s1)
				lhu     $v1, 2($s2)
				nop
				addu    $v0, $v1
				sh      $v0, 2($s1)
				lhu     $v0, 4($s1)
				lhu     $v1, 4($s2)

#endif
				break;
		}
		}
	}
	//loc_800179C0
#if 0
		
		loc_800179B0:
	nop
		addu    $v0, $v1
		j       def_800173A8     # jumptable 800173A8 default case, case 2
		sh      $v0, 4($s1)

		loc_800179C0:
			lhu     $v0, 0($s3)
				lhu     $v1, 2($s3)
				lhu     $a0, 4($s3)
				sh      $v0, 0($s1)
				sh      $v1, 2($s1)
				sh      $a0, 4($s1)

				def_800173A8:            # jumptable 800173A8 default case, case 2
				lw      $ra, 0x28 + var_s14($sp)
				lw      $s4, 0x28 + var_s10($sp)
				lw      $s3, 0x28 + var_sC($sp)
				lw      $s2, 0x28 + var_s8($sp)
				lw      $s1, 0x28 + var_s4($sp)
				lw      $s0, 0x28 + var_s0($sp)
				jr      $ra
				addiu   $sp, 0x40
#endif
}

void CriticalDampAngle(long dampMode, short* currentVal, short target, short* vel, short* accl, int smooth)
{
	short current;
	
	target &= 0xFFF;

	current = (currentVal[0] & 0xFFF);
	
	if (target - current >= 2048)
	{
		current = current + 4096;
	}
	else if ((currentVal[0] & 0xFFF) - target >= 2049)
	{
		target |= 4096;
	}

	CriticalDampValue(dampMode, &current, target, vel, accl, smooth);

	current &= 0xFFF;

	currentVal[0] = current;
}

short CAMERA_CalcZRotation(_Position* target, _Position* position)//Matching - 99.83%
{
	struct _SVector onPlane;
	struct _SVector sv;
	short _x0;
	short _y0;
	short _z0;
	short _x1;
	short _y1;
	short _z1;
	struct _SVector* _v;


	_x0 = position->x;
	_y0 = position->y;
	_z0 = position->z;

	_x1 = target->x;
	_y1 = target->y;
	_z1 = target->z;

	_v = &onPlane;

	_v->x = _x0 - _x1;
	_v->y = _y0 - _y1;
	_v->z = _z0 - _z1;

	sv.x = onPlane.x;
	sv.y = onPlane.y;
	sv.z = 0;

	CAMERA_LengthSVector(&sv);

	return (ratan2(onPlane.y, onPlane.x) + 1024) & 0xFFF;
}

void CAMERA_CalcRotation(struct _Rotation* rotation, _Position* target, _Position* position)
{
	struct _SVector sv;
	struct _SVector onPlane;
	short _x0;
	short _y0;
	short _z0;
	short _x1;
	short _y1;
	short _z1;
	struct _SVector* _v;

	_x0 = position->x;
	_y0 = position->y;
	_z0 = position->z;

	_x1 = target->x;
	_y1 = target->y;
	_z1 = target->z;

	_v = &sv;
	_v->x = _x0 - _x1;
	_v->y = _y0 - _y1;
	_v->z = _z0 - _z1;

	onPlane.x = _v->x;
	onPlane.z = 0;
	onPlane.y = _v->y;

	rotation->x = -(short)ratan2(onPlane.z, CAMERA_LengthSVector(&onPlane));
	rotation->y = 0;
	rotation->z = (short)ratan2(sv.y, sv.z) + 1024;
}

void CAMERA_CalcFSRotation(struct Camera* camera, struct _Rotation* rotation, _Position* target, _Position* position) // Matching - 100%
{
	_SVector sv;
	_SVector onPlane;
	_SVector sv2;
	short _x0;
	short _y0;
	short _z0;
	short _x1;
	short _y1;
	short _z1;
	_SVector* _v;
	_SVector* _v0;

	_v = (_SVector*)position;
	_v0 = (_SVector*)target;
	_x0 = _v->x;
	_y0 = _v->y;
	_z0 = _v->z;
	_x1 = _v0->x;
	_y1 = _v0->y;
	_z1 = _v0->z;

	_v = (_SVector*)&sv;
	_v->x = _x0 - _x1;
	_v->y = _y0 - _y1;
	_v->z = _z0 - _z1;

	_v0 = (_SVector*)&sv2;
	_v0->x = _x0 - _x1;
	_v0->y = _y0 - _y1;
	_v0->z = _z0 - _z1;

	onPlane.x = sv2.x;
	onPlane.y = sv2.y;
	onPlane.z = 0;

	rotation->x = (short)-ratan2(sv2.z, CAMERA_LengthSVector(&onPlane));
	rotation->y = 0;
	rotation->z = (short)ratan2((short)sv2.y, sv2.x) + 1024;
}

void CAMERA_Relocate(struct Camera *camera, struct _SVector *offset, int streamSignalFlag)
{
	if (streamSignalFlag || camera->mode != 5)
	{
		camera->core.position.x += offset->x;
		camera->core.position.y += offset->y;
		camera->core.position.z += offset->z;

		camera->focusPoint.x += offset->x;
		camera->focusPoint.y += offset->y;
		camera->focusPoint.z += offset->z;

		camera->targetPos.x += offset->x;
		camera->targetPos.y += offset->y;
		camera->targetPos.z += offset->z;

		camera->targetFocusPoint.x += offset->x;
		camera->targetFocusPoint.y += offset->y;
		camera->targetFocusPoint.z += offset->z;

		camera->newFocusInstancePos.x += offset->x;
		camera->newFocusInstancePos.y += offset->y;
		camera->newFocusInstancePos.z += offset->z;
	}
}

struct _TFace* CAMERA_SphereToSphereWithLines(struct Camera* camera, struct CameraCollisionInfo* colInfo, int secondcheck_flag) // Matching - 99.37%
{
	long minLength;                      // stack offset -68     sp(0x134)
	struct _SVector sv;                  // stack offset -344    sp(0x20)
	struct _SVector startPt[5];          // stack offset -336    sp(0x28)
	struct _SVector endPt[5];            // stack offset -296    sp(0x50)
	struct _SVector startLine;           // stack offset -256    sp(0x78)
	struct _Vector adjStartLine;         // stack offset -248    sp(0x80)
	struct _SVector endLine;             // stack offset -232    sp(0x90)
	struct _Vector adjEndLine;           // stack offset -224    sp(0x98)
	struct _Vector CamLineNormalized;    // stack offset -208    sp(0xA8)
	struct _Rotation rotation;           // stack offset -192    sp(0xB8)
	MATRIX matrix;                       // stack offset -184    sp(0xC0)
	struct _TFace* result;               // stack offset -64     sp(0x138)
	long i;                              // stack offset -60     sp(0x13C)
	long init;                           // stack offset -56     sp(0x140)
	struct Level* level;                 // stack offset -52     sp(0x144)
	// struct _Instance *focusInstance;  // $v0
	struct _Vector ACE_vect;             // stack offset -152    sp(0xE0)
	struct _LCollideInfo lcol;           // stack offset -136    sp(0xF0)
	int ACE_force;                       // $s2
	int in_warpRoom;                     // stack offset -48     sp(0x148)
	int flag;                            // $fp
	short backface_flag;                 // stack offset -72     sp(0x130)
	struct _PCollideInfo pCollideInfo;   // stack offset -120    sp(0x100)
	int n;                               // $s2
	struct Level* thislevel;             // $s1
	struct _SVector* _v;                 // stack offset -44     sp(0x14C)

	minLength = 0;
	result = NULL;
	init = 1;
	ACE_force = 0;

	level = STREAM_GetLevelWithID(camera->focusInstance->currentStreamUnitID);

	if ((level != NULL) && (MEMPACK_MemoryValidFunc((char*)level) != 0))
	{
		colInfo->line = -1;
		colInfo->flags = 0;
		colInfo->numCollided = 0;
		startLine.x = 0;
		startLine.z = 0;
		endLine.x = 0;
		endLine.z = 0;

		CAMERA_CalcFSRotation(camera, &rotation, &colInfo->end->position, &colInfo->start->position);

		if ((camera->flags & 0x10000U) != 0)
		{
			colInfo->start->position.x += (short)((colInfo->end->position.x - colInfo->start->position.x) >> 5);
			colInfo->start->position.y += (short)((colInfo->end->position.y - colInfo->start->position.y) >> 5);
			colInfo->start->position.z += (short)((colInfo->end->position.z - colInfo->start->position.z) >> 5);
		}

		MATH3D_SetUnityMatrix(&matrix);
		RotMatrixZ(rotation.z + 1024, &matrix);

		if (((camera->flags & 0x10000U) == 0) && ((camera->instance_mode & 0x4000000) == 0) && (camera->mode != 6))
		{
			startLine.y = 4096;

			ApplyMatrix(&matrix, (SVECTOR*)&startLine, (VECTOR*)&ACE_vect);

			ACE_amount =
				(ACE_vect.x * camera->focusInstanceVelVec.x) +
				(ACE_vect.y * camera->focusInstanceVelVec.y) +
				(ACE_vect.z * camera->focusInstanceVelVec.z);

			ACE_amount >>= 12;

			if ((camera->always_rotate_flag != 0) || (camera->forced_movement != 0))
			{
				if (camera->forced_movement != 0)
				{
					if (camera->rotDirection <= 0)
					{
						if (camera->rotDirection < 0)
						{
							ACE_force = -72;
						}
					}
					else
					{
						ACE_force = 72;
					}
				}
				else
				{
					ACE_force = -72;

					if ((CAMERA_SignedAngleDifference(camera->focusRotation.z, camera->targetFocusRotation.z) << 16) < 0)
					{
						ACE_force = 72;
					}
				}

				if (ACE_amount > 0)
				{
					if (ACE_force > 0)
					{
						ACE_force -= ACE_amount;

						if (ACE_force < 0)
						{
							ACE_force = 0;
						}
					}
				}

				else if (ACE_force < 0)
				{
					ACE_force -= ACE_amount;

					if (ACE_force > 0)
					{
						ACE_force = 0;
					}
				}
			}
		}
		else
		{
			ACE_amount = 0;
		}

		startLine.y = 32;
		endLine.y = 290;

		if (ACE_amount > 0)
		{
			startLine.y += (short)ACE_amount;
			endLine.y += (short)((ACE_amount * 5) + (ACE_amount / 2));
		}
		else
		{
			startLine.y -= (short)(ACE_amount * 2);
		}

		if (ACE_force > 0)
		{
			endLine.y += ACE_force * 5;
		}

		ApplyMatrix(&matrix, (SVECTOR*)&startLine, (VECTOR*)&adjStartLine);
		ApplyMatrix(&matrix, (SVECTOR*)&endLine, (VECTOR*)&adjEndLine);

		{
			struct _Vector* _v0;
			struct _Vector* _v1;
			struct _SVector* _v2;
			struct _SVector* _v3;

			_v0 = &adjStartLine;
			_v1 = &adjEndLine;
			_v2 = &startPt[1];
			_v3 = &endPt[1];

			CAMERA_Add_Vec_To_Pos((struct _Position*)_v2, &colInfo->start->position, _v0);
			CAMERA_Add_Vec_To_Pos((struct _Position*)_v3, &colInfo->end->position, _v1);
		}

		startLine.y = 32;
		endLine.y = 290;

		if (ACE_amount < 0)
		{
			startLine.y -= (short)ACE_amount;
			endLine.y -= (short)((ACE_amount * 5) + (ACE_amount / 2));
		}
		else
		{
			startLine.y += (short)(ACE_amount * 2);
		}

		if (ACE_force < 0)
		{
			endLine.y -= ACE_force * 5;
		}

		ApplyMatrix(&matrix, (SVECTOR*)&startLine, (VECTOR*)&adjStartLine);
		ApplyMatrix(&matrix, (SVECTOR*)&endLine, (VECTOR*)&adjEndLine);

		{
			struct _Vector* _v0;
			struct _Vector* _v1;
			struct _SVector* _v2;
			struct _SVector* _v3;

			_v0 = &adjStartLine;
			_v1 = &adjEndLine;
			_v2 = &startPt[2];
			_v3 = &endPt[2];

			CAMERA_Sub_Vec_From_Pos((struct _Position*)_v2, &colInfo->start->position, _v0);
			CAMERA_Sub_Vec_From_Pos((struct _Position*)_v3, &colInfo->end->position, _v1);
		}

		endLine.y = 180;
		startLine.y = 32;

		MATH3D_SetUnityMatrix(&matrix);
		RotMatrixX(rotation.x + 1024, &matrix);
		RotMatrixZ(rotation.z, &matrix);

		ApplyMatrix(&matrix, (SVECTOR*)&startLine, (VECTOR*)&adjStartLine);
		ApplyMatrix(&matrix, (SVECTOR*)&endLine, (VECTOR*)&adjEndLine);
		{
			struct _Vector* _v0;
			struct _Vector* _v1;
			struct _SVector* _v2;
			struct _SVector* _v3;

			_v0 = &adjStartLine;
			_v1 = &adjEndLine;
			_v2 = &startPt[3];
			_v3 = &endPt[3];

			CAMERA_Add_Pos_To_Vec((struct _Position*)_v2, _v0, &colInfo->start->position);
			CAMERA_Add_Pos_To_Vec((struct _Position*)_v3, _v1, &colInfo->end->position);
		}

		{
			struct _Vector* _v0;
			struct _Vector* _v1;
			struct _SVector* _v2;
			struct _SVector* _v3;

			_v0 = &adjStartLine;
			_v1 = &adjEndLine;
			_v2 = &startPt[4];
			_v3 = &endPt[4];

			CAMERA_Sub_Vec_From_Pos((struct _Position*)_v2, &colInfo->start->position, _v0);
			CAMERA_Sub_Vec_From_Pos((struct _Position*)_v3, &colInfo->end->position, _v1);
		}

		{
			struct _SVector* _v2 = &startPt[1];
			struct _SVector* _v3 = &right_point;

			CAMERA_Add_SVec_To_Pos(_v3, (struct _Position*)_v2, &camera->focusInstanceVelVec);
		}

		{
			struct _SVector* _v2 = &startPt[2];
			struct _SVector* _v3 = &left_point;

			CAMERA_Add_SVec_To_Pos(_v3, (struct _Position*)_v2, &camera->focusInstanceVelVec);
		}

		startLine.y = 4096;

		ApplyMatrix(&matrix, (SVECTOR*)&startLine, (VECTOR*)&adjStartLine);

		CAMERA_Copy_Vec_To_SVec(&camera_plane, &adjStartLine);

		startLine.y = 0;
		startLine.z = 4096;

		ApplyMatrix(&matrix, (SVECTOR*)&startLine, (VECTOR*)&CamLineNormalized);

		{
			struct _SVector* _v2 = &startPt[0];
			CAMERA_Copy_Pos_To_SVec(_v2, &colInfo->start->position);
		}

		{
			struct _SVector* _v2 = &endPt[0];
			CAMERA_Copy_Pos_To_SVec(_v2, &colInfo->end->position);
		}

		colInfo->lenCenterToExtend = (int)camera->targetFocusDistance;
		in_warpRoom = (unsigned int)(unsigned short)STREAM_GetStreamUnitWithID(level->streamUnitID)->flags & 1;

		for (i = 0, _v = &sv; i < 5; i++)
		{
			if ((colInfo->cldLines & (1 << i)) != 0)
			{
				if ((i - 1) < 2U)
				{
					flag = 1;
				}
				else
				{
					flag = 0;
				}

				backface_flag = 0;
				pCollideInfo.collideType = 1;
				pCollideInfo.newPoint = (SVECTOR*)&endPt[i];
				pCollideInfo.oldPoint = (SVECTOR*)&startPt[i];
				pCollideInfo.instance = NULL;
				colInfo->tfaceList[i] = COLLIDE_PointAndTerrainFunc(level->terrain, &pCollideInfo, flag, &backface_flag, 208, 32, &lcol);
				colInfo->tfaceTerrain[i] = level->terrain;

				if (colInfo->tfaceList[i] == NULL)
				{
					struct _StreamUnit* streamUnit = StreamTracker.StreamList;
					for (n = 0; n < 16; n++, streamUnit++)
					{
						thislevel = streamUnit->level;
						if ((streamUnit->used == 2) && (thislevel != level) &&
							(MEMPACK_MemoryValidFunc((char*)thislevel) != 0) &&
							(in_warpRoom == 0 || ((streamUnit->flags & 1) == 0)))
						{
							colInfo->tfaceList[i] = COLLIDE_PointAndTerrainFunc(thislevel->terrain, &pCollideInfo, flag, &backface_flag, 208, 32, &lcol);

							if (colInfo->tfaceList[i] != NULL)
							{
								colInfo->tfaceTerrain[i] = thislevel->terrain;
								break;
							}
						}
					}
				}

				colInfo->bspTree[i] = lcol.curTree;

				if (colInfo->tfaceList[i] != NULL)
				{
					if (secondcheck_flag != 0)
					{
						return colInfo->tfaceList[i];
					}

					colInfo->numCollided++;

					{
						struct _SVector* _v0;
						struct _SVector* _v1;

						_v0 = endPt;
						_v1 = startPt;
						CAMERA_Sub_SVec_From_SVec(&sv, &_v0[i], &_v1[i]);

						colInfo->lengthList[i] = (short)(((sv.x * CamLineNormalized.x) + (sv.y * CamLineNormalized.y) + (sv.z * CamLineNormalized.z)) >> 0xc);
					}

					if (backface_flag == 0 || 100 <= colInfo->lengthList[i])
					{
						colInfo->lengthList[i] -= 100;

						if (colInfo->lengthList[i] < 220)
						{
							colInfo->lengthList[i] = 220;
						}

						if (init != 0 || (int)colInfo->lengthList[i] < minLength)
						{
							colInfo->line = i;
							minLength = (int)colInfo->lengthList[i];
							init = 0;
							colInfo->lenCenterToExtend = minLength;
							result = colInfo->tfaceList[i];
						}

						colInfo->flags |= (1 << i);
					}
				}
				else
				{
					{
						struct _SVector* _v0;
						struct _SVector* _v1;

						_v0 = endPt;
						_v1 = startPt;
						CAMERA_Sub_SVec_From_SVec(&sv, &_v0[i], &_v1[i]);

						colInfo->lengthList[i] = (short)(((sv.x * CamLineNormalized.x) + (sv.y * CamLineNormalized.y) + (sv.z * CamLineNormalized.z)) >> 0xc);
					}
				}
			}
		}

		if (colInfo->line == 2)
		{
			hitline_rot = CAMERA_CalcZRotation((struct _Position*)&startPt[2], (struct _Position*)&endPt[2]);
		}
		else if (colInfo->line == 1)
		{
			hitline_rot = CAMERA_CalcZRotation((struct _Position*)&startPt[1], (struct _Position*)&endPt[1]);
		}
		else if ((colInfo->flags & 4) != 0)
		{
			hitline_rot = CAMERA_CalcZRotation((struct _Position*)&startPt[2], (struct _Position*)&endPt[2]);
		}
		else if ((colInfo->flags & 2) != 0)
		{
			hitline_rot = CAMERA_CalcZRotation((struct _Position*)&startPt[1], (struct _Position*)&endPt[1]);
		}
	}

	return result;
}

long CAMERA_CalcTilt(struct _Normal* normal, short zRot)//Matching - 99.66%
{
	MATRIX matrix;
	VECTOR newNormal;

	MATH3D_SetUnityMatrix(&matrix);

	RotMatrixZ(-zRot, &matrix);

	ApplyMatrix(&matrix, (SVECTOR*)normal, &newNormal);

	return -(short)ratan2(newNormal.vy, newNormal.vz);
}

void CAMERA_SetLookFocusAndBase(struct _Instance* focusInstance, struct _Position* focusPoint)
{
	struct _Position lookFocus;
	
	lookFocus.x = focusInstance->position.x;
	lookFocus.y = focusInstance->position.y;
	lookFocus.z = focusInstance->position.z + 512;

	focusPoint->x = lookFocus.x;
	focusPoint->y = lookFocus.y;
	focusPoint->z = lookFocus.z;
}

void CAMERA_SetLookFocusAndDistance(struct Camera* camera, VECTOR* focuspoint, int distance)
{ 
	camera->targetFocusPoint.x = (short)focuspoint->vx;
	camera->targetFocusPoint.y = (short)focuspoint->vy;
	camera->targetFocusPoint.z = (short)focuspoint->vz;

	Camera_lookDist = distance;
}


// autogenerated function stub: 
// void /*$ra*/ CAMERA_LookProcess(struct Camera *camera /*$s2*/)
void CAMERA_LookProcess(struct Camera *camera)
{ // line 2420, offset 0x80018608
	///* begin block 1 */
	//	// Start line: 2421
	//	// Start offset: 0x80018608
	//	// Variables:
	//		struct _Instance *focusInstance; // $s1
	//		int smooth; // $t0
	//		int distance; // $a3
	//		long dampMode; // $a0

	//	/* begin block 1.1 */
	//		// Start line: 2466
	//		// Start offset: 0x800186F8
	//		// Variables:
	//			short _x1; // $v1
	//			short _y1; // $a0
	//			short _z1; // $a1
	//			_Position *_v0; // $v0
	//			_Position *_v1; // $v0
	//	/* end block 1.1 */
	//	// End offset: 0x800186F8
	//	// End Line: 2466

	//	/* begin block 1.2 */
	//		// Start line: 2487
	//		// Start offset: 0x80018760
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $v1
	//			short _z1; // $a0
	//	/* end block 1.2 */
	//	// End offset: 0x80018760
	//	// End Line: 2487

	//	/* begin block 1.3 */
	//		// Start line: 2487
	//		// Start offset: 0x80018760
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $v1
	//			short _z1; // $a1
	//			struct _Rotation *_v0; // $v0
	//	/* end block 1.3 */
	//	// End offset: 0x80018760
	//	// End Line: 2487

	//	/* begin block 1.4 */
	//		// Start line: 2487
	//		// Start offset: 0x80018760
	//		// Variables:
	//			short _x1; // $v1
	//			short _y1; // $a0
	//			struct _Rotation *_v0; // $v0
	//	/* end block 1.4 */
	//	// End offset: 0x80018760
	//	// End Line: 2487
	///* end block 1 */
	//// End offset: 0x80018760
	//// End Line: 2493

	///* begin block 2 */
	//	// Start line: 5917
	///* end block 2 */
	//// End Line: 5918

	///* begin block 3 */
	//	// Start line: 5921
	///* end block 3 */
	//// End Line: 5922
	UNIMPLEMENTED();
}

void CAMERA_Normalize(struct _SVector *svector)
{
	long len;

	len = CAMERA_LengthSVector(svector);

	if (len != 0)
	{
		svector->x = (short)((svector->x << 12) / len);
		svector->y = (short)((svector->y << 12) / len);
		svector->z = (short)((svector->z << 12) / len);
	}
}

void CAMERA_HandleTransitions(struct Camera *camera)
{
	if (camera->rotState == 3)
	{
		camera->targetFocusRotation.z = camera->signalRot.z;

		camera->collisionTargetFocusRotation.z = camera->signalRot.z;

		if (camera->forced_movement != 1 || (camera->lock & 0x4))
		{
			if (CAMERA_AngleDifference(camera->targetFocusRotation.z, camera->focusRotation.z) < 4)
			{
				camera->rotState = 0;
				camera->focusRotVel.z = 0;
				camera->focusRotAccl.z = 0;
			}
		}
		else
		{
			camera->rotState = 0;
			camera->focusRotVel.z = 0;
			camera->focusRotAccl.z = 0;
		}
	}

	if (camera->tiltState == 3)
	{
		camera->targetFocusRotation.x = camera->signalRot.x;

		if (camera->forced_movement != camera->tiltState || (camera->lock & 0x2))
		{
			if (CAMERA_AngleDifference(camera->focusRotation.x, camera->signalRot.x) < 4)
			{
				camera->tiltState = 0;
				camera->focusRotVel.x = 0;
				camera->focusRotAccl.x = 0;
			}
		}
		else
		{
			camera->tiltState = 0;
			camera->focusRotVel.x = 0;
			camera->focusRotAccl.x = 0;
		}
	}

	if (camera->distanceState == 3)
	{
		if (camera->forced_movement != 2 || (camera->lock & 0x1))
		{
			if (camera->targetFocusDistance - camera->signalFocusDistance >= 0)
			{
				if (camera->targetFocusDistance - camera->signalFocusDistance < 4)
				{
					camera->posState = 0;

					camera->focusDistanceVel = 0;

					camera->focusDistanceAccl = 0;

					camera->targetFocusDistance = camera->signalFocusDistance;
				}
				else
				{
					camera->targetFocusDistance = camera->signalFocusDistance;
				}
			}
			else
			{
				if (camera->signalFocusDistance - camera->targetFocusDistance < 4)
				{
					camera->distanceState = 0;
				}
				else
				{
					camera->targetFocusDistance = camera->signalFocusDistance;
				}
			}
		}
		else
		{
			camera->distanceState = 0;

			camera->focusDistanceVel = 0;

			camera->focusDistanceAccl = 0;

			camera->targetFocusDistance = camera->signalFocusDistance;
		}
	}

	if (camera->posState == 3)
	{
		if (camera->mode != 5 || (camera->flags & 0x1000))
		{
			camera->posState = 0;
		}
	}
}

void CAMERA_CalcFocusOffset(struct _SVector* offset, struct Camera* camera)
{
	struct _Vector adjustedOffset;
	struct _SVector temp;
	struct _Instance* focusInstance;
	short _x1;
	short _y1;
	short _z1;

	focusInstance = camera->focusInstance;

	temp.x = camera->focusOffset.x;
	temp.y = camera->focusOffset.y;
	temp.z = camera->focusOffset.z;

	if (camera->instance_mode < 0)
	{
		temp.y += 256;
	}

	gte_SetRotMatrix(focusInstance->matrix);

	gte_ldv0(&temp);

	gte_rtv0();

	gte_stlvnl(&adjustedOffset);

	_x1 = (short)adjustedOffset.x;
	_y1 = (short)adjustedOffset.y;
	_z1 = (short)adjustedOffset.z;

	offset->x = _x1;
	offset->y = _y1;
	offset->z = _z1;
}

void CAMERA_CalcFocusOffsetForSwim(struct _SVector* offset, struct Camera* camera)//Matching - 100%
{
	struct _Instance* focusInstance;
	short _x1;
	short _y1;
	short _z1;
	struct _Vector adjustedOffset;
	struct _SVector temp;
	struct _Vector* _v1;

	focusInstance = camera->focusInstance;

	temp.x = camera->focusOffset.x;
	temp.y = camera->focusOffset.y;
	temp.z = camera->focusOffset.z;

	ApplyMatrix(&focusInstance->matrix[1], (SVECTOR*)&temp, (VECTOR*)&adjustedOffset);

	_v1 = &adjustedOffset;

	_x1 = (short)_v1->x;
	_y1 = (short)_v1->y;
	_z1 = (short)_v1->z;

	offset->x = _x1;
	offset->y = _y1;
	offset->z = _z1;
}


// autogenerated function stub: 
// short /*$ra*/ CAMERA_CalcIntersectAngle(struct _SVector *linept /*$s0*/, struct _SVector *vertex0 /*$a1*/, struct _SVector *vertex1 /*$a2*/, short *high /*$s2*/, short *low /*stack 16*/)
short CAMERA_CalcIntersectAngle(struct _SVector *linept, struct _SVector *vertex0, struct _SVector *vertex1, short *high, short *low)
{ // line 2727, offset 0x80018ba8
	/* begin block 1 */
		// Start line: 2728
		// Start offset: 0x80018BA8
		// Variables:
			struct _SVector point; // stack offset -24
			short zrot; // $a0
			long camera_plane_d; // $v0
	/* end block 1 */
	// End offset: 0x80018C80
	// End Line: 2747

	/* begin block 2 */
		// Start line: 6608
	/* end block 2 */
	// End Line: 6609

	/* begin block 3 */
		// Start line: 6613
	/* end block 3 */
	// End Line: 6614
			UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// short /*$ra*/ CAMERA_GetLineAngle(struct Camera *camera /*$a0*/, struct CameraCollisionInfo *colInfo /*$a1*/, struct _SVector *linept /*$a2*/, int line /*$s5*/)
short CAMERA_GetLineAngle(struct Camera *camera, struct CameraCollisionInfo *colInfo, struct _SVector *linept, int line)
{ // line 2749, offset 0x80018c98
	/* begin block 1 */
		// Start line: 2750
		// Start offset: 0x80018C98
		// Variables:
			struct _Terrain *terrain; // $t1
			struct _SVector *vertex0; // $s1
			struct _SVector *vertex1; // $s0
			struct _SVector *vertex2; // $s3
			struct _SVector new_linept; // stack offset -48
			short high; // stack offset -40
			short low; // stack offset -38
	/* end block 1 */
	// End offset: 0x80018E3C
	// End Line: 2779

	/* begin block 2 */
		// Start line: 6675
	/* end block 2 */
	// End Line: 6676
			UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// long /*$ra*/ CAMERA_ACForcedMovement(struct Camera *camera /*$s1*/, struct CameraCollisionInfo *colInfo /*$s0*/)
long CAMERA_ACForcedMovement(struct Camera *camera, struct CameraCollisionInfo *colInfo)
{ // line 2793, offset 0x80018e60
	/* begin block 1 */
		// Start line: 2794
		// Start offset: 0x80018E60
		// Variables:
			long dp; // $v0
			struct _Normal normal; // stack offset -32
			struct _SVector sv; // stack offset -24

		/* begin block 1.1 */
			// Start line: 2794
			// Start offset: 0x80018E60
			// Variables:
				short _x0; // $a1
				short _y0; // $a2
				short _z0; // $v0
				short _x1; // $a3
				short _y1; // $t0
				short _z1; // $v1
				struct _SVector *_v; // $a0
				_Position *_v0; // $v0
				_Position *_v1; // $v1
		/* end block 1.1 */
		// End offset: 0x80018E60
		// End Line: 2794
	/* end block 1 */
	// End offset: 0x80018F18
	// End Line: 2832

	/* begin block 2 */
		// Start line: 6883
	/* end block 2 */
	// End Line: 6884
				UNIMPLEMENTED();
	return 0;
}

void CAMERA_update_dist_debounced(struct Camera* camera, short dist)//Matching - 99.29%
{
	if (!(camera->instance_mode & 0x2000000) || dist >= 600 || combat_cam_weight >= 4040)
	{
		shorten_flag = 1;

		if (++shorten_count >= 3)
		{
			camera->collisionTargetFocusDistance = dist;
		}
	}
}

short CAMERA_dampgetline(short angle) // Matching - 91.63%
{
	static short target_angle;
	static short angle_vel;
	static short angle_accl;

	CriticalDampAngle(1, &target_angle, angle, &angle_vel, &angle_accl, 1024);
	if (target_angle > 2048)
	{
		target_angle -= 4096;
	}
	if ((abs(target_angle) < 32) && (abs(angle) > 31))
	{
		target_angle = angle < 0 ? -32 : 32;
	}
	return target_angle;
}

// autogenerated function stub: 
// long /*$ra*/ CAMERA_ACNoForcedMovement(struct Camera *camera /*$s0*/, struct CameraCollisionInfo *colInfo /*$s1*/)
long CAMERA_ACNoForcedMovement(struct Camera *camera, struct CameraCollisionInfo *colInfo)
{ // line 2873, offset 0x8001904c
	/* begin block 1 */
		// Start line: 2874
		// Start offset: 0x8001904C
		// Variables:
			long hit; // $s2
			short playerSamePos; // $s3
			short angle; // $a0

		/* begin block 1.1 */
			// Start line: 2900
			// Start offset: 0x800190C4
			// Variables:
				int n; // $a0
				int flag; // $t1
				short dist; // $a1
		/* end block 1.1 */
		// End offset: 0x80019278
		// End Line: 2956
	/* end block 1 */
	// End offset: 0x80019534
	// End Line: 3124

	/* begin block 2 */
		// Start line: 7059
	/* end block 2 */
	// End Line: 7060
				UNIMPLEMENTED();
	return 0;
}

long CAMERA_AbsoluteCollision(struct Camera* camera, struct CameraCollisionInfo* colInfo) // Matching - 99.80%
{
	long hit = 0;

	if ((gameTrackerX.debugFlags & 0x10000U) != 0)
	{
		return hit;
	}

	camera->focusRotation.x = camera->focusRotation.x & 0xfff;

	if (((camera->flags & 0x10000) != 0 ||
		(camera->instance_mode & 0x24000000) != 0 ||
		(camera->flags & 0x2000) != 0 ||
		camera->rotState != 0 ||
		camera->always_rotate_flag != 0) &&
		(camera->lock & 1U) == 0)
	{
		if ((camera->flags & 0x10000U) != 0)
		{
			if ((colInfo->flags & 0x18U) != 0)
			{
				camera->collisionTargetFocusDistance = (short)colInfo->lenCenterToExtend - 150;
				if (400 > camera->collisionTargetFocusDistance)
				{
					camera->collisionTargetFocusDistance = 400;
				}
			}
			else
			{
				hit = CAMERA_ACForcedMovement(camera, colInfo);
			}
		}
		else
		{
			camera->collisionTargetFocusDistance = (short)colInfo->lenCenterToExtend;
		}
	}
	else if (*(int*)&camera->forced_movement != 0)
	{
		hit = CAMERA_ACNoForcedMovement(camera, colInfo);
	}
	else
	{
		hit = CAMERA_ACForcedMovement(camera, colInfo);
	}

	return hit;
}

short CAMERA_update_z_damped(struct Camera* camera, short current, short target)
{
	static short upvel;
	static short upaccl;
	static short upmaxVel;
	short current_tmp;

	current_tmp = current;

	if (target < current)
	{
		if ((camera->instance_mode & 0x1038))
		{
			if (camera->real_focuspoint.z >= camera->focuspoint_fallz)
			{
				if ((current - target) >= 0)
				{
					upmaxVel = (current - target) / 6;

				}
				else
				{
					upmaxVel = (target - current) / 6;
				}
			}
		}
		else
		{
			if ((current - target) < 0)
			{
				upmaxVel = (target - current) << 1;
			}
			else
			{
				upmaxVel = (current - target) << 1;
			}
		}
	}
	else
	{
		if ((current - target) >= 0)
		{
			if ((current - target) < 5)
			{
				upvel = 0;
				upaccl = 0;
				upmaxVel = 0;

				return current;
			}
			else
			{
				if ((current - target) >= 0)
				{
					upmaxVel = ((current - target) / 6) - ((current - target) >> 31);
				}
				else
				{
					upmaxVel = ((target - current) / 6) - ((target - current) >> 31);
				}

				if (upmaxVel < 50)
				{
					upmaxVel = 50;
				}
			}
		}
		else
		{
			if ((target - current) < 5)
			{
				upvel = 0;
				upaccl = 0;
				upmaxVel = 0;
				
				return current;
			}
			else
			{
				if ((camera->instance_mode & 0x100))
				{
					if ((current - target) >= 0)
					{
						upmaxVel = ((current - target) / 6) - ((current - target) >> 31);
					}
					else
					{
						upmaxVel = ((target - current) / 6) - ((target - current) >> 31);
					}

					if (upmaxVel < 50)
					{
						upmaxVel = 50;
					}
				}
				else
				{
					if ((current - target) >= 0)
					{
						upmaxVel = ((current - target) / 6) - ((current - target) >> 31);
					}
					else
					{
						upmaxVel = ((target - current) / 6) - ((target - current) >> 31);
					}
				}

				if ((current - target) >= 0)
				{
					if ((current - target) < upmaxVel)
					{
						if ((current - target) >= 0)
						{
							upmaxVel = current - target;
						}
						else
						{
							upmaxVel = target - current;
						}
					}
				}
				else
				{
					if ((target - current) < upmaxVel)
					{
						if ((current - target) >= 0)
						{
							upmaxVel = current - target;
						}
						else
						{
							upmaxVel = target - current;
						}
					}
				}
			}
		}
	}

	CriticalDampValue(1, &current_tmp, target, &upvel, &upaccl, upmaxVel);

	return current_tmp;
}


// autogenerated function stub: 
// void /*$ra*/ CAMERA_CombatCamDist(struct Camera *camera /*$s0*/)
void CAMERA_CombatCamDist(struct Camera *camera)
{ // line 3310, offset 0x800198e8
	/* begin block 1 */
		// Start line: 3311
		// Start offset: 0x800198E8
		// Variables:
			DVECTOR xy; // stack offset -48
			DVECTOR xy2; // stack offset -40
			struct _SVector position; // stack offset -32
			long z; // stack offset -24
			struct _Instance *instance; // $s1
	/* end block 1 */
	// End offset: 0x80019B98
	// End Line: 3392

	/* begin block 2 */
		// Start line: 7966
	/* end block 2 */
	// End Line: 7967
			UNIMPLEMENTED();
}

void CAMERA_GenericCameraProcess(struct Camera* camera)//Matching - 91.84%
{
	struct _Instance* focusInstance;
	struct _Position targetCamPos;
	short angle;
	struct _Rotation test_rot;
	struct _Instance* warpInstance;
	int tmp;
	int mod;
	int dist;
	short _x1;
	short _y1;
	short _z1;
	struct _Position* _v0;
	struct _Position* _v1;
	struct _Position target;

	focusInstance = camera->focusInstance;

	if ((camera->flags & 0x10000) || (camera->instance_mode & 0x4000000))
	{
		camera->rotationVel.z = 64;
	}

	if (!(camera->instance_mode & 0x2000000))
	{
		combat_cam_weight = 2048;
	}

	CAMERA_SetMaxVel(camera);

	CAMERA_SetFocus(camera, &camera->targetFocusPoint);

	if ((camera->flags & 0x10000) || (camera->instance_mode & 0x4000000))
	{
		CAMERA_FollowPlayerTilt(camera, focusInstance);
	}
	else
	{
		CAMERA_UpdateFocusRotationX(camera, focusInstance);

		if (camera->mode == 13 && camera->instance_xyvel > 0 && !(camera->instance_mode & 0x82000400) && camera->always_rotate_flag == 0 && camera->rotState != 3 && !(camera->instance_mode & 0x2000) && gameTrackerX.cheatMode != 1)
		{
			CAMERA_CalcRotation(&test_rot, &camera->targetFocusPoint, &camera->core.position);

			angle = CAMERA_SignedAngleDifference(test_rot.z, camera->focusRotation.z);

			if ((camera->instance_mode & 0x2))
			{
				angle >>= 1;
			}
			else
			{
				angle = (angle * 3) >> 2;
			}

			camera->targetFocusRotation.z = camera->focusRotation.z + angle;
		}
	}

	if ((int)camera->instance_mode < 0)
	{
		warpInstance = RAZIEL_QueryEngagedInstance(14);

		if (warpInstance != NULL)
		{
			camera->tfaceTilt = 3988;

			camera->rotationVel.z = 64;

			camera->smooth = 8;

			camera->always_rotate_flag = 1;

			camera->forced_movement = 0;

			tmp = (((((warpInstance->rotation.z & 0xFFF) - 1025) < 0x7FF) ^ 1)) << 11;

			camera->collisionTargetFocusRotation.z = tmp;

			camera->targetFocusRotation.z = tmp;
		}
	}
	else
	{
		if ((camera->flags & 0x2000) || (camera->instance_mode & 0x20000000))
		{
			if (!(camera->instance_mode & 0x2000000))
			{
				camera->rotationVel.z = 64;

				CAMERA_FollowGoBehindPlayer(camera);
			}
		}
		else
		{
			if ((camera->flags & 0x10000) || (camera->instance_mode & 0x4000000))
			{
				CAMERA_FollowGoBehindPlayer(camera);
			}
			else
			{
				if ((camera->instance_mode & 0x400) && !(camera->prev_instance_mode & 0x400))
				{
					tmp = camera->core.rotation.z < 0 ? camera->core.rotation.z + 1023 : camera->core.rotation.z;
					tmp >>= 10;
					tmp <<= 10;

					mod = (short)(camera->core.rotation.z - tmp);

					if (mod >= 513)
					{
						mod = 1024 - mod;
					}
					else
					{
						mod = -mod;
					}

					if (mod < 0)
					{
						tmp = mod;
						tmp = -tmp;
					}
					else
					{
						tmp = mod;
					}

					if (tmp >= 129)
					{
						camera->targetFocusRotation.z = ((mod + camera->core.rotation.z) & 0xFFF);
					}
				}

				CAMERA_FollowGoBehindPlayerWithTimer(camera);
			}
		}
	}

	if (!(camera->flags & 0x1800))
	{
		if ((camera->instance_mode & 0x2000000))
		{
			CAMERA_CombatCamDist(camera);
		}
		else
		{
			combat_cam_distance = camera->targetFocusDistance;
		}

		CAMERA_CalcPosition(&targetCamPos, &camera->focusPoint, &camera->focusRotation, combat_cam_distance);

		camera->data.Follow.hit = CAMERA_DoCameraCollision2(camera, &targetCamPos, 0);
	}

	if ((int)camera->instance_mode < 0)
	{
		camera->collisionTargetFocusDistance = 2000;
	}

	CAMERA_UpdateFocusDistance(camera);

	CAMERA_UpdateFocusTilt(camera);

	CAMERA_UpdateFocusRotate(camera);


	if ((camera->flags & 0x1800))
	{
		_v0 = &camera->targetFocusPoint;
		_v1 = &camera->focusPoint;

		_x1 = _v0->x;
		_y1 = _v0->y;
		_z1 = _v0->z;

		_v1->x = _x1;
		_v1->y = _y1;
		_v1->z = _z1;

		camera->focusDistance = camera->targetFocusDistance;

		if ((camera->flags & 0x1000))
		{
			CAMERA_SetZRotation(camera, camera->teleportZRot);
		}
	}
	else
	{
		target = camera->targetFocusPoint;

		if (!(camera->flags & 0x10000))
		{
			target.z = camera->focusPoint.z;
		}

		CriticalDampPosition(1, &camera->focusPoint, &target, &camera->focusPointVel, &camera->focusPointAccl, camera->maxVel);

		if (!(camera->flags & 0x10000))
		{
			camera->focusPoint.z = CAMERA_update_z_damped(camera, camera->focusPoint.z, camera->targetFocusPoint.z);
		}
	}

	CAMERA_CalcFollowPosition(camera, &camera->focusRotation);

	CAMERA_CalculateLead(camera);

	CAMERA_UpdateFocusRoll(camera);
}

// autogenerated function stub: 
// void /*$ra*/ CAMERA_CinematicProcess(struct Camera *camera /*$s2*/)
void CAMERA_CinematicProcess(struct Camera *camera)
{ // line 3608, offset 0x8001a028
	///* begin block 1 */
	//	// Start line: 3609
	//	// Start offset: 0x8001A028
	//	// Variables:
	//		struct _SVector *camPos; // $s0
	//		struct _SVector *camTarget; // $s3
	//		struct MultiSpline *posSpline; // $s4
	//		struct MultiSpline *targetSpline; // $s1

	//	/* begin block 1.1 */
	//		// Start line: 3632
	//		// Start offset: 0x8001A0A0
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $v1
	//			short _z1; // $a0
	//	/* end block 1.1 */
	//	// End offset: 0x8001A0A0
	//	// End Line: 3632

	//	/* begin block 1.2 */
	//		// Start line: 3636
	//		// Start offset: 0x8001A0BC
	//		// Variables:
	//			short _x1; // $v1
	//			short _y1; // $a0
	//			short _z1; // $v0
	//			struct _SVector *_v1; // $v0
	//	/* end block 1.2 */
	//	// End offset: 0x8001A0D4
	//	// End Line: 3636

	//	/* begin block 1.3 */
	//		// Start line: 3641
	//		// Start offset: 0x8001A0E4
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $v1
	//			short _z1; // $a0
	//			struct _SVector *_v0; // $v0
	//	/* end block 1.3 */
	//	// End offset: 0x8001A0E4
	//	// End Line: 3641

	//	/* begin block 1.4 */
	//		// Start line: 3649
	//		// Start offset: 0x8001A12C
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $v1
	//			short _z1; // $a2
	//			_Position *_v0; // $v0
	//	/* end block 1.4 */
	//	// End offset: 0x8001A12C
	//	// End Line: 3649

	//	/* begin block 1.5 */
	//		// Start line: 3662
	//		// Start offset: 0x8001A18C
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $v1
	//			short _z1; // $a0
	//			_Position *_v0; // $v0
	//	/* end block 1.5 */
	//	// End offset: 0x8001A18C
	//	// End Line: 3662

	//	/* begin block 1.6 */
	//		// Start line: 3710
	//		// Start offset: 0x8001A3E0
	//	/* end block 1.6 */
	//	// End offset: 0x8001A4DC
	//	// End Line: 3727

	//	/* begin block 1.7 */
	//		// Start line: 3730
	//		// Start offset: 0x8001A4DC
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $v1
	//			short _z1; // $a0
	//			struct _Rotation *_v0; // $v0
	//	/* end block 1.7 */
	//	// End offset: 0x8001A4DC
	//	// End Line: 3730
	///* end block 1 */
	//// End offset: 0x8001A4DC
	//// End Line: 3730

	///* begin block 2 */
	//	// Start line: 8584
	///* end block 2 */
	//// End Line: 8585
	UNIMPLEMENTED();
}

int CAMERA_GetDistSq(struct _SVector* point1, struct _SVector* point2)//Matching - 93.55%
{
	struct _Vector d;

	d.x = point1->x - point2->x;
	d.y = point1->y - point2->y;
	d.z = point1->z - point2->z;

	gte_ldlvl(&d);
	gte_sqr0();
	gte_stlvnl(&d);

	return d.x + d.y + d.z;
}

void CAMERA_NearestPointOnLineVec(_SVector* linePoint, _SVector* start, _SVector* line, _Position* point) // Matching - 88.00%
{
	MATRIX nmat;
	struct _Vector dpv;
	long t;

	nmat.m[0][0] = line->x;
	nmat.m[0][1] = line->y;
	nmat.m[0][2] = line->z;
	nmat.m[1][0] = start->x;
	nmat.m[1][1] = start->y;
	nmat.m[1][2] = start->z;
	nmat.m[2][0] = point->x;
	nmat.m[2][1] = point->y;
	nmat.m[2][2] = point->z;

	ApplyMatrix(&nmat, (SVECTOR*)line, (VECTOR*)&dpv);

	t = (dpv.y - dpv.z) * 4096;
	if (dpv.x != 0) {
		t = -t / dpv.x;
	}

	linePoint->x = start->x + (short)(line->x * t >> 12);
	linePoint->y = start->y + (short)(line->y * t >> 12);
	linePoint->z = start->z + (short)(line->z * t >> 12);
}

int CAMERA_CheckPoint(int linePoint, int linept1, int linept2)
{
	if (linept1 < linePoint - 20)
	{
		if (linept2 < linePoint - 20)
		{
			return 1;
		}
	}

	if (linePoint + 20 < linept1)
	{
		if (linePoint + 20 < linept2)
		{
			return 1;
		}
	}

	return 0;
}

int CAMERA_CheckIfPointOnLine(struct _SVector* linePoint, struct _SVector* linept1, struct _SVector* linept2)//Matching - 99.58%
{
	if (!CAMERA_CheckPoint(linePoint->x, linept1->x, linept2->x))
	{
		if (!CAMERA_CheckPoint(linePoint->y, linept1->y, linept2->y))
		{
			return CAMERA_CheckPoint(linePoint->z, linept1->z, linept2->z) == 0;
		}
	}
	else
	{
		return 0;
	}

	return 0;
}


// autogenerated function stub: 
// int /*$ra*/ CAMERA_FindLinePoint(_Position *point /*$s0*/, struct _SVector *linept1 /*$s5*/, struct _SVector *linept2 /*$s6*/, int target_dist_sq /*$s1*/, struct _SVector *results /*stack 16*/)
int CAMERA_FindLinePoint(_Position *point, struct _SVector *linept1, struct _SVector *linept2, int target_dist_sq, struct _SVector *results)
{ // line 3798, offset 0x8001a7c0
	///* begin block 1 */
	//	// Start line: 3799
	//	// Start offset: 0x8001A7C0
	//	// Variables:
	//		struct _SVector outPoint; // stack offset -56
	//		struct _SVector line; // stack offset -48
	//		int calc; // $s1
	//		int hits; // $s4

	//	/* begin block 1.1 */
	//		// Start line: 3799
	//		// Start offset: 0x8001A7C0
	//		// Variables:
	//			short _x0; // $v0
	//			short _y0; // $v1
	//			short _z0; // $a3
	//			short _x1; // $t0
	//			short _y1; // $t1
	//			short _z1; // $t2
	//			struct _SVector *_v; // $s2
	//	/* end block 1.1 */
	//	// End offset: 0x8001A7C0
	//	// End Line: 3799

	//	/* begin block 1.2 */
	//		// Start line: 3813
	//		// Start offset: 0x8001A854
	//		// Variables:
	//			struct _SVector linePoint; // stack offset -40
	//			int n; // $s3

	//		/* begin block 1.2.1 */
	//			// Start line: 3827
	//			// Start offset: 0x8001A908
	//			// Variables:
	//				short _x1; // $v0
	//				short _y1; // $v1
	//				short _z1; // $a0
	//		/* end block 1.2.1 */
	//		// End offset: 0x8001A908
	//		// End Line: 3827
	//	/* end block 1.2 */
	//	// End offset: 0x8001A95C
	//	// End Line: 3833
	///* end block 1 */
	//// End offset: 0x8001A95C
	//// End Line: 3835

	///* begin block 2 */
	//	// Start line: 8994
	///* end block 2 */
	//// End Line: 8995
	UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ CAMERA_SplineGetNearestPoint2(struct Camera *camera /*stack 0*/, struct Spline *spline /*stack 4*/, struct _SVector *point /*stack 8*/, int *currkey /*stack 12*/, struct _SVector *ret_dpoint /*stack 16*/)
void CAMERA_SplineGetNearestPoint2(struct Camera *camera, struct Spline *spline, struct _SVector *point, int *currkey, struct _SVector *ret_dpoint)
{ // line 3838, offset 0x8001a984
	///* begin block 1 */
	//	// Start line: 3839
	//	// Start offset: 0x8001A984
	//	// Variables:
	//		struct SplineKey *key; // stack offset -56
	//		int n; // $s2
	//		int target_dist_sq; // stack offset -52
	//		long dist; // $fp

	//	/* begin block 1.1 */
	//		// Start line: 3839
	//		// Start offset: 0x8001A984
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $v1
	//			short _z1; // $a0
	//	/* end block 1.1 */
	//	// End offset: 0x8001A984
	//	// End Line: 3839

	//	/* begin block 1.2 */
	//		// Start line: 3858
	//		// Start offset: 0x8001AA38
	//		// Variables:
	//			struct _SVector point1; // stack offset -88
	//			struct _SVector point2; // stack offset -80
	//			struct _SVector results[2]; // stack offset -72
	//			int i; // $s1
	//			int hits; // $s3

	//		/* begin block 1.2.1 */
	//			// Start line: 3862
	//			// Start offset: 0x8001AA54
	//			// Variables:
	//				short _x1; // $v1
	//				short _y1; // $a2
	//				short _z1; // $v0
	//				struct _SVector *_v0; // $s7
	//				struct vecS *_v1; // $v0
	//		/* end block 1.2.1 */
	//		// End offset: 0x8001AA54
	//		// End Line: 3862

	//		/* begin block 1.2.2 */
	//			// Start line: 3862
	//			// Start offset: 0x8001AA54
	//			// Variables:
	//				short _x1; // $v0
	//				short _y1; // $a2
	//				short _z1; // $v1
	//				struct _SVector *_v0; // $s6
	//				struct vecS *_v1; // $v1
	//		/* end block 1.2.2 */
	//		// End offset: 0x8001AA54
	//		// End Line: 3862

	//		/* begin block 1.2.3 */
	//			// Start line: 3869
	//			// Start offset: 0x8001AACC
	//			// Variables:
	//				int tmpdist; // $a1

	//			/* begin block 1.2.3.1 */
	//				// Start line: 3880
	//				// Start offset: 0x8001AAE8
	//				// Variables:
	//					short _x1; // $v0
	//					short _y1; // $v1
	//					short _z1; // $a0
	//			/* end block 1.2.3.1 */
	//			// End offset: 0x8001AAE8
	//			// End Line: 3880
	//		/* end block 1.2.3 */
	//		// End offset: 0x8001AB0C
	//		// End Line: 3884
	//	/* end block 1.2 */
	//	// End offset: 0x8001AB1C
	//	// End Line: 3892
	///* end block 1 */
	//// End offset: 0x8001AB3C
	//// End Line: 3893

	///* begin block 2 */
	//	// Start line: 9111
	///* end block 2 */
	//// End Line: 9112
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ CAMERA_SplineGetNearestPoint(struct Spline *spline /*$s4*/, struct _SVector *point /*$s6*/, int *currkey /*$s7*/, struct _SVector *ret_dpoint /*stack 12*/)
void CAMERA_SplineGetNearestPoint(struct Spline *spline, struct _SVector *point, int *currkey, struct _SVector *ret_dpoint)
{ // line 3895, offset 0x8001ab6c
	///* begin block 1 */
	//	// Start line: 3896
	//	// Start offset: 0x8001AB6C
	//	// Variables:
	//		struct _SVector dpoint; // stack offset -96
	//		struct _SVector dpoint2; // stack offset -88
	//		struct _SVector dpoint3; // stack offset -80
	//		struct _SVector point0; // stack offset -72
	//		struct _SVector point1; // stack offset -64
	//		struct _SVector point2; // stack offset -56
	//		struct _SVector point3; // stack offset -48
	//		int current_keyframe; // $s0
	//		int dist; // $fp
	//		int ret; // $s5
	//		int ret2; // $s4
	//		int ret3; // $s0
	//		struct SplineKey *key; // $s1
	//		int circular_spline; // $s2
	//		int prev_key; // $s2
	//		int next_key; // $s3

	//	/* begin block 1.1 */
	//		// Start line: 3921
	//		// Start offset: 0x8001AC98
	//		// Variables:
	//			short _x1; // $v1
	//			short _y1; // $t0
	//			short _z1; // $a3
	//			struct _SVector *_v0; // $a1
	//			struct vecS *_v1; // $a3
	//	/* end block 1.1 */
	//	// End offset: 0x8001AC98
	//	// End Line: 3921

	//	/* begin block 1.2 */
	//		// Start line: 3921
	//		// Start offset: 0x8001AC98
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $t0
	//			short _z1; // $v1
	//			struct _SVector *_v0; // $a2
	//			struct vecS *_v1; // $v1
	//	/* end block 1.2 */
	//	// End offset: 0x8001AC98
	//	// End Line: 3921

	//	/* begin block 1.3 */
	//		// Start line: 3936
	//		// Start offset: 0x8001AD60
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $t0
	//			short _z1; // $v1
	//			struct _SVector *_v0; // $a2
	//			struct vecS *_v1; // $v1
	//	/* end block 1.3 */
	//	// End offset: 0x8001AD60
	//	// End Line: 3936

	//	/* begin block 1.4 */
	//		// Start line: 3949
	//		// Start offset: 0x8001ADC0
	//		// Variables:
	//			short _x1; // $v0
	//			short _y1; // $t0
	//			short _z1; // $v1
	//			struct _SVector *_v0; // $a1
	//			struct vecS *_v1; // $v1
	//	/* end block 1.4 */
	//	// End offset: 0x8001ADC0
	//	// End Line: 3949
	///* end block 1 */
	//// End offset: 0x8001AED4
	//// End Line: 3975

	///* begin block 2 */
	//	// Start line: 9304
	///* end block 2 */
	//// End Line: 9305
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ CAMERA_SplineHelpMove(struct Camera *camera /*$s1*/)
void CAMERA_SplineHelpMove(struct Camera *camera)
{ // line 3981, offset 0x8001af04
	/* begin block 1 */
		// Start line: 3982
		// Start offset: 0x8001AF04
		// Variables:
			struct _SVector camPos; // stack offset -48
			_Position pos; // stack offset -40
			struct _Rotation rotation; // stack offset -32
			struct _Instance *focusInstance; // $s3
			struct MultiSpline *posSpline; // $s2
	/* end block 1 */
	// End offset: 0x8001B058
	// End Line: 4026

	/* begin block 2 */
		// Start line: 9489
	/* end block 2 */
	// End Line: 9490
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ CAMERA_SplineProcess(struct Camera *camera /*$s1*/)
void CAMERA_SplineProcess(struct Camera *camera)
{ // line 4028, offset 0x8001b074
	/* begin block 1 */
		// Start line: 4029
		// Start offset: 0x8001B074
		// Variables:
			struct _SVector camPos; // stack offset -48
			struct _SVector sv; // stack offset -40
			short targetFocusDistance; // $s4
			struct MultiSpline *posSpline; // $s3
			short smooth; // $s0
			short dist_smooth; // $s2
			static short hold_flag; // offset 0x5c
			struct _Rotation targetFocusRotation; // stack offset -32

		/* begin block 1.1 */
			// Start line: 4114
			// Start offset: 0x8001B1C0
			// Variables:
				short _x0; // $v0
				short _y0; // $a1
				short _z0; // $a2
				short _x1; // $v1
				short _y1; // $v1
				short _z1; // $v0
				struct _SVector *_v; // $a0
				struct _SVector *_v1; // $v0
		/* end block 1.1 */
		// End offset: 0x8001B1C0
		// End Line: 4114
	/* end block 1 */
	// End offset: 0x8001B3A8
	// End Line: 4177

	/* begin block 2 */
		// Start line: 9602
	/* end block 2 */
	// End Line: 9603
				UNIMPLEMENTED();
}

void CAMERA_ShakeCamera(struct Camera* camera)
{
	if (camera->shake > 0)
	{
		camera->core.position.x += ((camera_shakeOffset[(camera->shakeFrame & 0xF)].x * camera->shakeScale) >> 12);
		camera->core.position.y += ((camera_shakeOffset[(camera->shakeFrame & 0xF)].y * camera->shakeScale) >> 12);
		camera->core.position.z += ((camera_shakeOffset[(camera->shakeFrame & 0xF)].z * camera->shakeScale) >> 12);

		camera->shake -= gameTrackerX.timeMult;

		if (camera->shake < 0)
		{
			camera->shake = 0;
		}

		camera->shakeFrame++;
	}
}

void CAMERA_Process(struct Camera* camera)
{
	struct _Instance* focusInstance;
	short _x0;
	short _y0;
	short _z0;
	short _x1;
	short _y1;
	short _z1;
	struct _SVector* _v;
	struct _Position* _v0;
	struct _Position* _v1;

	focusInstance = camera->focusInstance;
	
	camera->focuspoint_fallz -= camera->newFocusInstancePos.z - focusInstance->oldPos.z;

	camera->oldFocusInstancePos = camera->newFocusInstancePos;

	camera->newFocusInstancePos = focusInstance->position;

	camera->oldFocusInstanceRot = camera->newFocusInstanceRot;

	camera->newFocusInstanceRot = focusInstance->rotation;

	if (camera->signalPos.x == camera->core.position.x &&
		camera->signalPos.y == camera->core.position.y && 
		camera->signalPos.z == camera->core.position.z)
	{
		camera_still = 1;
	}
	else
	{
		camera_still = 0;
	}

	camera->signalPos.x = camera->core.position.x;
	camera->signalPos.y = camera->core.position.y;
	camera->signalPos.z = camera->core.position.z;
	
	camera->prev_instance_mode = camera->instance_mode;
	camera->x_rot_change = 0;

	camera->instance_mode = CAMERA_QueryMode(camera);

	_v0 = &camera->newFocusInstancePos;
	_v1 = &camera->oldFocusInstancePos;

	_x0 = _v0->x;
	_y0 = _v0->y;
	_z0 = _v0->z;

	_x1 = _v1->x;
	_y1 = _v1->y;
	_z1 = _v1->z;

	_v = &camera->focusInstanceVelVec;
	
	_v->x = _x0 - _x1;
	_v->y = _y0 - _y1;
	_v->z = _z0 - _z1;

	camera->instance_prev_xyvel = camera->instance_xyvel;

	camera->instance_xyvel = MATH3D_FastSqrt0((camera->focusInstanceVelVec.x * camera->focusInstanceVelVec.x) + (camera->focusInstanceVelVec.y * camera->focusInstanceVelVec.y));

	if (shorten_flag == 0)
	{
		shorten_count = 0;
	}

	shorten_flag = 0;

	if (camera->cuckooTimer > 0)
	{
		camera->cuckooTimer--;
	}

	if (camera->mode != 6)
	{
		CAMERA_HandleTransitions(camera);
	}

	switch (camera->mode)
	{
	case 0:
	case 12:
	case 13:
		CAMERA_GenericCameraProcess(camera);
		break;
	case 2:
	case 4:
		CAMERA_SplineProcess(camera);
		break;
	case 5:
		CAMERA_CinematicProcess(camera);
		break;
	case 6:
		CAMERA_LookProcess(camera);
		break;
	default:
		break;
	}

	camera->flags &= 0xFFFFF7FF;
	camera->flags &= 0xFFFFEFFF;

	CAMERA_ShakeCamera(camera);
}

void CAMERA_CenterCamera(struct Camera* camera)//Matching - 99.57%
{
	int tmp1;
	int tmp2;

	if ((camera->instance_mode & 0x2000000))
	{
		tmp1 = (short)CAMERA_AngleDifference(camera->focusRotation.z, (short)(camera->focusInstance->rotation.z + 1024));

		tmp2 = (short)CAMERA_AngleDifference(camera->focusRotation.z, (short)(camera->focusInstance->rotation.z - 1024));

		if (tmp1 < tmp2)
		{
			CenterFlag = camera->focusInstance->rotation.z + 1024;
		}
		else
		{
			CenterFlag = camera->focusInstance->rotation.z - 1024;
		}
	}
	else
	{
		CenterFlag = camera->focusInstance->rotation.z + 2048;
	}

	camera->focusRotAccl.z = 0;

	camera->focusRotVel.z = 0;

	CenterFlag &= 0xFFF;
}

void CAMERA_SetLookRot(struct Camera* camera, int x_rotation, int z_rotation)
{
	camera->lookRot.x = x_rotation;
	camera->lookRot.z = z_rotation;
}

void CAMERA_StartLookaroundMode(struct Camera* camera)//Matching - 98.33%
{
	Camera_lookHeight = 512;
	Camera_lookDist = 650;

	CAMERA_SaveMode(camera, camera->mode);

	camera->mode = 6;

	camera->savedFocusDistance = camera->targetFocusDistance;
	camera->savedFocusRotation = camera->targetFocusRotation;

	camera->targetFocusDistance = 650;

	camera->lookRot.x = 0;
	camera->lookRot.y = 0;
	camera->lookRot.z = 0;

	CAMERA_SetLookFocusAndBase(camera->focusInstance, &camera->targetFocusPoint);
}

void CAMERA_StartSwimThrowMode(struct Camera* camera)
{
	CameraLookStickyFlag = 1;

	CAMERA_StartLookaroundMode(camera);

	PLAYER_SetLookAround(camera->focusInstance);

	camera->lookTimer = 2;
}

void CAMERA_EndSwimThrowMode(struct Camera* camera)
{
	CameraLookStickyFlag = 0;

	CAMERA_EndLook(camera);
}

void CAMERA_ForceEndLookaroundMode(struct Camera* camera)
{
	CameraLookStickyFlag = 0;

	CAMERA_EndLook(camera);

	camera->flags &= 0xFFFDFFFF;
}

void CAMERA_Control(struct Camera* camera, struct _Instance* playerInstance)//Matching - 89.05%
{
	long* controlCommand;
	int lookmode;
	struct _Instance* focusInstance;

	controlCommand = &gameTrackerX.controlCommand[0][0];

	focusInstance = camera->focusInstance;

	if (!(gameTrackerX.streamFlags & 0x100000))
	{
		lookmode = 1;

		if (CameraLookStickyFlag == 0)
		{
			lookmode = ((unsigned)(gameTrackerX.controlCommand[0][0] & 0xC00) ^ 0xC00) < 1;
		}

		camera->last_forced_movement = camera->forced_movement;

		if (camera->instance_xyvel != 0 || camera->forced_movement != 1 || (camera->instance_mode & 0x2000000))
		{
			camera->forced_movement = 0;
		}

		if (camera->mode != 8 && (int)camera->instance_mode >= 0)
		{
			if (CenterFlag != -1)
			{
				if (!(camera->instance_mode & 0x2000000))
				{
					camera->forced_movement = 1;
				}

				CriticalDampAngle(1, &camera->focusRotation.z, CenterFlag, &camera->focusRotVel.z, &camera->focusRotAccl.z, 144);

				camera->collisionTargetFocusRotation.z = camera->focusRotation.z;

				camera->targetFocusRotation.z = camera->focusRotation.z;

				if ((short)CAMERA_AngleDifference(camera->focusRotation.z, CenterFlag) < 8)
				{
					CenterFlag = -1;
				}
			}
			else
			{
				if (gameTrackerX.cheatMode != 1 || !(controlCommand[0] & 0xF))
				{
					if (!(camera->lock & 0x4) && !(camera->flags & 0x10000))
					{
						if (camera->mode == 0 || camera->mode == 12 ||
							camera->mode == 4 || camera->mode == 13)
						{
							if (!(playerInstance->flags & 0x100))
							{
								if ((controlCommand[0] & 0x400) && lookmode == 0)
								{
									if (++camera->leftTimer >= 3)
									{
										camera->rotDirection = -1;

										camera->focusRotation.z = (camera->focusRotation.z - ((gameTrackerX.timeMult * 32) >> 12)) & 0xFFF;

										camera->forced_movement = 1;

										camera->data.Follow.stopTimer = 0xE5A20000;

										camera->focusRotation.z &= 0xFFF;

										camera->collisionTargetFocusRotation.z = camera->focusRotation.z;

										camera->targetFocusRotation.z = camera->focusRotation.z;

										camera->lastModTime = gameTrackerX.frameCount;
									}
								}
								else
								{
									if ((unsigned)((unsigned short)camera->leftTimer - 1) < 3 && lookmode == 0)
									{
										CAMERA_CenterCamera(camera);
									}

									camera->leftTimer = 0;
								}

								if ((controlCommand[0] & 0x800) && lookmode == 0)
								{
									if (++camera->rightTimer >= 3)
									{
										camera->rotDirection = 1;

										camera->forced_movement = 1;

										camera->focusRotation.z = (camera->focusRotation.z + ((gameTrackerX.timeMult * 32) >> 12)) & 0xFFF;

										camera->data.Follow.stopTimer = 0xE5A20000;

										camera->focusRotation.z &= 0xFFF;

										camera->collisionTargetFocusRotation.z = camera->focusRotation.z;

										camera->targetFocusRotation.z = camera->focusRotation.z;

										camera->lastModTime = gameTrackerX.frameCount;
									}
								}
								else
								{
									if ((unsigned)((unsigned short)camera->rightTimer - 1) < 3 && lookmode == 0)
									{
										CAMERA_CenterCamera(camera);
									}
									else
									{
										camera->rightTimer = 0;
									}
								}
							}
							else
							{
								camera->rightTimer = 0;

								camera->leftTimer = 0;
							}
						}
						else
						{
							camera->rightTimer = 0;

							camera->leftTimer = 0;
						}
					}
					else
					{
						camera->rightTimer = 0;

						camera->leftTimer = 0;
					}

					if ((gameTrackerX.debugFlags2 & 0x2000000))
					{
						if (!(camera->lock & 0x1))
						{
							if ((controlCommand[0] & 0x40000008) == 0x40000008)
							{
								camera->targetFocusDistance -= 20;

								if (camera->targetFocusDistance < 200)
								{
									camera->targetFocusDistance = 200;
								}
							}

							if ((controlCommand[0] & 0x40000004) == 0x40000004)
							{
								camera->targetFocusDistance += 20;

								if (camera->targetFocusDistance >= 4097)
								{
									camera->targetFocusDistance = 4096;
								}
							}
						}

						if (!(camera->lock & 0x2))
						{
							if ((controlCommand[0] & 0x40000002) == 0x40000002)
							{
								camera->extraXRot += 16;
							}

							if ((controlCommand[0] & 0x40000001) == 0x40000001)
							{
								camera->extraXRot -= 16;
							}
						}
					}

					if (lookmode != 0 && !(playerInstance->flags & 0x100))
					{
						if ((camera->flags & 0x20000) && camera->mode != 5)
						{
							if (++camera->lookTimer == 2)
							{
								if (PLAYER_OkToLookAround(focusInstance) && !(camera->lock & 0x200))
								{
									CAMERA_StartLookaroundMode(camera);

									PLAYER_SetLookAround(focusInstance);

									camera->collideRotControl = 0;
								}
								else
								{
									camera->lookTimer = 0;
								}
							}
							else if (camera->lookTimer >= 3 && camera->mode == 6)
							{
								PLAYER_TurnHead(focusInstance, &camera->lookRot.x, &camera->lookRot.z, &gameTrackerX);

								camera->collideRotControl = 0;
							}
						}
						else
						{
							camera->collideRotControl = 0;
						}
					}
					else
					{
						camera->flags |= 0x20000;

						CAMERA_EndLook(camera);
					}
				}
			}
		}
	}
}

void CAMERA_EndLook(struct Camera* camera)
{
	struct _Instance* focusInstance;

	focusInstance = camera->focusInstance;

	if (camera->lookTimer >= 2 && camera->mode == 6)
	{
		CAMERA_RestoreMode(camera);

		camera->forced_movement = 0;

		camera->smooth = 8;

		camera->targetFocusDistance = camera->savedFocusDistance;

		camera->targetFocusRotation = camera->savedFocusRotation;

		if (camera->mode != 4 && camera->mode != 2)
		{
			camera->targetFocusRotation.z = (focusInstance->rotation.z + 2048) & 0xFFF;

			CAMERA_CenterCamera(camera);
		}

		camera->always_rotate_flag = 1;

		camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;

		if (!(camera->flags & 0x10000))
		{
			camera->actual_x_rot = camera->core.rotation.x;
		}

		PLAYER_ReSetLookAround(focusInstance);
	}

	camera->lookTimer = 0;
}

void CAMERA_ChangeToUnderWater(struct Camera* camera, struct _Instance* instance) // Matching - 100%
{
	if (instance == camera->focusInstance)
	{
		if (camera->mode != 4)
		{
			if (camera->mode != 5)
			{
				if (camera->mode != 13)
				{
					CAMERA_SetMode(camera, 13);
				}
				else
				{
					CAMERA_CreateNewFocuspoint(camera);
				}
				camera->smooth = 8;
				camera->targetFocusDistance = 1600;
				camera->collisionTargetFocusDistance = 1600;
				camera->signalFocusDistance = 1600;
				camera->rotationVel.z = 32;
				camera->always_rotate_flag = 0;
			}
			else
			{
				CAMERA_SaveMode(camera, 13);
			}
		}
		camera->flags = camera->flags | 0x10000;
	}
}

void CAMERA_ChangeToOutOfWater(struct Camera* camera, struct _Instance* instance) //Matching - 99.48%
{
	if (camera->focusInstance == instance)
	{
		if (camera->mode != 5)
		{
			CAMERA_SetMode(camera, 13);
			
			camera->rotationVel.z = 0;

			camera->targetTilt = 0;
		}
		else
		{
			CAMERA_SaveMode(camera, 13);
		}

		camera->flags &= 0xFFFEFFFF;
	}
}

void CAMERA_UpdateFocusDistance(struct Camera* camera)
{
	int smooth;
	long dampMode;

	if (camera->forced_movement != 2 && !(camera->lock & 0x1))
	{
		if (camera->targetFocusDistance < camera->collisionTargetFocusDistance &&
			!(camera->instance_mode & 0x82000000))
		{
			camera->collisionTargetFocusDistance = camera->targetFocusDistance;
		}

		dampMode = 6;

		if (camera->collisionTargetFocusDistance < camera->focusDistance)
		{
			if (camera->collisionTargetFocusDistance < camera->targetFocusDistance)
			{
				smooth = 512;

				dampMode = 5;
			}
			else
			{
				smooth = 128;

				dampMode = 1;
			}
		}
		else
		{
			smooth = 64;
		}

		CriticalDampValue(dampMode, &camera->focusDistance, camera->collisionTargetFocusDistance, &camera->focusDistanceVel, &camera->focusDistanceAccl, smooth);
	}
}

void CAMERA_UpdateFocusTilt(struct Camera* camera)
{
	if ((camera->flags & 0x1800))
	{
		camera->x_rot_change = 0;
		camera->focusRotation.x = camera->tfaceTilt;
	}
	else
	{
		camera->x_rot_change = camera->focusRotation.x;

		if (camera->forced_movement != 3 && !(camera->lock & 0x2) || (camera->flags & 0x10000))
		{
			CriticalDampAngle(1, &camera->focusRotation.x, camera->tfaceTilt, &camera->focusRotVel.x, &camera->focusRotAccl.x, 32);
		}
		else
		{
			camera->focusRotation.x = camera->targetFocusRotation.x;
		}

		camera->x_rot_change = CAMERA_SignedAngleDifference(camera->x_rot_change, camera->focusRotation.x);
	}
}

void CAMERA_UpdateFocusRoll(struct Camera* camera)
{
	long tmp_inc; // $s0

	//s1 = camera
	//v1 = roll_inc

	if (roll_inc != 0)
	{
		//a0 = gameTrackerX.timeMult
		//v0 = 4096
		tmp_inc = roll_inc;

		if (gameTrackerX.timeMult != 4096 && roll_inc < 0)
		{
			///@FIXME some macro used here?
		}
		//loc_8001C628
	}
	//loc_8001C6C0
#if 0
		bgez    $v1, loc_8001C60C
		move    $v0, $v1
		addiu   $v0, $v1, 0x1F

		loc_8001C60C:
	sra     $v0, 5
		mult    $v0, $a0
		mflo    $v0
		bgez    $v0, loc_8001C628
		sra     $s0, $v0, 7
		addiu   $v0, 0x7F
		sra     $s0, $v0, 7

		loc_8001C628:
	lw      $v0, -0x71B0($gp)
		nop
		bgez    $v0, loc_8001C63C
		nop
		addiu   $v0, 0xFFF

		loc_8001C63C :
		lw      $a1, -0x71B4($gp)
		sll     $v0, 4
		bgez    $a1, loc_8001C650
		sra     $a0, $v0, 16
		addiu   $a1, 0xFFF

		loc_8001C650 :
		sll     $a1, 4
		jal     sub_800166F0
		sra     $a1, 16
		move    $v1, $s0
		sll     $v0, 16
		bgez    $s0, loc_8001C670
		sra     $a0, $v0, 16
		addiu   $v1, $s0, 0xFFF

		loc_8001C670:
	sra     $v0, $v1, 12
		bgez    $v0, loc_8001C680
		nop
		negu    $v0, $v0

		loc_8001C680 :
	slt     $v0, $a0
		bnez    $v0, loc_8001C69C
		nop
		lw      $v0, -0x71B4($gp)
		sw      $zero, -0x71AC($gp)
		j       loc_8001C6A8
		nop

		loc_8001C69C :
	lw      $v0, -0x71B0($gp)
		nop
		addu    $v0, $s0

		loc_8001C6A8 :
	sw      $v0, -0x71B0($gp)
		lui     $v1, 0xFF
		lw      $v0, -0x71B0($gp)
		li      $v1, 0xFFFFFF
		and $v0, $v1
		sw      $v0, -0x71B0($gp)

		loc_8001C6C0:
	lw      $v0, -0x71B0($gp)
		nop
		bgez    $v0, loc_8001C6D4
		nop
		addiu   $v0, 0xFFF

		loc_8001C6D4 :
		sra     $v0, 12
		sh      $v0, 0xB2($s1)
		lw      $ra, 0x10 + var_s8($sp)
		lw      $s1, 0x10 + var_s4($sp)
		lw      $s0, 0x10 + var_s0($sp)
		jr      $ra
		addiu   $sp, 0x20
#endif
}

void CAMERA_UpdateFocusRotate(struct Camera* camera)
{
	int dampspeed;
	long dampmode;
	
	if (camera->forced_movement != 1)
	{
		if (!(camera->lock & 0x4))
		{
			dampmode = 1;

			if (camera->always_rotate_flag != 0)
			{
				dampspeed = -camera->smooth;
			}
			else
			{
				if (!(camera->flags & 0x10000))
				{
					dampspeed = 160;

					dampmode = 5;
				}
				else
				{
					dampspeed = 128;
				}
			}

			CriticalDampAngle(dampmode, &camera->focusRotation.z, camera->collisionTargetFocusRotation.z, &camera->focusRotVel.z, &camera->focusRotAccl.z, dampspeed);
		
			if (CAMERA_AngleDifference(camera->targetFocusRotation.z, camera->focusRotation.z) < 4)
			{
				camera->always_rotate_flag = 0;
			}

		}
		else
		{
			camera->focusRotation.z = camera->targetFocusRotation.z;
		}
	}
	else
	{
		if ((camera->lock & 0x4))
		{
			camera->focusRotation.z = camera->targetFocusRotation.z;
		}
	}
}

void CAMERA_UpdateFocusRotationX(struct Camera* camera, struct _Instance* focusInstance)
{
	short cameraPlayerRotX;
	short dist;
	short tfaceFlag;
	struct _Normal normal;
	int mult;
	int tmpsmooth;

	dist = camera->targetFocusDistance;

	tfaceFlag = 0;

	if (focusInstance->tface != NULL && ((struct Level*)focusInstance->tfaceLevel)->terrain != NULL)
	{
		if (((struct _TFace*)focusInstance->tface)->textoff != 0xFFFF)
		{
			if (((struct TextureFT3*)((char*)((struct Level*)focusInstance->tfaceLevel)->terrain->StartTextureList) + ((struct _TFace*)focusInstance->tface)->textoff)->attr & 0x8000)
			{
				if (dist < 2912)
				{
					COLLIDE_GetNormal(((struct _TFace*)focusInstance->tface)->normal, (short*)((struct Level*)focusInstance->tfaceLevel)->terrain->normalList, (struct _SVector*)&normal);

					if (normal.z < 3950)
					{
						tfaceFlag = 1;

						camera->targetTilt = (short)(CAMERA_CalcTilt(&normal, camera->focusRotation.z) * 9);
						if (camera->targetTilt < 0)
						{
							camera->targetTilt += 15;
						}

						camera->targetTilt >>= 4;

						if (camera->targetTilt < -256)
						{
							camera->targetTilt = -256;
						}
						else if (camera->targetTilt >= 257)
						{
							camera->targetTilt = 256;
						}

						if ((2912 - dist) < 512)
						{
							camera->targetTilt = camera->targetTilt * (2912 - dist);

							if (camera->targetTilt < 0)
							{
								camera->targetTilt += 511;
							}

							camera->targetTilt >>= 9;
						}

						CriticalDampAngle(1, &camera->tilt, camera->targetTilt, &camera->tiltVel, &camera->tiltAccl, 8);
					}
				}
			}
		}
	}

	if (tfaceFlag == 0)
	{
		tmpsmooth = 24;

		if (!(camera->instance_mode & 0x38))
		{
			camera->targetTilt = 0;
		}

		if ((camera->instance_mode & 0x2000))
		{
			if (camera->real_focuspoint.z < camera->focuspoint_fallz)
			{
				camera->targetTilt = -384;

				tmpsmooth = 12;
			}
		}

		CriticalDampAngle(1, &camera->tilt, camera->targetTilt, &camera->tiltVel, &camera->tiltAccl, tmpsmooth);
	}

	cameraPlayerRotX = ((camera->extraXRot + camera->targetFocusRotation.x + camera->tilt) & 0xFFF);

	if ((unsigned)(cameraPlayerRotX - 769) < 1279)
	{
		cameraPlayerRotX = 768;
	}
	else
	{
		if ((unsigned)(cameraPlayerRotX - 2048) < 1280)
		{
			cameraPlayerRotX = -768;
		}
	}

	cameraPlayerRotX = cameraPlayerRotX & 0xFFF;

	camera->tfaceTilt = cameraPlayerRotX;

	if ((camera->instance_mode & 0x2000000))
	{
		if (cameraPlayerRotX >= 2049)
		{
			cameraPlayerRotX |= 0xF000;
		}

		if (combat_cam_angle < cameraPlayerRotX)
		{
			camera->tfaceTilt = combat_cam_angle + 0x1000;
		}
	}
}

// autogenerated function stub: 
// void /*$ra*/ CAMERA_FollowPlayerTilt(struct Camera *camera /*$s1*/, struct _Instance *focusInstance /*$a1*/)
void CAMERA_FollowPlayerTilt(struct Camera *camera, struct _Instance *focusInstance)
{ // line 5178, offset 0x8001c6a0
	/* begin block 1 */
		// Start line: 5179
		// Start offset: 0x8001C6A0
		// Variables:
			int speed; // $s2
			int mode; // $s3
			long focusInstanceStatus; // $a0

		/* begin block 1.1 */
			// Start line: 5203
			// Start offset: 0x8001C724
			// Variables:
				struct _SVector offset; // stack offset -40
				struct _Normal normal; // stack offset -32

			/* begin block 1.1.1 */
				// Start line: 5227
				// Start offset: 0x8001C7DC
				// Variables:
					int waterZ; // $v0
					int target; // $v0
					int height; // $s0

				/* begin block 1.1.1.1 */
					// Start line: 5235
					// Start offset: 0x8001C81C
					// Variables:
						int fdsq; // $v0
				/* end block 1.1.1.1 */
				// End offset: 0x8001C85C
				// End Line: 5243
			/* end block 1.1.1 */
			// End offset: 0x8001C85C
			// End Line: 5244
		/* end block 1.1 */
		// End offset: 0x8001C85C
		// End Line: 5245
	/* end block 1 */
	// End offset: 0x8001C85C
	// End Line: 5250

	/* begin block 2 */
		// Start line: 11914
	/* end block 2 */
	// End Line: 11915
						UNIMPLEMENTED();
}

void CAMERA_FollowGoBehindPlayerWithTimer(struct Camera* camera)
{
	struct _Instance* focusInstance;
	
	focusInstance = camera->focusInstance;
	
	if (camera->data.Follow.hit != 0)
	{
		camera->data.Follow.stopTimer = 0xE5A20000;
	}

	if (CAMERA_FocusInstanceMoved(camera) != 0)
	{
		CameraCenterDelay *= 139264;
	}
	else
	{
		if (!(gameTrackerX.streamFlags & 0x100000))
		{
			camera->data.Follow.stopTimer += gameTrackerX.timeMult;
		}
	}

	if (camera->data.Follow.stopTimer > 0)
	{
		Decouple_AngleMoveToward(&camera->targetFocusRotation.z, focusInstance->rotation.z + 2048,  32);

		Decouple_AngleMoveToward(&camera->collisionTargetFocusRotation.z, focusInstance->rotation.z + 2048, 32);

		CriticalDampAngle(1, &camera->focusRotation.z, camera->collisionTargetFocusRotation.z, &camera->focusRotVel.z, &camera->focusRotAccl.z, 32);
	
		camera->forced_movement = 1;
	}
}

void CAMERA_FollowGoBehindPlayer(struct Camera* camera)
{
	struct _Instance* focusInstance;

	focusInstance = camera->focusInstance;

	Decouple_AngleMoveToward(&camera->targetFocusRotation.z, focusInstance->rotation.z + 2048, camera->rotationVel.z);
}

void CAMERA_CalculateLead(struct Camera* camera)
{
	short lead_target;
	short lead_smooth;
	int speedxy;
	short angle;
	int calc;

	if (!(camera->flags & 0x10000) && camera->mode != 6 && !(camera->instance_mode & 0x3002043))
	{
		speedxy = camera->instance_xyvel;

		angle = CAMERA_SignedAngleDifference((short)(camera->core.rotation.z + 2048), camera->focusInstance->rotation.z);

		if (speedxy >= 0x17)
		{
			camera->lead_timer++;
		}
		else
		{
			if (camera->instance_prev_xyvel == 0)
			{
				camera->lead_timer = 0;
			}
		}

		lead_target = 0;

		if (speedxy >= 0x17)
		{
			if (angle < 0)
			{
				angle = -angle;
			}

			lead_smooth = 3;

			if ((unsigned)(angle - 401) < 1199)
			{
				lead_smooth = camera->lead_timer;

				if (lead_smooth >= 0x24)
				{
					lead_smooth = ((lead_smooth - 0x23) * 3) + 0x23;
				}

				if (angle > 0)
				{
					lead_target = 80;

					if (lead_smooth < 80)
					{
						lead_target = lead_smooth;
					}
				}
				else
				{
					lead_target = -lead_smooth;

					if (lead_smooth >= 80)
					{
						lead_target = -80;
					}
				}

				if ((short)CAMERA_AngleDifference(lead_target, camera->lead_angle) >= 0x51)
				{
					lead_smooth = 12;
				}
				else
				{
					lead_smooth = 8;
				}
			}
		}
		else
		{
			lead_smooth = 3;
		}
	}
	else
	{
		camera->lead_timer = 0;

		lead_target = 0;

		lead_smooth = 3;
	}

	CriticalDampAngle(1, &camera->lead_angle, lead_target, &camera->lead_vel, &camera->lead_accl, lead_smooth);

	calc = camera->lead_angle;

	camera->core.rotation.z = (camera->core.rotation.z + calc) & 0xFFF;
}


void CAMERA_CalcFollowPosition(struct Camera* camera, struct _Rotation* rotation)
{ 
	struct _Instance* focusInstance;
	short _x1;
	short _y1;
	short _z1;
	struct _Rotation* _v0;
	short target_rotx;
	int hypotXY;
	int smooth;
	int diff;
	struct _Vector dpv;
	int zdiff;
	int velz;
	int ground;
	int pos;

	focusInstance = camera->focusInstance;

	CAMERA_CalcPosition(&camera->targetPos, &camera->focusPoint, rotation, camera->focusDistance);

	camera->core.position.x = camera->targetPos.x;
	camera->core.position.y = camera->targetPos.y;
	camera->core.position.z = camera->targetPos.z;

	_x1 = rotation->x;
	_y1 = rotation->y;
	_z1 = rotation->z;

	_v0 = &camera->core.rotation;

	_v0->x = _x1;
	_v0->y = _y1;
	_v0->z = _z1;

	if (!(camera->flags & 0x10000))
	{
		camera->actual_x_rot -= camera->x_rot_change;

		dpv.x = camera->real_focuspoint.x - camera->targetPos.x;
		dpv.z = 0;
		dpv.y = camera->real_focuspoint.y - camera->targetPos.y;

		gte_ldlvl(&dpv);

		gte_sqr0();

		gte_stlvnl(&dpv);

		hypotXY = MATH3D_FastSqrt0(dpv.x + dpv.y);

		diff = ratan2(camera->real_focuspoint.z - camera->targetPos.z, hypotXY);

		target_rotx = diff;

		if ((camera->instance_mode & 0x1038))
		{
			velz = camera->focusInstanceVelVec.z;

			if (focusInstance->shadowPosition.z != focusInstance->position.z)
			{
				if (velz < 0)
				{
					if (velz < -260)
					{
						if (-520 - velz > 0)
						{
							velz = 0;
						}
					}

					pos = camera->real_focuspoint.z + (velz * 2);

					ground = focusInstance->shadowPosition.z + 352;

					if (pos < ground)
					{
						ground -= camera->targetPos.z;
					}
					else
					{
						ground = pos - camera->targetPos.z;
					}

					target_rotx = (short)ratan2(ground, hypotXY);

					if (CAMERA_SignedAngleDifference(target_rotx, camera->actual_x_rot) < 0)
					{
						target_rotx = camera->actual_x_rot;
					}
				}

				target_rotx = camera->core.rotation.x;
			}
		}
		else
		{
			if (CAMERA_AngleDifference(target_rotx, camera->core.rotation.x) < 4)
			{
				target_rotx = camera->core.rotation.x;
			}
		}

		if ((camera->flags & 0x1800))
		{
			camera->actual_x_rot = target_rotx;
			camera->x_rot_change = 0;
		}

		zdiff = CAMERA_SignedAngleDifference(target_rotx, camera->actual_x_rot);

		if ((camera->instance_mode & 0x2000))
		{
			if (zdiff >= 81)
			{
				smooth = zdiff - 80 / 3;

				if (smooth >= 4)
				{
					if (smooth >= 25)
					{
						smooth = 24;
					}
					else
					{
						smooth = 4;
					}
				}
				else
				{
					smooth = 4;
				}
			}
		}
		else
		{
			smooth = 24;
		}

		if (smooth != 0)
		{
			CriticalDampAngle(1, &camera->actual_x_rot, target_rotx, &camera->actual_vel_x, &camera->actual_acc_x, smooth);
		}
		else
		{
			camera->actual_acc_x = 0;
			camera->actual_vel_x = 0;
		}

		camera->core.rotation.x = camera->actual_x_rot;
	}
	else
	{
		camera->actual_x_rot = camera->core.rotation.x;
	}

	camera->lagZ = camera->core.rotation.z;
}

static inline void CAMERA_SetupColInfo_CopyPosition(struct _Position* _v1, struct _Position* _v0)
{
	short _x1, _y1, _z1;

	_x1 = _v1->x;
	_y1 = _v1->y;
	_z1 = _v1->z;

	_v0->x = _x1;
	_v0->y = _y1;
	_v0->z = _z1;
}

void CAMERA_SetupColInfo(struct Camera* camera, struct CameraCollisionInfo* colInfo, struct _Position* targetCamPos) // Matching - 99.58%
{
	static short toggle = 0;

	if (camera->mode == 6)
	{
		CAMERA_SetupColInfo_CopyPosition(&camera->targetFocusPoint, &camera->focusSphere.position);
	}
	else
	{
		CAMERA_SetupColInfo_CopyPosition(&camera->real_focuspoint, &camera->focusSphere.position);

	}

	CAMERA_SetupColInfo_CopyPosition(targetCamPos, &camera->posSphere.position);

	colInfo->start = &camera->focusSphere;
	colInfo->end = &camera->posSphere;

	if (camera->data.Follow.tface == NULL)
	{
		colInfo->cldLines = 6;

		if (toggle != 0)
		{
			colInfo->cldLines = 14;
		}
		else
		{
			colInfo->cldLines = 22;
		}
	}
	else
	{
		colInfo->cldLines = 30;
	}

	if (toggle != 0)
	{
		toggle = 0;
	}
	else
	{
		toggle = 1;
	}
}

void CAMERA_DoPanicCheck(struct Camera* camera, struct CameraCollisionInfo* tmpcolInfo, struct _Rotation* rotation, short* best_z, short* max_dist) // Matching - 99.81%
{
	struct _Position targetCamPos;
	short _x1, _y1, _z1;
	struct _Position* _v0;
	struct _Position* _v1;

	CAMERA_CalcPosition(&targetCamPos, &camera->focusPoint, rotation, camera->targetFocusDistance);

	_v0 = &targetCamPos;
	_v1 = &camera->posSphere.position;

	_x1 = _v0->x;
	_y1 = _v0->y;
	_z1 = _v0->z;

	_v1->x = _x1;
	_v1->y = _y1;
	_v1->z = _z1;

	CAMERA_SphereToSphereWithLines(camera, tmpcolInfo, 0);

	if ((tmpcolInfo->numCollided == 0) || ((int)*max_dist < tmpcolInfo->lenCenterToExtend))
	{
		*best_z = rotation->z;
		*max_dist = *(short*)&tmpcolInfo->lenCenterToExtend;
	}
}

void CAMERA_Panic(struct Camera* camera, short min_dist) // Matching - 99.52%
{
	struct _Position targetCamPos;
	struct _Rotation rotation;
	int n;
	short best_z;
	short max_dist;
	struct CameraCollisionInfo tmpcolInfo;
	short free_count1;
	short free_count2;

	free_count1 = 0;
	free_count2 = 0;
	max_dist = min_dist;
	CAMERA_SetupColInfo(camera, &tmpcolInfo, &targetCamPos);
	n = 0;
	rotation = camera->focusRotation;
	best_z = rotation.z;
	while (1)
	{
		rotation.z = camera->focusRotation.z + n;
		CAMERA_DoPanicCheck(camera, &tmpcolInfo, &rotation, &best_z, &max_dist);

		if ((tmpcolInfo.numCollided == 0) && (free_count1++, 2 < free_count1 * 0x10000 >> 0x10))
		{
			break;
		}

		rotation.z = camera->focusRotation.z - n;
		CAMERA_DoPanicCheck(camera, &tmpcolInfo, &rotation, &best_z, &max_dist);

		if ((tmpcolInfo.numCollided == 0) && (free_count2++, 2 < free_count2 * 0x10000 >> 0x10))
		{
			break;
		}

		n += 0x80;
		if (n > 0x7FF)
		{
			break;
		}
	}
	if (max_dist == min_dist)
	{
		panic_count = -0x7fff;
	}
	else
	{
		camera->always_rotate_flag = 1;
		camera->rotState = 3;
		camera->smooth = -0x70;
		camera->targetFocusRotation.z = best_z;
		camera->collisionTargetFocusRotation.z = best_z;
		camera->signalRot.z = best_z;
	}
}

static inline int GetSecondCheckFlag(struct Camera* camera)
{
	if ((camera->flags & 0x10000U) != 0)
	{
		return 0;
	}

	if (camera->real_focuspoint.z - camera->targetFocusPoint.z >= 0)
	{
		if (camera->real_focuspoint.z - camera->targetFocusPoint.z < 5)
		{
			return 0;
		}
	}
	else if (camera->targetFocusPoint.z - camera->real_focuspoint.z < 5)
	{
		return 0;
	}

	return 1;
}

long CAMERA_DoCameraCollision2(struct Camera* camera, _Position* targetCamPos, int simpleflag) // Matching - 99.75%
{
	int secondcheck_flag;
	long hit;
	struct CameraCollisionInfo colInfo;
	static int collisiontimeDown = 0;
	short _x1;
	short _y1;
	short _z1;
	struct _Position* _v0;
	struct _Position* _v1;
	int speed;
	int angle1;
	int angle2;

	hit = 0;
	CAMERA_SetupColInfo(camera, &colInfo, targetCamPos);
	secondcheck_flag = GetSecondCheckFlag(camera);
	camera->data.Follow.tface = CAMERA_SphereToSphereWithLines(camera, &colInfo, secondcheck_flag);
	if ((camera->instance_mode & 0x2000000) != 0 && 0 < colInfo.numCollided && colInfo.lenCenterToExtend < 600)
	{
		CenterFlag = -1;
		if (colInfo.numCollided != 1 || (colInfo.flags & 6) == 0)
		{
			if (combat_cam_weight < 0x1000)
			{
				combat_cam_weight += 0x90;

				if (0xfff < combat_cam_weight)
				{
					combat_cam_weight = 0x1000;
				}

				if (combat_cam_weight < 0xf3c)
				{
					return 1;
				}
			}
		}
	}

	if (((camera->flags & 0x12000U) == 0 && camera->instance_xyvel == 0) &&
		((((secondcheck_flag == 0 &&
			(((camera->always_rotate_flag == 0 && camera->forced_movement == 0) && 0 < colInfo.numCollided)))
			&& (colInfo.numCollided == 4 || camera_still != 0)) && colInfo.lenCenterToExtend < 400)))
	{
		panic_count++;
		if (((gameTrackerX.controlCommand[0][0] & 1U) != 0 && (short)panic_count > 10) ||
			((gameTrackerX.controlCommand[0][0] & 1U) == 0 && (short)panic_count > 1))
		{
			CAMERA_Panic(camera, (short)colInfo.lenCenterToExtend);
		}
	}
	else
	{
		panic_count = 0;
	}

	if (camera->data.Follow.tface != NULL && secondcheck_flag != 0)
	{
		_v0 = &camera->targetFocusPoint;
		_x1 = _v0->x;
		_y1 = _v0->y;
		_z1 = _v0->z;
		_v1 = &camera->focusSphere.position;
		_v1->x = _x1;
		_v1->y = _y1;
		_v1->z = _z1;
		camera->data.Follow.tface = CAMERA_SphereToSphereWithLines(camera, &colInfo, 0);
	}

	if (simpleflag != 0)
	{
		if ((camera->data).Follow.tface != NULL)
		{
			if (camera->mode == 4 || camera->mode == 2 || camera->mode == 6)
			{
				camera->collisionTargetFocusDistance = colInfo.lengthList[colInfo.line];
			}

			return 1;
		}

		return 0;
	}

	if (0 < collisiontimeDown)
	{
		collisiontimeDown--;
	}

	if (camera->always_rotate_flag != 0)
	{
		colInfo.numCollided = 4;
	}

	if (((camera->flags & 0x10000U) != 0 || (camera->instance_mode & 0x24000000) != 0) ||
		((camera->flags & 0x2000U) != 0 && (camera->instance_mode & 0x2000000) == 0) ||
		camera->always_rotate_flag != 0)
	{
		if ((camera->data).Follow.tface != NULL)
		{
			hit = CAMERA_AbsoluteCollision(camera, &colInfo);
			collisiontimeDown = 0x1e;
		}
		else
		{
			camera->collisionTargetFocusDistance = camera->targetFocusDistance;
		}

		if ((camera->flags & 0x10000U) != 0)
		{
			if ((AngleDiff(camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z) << 0x10) >= 0)
			{
				speed = AngleDiff(camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z);
			}
			else
			{
				speed = -AngleDiff(camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z);
			}

			if ((colInfo.flags & 6) == 0)
			{
				CAMERA_dampgetline(0);
			}

			if (speed < 0x400 || (colInfo.flags & 6) != 0)
			{
				speed /= 16;

				if (speed < 8)
				{
					speed = 8;
				}

				if (0x20 < speed)
				{
					speed = 0x20;
				}
			}
			else
			{
				speed = 0x40;
			}

			Decouple_AngleMoveToward(&camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z, speed);
			return hit;
		}
		else
		{
			Decouple_AngleMoveToward(&camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z, 0x40);
			return hit;
		}
	}

	if (camera->data.Follow.tface != NULL)
	{
		hit = CAMERA_AbsoluteCollision(camera, &colInfo);
		collisiontimeDown = 0x1e;
		camera->targetFocusRotation.z = camera->collisionTargetFocusRotation.z;
	}
	else
	{
		CAMERA_dampgetline(0);

		if ((camera->mode == 0xd) && (0 < camera->instance_xyvel))
		{
			if (CAMERA_AngleDifference(camera->collisionTargetFocusRotation.z, camera->focusRotation.z) < 5)
			{
				camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;
				if (collisiontimeDown == 0)
				{
					camera->collision_lastPush = 0;
				}
			}
			else
			{
				angle1 = CAMERA_SignedAngleDifference(camera->collisionTargetFocusRotation.z, camera->focusRotation.z);
				angle2 = CAMERA_SignedAngleDifference(camera->targetFocusRotation.z, camera->focusRotation.z);
				if ((angle1 < 0 && angle2 < 0) || (angle1 > 0 && angle2 > 0))
				{
					if (abs(angle2) > abs(angle1))
					{
						camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;
					}
				}
			}
		}
		else if (collisiontimeDown == 0)
		{
			Decouple_AngleMoveToward(&camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z, 0x40);
			camera->collision_lastPush = 0;
		}

		if ((camera->instance_mode & 0x2000000) != 0)
		{
			collisiontimeDown = 0;
			camera->collisionTargetFocusDistance = combat_cam_distance;
		}
		else
		{
			camera->collisionTargetFocusDistance = camera->targetFocusDistance;
		}
	}

	return hit;
}

int CAMERA_FocusInstanceMoved(struct Camera* camera)//Matching - 100%
{
	return (camera->newFocusInstancePos.x != camera->oldFocusInstancePos.x ||
		camera->newFocusInstancePos.y != camera->oldFocusInstancePos.y ||
		camera->newFocusInstancePos.z != camera->oldFocusInstancePos.z ||
		camera->newFocusInstanceRot.x != camera->oldFocusInstanceRot.x ||
		camera->newFocusInstanceRot.y != camera->oldFocusInstanceRot.y ||
		camera->newFocusInstanceRot.z != camera->oldFocusInstanceRot.z);
}
