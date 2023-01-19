#ifndef PLANCOLL_H
#define PLANCOLL_H

#include "Game/CORE.H"
#include "PLANAPI.H"
#include "Game/COLLIDE.H"

struct _fth {
	char x; // size=0, offset=0
	char y; // size=0, offset=1
	char z; // size=0, offset=2
	char nodeType; // size=0, offset=3
};

extern int PLANCOLL_DoesLOSExistFinal(_Position *startPos, _Position *endPos, int collideType, int passThroughHit, int zoffset); // 0x80098B14

extern int PLANCOLL_CheckUnderwaterPoint(_Position *position); // 0x80098C18

extern int PLANCOLL_FindTerrainHitFinal(struct _PlanCollideInfo *pci, int *placement, int distBefore, int distAfter, int start, int end); // 0x80098D84

extern int PLANCOLL_DoesStraightLinePathExist(_Position *startPos, _Position *endPos, int collideType); // 0x80099044

extern int PLANCOLL_DoesWaterPathUpExist(_Position *startPos, _Position *endPos, int collideType, _Position *peakPos, int passThroughHit); // 0x800990D0


#endif
