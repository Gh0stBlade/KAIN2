#ifndef CAMERA_H
#define CAMERA_H


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

extern void CAMERA_SetTarget(struct Camera *camera, struct _Position *pos); // 0x80016584

extern void CAMERA_CalcPosition(struct _Position *position, struct _Position *base, struct _Rotation *rotation, short distance); // 0x80016600

extern void CAMERA_SetFocus(struct Camera *camera, struct _Position *targetfocusPoint); // 0x80016728

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

extern void CriticalDampPosition(long dampMode, struct _Position *position, struct _Position *targetPos, struct _SVector *vel, struct _SVector *accl, int smooth); // 0x80016F78

extern void CriticalDampAngle(long dampMode, short *currentVal, short target, short *vel, short *accl, int smooth); // 0x8001763C

extern short CAMERA_CalcZRotation(struct _Position *target, struct _Position *position); // 0x800176DC

extern void CAMERA_CalcRotation(struct _Rotation *rotation, struct _Position *target, struct _Position *position); // 0x80017754

extern void CAMERA_CalcFSRotation(struct Camera *camera, struct _Rotation *rotation, struct _Position *target, struct _Position *position); // 0x800177F0

extern void CAMERA_Relocate(struct Camera *camera, struct _SVector *offset, int streamSignalFlag); // 0x8001789C

extern struct _TFace * CAMERA_SphereToSphereWithLines(struct Camera *camera, struct CameraCollisionInfo *colInfo, int secondcheck_flag); // 0x800179E8

extern long CAMERA_CalcTilt(struct _Normal *normal, short zRot); // 0x80018524

extern void CAMERA_SetLookFocusAndBase(struct _Instance *focusInstance, struct _Position *focusPoint); // 0x80018598

extern void CAMERA_SetLookFocusAndDistance(struct Camera *camera, struct VECTOR *focuspoint, int distance); // 0x800185E0

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

extern void CAMERA_NearestPointOnLineVec(struct _SVector *linePoint, struct _SVector *start, struct _SVector *line, struct _Position *point); // 0x8001A5C0

extern int CAMERA_CheckPoint(int linePoint, int linept1, int linept2); // 0x8001A6EC

extern int CAMERA_CheckIfPointOnLine(struct _SVector *linePoint, struct _SVector *linept1, struct _SVector *linept2); // 0x8001A730

extern int CAMERA_FindLinePoint(struct _Position *point, struct _SVector *linept1, struct _SVector *linept2, int target_dist_sq, struct _SVector *results); // 0x8001A7C0

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

extern void CAMERA_SetupColInfo(struct Camera *camera, struct CameraCollisionInfo *colInfo, struct _Position *targetCamPos); // 0x8001CE8C

extern void CAMERA_DoPanicCheck(struct Camera *camera, struct CameraCollisionInfo *tmpcolInfo, struct _Rotation *rotation, short *best_z, short *max_dist); // 0x8001CF4C

extern void CAMERA_Panic(struct Camera *camera, short min_dist); // 0x8001D01C

extern long CAMERA_DoCameraCollision2(struct Camera *camera, struct _Position *targetCamPos, int simpleflag); // 0x8001D1CC

extern int CAMERA_FocusInstanceMoved(struct Camera *camera); // 0x8001D7D0


#endif
