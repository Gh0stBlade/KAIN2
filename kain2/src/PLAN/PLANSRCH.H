#ifndef PLANSRCH_H
#define PLANSRCH_H


extern int PLANSRCH_ValidNode(struct PlanningNode *node, int validNodeTypes); // 0x80099DAC

extern struct PlanningNode * PLANSRCH_FindNodeToExpand(struct PlanningNode *planningPool, struct PlanningNode *goalNode, int validNodeTypes); // 0x80099DC4

extern void PLANSRCH_ExpandNode(struct PlanningNode *planningPool, struct PlanningNode *nodeToExpand); // 0x80099ED8

extern void PLANSRCH_InitNodesForSearch(struct PlanningNode *planningPool); // 0x80099FFC

extern struct PlanningNode * PLANSRCH_FindPathInGraph(struct PlanningNode *planningPool, struct PlanningNode *startNode, struct PlanningNode *goalNode, int validNodeTypes); // 0x8009A048


#endif
