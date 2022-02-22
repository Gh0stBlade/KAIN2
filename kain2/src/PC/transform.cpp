#include "transform.h"

void(* TRANS_DoTransform)(TRANS_VEC*, int x, int y, int z);
void(*SetTransformMatrix)();

void TRANS_Init()
{
	SetTransformMatrix = nullptr;
	TRANS_DoTransform = nullptr;
}
