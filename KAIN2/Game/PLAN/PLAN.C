#include "Game/CORE.H"
#include "PLAN.H"
#include "Game/PLAN/PLANPOOL.H"
#include "Game/PLAN/PLANCOLL.H"
#include "Game/MATH3D.H"
#include <Game/STREAM.H>
#include <Game/MEMPACK.H>

long PLAN_CalcMinDistFromExistingNodes(struct _Position* pos, struct PlanningNode* planningPool, int distanceType) // Matching - 100%
{
    struct PlanningNode *closestNode;
    long minDist;

    closestNode = PLANPOOL_GetClosestNode(pos, planningPool, distanceType);
    if (closestNode != NULL)
    {
        if (distanceType == 0)
        {
            minDist = MATH3D_LengthXY(pos->x - closestNode->pos.x, pos->y - closestNode->pos.y);
        }
        else
        {
            minDist = MATH3D_LengthXYZ(pos->x - closestNode->pos.x, pos->y - closestNode->pos.y, pos->z - closestNode->pos.z);
        }
    }
    else
    {
        minDist = 0x7FFFFFFF;
    }
    return minDist;
}

void PLAN_UpdatePlanMkrNodes(struct PlanningNode* planningPool, struct _Position* playerPos) // Matching - 100%
{
	int i;
	int d;
	long zDiff;
	struct _StreamUnit* streamUnit;
	int temp;  // not from SYMDUMP

	streamUnit = (struct _StreamUnit*)&StreamTracker.StreamList;

	for (d = 0; d < 16; d++, streamUnit++)
	{
		if ((streamUnit->used == 2) && (MEMPACK_MemoryValidFunc((char*)streamUnit->level) != 0))
		{
			int numPlanMkrs;
			struct _PlanMkr* planMkr;
			long suID;

			planMkr = streamUnit->level->PlanMarkerList;

			numPlanMkrs = streamUnit->level->NumberOfPlanMarkers;

			suID = streamUnit->StreamUnitID;

			if (numPlanMkrs == 0)
			{
				continue;
			}

			for (i = 0; i < numPlanMkrs; i++, planMkr++)
			{
				long nodeType;
				long nodeID;

				temp = MATH3D_LengthXY(planMkr->pos.x - playerPos->x, planMkr->pos.y - playerPos->y);

				zDiff = abs(playerPos->z - planMkr->pos.z);

				if ((temp >= 8000) || (zDiff >= 4000))
				{
					continue;
				}

				nodeID = planMkr->id & ~0xF000;

				if (!(planMkr->id & 0x1000))
				{
					if ((planMkr->id & 0x8000))
					{
						nodeType = 12;
					}
					else if ((planMkr->id & 0x4000))
					{
						nodeType = 28;
					}
					else if ((planMkr->id & 0x2000))
					{
						nodeType = 20;
					}
					else
					{
						nodeType = 4;
					}

					if (PLANPOOL_GetNodeWithID(planningPool, nodeType, nodeID, suID) == 0)
					{
						PLANPOOL_AddNodeToPool(&planMkr->pos, planningPool, (short)nodeType, nodeID & 0xFFF, streamUnit->StreamUnitID);
					}
				}
			}
		}
	}


	for (i = 0; i < poolManagementData->numNodesInPool; i++)
	{
		if ((planningPool[i].nodeType & 0x7) == 4)
		{
			temp = MATH3D_LengthXY(planningPool[i].pos.x - playerPos->x, planningPool[i].pos.y - playerPos->y);

			zDiff = abs(playerPos->z - planningPool[i].pos.z);

			if ((temp >= 10001) || (zDiff >= 5001))
			{
				PLANPOOL_DeleteNodeFromPool(&planningPool[i], planningPool);
			}
		}
	}
}


void PLAN_UpdatePlayerNode(struct PlanningNode* planningPool, struct _Position* playerPos) // Matching - 100%
{
	struct PlanningNode* playerNode;
	int nodePlacement;
	int foundHit;
	struct _PlanCollideInfo pci;

	playerNode = PLANPOOL_GetFirstNodeOfSource(planningPool, 1);

	if (playerNode != NULL)
	{
		foundHit = PLANCOLL_CheckUnderwaterPoint(playerPos);

		if (foundHit != -1)
		{
			PLANPOOL_ChangeNodePosition(playerPos, playerNode, planningPool);

			playerNode->nodeType = 25;

			playerNode->streamUnitID = foundHit;

			return;
		}

		{
			short _x1;
			short _y1;
			short _z1;
			struct _Position* _v0;


			_v0 = &pci.collidePos;

			_x1 = playerPos->x;
			_y1 = playerPos->y;
			_z1 = playerPos->z;

			_v0->x = _x1;
			_v0->y = _y1;
			_v0->z = _z1;
		}

		if (PLANCOLL_FindTerrainHitFinal(&pci, &nodePlacement, 256, -640, 0, 5) != 0)
		{
			PLANPOOL_ChangeNodePosition(&pci.collidePos, playerNode, planningPool);

			playerNode->nodeType = ((((short)nodePlacement & 3) * 8) | 1);

			playerNode->streamUnitID = pci.StreamUnitID;
		}
		else if (MATH3D_LengthXYZ(playerPos->x - playerNode->pos.x, playerPos->y - playerNode->pos.y, playerPos->z - playerNode->pos.z) >= 12001)
		{
			PLANPOOL_ChangeNodePosition(playerPos, playerNode, planningPool);

			playerNode->nodeType = 1;

			playerNode->streamUnitID = gameTrackerX.playerInstance->currentStreamUnitID;
		}
	}
}


void PLAN_AddRandomNode(struct PlanningNode* planningPool, struct _Position* playerPos) // Matching - 100%
{
	int i;
	struct _PlanCollideInfo pci;
	int successFlag;

	successFlag = 0;

	for (i = 0; i < 5; i++)
	{
		short _x1;
		short _y1;
		short _z1;
		struct _Position* _v0;

		_v0 = &pci.collidePos;

		_x1 = playerPos->x;
		_y1 = playerPos->y;
		_z1 = playerPos->z;

		_v0->x = _x1;
		_v0->y = _y1;
		_v0->z = _z1;

		pci.collidePos.x += (rand() % 24000) - 12000;

		pci.collidePos.y += (rand() % 24000) - 12000;

		if ((MATH3D_LengthXYZ(playerPos->x - pci.collidePos.x, playerPos->y - pci.collidePos.y, playerPos->z - pci.collidePos.z) < 12000) && (PLAN_CalcMinDistFromExistingNodes(&pci.collidePos, planningPool, 0) >= 1001))
		{
			successFlag = 1;
			break;
		}
	}

	if ((successFlag == 1) && ((PLANCOLL_FindTerrainHitFinal(&pci, 0, 256, -2000, 0, 0) != 0) || (PLANCOLL_FindTerrainHitFinal(&pci, 0, 2000, 0, 0, 0) != 0)))
	{
		struct _SVector normal;

		COLLIDE_GetNormal(pci.tFace->normal, (short*)(STREAM_GetLevelWithID(pci.StreamUnitID))->terrain->normalList, &normal);

		if (normal.z >= 2049)
		{
			PLANPOOL_AddNodeToPool(&pci.collidePos, planningPool, 0, 0, pci.StreamUnitID);
		}
	}
}

void PLAN_DeleteRandomNode(struct PlanningNode* planningPool) // Matching - 100%
{ 
	PLANPOOL_DeleteNodeFromPool(PLANPOOL_GetFirstNodeOfSource(planningPool, 0), planningPool);
}


void PLAN_DeleteOutOfRangeNodesOfSource(struct PlanningNode* planningPool, struct _Position* playerPos, int nodeSourceToCheck, long removeDist) // Matching - 100%
{
	int i;

	for (i = 0; i < poolManagementData->numNodesInPool; i++)
	{
		if (((planningPool[i].nodeType & 7) == nodeSourceToCheck) && (removeDist < MATH3D_LengthXYZ(playerPos->x - planningPool[i].pos.x, playerPos->y - planningPool[i].pos.y, playerPos->z - planningPool[i].pos.z)))
		{
			PLANPOOL_DeleteNodeFromPool(&planningPool[i], planningPool);
		}
	}
}

void PLAN_AddOrRemoveRandomNodes(struct PlanningNode* planningPool, struct _Position* playerPos) // Matching - 100%
{
	int numNodeError;

	numNodeError = poolManagementData->numNodesInPool - 16;

	if (numNodeError < 0)
	{
		PLAN_AddRandomNode(planningPool, playerPos);
	}
	else
	{
		if (numNodeError > 0)
		{
			PLAN_DeleteRandomNode(planningPool);
		}
	}
}


void PLAN_AddInitialNodes(struct PlanningNode* planningPool, struct _Instance* player) // Matching - 100%
{
	struct _PlanCollideInfo pci;
	short _x1;
	short _y1;
	short _z1;
	struct _Position* _v0;
	struct _Position* _v1;

	_v0 = &pci.collidePos;
	_v1 = &player->position;
	_x1 = _v1->x;
	_y1 = _v1->y;
	_z1 = _v1->z;
	_v0->x = _x1;
	_v0->y = _y1;
	_v0->z = _z1;
	PLANCOLL_FindTerrainHitFinal(&pci, NULL, 256, -1024, 0, 0);
	PLANPOOL_AddNodeToPool(&pci.collidePos, planningPool, 1, 0, player->currentStreamUnitID);
	PLAN_UpdatePlanMkrNodes(planningPool, &player->position);
	poolManagementData->playerPosAtLastPlanMkrUpdate = player->position;
}

void PLAN_AddOrRemoveNodes(struct PlanningNode* planningPool, struct _Instance* player) // Matching - 100%
{
	if (MATH3D_LengthXYZ(
		player->position.x - poolManagementData->playerPosAtLastPlanMkrUpdate.x,
		player->position.y - poolManagementData->playerPosAtLastPlanMkrUpdate.y,
		player->position.z - poolManagementData->playerPosAtLastPlanMkrUpdate.z)
		> 500)
	{
		PLAN_UpdatePlayerNode(planningPool, &player->position);
		PLAN_UpdatePlanMkrNodes(planningPool, &player->position);
		PLAN_DeleteOutOfRangeNodesOfSource(planningPool, &player->position, 0, 12000);
		PLAN_DeleteOutOfRangeNodesOfSource(planningPool, &player->position, 2, 12000);
		PLAN_DeleteOutOfRangeNodesOfSource(planningPool, &player->position, 3, 12000);
		poolManagementData->playerPosAtLastPlanMkrUpdate = player->position;
	}
	PLAN_AddOrRemoveRandomNodes(planningPool, &player->position);
}

struct PlanningNode* PLAN_FindNodeMostInNeedOfConnectivityExpansion(struct PlanningNode* planningPool) // Matching - 100%
{
	int i;
	int numConnections;
	int minNumConnections;
	struct PlanningNode* nodeToReturn;

	minNumConnections = 65535;

	nodeToReturn = NULL;

	for (i = 0; i < poolManagementData->numNodesInPool; i++)
	{
		numConnections = PLANPOOL_NumConnectionsForNode(&planningPool[i]);

		if ((numConnections < minNumConnections) && (PLANPOOL_NumConnectionsExaminedForNode(&planningPool[i]) != poolManagementData->numNodesInPool))
		{
			minNumConnections = numConnections;

			nodeToReturn = &planningPool[i];
		}
	}

	return nodeToReturn;
}