#ifndef CAMERA_H
#define CAMERA_H

#include "CORE.H"

struct _CameraCore_Type 
{
	struct _Position position; // size=6, offset=0
	short pad1; // size=0, offset=6
	//struct MATRIX vvNormalWorVecMat[2]; // size=64, offset=8
	MATRIX vvNormalWorVecMat[2]; // size=64, offset=8
	long vvPlaneConsts[6]; // size=24, offset=72
	//struct MATRIX* wcTransform; // size=32, offset=96
	MATRIX* wcTransform; // size=32, offset=96
	long farPlane; // size=0, offset=100
	long projDistance; // size=0, offset=104
	long nearPlane; // size=0, offset=108
	//struct MATRIX* wcTransform2; // size=32, offset=112
	MATRIX* wcTransform2; // size=32, offset=112
	//struct MATRIX* cwTransform2; // size=32, offset=116
	MATRIX* cwTransform2; // size=32, offset=116
	struct _Normal viewVolumeNormal[5]; // size=40, offset=120
	int leftX; // size=0, offset=160
	int rightX; // size=0, offset=164
	int topY; // size=0, offset=168
	int bottomY; // size=0, offset=172
	struct _Rotation rotation; // size=8, offset=176
	short pad2; // size=0, offset=184
	struct _SVector screenScale; // size=8, offset=186
	short pad3; // size=0, offset=194
	struct _Position debugPos; // size=6, offset=196
	short pad4; // size=0, offset=202
	struct _Rotation debugRot; // size=8, offset=204
	short pad5; // size=0, offset=212
	struct _Position nondebugPos; // size=6, offset=214
	short pad6; // size=0, offset=220
};

struct _CameraKey 
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short id; // size=0, offset=6
	short rx; // size=0, offset=8
	short ry; // size=0, offset=10
	short rz; // size=0, offset=12
	short flags; // size=0, offset=14
	short tx; // size=0, offset=16
	short ty; // size=0, offset=18
	short tz; // size=0, offset=20
	short pad; // size=0, offset=22
};

struct _SavedCinematic 
{
	struct _Position position; // size=6, offset=0
	struct _Position focusPoint; // size=6, offset=6
	struct _Position base; // size=6, offset=12
	struct _Position targetPos; // size=6, offset=18
	struct _Position targetFocusPoint; // size=6, offset=24
	struct _Position targetBase; // size=6, offset=30
	struct _Rotation baseRot; // size=8, offset=36
	struct _Rotation focusRotation; // size=8, offset=44
	struct _Rotation targetFocusRotation; // size=8, offset=52
	short focusDistance; // size=0, offset=60
	short targetFocusDistance; // size=0, offset=62
	struct _SVector focusPointVel; // size=8, offset=64
	struct _SVector focusPointAccl; // size=8, offset=72
	struct _SVector baseVel; // size=8, offset=80
	struct _SVector baseAccl; // size=8, offset=88
	long maxVel; // size=0, offset=96
	struct MultiSpline* posSpline; // size=0, offset=100
	struct MultiSpline* targetSpline; // size=0, offset=104
	struct Level* level; // size=0, offset=108
};

struct CameraCollisionInfo 
{
	struct _TFace* tfaceList[5]; // size=20, offset=0
	struct _Terrain* tfaceTerrain[5]; // size=20, offset=20
	short bspTree[6]; // size=12, offset=40
	short lengthList[6]; // size=12, offset=52
	long flags; // size=0, offset=64
	long line; // size=0, offset=68
	long numCollided; // size=0, offset=72
	long lenCenterToExtend; // size=0, offset=76
	struct _Sphere* start; // size=12, offset=80
	struct _Sphere* end; // size=12, offset=84
	long cldLines; // size=0, offset=88
};

// @fixme needs updating
struct Camera // hashcode: 0x0670C20F (dec: 108053007)
{
	struct _CameraCore_Type core; // size=224, offset=0
	long shake; // size=0, offset=224
	short shakeFrame; // size=0, offset=228
	short shakeScale; // size=0, offset=230
	long flags; // size=0, offset=232
	long lock; // size=0, offset=236
	short mode; // size=0, offset=240
	short state; // size=0, offset=242
	short posState; // size=0, offset=244
	short rotState; // size=0, offset=246
	short tiltState; // size=0, offset=248
	short distanceState; // size=0, offset=250
	short lagZ; // size=0, offset=252
	short tilt; // size=0, offset=254
	_Position focusPoint; // size=6, offset=256
	short focusDistance; // size=0, offset=262
	struct _Instance* focusInstance; // size=616, offset=264
	_Position oldFocusInstancePos; // size=6, offset=268
	_Position newFocusInstancePos; // size=6, offset=274
	struct _Rotation oldFocusInstanceRot; // size=8, offset=280
	struct _Rotation newFocusInstanceRot; // size=8, offset=288
	struct _SVector focusInstanceVelVec; // size=8, offset=296
	_Position focusLeadPivot; // size=6, offset=304
	_Position focusOffset; // size=6, offset=310
	struct _Rotation focusRotation; // size=8, offset=316
	short tiltVel; // size=0, offset=324
	short tiltAccl; // size=0, offset=326
	struct _SVector positionVel; // size=8, offset=328
	struct _SVector positionAccl; // size=8, offset=336
	struct _SVector focusPointVel; // size=8, offset=344
	struct _SVector focusPointAccl; // size=8, offset=352
	short focusDistanceVel; // size=0, offset=360
	short focusDistanceAccl; // size=0, offset=362
	struct _SVector focusRotVel; // size=8, offset=364
	struct _SVector focusRotAccl; // size=8, offset=372
	struct _SVector maxFocusVel; // size=8, offset=380
	struct _SVector rotationVel; // size=8, offset=388
	struct _SVector rotationAccl; // size=8, offset=396
	short maxVel; // size=0, offset=404
	short maxRot; // size=0, offset=406
	_Position targetPos; // size=6, offset=408
	short targetTilt; // size=0, offset=414
	struct _Rotation targetRotation; // size=8, offset=416
	short targetFocusDistance; // size=0, offset=424
	_Position targetFocusPoint; // size=6, offset=426
	short collisionTargetFocusDistance; // size=0, offset=432
	struct _Rotation targetFocusRotation; // size=8, offset=434
	short tfaceTilt; // size=0, offset=442
	struct _Rotation collisionTargetFocusRotation; // size=8, offset=444
	short smooth; // size=0, offset=452
	struct _CameraKey* cameraKey; // size=24, offset=456
	struct _Sphere focusSphere; // size=12, offset=460
	struct _Sphere smallBaseSphere; // size=12, offset=472
	struct _Sphere posSphere; // size=12, offset=484
	short lookTimer; // size=0, offset=496
	short lookSavedMode; // size=0, offset=498
	short presetIndex; // size=0, offset=500
	short presetFocusDistance[3]; // size=6, offset=502
	long minFocusDistance; // size=0, offset=508
	long maxFocusDistance; // size=0, offset=512
	long minTilt; // size=0, offset=516
	long maxTilt; // size=0, offset=520
	long pad[3]; // size=12, offset=524
	long minXYDist; // size=0, offset=536
	long maxXYDist; // size=0, offset=540
	long oppositeError; // size=0, offset=544
	long tempsplinePointAhead; // size=0, offset=548
	long noTeleport; // size=0, offset=552
	long focusDistanceList[3][3]; // size=36, offset=556
	long tiltList[3][3]; // size=36, offset=592
	long cineControl; // size=0, offset=628
	short stack; // size=0, offset=632
	short targetStack; // size=0, offset=634
	long savedMode[3]; // size=12, offset=636
	long savedTargetFocusDistance[3]; // size=12, offset=648
	struct _Rotation savedFocusRotation; // size=8, offset=660
	short savedFocusDistance; // size=0, offset=668
	short signalFocusDistance; // size=0, offset=670
	struct _SavedCinematic savedCinematic[3]; // size=336, offset=672
	struct _Rotation savedfocusRotation; // size=8, offset=1008
	_Position signalPos; // size=6, offset=1016
	short pad11; // size=0, offset=1022
	struct _Rotation signalRot; // size=8, offset=1024
	short forced_movement; // size=0, offset=1032
	short last_forced_movement; // size=0, offset=1034
	short rotDirection; // size=0, offset=1036
	unsigned long lastModTime; // size=0, offset=1040
	struct _Rotation lookRot; // size=8, offset=1044
	struct MultiSpline* Spline00; // size=0, offset=1052
	struct MultiSpline* Spline01; // size=0, offset=1056
	struct {
		struct {
			struct MultiSpline* posSpline; // size=0, offset=0
			struct MultiSpline* targetSpline; // size=0, offset=4
			struct _SVector lastSplinePos; // size=8, offset=8
			short splinePointAhead; // size=0, offset=16
			short cinema_done; // size=0, offset=18
			int splinecam_currkey; // size=0, offset=20
			int splinecam_helpkey; // size=0, offset=24
		} Cinematic; // size=28, offset=0, found in object files: debug.o, debug.o
		struct {
			_Position offset; // size=6, offset=0
			struct _Rotation rotation; // size=8, offset=6
			long focusDistance; // size=0, offset=16
		} Idle; // size=20, offset=28, found in object files: debug.o, debug.o
		struct {
			struct _TFace* tface; // size=12, offset=0
			struct _TFace* oldTFace; // size=12, offset=4
			struct _SVector oldTargetRot; // size=8, offset=8
			long timer; // size=0, offset=16
			struct _SVector oldFocusInstancePos; // size=8, offset=20
		} Behind; // size=28, offset=48, found in object files: debug.o, debug.o
		struct {
			long stopTimer; // size=0, offset=0
			long stopHit; // size=0, offset=4
			struct _TFace* tface; // size=12, offset=8
			struct _TFace* oldTFace; // size=12, offset=12
			long hit; // size=0, offset=16
		} Follow; // size=20, offset=76, found in object files: debug.o, debug.o
	} data; // size=96, offset=1060, found in object files: debug.o, debug.o

	long cuckooTimer; // size=0, offset=1156
	short teleportZRot; // size=0, offset=1160
	short extraZRot; // size=0, offset=1162
	short extraXRot; // size=0, offset=1164
	short leftTimer; // size=0, offset=1166
	short rightTimer; // size=0, offset=1168
	short collideRotControl; // size=0, offset=1170
	short collision_lastPush; // size=0, offset=1172
	short collision_timer; // size=0, offset=1174
	unsigned long prev_instance_mode; // size=0, offset=1176
	unsigned long instance_mode; // size=0, offset=1180
	long instance_xyvel; // size=0, offset=1184
	long instance_prev_xyvel; // size=0, offset=1188
	_Position real_focuspoint; // size=6, offset=1192
	short always_rotate_flag; // size=0, offset=1198
	short follow_flag; // size=0, offset=1200
	short actual_x_rot; // size=0, offset=1202
	short actual_vel_x; // size=0, offset=1204
	short actual_acc_x; // size=0, offset=1206
	short x_rot_change; // size=0, offset=1208
	short focuspoint_fallz; // size=0, offset=1210
	short lead_angle; // size=0, offset=1212
	short lead_vel; // size=0, offset=1214
	short lead_accl; // size=0, offset=1216
	short lead_timer; // size=0, offset=1218
};

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern void CAMERA_CalculateViewVolumeNormals(struct Camera *camera); // 0x800146E8
extern void CAMERA_CalcVVClipInfo(struct Camera *camera); // 0x80014A24
extern void CAMERA_SetViewVolume(struct Camera *camera); // 0x80014B44
extern void CAMERA_SetProjDistance(struct Camera *camera, long distance); // 0x80014B70
extern void CAMERA_CreateNewFocuspoint(struct Camera *camera); // 0x80014C14
extern void CAMERA_SaveMode(struct Camera *camera, long mode); // 0x80014CB8
extern void CAMERA_RestoreMode(struct Camera *camera); // 0x80014FF0
extern void CAMERA_Save(struct Camera *camera, long save); // 0x80015498
extern void CAMERA_Restore(struct Camera *camera, long restore); // 0x80015568
extern struct _SVector * SplineGetNextPointDC(struct Spline *spline, struct SplineDef *def); // 0x80015624
extern void CAMERA_SetMode(struct Camera *camera, long mode); // 0x80015680
extern void CAMERA_Initialize(struct Camera *camera); // 0x80015B30
extern void CAMERA_SetInstanceFocus(struct Camera *camera, struct _Instance *instance); // 0x80015E30
extern void CAMERA_SetZRotation(struct Camera *camera, short zrot); // 0x80015E90
extern long CAMERA_LengthSVector(struct _SVector *sv); // 0x80015EB4
extern void CAMERA_SetValue(struct Camera *camera, long index, long value); // 0x80015F08
extern short CAMERA_AngleDifference(short angle0, short angle1); // 0x80016334
extern short CAMERA_SignedAngleDifference(short angle0, short angle1); // 0x800163AC
extern unsigned long CAMERA_QueryMode(struct Camera *camera); // 0x800163DC
extern void CAMERA_SetMaxVel(struct Camera *camera); // 0x80016478
extern void CAMERA_SetTarget(struct Camera *camera, _Position *pos); // 0x80016584
extern void CAMERA_CalcPosition(_Position *position, _Position *base, struct _Rotation *rotation, short distance); // 0x80016600
extern void CAMERA_SetFocus(struct Camera *camera, _Position *targetfocusPoint); // 0x80016728
extern void CAMERA_Lock(struct Camera *camera, long lock); // 0x800169A4
extern void CAMERA_Unlock(struct Camera *camera, long unlock); // 0x800169B8
extern void CAMERA_SetSmoothValue(struct Camera *camera, long smooth); // 0x800169CC
extern void CAMERA_SetTimer(struct Camera *camera, long time); // 0x80016A1C
extern void CAMERA_Adjust_tilt(struct Camera *camera, long tilt); // 0x80016A3C
extern void CAMERA_Adjust_distance(struct Camera *camera, long dist); // 0x80016A78
extern void CAMERA_Adjust_rotation(struct Camera *camera, long rotation); // 0x80016AF4
extern void CAMERA_Adjust_roll(long roll_degrees, int frames); // 0x80016B78
extern void CAMERA_Adjust(struct Camera *camera, long adjust); // 0x80016BFC
extern void CAMERA_ChangeTo(struct Camera *camera, struct _CameraKey *cameraKey); // 0x80016CBC
extern void CAMERA_SetShake(struct Camera *camera, long shake, long scale); // 0x80016CC4
extern void Decouple_AngleMoveToward(short *current_ptr, short destination, short step); // 0x80016D74
extern void CriticalDampValue(long dampMode, short *sourceVal, short targetVal, short *vel, short *accl, int smooth); // 0x80016DC4
extern void CriticalDampPosition(long dampMode, _Position *position, _Position *targetPos, struct _SVector *vel, struct _SVector *accl, int smooth); // 0x80016F78
extern void CriticalDampAngle(long dampMode, short *currentVal, short target, short *vel, short *accl, int smooth); // 0x8001763C
extern short CAMERA_CalcZRotation(_Position *target, _Position *position); // 0x800176DC
extern void CAMERA_CalcRotation(struct _Rotation *rotation, _Position *target, _Position *position); // 0x80017754
extern void CAMERA_CalcFSRotation(struct Camera *camera, struct _Rotation *rotation, _Position *target, _Position *position); // 0x800177F0
extern void CAMERA_Relocate(struct Camera *camera, struct _SVector *offset, int streamSignalFlag); // 0x8001789C
extern struct _TFace * CAMERA_SphereToSphereWithLines(struct Camera *camera, struct CameraCollisionInfo *colInfo, int secondcheck_flag); // 0x800179E8
extern long CAMERA_CalcTilt(struct _Normal *normal, short zRot); // 0x80018524
extern void CAMERA_SetLookFocusAndBase(struct _Instance *focusInstance, _Position *focusPoint); // 0x80018598
extern void CAMERA_SetLookFocusAndDistance(struct Camera *camera, VECTOR *focuspoint, int distance); // 0x800185E0
extern void CAMERA_LookProcess(struct Camera *camera); // 0x80018608
extern void CAMERA_Normalize(struct _SVector *svector); // 0x8001885C
extern void CAMERA_HandleTransitions(struct Camera *camera); // 0x800188D4
extern void CAMERA_CalcFocusOffset(struct _SVector *offset, struct Camera *camera); // 0x80018A88
extern void CAMERA_CalcFocusOffsetForSwim(struct _SVector *offset, struct Camera *camera); // 0x80018B3C
extern short CAMERA_CalcIntersectAngle(struct _SVector *linept, struct _SVector *vertex0, struct _SVector *vertex1, short *high, short *low); // 0x80018BA8
extern short CAMERA_GetLineAngle(struct Camera *camera, struct CameraCollisionInfo *colInfo, struct _SVector *linept, int line); // 0x80018C98
extern long CAMERA_ACForcedMovement(struct Camera *camera, struct CameraCollisionInfo *colInfo); // 0x80018E60
extern void CAMERA_update_dist_debounced(struct Camera *camera, short dist); // 0x80018F30
extern short CAMERA_dampgetline(short angle); // 0x80018FA0
extern long CAMERA_ACNoForcedMovement(struct Camera *camera, struct CameraCollisionInfo *colInfo); // 0x8001904C
extern long CAMERA_AbsoluteCollision(struct Camera *camera, struct CameraCollisionInfo *colInfo); // 0x80019550
extern short CAMERA_update_z_damped(struct Camera *camera, short current, short target); // 0x80019678
extern void CAMERA_CombatCamDist(struct Camera *camera); // 0x800198E8
extern void CAMERA_GenericCameraProcess(struct Camera *camera); // 0x80019BAC
extern void CAMERA_CinematicProcess(struct Camera *camera); // 0x8001A028
extern int CAMERA_GetDistSq(struct _SVector *point1, struct _SVector *point2); // 0x8001A544
extern void CAMERA_NearestPointOnLineVec(struct _SVector *linePoint, struct _SVector *start, struct _SVector *line, _Position *point); // 0x8001A5C0
extern int CAMERA_CheckPoint(int linePoint, int linept1, int linept2); // 0x8001A6EC
extern int CAMERA_CheckIfPointOnLine(struct _SVector *linePoint, struct _SVector *linept1, struct _SVector *linept2); // 0x8001A730
extern int CAMERA_FindLinePoint(_Position *point, struct _SVector *linept1, struct _SVector *linept2, int target_dist_sq, struct _SVector *results); // 0x8001A7C0
extern void CAMERA_SplineGetNearestPoint2(struct Camera *camera, struct Spline *spline, struct _SVector *point, int *currkey, struct _SVector *ret_dpoint); // 0x8001A984
extern void CAMERA_SplineGetNearestPoint(struct Spline *spline, struct _SVector *point, int *currkey, struct _SVector *ret_dpoint); // 0x8001AB6C
extern void CAMERA_SplineHelpMove(struct Camera *camera); // 0x8001AF04
extern void CAMERA_SplineProcess(struct Camera *camera); // 0x8001B074
extern void CAMERA_ShakeCamera(struct Camera *camera); // 0x8001B3C8
extern void CAMERA_Process(struct Camera *camera); // 0x8001B4B0
extern void CAMERA_CenterCamera(struct Camera *camera); // 0x8001B6FC
extern void CAMERA_SetLookRot(struct Camera *camera, int x_rotation, int z_rotation); // 0x8001B7E4
extern void CAMERA_StartLookaroundMode(struct Camera *camera); // 0x8001B7F0
extern void CAMERA_StartSwimThrowMode(struct Camera *camera); // 0x8001B880
extern void CAMERA_EndSwimThrowMode(struct Camera *camera); // 0x8001B8C0
extern void CAMERA_ForceEndLookaroundMode(struct Camera *camera); // 0x8001B8E4
extern void CAMERA_Control(struct Camera *camera, struct _Instance *playerInstance); // 0x8001B920
extern void CAMERA_EndLook(struct Camera *camera); // 0x8001BE68
extern void CAMERA_ChangeToUnderWater(struct Camera *camera, struct _Instance *instance); // 0x8001BF5C
extern void CAMERA_ChangeToOutOfWater(struct Camera *camera, struct _Instance *instance); // 0x8001C008
extern void CAMERA_UpdateFocusDistance(struct Camera *camera); // 0x8001C07C
extern void CAMERA_UpdateFocusTilt(struct Camera *camera); // 0x8001C14C
extern void CAMERA_UpdateFocusRoll(struct Camera *camera); // 0x8001C218
extern void CAMERA_UpdateFocusRotate(struct Camera *camera); // 0x8001C334
extern void CAMERA_UpdateFocusRotationX(struct Camera *camera, struct _Instance *focusInstance); // 0x8001C420
extern void CAMERA_FollowPlayerTilt(struct Camera *camera, struct _Instance *focusInstance); // 0x8001C6A0
extern void CAMERA_FollowGoBehindPlayerWithTimer(struct Camera *camera); // 0x8001C8B0
extern void CAMERA_FollowGoBehindPlayer(struct Camera *camera); // 0x8001C9AC
extern void CAMERA_CalculateLead(struct Camera *camera); // 0x8001C9E8
extern void CAMERA_CalcFollowPosition(struct Camera *camera, struct _Rotation *rotation); // 0x8001CB90
extern void CAMERA_SetupColInfo(struct Camera *camera, struct CameraCollisionInfo *colInfo, _Position *targetCamPos); // 0x8001CE8C
extern void CAMERA_DoPanicCheck(struct Camera *camera, struct CameraCollisionInfo *tmpcolInfo, struct _Rotation *rotation, short *best_z, short *max_dist); // 0x8001CF4C
extern void CAMERA_Panic(struct Camera *camera, short min_dist); // 0x8001D01C
extern long CAMERA_DoCameraCollision2(struct Camera *camera, _Position *targetCamPos, int simpleflag); // 0x8001D1CC
extern int CAMERA_FocusInstanceMoved(struct Camera *camera); // 0x8001D7D0

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
