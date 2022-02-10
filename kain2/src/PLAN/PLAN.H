#ifndef PLAN_H
#define PLAN_H


extern long PLAN_CalcMinDistFromExistingNodes(struct _Position *pos, struct PlanningNode *planningPool, int distanceType); // 0x80097188

extern void PLAN_UpdatePlanMkrNodes(struct PlanningNode *planningPool, struct _Position *playerPos); // 0x80097224

extern void PLAN_UpdatePlayerNode(struct PlanningNode *planningPool, struct _Position *playerPos); // 0x80097470

extern void PLAN_AddRandomNode(struct PlanningNode *planningPool, struct _Position *playerPos); // 0x800975C4

extern void PLAN_DeleteRandomNode(struct PlanningNode *planningPool); // 0x800977D0

extern void PLAN_DeleteOutOfRangeNodesOfSource(struct PlanningNode *planningPool, struct _Position *playerPos, int nodeSourceToCheck, long removeDist); // 0x80097804

extern void PLAN_AddOrRemoveRandomNodes(struct PlanningNode *planningPool, struct _Position *playerPos); // 0x800978DC

extern void PLAN_AddInitialNodes(struct PlanningNode *planningPool, struct _Instance *player); // 0x8009792C

extern void PLAN_AddOrRemoveNodes(struct PlanningNode *planningPool, struct _Instance *player); // 0x800979E8

extern struct PlanningNode * PLAN_FindNodeMostInNeedOfConnectivityExpansion(struct PlanningNode *planningPool); // 0x80097AD0


#endif
