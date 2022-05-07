#ifndef PLANPOOL_H
#define PLANPOOL_H


extern int PLANPOOL_NumNodesInPlan(struct PlanningNode *goalNode, struct PlanningNode *planningPool); // 0x80099208

extern int PLANPOOL_CountBitsSetInLong(long mylong); // 0x80099254

extern int PLANPOOL_NumConnectionsExaminedForNode(struct PlanningNode *node); // 0x80099274

extern int PLANPOOL_NumConnectionsForNode(struct PlanningNode *node); // 0x80099298

extern struct PlanningNode * PLANPOOL_GetFirstNodeOfSource(struct PlanningNode *planningPool, char nodeSource); // 0x800992BC

extern int PLANPOOL_NumberOfNodesOfType(struct PlanningNode *planningPool, char nodeType); // 0x8009930C

extern int PLANPOOL_NumberOfNodesOfSource(struct PlanningNode *planningPool, char nodeSource); // 0x80099358

extern struct PlanningNode * PLANPOOL_GetNodeWithID(struct PlanningNode *planningPool, int type, int id, long suID); // 0x800993A8

extern struct PlanningNode * PLANPOOL_GetNodeByPosition(_Position *currentPos, struct PlanningNode *planningPool); // 0x80099414

extern struct PlanningNode * PLANPOOL_GetClosestNode(_Position *pos, struct PlanningNode *planningPool, char distanceType); // 0x800994E8

extern int PLANPOOL_AppropriatePair(struct PlanningNode *node1, struct PlanningNode *node2); // 0x800995E8

extern int PLANPOOL_AreTwoNodesConnected(struct PlanningNode *node1, struct PlanningNode *node2, struct PlanningNode *planningPool); // 0x8009975C

extern void PLANPOOL_MarkTwoNodesAsConnected(struct PlanningNode *node1, struct PlanningNode *node2, struct PlanningNode *planningPool); // 0x800997CC

extern void PLANPOOL_MarkTwoNodesAsNotConnected(struct PlanningNode *node1, struct PlanningNode *node2, struct PlanningNode *planningPool); // 0x800998C8

extern struct PlanningNode * PLANPOOL_GetClosestUnexploredValidNeighbor(struct PlanningNode *startNode, struct PlanningNode *planningPool); // 0x80099924

extern void PLANPOOL_ChangeNodePosition(_Position *newPos, struct PlanningNode *nodeToChange, struct PlanningNode *planningPool); // 0x80099A34

extern struct PlanningNode * PLANPOOL_AddNodeToPool(_Position *pos, struct PlanningNode *planningPool, short nodeType, short nodeID, long streamUnitID); // 0x80099AEC

extern void PLANPOOL_DeleteNodeFromPool(struct PlanningNode *nodeToDelete, struct PlanningNode *planningPool); // 0x80099BE0


#endif
