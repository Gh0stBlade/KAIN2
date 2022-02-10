#ifndef PHYSICS_H
#define PHYSICS_H


extern void SetNoPtCollideInFamily(struct _Instance *instance); // 0x80074628

extern void ResetNoPtCollideInFamily(struct _Instance *instance); // 0x80074670

extern void PHYSICS_CheckLineInWorld(struct _Instance *instance, struct _PCollideInfo *pcollideInfo); // 0x800746BC

extern void PHYSICS_CheckLineInWorldMask(struct _Instance *instance, struct _PCollideInfo *pcollideInfo); // 0x800746E0

extern int PhysicsCheckLinkedMove(struct _Instance *instance, int Data, short Mode); // 0x80074754

extern void PhysicsDefaultLinkedMoveResponse(struct _Instance *instance, struct evPhysicsLinkedMoveData *Data, int updateTransforms); // 0x80074BB0

extern int PhysicsCheckGravity(struct _Instance *instance, int Data, short Mode); // 0x80074C38

extern void PhysicsDefaultGravityResponse(struct _Instance *instance, struct evPhysicsGravityData *Data); // 0x800753F0

extern int PhysicsCheckEdgeGrabbing(struct _Instance *instance, struct GameTracker *gameTracker, int Data, short Mode); // 0x80075468

extern void PhysicsDefaultEdgeGrabResponse(struct _Instance *instance, struct evPhysicsEdgeData *Data, int blockFlag); // 0x80075B20

extern int PhysicsCheckSliding(struct _Instance *instance, int Data, short Mode); // 0x80075DC4

extern int PhysicsUpdateTface(struct _Instance *instance, int Data); // 0x80076270

extern int PhysicsCheckBlockers(struct _Instance *instance, struct GameTracker *gameTracker, int Data, short Mode); // 0x8007636C

extern int PhysicsCheckSwim(struct _Instance *instance, int Data, short Mode); // 0x8007670C

extern int PhysicsDefaultCheckSwimResponse(struct _Instance *instance, struct evPhysicsSwimData *Data); // 0x800768FC

extern void PhysicsForceSetWater(struct _Instance *instance, int *Time, int Depth, int rate, int maxAmplitude); // 0x80076AF4

extern int PhysicsCheckLOS(struct _Instance *instance, int Data, int Mode); // 0x80076BCC

extern int PhysicsCheckDropHeight(struct _Instance *instance, int Data, int Mode); // 0x80076BFC

extern int PhysicsCheckDropOff(struct _Instance *instance, int Data, short Mode); // 0x80076E8C

extern int PhysicsFollowWall(struct _Instance *instance, struct GameTracker *gameTracker, int Data, short Mode); // 0x800770AC

extern void PhysicsMoveLocalZClamp(struct _Instance *instance, long segment, long time, long clamp); // 0x800775BC

extern void PhysicsMove(struct _Instance *instance, struct _Position *position, long time); // 0x80077694

extern void PhysicsSetVelFromZRot(struct _Instance *instance, short angle, long magnitude); // 0x80077830

extern void PhysicsSetVelFromRot(struct _Instance *instance, struct _Rotation *rot, long magnitude); // 0x800778A0

extern void PHYSICS_SetVAndAFromRot(struct _Instance *instance, struct _Rotation *rot, long v, long a); // 0x80077910

extern long PHYSICS_FindAFromDAndT(long d, long t); // 0x80077A00

extern long PHYSICS_FindVFromAAndD(long a, long d); // 0x80077A3C

extern void PHYSICS_StopIfCloseToTarget(struct _Instance *instance, int x, int y, int z); // 0x80077A74

extern int PHYSICS_CheckForTerrainCollide(struct _Instance *instance, struct SVECTOR *startVec, struct SVECTOR *endVec, int segment); // 0x80077B54

extern int PHYSICS_CheckForObjectCollide(struct _Instance *instance, struct SVECTOR *startVec, struct SVECTOR *endVec, int segment); // 0x80077B9C

extern int PHYSICS_CheckForValidMove(struct _Instance *instance, struct SVECTOR *startVec, struct SVECTOR *endVec, int segment); // 0x80077BE0

extern int PHYSICS_CheckFaceStick(struct _PCollideInfo *CInfo); // 0x80077C4C

extern int PHYSICS_CheckDontGrabEdge(struct _PCollideInfo *CInfo); // 0x80077CB0

extern void PHYSICS_GenericLineCheckSetup(short x, short y, short z, struct SVECTOR *inVec); // 0x80077DD8

extern void PHYSICS_GenericLineCheck(struct _Instance *instance, struct MATRIX *transMat, struct MATRIX *rotMat, struct _PCollideInfo *cInfo); // 0x80077DE8

extern void PHYSICS_GenericLineCheckMask(struct _Instance *instance, struct MATRIX *transMat, struct MATRIX *rotMat, struct _PCollideInfo *cInfo); // 0x80077E0C


#endif
