#ifndef PLANAPI_H
#define PLANAPI_H

struct EnemyPlanData // hashcode: 0xE792BD02 (dec: -409813758)
{
	unsigned char nodeSkipArray[8]; // size=8, offset=0
	unsigned char nodeTypeArray[8]; // size=8, offset=8
	struct _Position wayPointArray[8]; // size=48, offset=16
	short goalUnitID; // size=0, offset=64
	unsigned char numWayPoints; // size=0, offset=66
};

struct PlanningNode // hashcode: 0xDB9F3065 (dec: -610324379)
{
	struct _Position pos; // size=6, offset=0
	unsigned short flags; // size=0, offset=6
	unsigned long connectionStatus; // size=0, offset=8
	unsigned long connections; // size=0, offset=12
	unsigned short cost; // size=0, offset=16
	unsigned short parent; // size=0, offset=18
	unsigned short nodeType; // size=0, offset=20
	unsigned short id; // size=0, offset=22
	long streamUnitID; // size=0, offset=24
};

extern void PLANAPI_ConvertPlanIntoEnmyPlanDataFormat(struct PlanningNode *goalNode, struct EnemyPlanData *planData, struct PlanningNode *planningPool); // 0x80097B8C
extern int PLANAPI_FindPathBetweenNodes(struct PlanningNode *startNode, struct PlanningNode *goalNode, struct EnemyPlanData *planData, int validNodeTypes); // 0x80097C40
extern void PLANAPI_DoTimingCalcsAndDrawing(long startTime, struct PlanningNode *planningPool); // 0x80097CC8
extern int PLANAPI_AddNodeOfTypeToPool(struct _Position *pos, int type); // 0x80097DE4
extern void PLANAPI_DeleteNodesFromPoolByType(int nodeSource); // 0x80097EC4
extern void PLANAPI_DeleteNodeFromPoolByUnit(long streamUnitID); // 0x80097F60
extern int PLANAPI_FindPathInGraphToTarget(struct _Position *startPos, struct EnemyPlanData *planData, int validNodeTypes); // 0x80097FF8
extern void PLANAPI_InitPlanning(void *planningPool); // 0x8009805C
extern short PLANAPI_PairType(struct PlanningNode *node1, struct PlanningNode *node2); // 0x800980DC
extern int PLANAPI_PassThroughHit(struct PlanningNode *node1, struct PlanningNode *node2); // 0x80098124
extern int PLANAPI_CheckTargetToEnemyNode(struct PlanningNode **node1, struct PlanningNode **node2); // 0x80098190
extern void PLANAPI_UpdatePlanningDatabase(struct GameTracker *gameTracker, struct _Instance *player); // 0x800982A0
extern int PLANAPI_NumNodesInPool(void *planningPool); // 0x8009874C
extern void PLANAPI_InitPlanMkrList(struct _StreamUnit *streamUnit); // 0x80098760
extern int PLANAPI_GetFlags(int type); // 0x80098884
extern int PLANAPI_FindNodePositionInUnit(struct _StreamUnit *streamUnit, struct _Position *pos, int id, int type); // 0x800988EC
extern int PLANAPI_FindClosestNodePositionInUnit(struct _StreamUnit *streamUnit, struct _Position *target, struct _Position *pos, int offset, int max, int type, int distanceCheck); // 0x80098990

#endif
