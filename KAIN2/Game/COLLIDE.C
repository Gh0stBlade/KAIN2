#include "CORE.H"
#include "COLLIDE.H"

#include "MEMPACK.H"
#include "Game/STREAM.H"
#include "Game/BSP.H"
#include "Game/CAMERA.H"
#include "Game/MATH3D.H"
#include "Game/PSX/COLLIDES.H"
#include "Game/EVENT.H"

long collide_ignoreAttr;
long collide_acceptAttr;
long dyna_clddyna[8]; // offset 0x800d056c
long stat_clddyna[8]; // offset 0x800d058c
long dyna_cldstat[8]; // offset 0x800d05ac

struct _SVector* collide_point0;
struct _SVector* collide_point1;
long collide_t0;
long collide_t1;
struct _SVector* collide_normal0;
struct _SVector* collide_normal1;

int COLLIDE_PointInTriangle(struct _SVector* v0, struct _SVector* v1, struct _SVector* v2, struct _SVector* point, struct _SVector* normal)//Matching - 86.63%
{
	int ny; // $v1
	int nx; // $t1
	DVECTOR* vert1; // $t2
	int v8; // $v0
	short tx; // $t0
	short ty; // $a3
	int x; // $v1
	int z; // $v0
	int y; // $v0
	DVECTOR* vert0; // $t1
	int inside_flag; // $t3
	int line_flag; // $t7
	int j; // $t6
	int yflag0; // $v1
	int yflag1;
	int vy; // $t0
	int xdiff; // $v1
	int ydiff; // $a1
	int v26; // $v1
	int v27; // $a1
	int vx; // $a1
	struct _Triangle2D* tri;

	ny = normal->y;
	nx = normal->x;

	if (nx < 0)
		nx = -nx;

	if (ny < 0)
		ny = -ny;

	tri = (struct _Triangle2D*)getScratchAddr(0);
	vert1 = (DVECTOR*)tri;

	if (ny < nx)
	{
		z = normal->z;
		if (z < 0)
			z = -z;
		if (z < nx)
		{
			tx = point->y;
			ty = point->z;
			*(int*)&tri->x0 = (unsigned short)v0->y | (v0->z << 16);
			*(int*)&tri->x1 = (unsigned short)v1->y | (v1->z << 16);
			*(int*)&tri->x2 = (unsigned short)v2->y | (v2->z << 16);
		}
		else
		{
			tx = point->x;
			ty = point->y;
			*(int*)&tri->x0 = (unsigned short)v0->x | (v0->y << 16);
			*(int*)&tri->x1 = (unsigned short)v1->x | (v1->y << 16);
			*(int*)&tri->x2 = (unsigned short)v2->x | (v2->y << 16);
		}
	}
	else
	{
		z = normal->z;
		if (z < 0)
			z = -z;
		if (z < ny)
		{
			tx = point->x;
			ty = point->z;
			*(int*)&tri->x0 = (unsigned short)v0->x | (v0->z << 16);
			*(int*)&tri->x1 = (unsigned short)v1->x | (v1->z << 16);
			*(int*)&tri->x2 = (unsigned short)v2->x | (v2->z << 16);
		}
		else
		{
			tx = point->x;
			ty = point->y;
			*(int*)&tri->x0 = (unsigned short)v0->x | (v0->y << 16);
			*(int*)&tri->x1 = (unsigned short)v1->x | (v1->y << 16);
			*(int*)&tri->x2 = (unsigned short)v2->x | (v2->y << 16);
		}
	}
	vert0 = (DVECTOR*)vert1 + 2;//t1

	inside_flag = 0;
	line_flag = 0;

	for (j = 3; j != 0; j--)
	{
		yflag0 = ((unsigned short)vert0->vy << 16 < (unsigned short)ty << 16) ^ 1;
		yflag1 = (vert1->vy < ty) ^ 1;

		if (yflag0 != yflag1)
		{
			ydiff = (vert0->vx < tx) ^ 1;
			if ((ydiff != vert1->vx >= tx) == 0)
			{
				if (ydiff != 0)
					inside_flag = inside_flag == 0;
			}
			else
			{
				ydiff = vert0->vy - vert1->vy;
				v26 = (vert1->vx - tx) * ydiff - (vert0->vx - vert1->vx) * (vert1->vy - ty);
				if (ydiff < 0)
				{
					ydiff = -ydiff;
					v26 = -v26;
				}
				v27 = ydiff / 2;
				if (v27 < v26)
				{
					inside_flag = inside_flag == 0;
				}
				else if (v26 >= -v27)
				{
					return 1;
				}
			}
			if (line_flag != 0)
				return inside_flag;
			line_flag = 1;
		}
		else
		{
			if (yflag1 != 0 && ty == vert0->vy)
			{
				if (ty == vert1->vy)
				{
					if ((vert0->vx < tx) != (vert1->vx < tx))
						return 1;
					if (tx == vert0->vx)
						return 1;
					yflag0 = vert1->vy >= ty;
					if (tx == vert1->vx)
						return 1;
				}
				else
				{
					yflag0 = vert1->vy >= ty;
					if (tx == vert0->vx)
						return 1;
				}
			}
		}
		vert0 = vert1++;
	}

	return inside_flag;
}

int COLLIDE_PointInTriangle2DPub(short* v0, short* v1, short* v2, short* point)
{
	struct _SVector normal;

	normal.x = 0;
	normal.y = 0;
	normal.z = 4096;

	return COLLIDE_PointInTriangle((struct _SVector*)v0, (struct _SVector*)v1, (struct _SVector*)v2, (struct _SVector*)point, (struct _SVector*)&normal);
}

long COLLIDE_GetNormal(short nNum, short* nrmlArray, struct _SVector* nrml)//Matching - 99.87%
{
	short* sPtr;
	long bitMask;

	if (nNum >= 0)
	{
		sPtr = &nrmlArray[nNum * 3];

		bitMask = *sPtr++;

		nrml->x = bitMask & 0x1FFF;
		nrml->y = *sPtr++;
		nrml->z = *sPtr;

		bitMask >>= 13;
	}
	else
	{
		sPtr = &nrmlArray[-nNum * 3];
		bitMask = *sPtr++;

		nrml->x = -(bitMask & 0x1FFF);
		nrml->y = -(*sPtr++);
		nrml->z = -(*sPtr);

		bitMask >>= 13;
	}

	return bitMask;
}

void COLLIDE_MakeNormal(struct _Terrain* terrain, struct _TFace* tface, struct _SVector* normal)
{
	struct _SVector* vertex0; // $v1
	struct _SVector* vertex1; // $v0
	struct _SVector* vertex2; // $a2
	int len; // $a0
	struct _Vector* a; // $v0
	struct _Vector* b; // $v0
	struct _Vector* n; // $t0
	long _x0; // $t1, $a0
	long _y0; // $t2, $a1
	long _z0; // $t0, $a3
	long _x1; // $a0, $v0
	long _y1; // $v0, $v0
	long _z1; // $a0, $v0

	vertex0 = (struct _SVector*)&terrain->vertexList[tface->face.v0];//v1
	vertex1 = (struct _SVector*)&terrain->vertexList[tface->face.v1];//v0
	vertex2 = (struct _SVector*)&terrain->vertexList[tface->face.v2];//a2

	_x0 = vertex1->x;
	_y0 = vertex1->y;
	_z0 = vertex1->z;

	_x1 = vertex0->x;
	_y1 = vertex0->y;
	_z1 = vertex0->z;

	_x0 -= _x1;
	_y0 -= _y1;
	_z0 -= _z1;

	a = (struct _Vector*)getScratchAddr(0);

	a->x = _x0;
	a->y = _y0;
	a->z = _z0;


	_z0 = vertex2->z;
	_z1 = vertex0->z;
	_z0 -= _z1;

	_y0 = vertex2->y;
	_y1 = vertex0->y;

	_y0 -= _y1;

	_x0 = vertex2->x;
	_x1 = vertex0->x;

	_x0 -= _x1;

	b = (struct _Vector*)getScratchAddr(4);

	b->x = _x0;
	b->y = _y0;
	b->z = _z0;

	n = (struct _Vector*)getScratchAddr(8);

	n->x = (((a->y * _z0) - (a->z * _y0)) >> 12);
	n->y = (((a->x * _z0) - (a->z * _x0)) >> 12);
	n->z = (((a->x * _y0) - (a->y * _x0)) >> 12);

	len = ABS(-n->z);

	if (len < ABS(-n->x))
	{
		len = ABS(n->x);
	}

	if (len < ABS(-n->z))
	{
		len = ABS(n->z);
	}

	if (len != 0)
	{
		normal->x = (short)((n->x << 12) / len);
		normal->y = (short)((n->y << 12) / len);
		normal->z = (short)((n->z << 12) / len);
	}
}

void COLLIDE_UpdateAllTransforms(struct _Instance* instance, SVECTOR* offset)//Matching - 100%
{
	MATRIX* swTransform;
	int i;
	long numMatrices;
	long ox;
	long oy;
	long oz;

	if (instance->matrix != NULL)
	{
		ox = offset->vx;
		oy = offset->vy;
		oz = offset->vz;

		if (instance->object->animList != NULL && !(instance->object->oflags2 & 0x40000000))
		{
			swTransform = instance->matrix - 1;
			numMatrices = instance->object->modelList[instance->currentModel]->numSegments + 1;
		}
		else
		{
			swTransform = instance->matrix;
			numMatrices = instance->object->modelList[instance->currentModel]->numSegments;
		}
		for (i = numMatrices; i != 0; i--, swTransform++)
		{
			swTransform->t[0] += ox;
			swTransform->t[1] += oy;
			swTransform->t[2] += oz;
		}
	}
}

void COLLIDE_MoveAllTransforms(struct _Instance* instance, struct _Position* offset)//Matching - 100%
{
	MATRIX* swTransform;
	int i;
	long numMatrices;
	long ox;
	long oy;
	long oz;

	if (instance->oldMatrix != NULL)
	{
		ox = offset->x;
		oy = offset->y;
		oz = offset->z;

		if (instance->object->animList != NULL && !(instance->object->oflags2 & 0x40000000))
		{
			swTransform = instance->oldMatrix - 1;
			numMatrices = instance->object->modelList[instance->currentModel]->numSegments + 1;
		}
		else
		{
			swTransform = instance->oldMatrix;
			numMatrices = instance->object->modelList[instance->currentModel]->numSegments;
		}
		for (i = numMatrices; i != 0; i--, swTransform++)
		{
			swTransform->t[0] += ox;
			swTransform->t[1] += oy;
			swTransform->t[2] += oz;
		}
	}
}

long COLLIDE_WithinYZBounds(struct _SVector* point, struct _HBox* hbox)//Matching - 86.52%
{
	if (point->y >= hbox->minY && hbox->maxY >= point->y)
	{
		if (point->z >= hbox->minZ)
		{
			return hbox->maxZ >= point->z;
		}
	}
	return 0;
}

long COLLIDE_WithinXZBounds(struct _SVector* point, struct _HBox* hbox)
{
	if (point->x >= hbox->minX && hbox->maxX >= point->x)
	{
		if (point->z >= hbox->minZ)
		{
			return hbox->maxZ >= point->z;
		}
	}
	return 0;
}

long COLLIDE_WithinXYBounds(struct _SVector* point, struct _HBox* hbox)
{
	if (point->x >= hbox->minX && hbox->maxX >= point->x)
	{
		if (point->y >= hbox->minY)
		{
			return hbox->maxY >= point->y;
		}
	}
	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ COLLIDE_LineWithBoxFace(short startDist /*$a0*/, long lineDist /*$a1*/, short planeDist /*$a2*/, struct _SVector *start /*$a3*/, struct _Vector *line /*stack 16*/, struct _HBox *hbox /*stack 20*/, TDRFuncPtr_COLLIDE_LineWithBoxFace6collideBoundFunc collideBoundFunc /*stack 24*/, struct _SVector *normal /*stack 28*/)
void COLLIDE_LineWithBoxFace(short startDist, long lineDist, short planeDist, struct _SVector *start, struct _Vector *line, struct _HBox *hbox, TDRFuncPtr_COLLIDE_LineWithBoxFace6collideBoundFunc collideBoundFunc, struct _SVector *normal)
{ // line 611, offset 0x8001ec74
#if 0
	/* begin block 1 */
		// Start line: 612
		// Start offset: 0x8001EC74
		// Variables:
			struct _SVector point; // stack offset -24
			long t; // stack offset -16

		/* begin block 1.1 */
			// Start line: 612
			// Start offset: 0x8001ED38
			// Variables:
				struct _SVector *point; // $t1
				struct _SVector *normal; // $s0
				struct _SVector *point0; // $a2
				struct _SVector *normal0; // $a3
				struct _SVector *point1; // $a1
				struct _SVector *normal1; // $t0

			/* begin block 1.1.1 */
				// Start line: 612
				// Start offset: 0x8001ED38

				/* begin block 1.1.1.1 */
					// Start line: 612
					// Start offset: 0x8001ED5C
					// Variables:
						short _z1; // $a0
						short _y1; // $v1
						short _x1; // $v0
				/* end block 1.1.1.1 */
				// End offset: 0x8001ED5C
				// End Line: 612

				/* begin block 1.1.1.2 */
					// Start line: 612
					// Start offset: 0x8001ED5C
					// Variables:
						short _z1; // $a0
						short _y1; // $v1
						short _x1; // $v0
				/* end block 1.1.1.2 */
				// End offset: 0x8001ED5C
				// End Line: 612

				/* begin block 1.1.1.3 */
					// Start line: 612
					// Start offset: 0x8001ED5C
					// Variables:
						short _z1; // $a0
						short _y1; // $v1
						short _x1; // $v0
				/* end block 1.1.1.3 */
				// End offset: 0x8001ED5C
				// End Line: 612

				/* begin block 1.1.1.4 */
					// Start line: 612
					// Start offset: 0x8001ED5C
					// Variables:
						short _z1; // $a0
						short _y1; // $v1
						short _x1; // $v0
				/* end block 1.1.1.4 */
				// End offset: 0x8001ED5C
				// End Line: 612

				/* begin block 1.1.1.5 */
					// Start line: 612
					// Start offset: 0x8001EDDC
					// Variables:
						short _z1; // $a0
						short _y1; // $v1
						short _x1; // $v0
				/* end block 1.1.1.5 */
				// End offset: 0x8001EDDC
				// End Line: 612

				/* begin block 1.1.1.6 */
					// Start line: 612
					// Start offset: 0x8001EDDC
					// Variables:
						short _z1; // $a0
						short _y1; // $v1
						short _x1; // $v0
				/* end block 1.1.1.6 */
				// End offset: 0x8001EDDC
				// End Line: 612
			/* end block 1.1.1 */
			// End offset: 0x8001EE10
			// End Line: 612
		/* end block 1.1 */
		// End offset: 0x8001EE10
		// End Line: 612
	/* end block 1 */
	// End offset: 0x8001EE10
	// End Line: 643

	/* begin block 2 */
		// Start line: 1111
	/* end block 2 */
	// End Line: 1112
#else
UNIMPLEMENTED();
#endif
}

long COLLIDE_IntersectLineAndBox(struct _SVector* point0, struct _SVector* normal0, struct _SVector* point1, struct _SVector* normal1, struct _SVector* end, struct _SVector* start, struct _HBox* hbox) // Matching - 95.63%
{
	struct _SVector normal;
	struct _Vector line;
	long _x0;
	long _y0;
	long _z0;
	struct _Vector* _v;
	short lineDist;

	collide_t0 = 0x1001;
	collide_t1 = 0x1001;
	collide_point0 = point0;
	collide_normal0 = normal0;
	collide_point1 = point1;
	collide_normal1 = normal1;

	_x0 = end->x;
	_y0 = end->y;
	_z0 = end->z;

	_x0 -= start->x;
	_y0 -= start->y;
	_z0 -= start->z;

	_v = &line;
	_v->x = _x0;
	_v->y = _y0;
	_v->z = _z0;

	normal.x = -0x1000;
	normal.y = 0;
	normal.z = 0;
	COLLIDE_LineWithBoxFace(-start->x, -line.x, -hbox->minX, start, _v, hbox, COLLIDE_WithinYZBounds, &normal);
	normal.x = 0x1000;
	normal.y = 0;
	normal.z = 0;
	COLLIDE_LineWithBoxFace(start->x, line.x, hbox->maxX, start, _v, hbox, COLLIDE_WithinYZBounds, &normal);
	normal.x = 0;
	normal.y = -0x1000;
	normal.z = 0;
	COLLIDE_LineWithBoxFace(-start->y, lineDist = -(short)line.y, -hbox->minY, start, _v, hbox, COLLIDE_WithinXZBounds, &normal);
	normal.x = 0;
	normal.y = 0x1000;
	normal.z = 0;
	COLLIDE_LineWithBoxFace(start->y, line.y, hbox->maxY, start, _v, hbox, COLLIDE_WithinXZBounds, &normal);
	normal.x = 0;
	normal.y = 0;
	normal.z = -0x1000;
	COLLIDE_LineWithBoxFace(-start->z, lineDist = -(short)line.z, -hbox->minZ, start, _v, hbox, COLLIDE_WithinXYBounds, &normal);
	normal.x = 0;
	normal.y = 0;
	normal.z = 0x1000;
	COLLIDE_LineWithBoxFace(start->z, line.z, hbox->maxZ, start, _v, hbox, COLLIDE_WithinXYBounds, &normal);

	if (collide_t1 != 0x1001)
	{
		return 2;
	}

	if (collide_t0 != 0x1001)
	{
		return 1;
	}

	return 0;
}

struct _TFace* COLLIDE_PointAndTerrain(struct _Terrain* terrain, struct _PCollideInfo* pcollideInfo, struct _LCollideInfo* lcol)//Matching - 99.58%
{
	return COLLIDE_PointAndTerrainFunc(terrain, pcollideInfo, 0, NULL, 0, 0, lcol);
}

struct _TFace* COLLIDE_PointAndTerrainFunc(struct _Terrain* terrain, struct _PCollideInfo* pCollideInfo, int Flags, short* Backface_Flag, long ignoreAttr, long acceptAttr, struct _LCollideInfo* lcolinfo)
{
	SVECTOR* OldPos; // $a0
	struct _Instance* instance; // $s7
	struct _TVertex* vertexList; // $v0
	short _x0;
	short _y0; // $t0 MAPDST
	short _z0; // $t1 MAPDST
	short _x1;
	short _y1; // $t0 MAPDST
	short _z1; // $t1 MAPDST
	short vy; // $v1
	void** stack; // $s1
	struct BSPTree* BSPTreeArray; // $v0
	struct BSPTree* v19; // $s4
	short v20; // $a0
	int collideType; // $v1
	int raziel_collide_override; // $v0
	int v23; // $v0
	short v24; // $v1
	int v25; // $v0
	int v35; // dc
	struct _BSPNode* bspNode; // $a1
	int v38; // $v1
	int v39; // $v1
	int v40; // $v1
	int v41; // $v1
	int v42; // $v1
	int v43; // $v1
	unsigned short* v44; // $s3
	int attr; // $v1
	int normal; // $v1
	short* v48; // $v1
	short v49; // $v0
	int v50; // $v0
	short* v51; // $v1
	short v52; // $v0
	struct _TVertex* v53; // $a0
	int v57; // $v1
	int v58; // $a1
	int v59; // $v0
	int v60; // $t8
	short v61; // $v1
	short v62; // $a1
	int back_spectral_error; // $a0
	int v67; // $v1
	int v68; // $a0
	void* v69; // $v0
	void* front; // $v0
	void* v71; // $v0
	void* back; // $v0
	short y; // $a0
	short z; // $v0
	SVECTOR* NewPos; // [sp+18h] [-28h]
	struct _Instance* v81; // [sp+20h] [-20h]
	struct _Terrain* v83; // [sp+24h] [-1Ch]
	struct _Terrain* v84; // [sp+24h] [-1Ch]
	int curTree; // [sp+38h] [-8h]
	struct PandTScratch* CSpad;
	struct _TFace* v45;
	short* nrmlArray;
	struct _SVector* _v;
	struct _SVector* _v0;
	struct _SVector* _v1;


	NewPos = pCollideInfo->newPoint;
	OldPos = pCollideInfo->oldPoint;

	CSpad = (struct PandTScratch*)getScratchAddr(12);



	instance = pCollideInfo->instance;
	CSpad->backface_flag = Backface_Flag;

	CSpad->flags = Flags;
	CSpad->slack = 0;
	CSpad->ignore_attr = ignoreAttr;
	CSpad->accept_attr = acceptAttr;
	CSpad->normalList = (short*)terrain->normalList;

	vertexList = terrain->vertexList;
	CSpad->result = 0;
	CSpad->vertexList = vertexList;

	_v = &CSpad->newPos;
	_x0 = NewPos->vx;
	_y0 = NewPos->vy;
	_z0 = NewPos->vz;
	_v->x = _x0;
	_v->y = _y0;
	_v->z = _z0;

	_v = &CSpad->oldPos;
	_x1 = OldPos->vx;
	_y1 = OldPos->vy;
	_z1 = OldPos->vz;
	_v->x = _x1;
	_v->y = _y1;
	_v->z = _z1;

	CSpad->line.x = _x1 - _x0;
	CSpad->line.y = _y1 - _y0;
	CSpad->line.z = _z1 - _z0;


	stack = (void**)getScratchAddr(43);

	if (gameTrackerX.gameData.asmData.MorphTime != 1000)
	{
		CSpad->in_spectral = 2;
		CSpad->slack = 2048;
	}
	else
	{
		if (gameTrackerX.gameData.asmData.MorphType == 1)
		{
			CSpad->in_spectral = 1;
		}
		else
		{
			CSpad->in_spectral = 0;

		}
	}
	if (CSpad->backface_flag)
		CSpad->backface_flag[0] = 0;
	curTree = 0;
	if (terrain->numBSPTrees > 0)
	{
		while (1)
		{
			BSPTreeArray = terrain->BSPTreeArray;
			*stack = stack;
			v19 = &BSPTreeArray[curTree];
			if (v19->ID < 0)
				goto LABEL_98;
			v20 = v19->flags;
			if ((v20 & 2) != 0)
			{
				collideType = pCollideInfo->collideType;
				if ((collideType & 0x80) == 0 && ((v20 & 0x40) == 0 || (collideType & 0x100) == 0))
					goto LABEL_98;
			}
			if ((v20 & 0x4100) == 16640)
				break;
			v23 = v20 & 0x4000;
			if ((v19->flags & 0x100) != 0)
			{
				v23 = v20 & 0x4000;
				if (!gameTrackerX.block_collide_override)
					goto LABEL_98;
			}
			if (v23)
			{
				raziel_collide_override = (unsigned char)gameTrackerX.raziel_collide_override;
			LABEL_19:
				if (!raziel_collide_override)
					goto LABEL_98;
			}
			if ((v19->flags & 0x1000) == 0 || (CSpad->ignore_attr & 0x10) == 0)
			{
				if (!(v19->flags & 0x2000) || gameTrackerX.monster_collide_override)
				{
					_x1 = CSpad->newPos.x - v19->globalOffset.x;
					_y1 = CSpad->newPos.y - v19->globalOffset.y;
					_z1 = CSpad->newPos.z - v19->globalOffset.z;
					CSpad->newPos.x = _x1;
					CSpad->newPos.y = _y1;
					CSpad->newPos.z = _z1;
					_x1 = CSpad->oldPos.x - v19->globalOffset.x;
					_y1 = CSpad->oldPos.y - v19->globalOffset.y;
					_z1 = CSpad->oldPos.z - v19->globalOffset.z;
					CSpad->oldPos.x = _x1;
					CSpad->oldPos.y = _y1;
					CSpad->oldPos.z = _z1;
					CSpad->posMatrix.m[0][0] = CSpad->newPos.x;
					CSpad->posMatrix.m[0][1] = CSpad->newPos.y;
					CSpad->posMatrix.m[0][2] = CSpad->newPos.z;
					CSpad->posMatrix.m[1][0] = _x1;
					CSpad->posMatrix.m[1][1] = _y1;
					CSpad->posMatrix.m[1][2] = _z1;

					gte_SetRotMatrix(&CSpad->posMatrix);
					v35 = v19->bspRoot == (struct _BSPNode*)++stack;
					if (!v35)
					{
						*stack = v19->bspRoot;

						while (1)
						{
							bspNode = (struct _BSPNode*)*stack;
							if (!(*((unsigned short*)*stack-- + 7) & 2))
								break;
							if (((short*)&bspNode->front)[1] + CSpad->slack >= CSpad->posMatrix.m[0][0] || ((short*)&bspNode->front)[1] + CSpad->slack >= CSpad->posMatrix.m[1][0])
							{
								if (CSpad->posMatrix.m[0][0] >= ((short*)&bspNode->d)[0] - CSpad->slack || CSpad->posMatrix.m[1][0] >= ((short*)&bspNode->d)[0] - CSpad->slack)
								{
									if (((short*)&bspNode->back)[0] + CSpad->slack >= CSpad->posMatrix.m[0][1] || ((short*)&bspNode->back)[0] + CSpad->slack >= CSpad->posMatrix.m[1][1])
									{
										if (CSpad->posMatrix.m[0][1] >= ((short*)&bspNode->d)[1] - CSpad->slack || CSpad->posMatrix.m[1][1] >= ((short*)&bspNode->d)[1] - CSpad->slack)
										{
											if (((short*)&bspNode->back)[1] + CSpad->slack >= CSpad->posMatrix.m[0][2] || ((short*)&bspNode->back)[1] + CSpad->slack >= CSpad->posMatrix.m[1][2])
											{
												if (CSpad->posMatrix.m[0][2] >= ((short*)&bspNode->front)[0] - CSpad->slack || CSpad->posMatrix.m[1][2] >= ((short*)&bspNode->front)[0] - CSpad->slack)
												{
													CSpad->i = bspNode->c;
													v44 = *(unsigned short**)&bspNode->a;
													if (CSpad->i)
													{
														v45 = (struct _TFace*)(v44);
														do
														{
															attr = (unsigned char)(v45)->attr;
															if ((attr & CSpad->ignore_attr) == 0 || (attr & CSpad->accept_attr) != 0)
															{
																if (CSpad->in_spectral == 2
																	&& (unsigned short)(v45)->normal != *((((int)-(((char*)v45 - (char*)terrain->faceList) * 0x55555555) >> 2)) + terrain->morphNormalIdx))
																{
																	//v81 = p_splitPoint;
																	v83 = terrain;
																	COLLIDE_MakeNormal(terrain, (struct _TFace*)v45, &CSpad->normal);
																	//p_splitPoint = v81;
																	terrain = v83;
																}
																else
																{
																	normal = (short)(v45)->normal;
																	nrmlArray = CSpad->normalList;
																	if (normal >= 0)
																	{
																		v48 = &nrmlArray[normal * 3];
																		v49 = *v48++;
																		CSpad->normal.x = v49 & 0x1FFF;
																		CSpad->normal.y = *v48;
																		v50 = (unsigned short)v48[1];
																	}
																	else
																	{
																		v51 = &nrmlArray[-normal * 3];
																		v52 = *v51++;
																		CSpad->normal.x = -(v52 & 0x1FFF);
																		CSpad->normal.y = -*v51;
																		v50 = -(unsigned short)v51[1];
																	}
																	CSpad->normal.z = v50;
																}
																v53 = &CSpad->vertexList[v45->face.v0];
																gte_ldv2_ext(v53);
																gte_ldv0(&CSpad->normal);
																gte_rtv0();
																gte_stlvnl(&CSpad->dpv);
																CSpad->dpv.x -= CSpad->dpv.z;
																v57 = CSpad->dpv.y - CSpad->dpv.z;
																CSpad->dpv.y -= CSpad->dpv.z;
																if (CSpad->dpv.x < 0 && v57 >= 0)
																	goto LABEL_102;
																if ((CSpad->flags & 1) != 0 && CSpad->dpv.x > 0 && v57 <= 0)
																{
																LABEL_102:
																	v58 = (CSpad->dpv.x - CSpad->dpv.y) ? (CSpad->dpv.y << 12) / (CSpad->dpv.x - CSpad->dpv.y) : 0;
																	CSpad->planePoint.x = CSpad->oldPos.x + ((CSpad->line.x * v58) >> 12);
																	CSpad->planePoint.y = CSpad->oldPos.y + ((CSpad->line.y * v58) >> 12);
																	CSpad->planePoint.z = CSpad->oldPos.z + ((CSpad->line.z * v58) >> 12);
																	v84 = terrain;
																	v59 = COLLIDE_PointInTriangle(
																		(struct _SVector*)v53,
																		(struct _SVector*)&CSpad->vertexList[(v45)->face.v1],
																		(struct _SVector*)&CSpad->vertexList[(v45)->face.v2],
																		&CSpad->planePoint,
																		&CSpad->normal);
																	terrain = v84;
																	if (v59)
																	{
																		if (CSpad->backface_flag
																			&& (CSpad->flags & 1) != 0
																			&& CSpad->dpv.x > 0
																			&& CSpad->dpv.y <= 0)
																		{
																			CSpad->backface_flag[0] = 1;
																		}
																		if (((v45)->attr & 8) != 0)
																		{
																			if ((gameTrackerX.gameFlags & 0x8000) != 0)
																			{
																				instance->waterFace = (struct _TFace*)v44;
																				instance->waterFaceTerrain = v84;

																				instance->splitPoint = CSpad->planePoint;

																				instance->splitPoint.x += v19->globalOffset.x;
																				instance->splitPoint.y += v19->globalOffset.y;
																				instance->splitPoint.z += v19->globalOffset.z;
																			}
																		}
																		else if ((unsigned short)(v45)->textoff == 0xFFFF
																			|| (*(short*)((char*)&v84->StartTextureList->attr
																				+ (unsigned short)(v45)->textoff) & 0x2000) == 0
																			|| ((v45)->attr & CSpad->accept_attr) != 0)
																		{
																			*(unsigned int*)&CSpad->newPos.z = *(unsigned int*)&CSpad->planePoint.z;
																			*(unsigned int*)&CSpad->newPos.x = *(unsigned int*)&CSpad->planePoint.x;
																			*(unsigned int*)&CSpad->posMatrix.m[0][0] = *(unsigned int*)&CSpad->newPos.x;
																			CSpad->result = (struct _TFace*)v44;
																			CSpad->posMatrix.m[0][2] = CSpad->newPos.z;

																			_v = &CSpad->newPos;
																			_v0 = &CSpad->oldPos;
																			_v1 = &CSpad->line;

																			_x0 = _v0->x;
																			_y0 = _v0->y;
																			_z0 = _v0->z;
																			_x1 = _v->x;
																			_y1 = _v->y;
																			_z1 = _v->z;

																			_x0 -= _x1;
																			_y0 -= _y1;
																			_z0 -= _z1;

																			_v1->x = _x0;
																			_v1->y = _y0;
																			_v1->z = _z0;

																			if (lcolinfo)
																			{
																				lcolinfo->tface = (struct _TFace*)v44;
																				lcolinfo->terrain = v84;
																				lcolinfo->curTree = curTree;
																			}
																		}
																		gte_ldsvrtrow0(&CSpad->posMatrix);
																	}
																}
															}
															++v45;
															v35 = CSpad->i--;
														} while (v35);
													}
													if (CSpad->result && !CSpad->in_spectral)
														*stack = stack;
												}
											}
										}
									}
								}
							}
						LABEL_96:
							if (*stack == stack)
								goto LABEL_97;
						}
						gte_ldv0(&bspNode->a);
						gte_rtv0();
						gte_stlvnl(&CSpad->dpv);
						CSpad->dpv.x -= bspNode->d;
						CSpad->dpv.y -= bspNode->d;
						if (CSpad->in_spectral)
						{
							back_spectral_error = bspNode->back_spectral_error;
							v67 = bspNode->front_spectral_error + 5;
						}
						else
						{
							back_spectral_error = bspNode->back_material_error;
							v67 = bspNode->front_material_error + 5;
						}
						v68 = back_spectral_error - 5;
						if (CSpad->dpv.y >= v67)
						{
							if (v67 >= CSpad->dpv.x)
							{
								goto LABEL_91;
							}
							else
							{
								goto LABEL_93;

							}
						}
						if (v68 >= CSpad->dpv.y)
						{
							if (CSpad->dpv.x >= CSpad->dpv.y)
							{
							LABEL_91:
								back = (void*)bspNode->back;
								if (back)
									*++stack = back;
							LABEL_93:
								front = (void*)bspNode->front;
							}
							else
							{
								v71 = (void*)bspNode->front;
								if (v71)
									*++stack = v71;
								front = (void*)bspNode->back;
							}
						}
						else
						{
							if (CSpad->dpv.x >= v68)
							{
								v69 = (void*)bspNode->front;
								if (v69)
									*++stack = v69;
							}
							front = (void*)bspNode->back;
						}
						if (front)
							*++stack = front;
						goto LABEL_96;
					}
				LABEL_97:
					_x1 = CSpad->newPos.x + v19->globalOffset.x;
					_y1 = CSpad->newPos.y + v19->globalOffset.y;
					_z1 = CSpad->newPos.z + v19->globalOffset.z;
					CSpad->newPos.x = _x1;
					CSpad->newPos.y = _y1;
					CSpad->newPos.z = _z1;
					_x1 = CSpad->oldPos.x + v19->globalOffset.x;
					_y1 = CSpad->oldPos.y + v19->globalOffset.y;
					_z1 = CSpad->oldPos.z + v19->globalOffset.z;
					CSpad->oldPos.x = _x1;
					CSpad->oldPos.y = _y1;
					CSpad->oldPos.z = _z1;
				}
			}
		LABEL_98:
			if (++curTree >= terrain->numBSPTrees)
				goto LABEL_99;
		}
		raziel_collide_override = (unsigned char)gameTrackerX.plan_collide_override;
		goto LABEL_19;
	}
LABEL_99:
	y = CSpad->newPos.y;
	z = CSpad->newPos.z;
	NewPos->vx = CSpad->newPos.x;
	NewPos->vy = y;
	NewPos->vz = z;
	return CSpad->result;
}

// autogenerated function stub: 
// int /*$ra*/ COLLIDE_PointAndHFace(struct _SVector *newPos /*$s4*/, struct _SVector *oldPos /*$s6*/, struct _HFace *hface /*$s5*/, struct _Model *model /*$s7*/, struct _SVector *hfNormal /*stack 16*/)
int COLLIDE_PointAndHFace(struct _SVector *newPos, struct _SVector *oldPos, struct _HFace *hface, struct _Model *model, struct _SVector *hfNormal)
{ // line 1330, offset 0x8001fc64
#if 0
	/* begin block 1 */
		// Start line: 1331
		// Start offset: 0x8001FC64
		// Variables:
			MATRIX *posMatrix; // $s1
			struct _SVector *normal; // $s3
			struct _Vector *dpv; // $s2
			struct _SVector *planePoint; // stack offset -48
			struct _SVector *vertex0; // $s0
			struct _SVector *vertex1; // $s1

		/* begin block 1.1 */
			// Start line: 1341
			// Start offset: 0x8001FCD0
			// Variables:
				short _x0; // $v0
				short _y0; // $v1
				short _z0; // $a1
				short _x1; // $a2
				short _y1; // $a3
				short _z1; // $t0
		/* end block 1.1 */
		// End offset: 0x8001FCD0
		// End Line: 1341

		/* begin block 1.2 */
			// Start line: 1341
			// Start offset: 0x8001FCD0
			// Variables:
				short _x0; // $v0
				short _y0; // $v1
				short _z0; // $a1
				short _x1; // $a2
				short _y1; // $a3
				short _z1; // $t0
				struct _SVector *_v; // $t1
		/* end block 1.2 */
		// End offset: 0x8001FCD0
		// End Line: 1341

		/* begin block 1.3 */
			// Start line: 1341
			// Start offset: 0x8001FCD0
			// Variables:
				short _x1; // $v0
				short _y1; // $v1
				short _z1; // $a1
				struct _SVector *_v0; // $t3
		/* end block 1.3 */
		// End offset: 0x8001FCD0
		// End Line: 1341
	/* end block 1 */
	// End offset: 0x8001FE7C
	// End Line: 1378

	/* begin block 2 */
		// Start line: 3219
	/* end block 2 */
	// End Line: 3220
#endif
				UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ COLLIDE_PointAndInstance(struct _PCollideInfo *pcollideInfo /*$s4*/, struct _Instance *instance /*stack 4*/)
void COLLIDE_PointAndInstance(struct _PCollideInfo *pcollideInfo, struct _Instance *instance)
{ // line 1389, offset 0x8001feac
#if 0
	/* begin block 1 */
		// Start line: 1390
		// Start offset: 0x8001FEAC
		// Variables:
			MATRIX *swTransform; // stack offset -92
			MATRIX *wsTransform; // stack offset -88
			MATRIX *swNormMat; // $fp
			struct _Vector *oldPosVec; // stack offset -84
			struct _Vector *lNormal; // stack offset -80
			struct _Vector *wNormal; // stack offset -76
			struct _Vector *dv; // $s2
			struct _Vector *newPosVec; // $s5
			struct _SVector *oldPos; // stack offset -72
			struct _SVector *newPos; // $s3
			struct _SVector *point; // stack offset -68
			long *flag; // stack offset -64
			struct _Model *model; // stack offset -60
			struct _HModel *hmodel; // $v0
			struct _HPrim *hprim; // stack offset -56
			int i; // stack offset -52
			long collideType; // stack offset -48

		/* begin block 1.1 */
			// Start line: 1416
			// Start offset: 0x8001FFCC
			// Variables:
				struct _HBox *hbox; // $s0
				struct _HFace *hface; // $s1
				struct _HSphere *hsphere; // $s0

			/* begin block 1.1.1 */
				// Start line: 1419
				// Start offset: 0x8001FFCC
				// TypeDefs:
					struct COLLIDE_258fake tmm
			/* end block 1.1.1 */
			// End offset: 0x8001FFCC
			// End Line: 1419

			/* begin block 1.1.2 */
				// Start line: 1452
				// Start offset: 0x80020184
				// Variables:
					long len; // $t0

				/* begin block 1.1.2.1 */
					// Start line: 1453
					// Start offset: 0x80020184
					// Variables:
						long a; // stack offset -104
						long b; // stack offset -100
						long c; // stack offset -96
				/* end block 1.1.2.1 */
				// End offset: 0x80020184
				// End Line: 1453
			/* end block 1.1.2 */
			// End offset: 0x80020248
			// End Line: 1459

			/* begin block 1.1.3 */
				// Start line: 1488
				// Start offset: 0x800203F0
				// Variables:
					struct _SVector hfNormal; // stack offset -136
			/* end block 1.1.3 */
			// End offset: 0x80020418
			// End Line: 1494

			/* begin block 1.1.4 */
				// Start line: 1509
				// Start offset: 0x80020450
				// Variables:
					struct _SVector hbNormal; // stack offset -136
					struct _SVector point0; // stack offset -128
					struct _SVector point1; // stack offset -120
					struct _SVector normal1; // stack offset -112

				/* begin block 1.1.4.1 */
					// Start line: 1551
					// Start offset: 0x80020590
					// Variables:
						short _x1; // $v0
						short _y1; // $v1
						short _z1; // $a3
				/* end block 1.1.4.1 */
				// End offset: 0x80020590
				// End Line: 1551
			/* end block 1.1.4 */
			// End offset: 0x800205D8
			// End Line: 1561
		/* end block 1.1 */
		// End offset: 0x80020690
		// End Line: 1580
	/* end block 1 */
	// End offset: 0x800206B0
	// End Line: 1582

	/* begin block 2 */
		// Start line: 3368
	/* end block 2 */
	// End Line: 3369
#endif
						UNIMPLEMENTED();
}

void COLLIDE_PointAndInstanceTrivialReject(struct _PCollideInfo* pcollideInfo, struct _Instance* instance)//Matching - 99.12%
{
	struct _Vector* dv;
	struct _SVector linePoint;
	long _x0;
	long _y0;
	long _z0;
	long _x1;
	long _y1;
	long _z1;
	struct _SVector* _v0;
	struct _Position* _v1;

	dv = (struct _Vector*)getScratchAddr(0);

	if (MEMPACK_MemoryValidFunc((char*)instance->object)
		&& !(instance->flags & 0x40)
		&& instance->hModelList
		&& (!(pcollideInfo->collideType & 0x40) || !(instance->object->oflags2 & 0x40)))
	{
		_v1 = &instance->position;

		COLLIDE_NearestPointOnLine_S(&linePoint, pcollideInfo->oldPoint, pcollideInfo->newPoint, &instance->position);

		_v0 = &linePoint;

		_x1 = _v0->x;
		_y1 = _v0->y;
		_z1 = _v0->z;

		_x0 = _v1->x;
		_y0 = _v1->y;
		_z0 = _v1->z;

		dv->x = (_x1 - _x0);
		dv->y = (_y1 - _y0);
		dv->z = (_z1 - _z0);

		dv->x >>= 1;
		dv->y >>= 1;
		dv->z >>= 1;

		if ((dv->x * dv->x) + (dv->y * dv->y) + (dv->z * dv->z) < (instance->object->modelList[instance->currentModel]->maxRadSq >> 2))
		{
			if (instance->matrix)
			{
				COLLIDE_PointAndInstance(pcollideInfo, instance);
			}
		}
	}
}

void COLLIDE_PointAndWorld(struct _PCollideInfo* pcollideInfo, struct Level* level)//Matching - 96.50%
{
	int i;
	struct _LCollideInfo lcol;
	struct _Instance* instance;
	struct Level* thislevel;
	struct _TFace* tface;
	int in_warpRoom;
	struct _Terrain* terrain;
	struct _InstanceList* instanceList;
	struct _StreamUnit* streamUnit;

	in_warpRoom = 0;

	pcollideInfo->type = 0;

	if ((pcollideInfo->collideType & 0x1))
	{
		tface = NULL;

		if (level != NULL && MEMPACK_MemoryValidFunc((char*)level))
		{
			terrain = level->terrain;

			tface = COLLIDE_PointAndTerrain(terrain, pcollideInfo, &lcol);

			if (tface != NULL)
			{
				pcollideInfo->type = 3;
				pcollideInfo->prim = tface;
				pcollideInfo->inst = (struct _Instance*)level;
				pcollideInfo->segment = lcol.curTree;

				if (gameTrackerX.gameData.asmData.MorphTime != 1000)
				{
					COLLIDE_MakeNormal(terrain, tface, (struct _SVector*)&pcollideInfo->wNormal);
				}
				else
				{
					COLLIDE_GetNormal((short)tface->normal, &terrain->normalList->x, (struct _SVector*)&pcollideInfo->wNormal);
				}
			}
			else if ((STREAM_GetStreamUnitWithID(level->streamUnitID)->flags & 0x1) != 0)
			{
				in_warpRoom = 1;
			}
		}
		if (tface == NULL)
		{
			streamUnit = &StreamTracker.StreamList[0];

			for (i = 0; i < 16; i++, streamUnit++)
			{
				thislevel = streamUnit->level;

				if (streamUnit->used == 2 && thislevel != level && (!in_warpRoom || !(streamUnit->flags & 0x1)))
				{
					if (MEMPACK_MemoryValidFunc((char*)thislevel))
					{
						terrain = thislevel->terrain;

						tface = COLLIDE_PointAndTerrain(terrain, pcollideInfo, &lcol);

						if (tface != NULL)
						{
							pcollideInfo->type = 3;
							pcollideInfo->prim = tface;
							pcollideInfo->inst = (struct _Instance*)thislevel;
							pcollideInfo->segment = lcol.curTree;

							if (gameTrackerX.gameData.asmData.MorphTime != 1000)
							{
								COLLIDE_MakeNormal(terrain, tface, (struct _SVector*)&pcollideInfo->wNormal);
							}
							else
							{
								COLLIDE_GetNormal((short)tface->normal, &terrain->normalList->x, (struct _SVector*)&pcollideInfo->wNormal);
							}
							break;
						}
					}
				}
			}

			if (tface == NULL)
			{
				pcollideInfo->type = 0;
				pcollideInfo->prim = 0;
				pcollideInfo->inst = 0;
				pcollideInfo->wNormal.vx = 0;
				pcollideInfo->wNormal.vy = 0;
				pcollideInfo->wNormal.vz = 0;
			}
		}
	}

	instanceList = gameTrackerX.instanceList;

	if ((pcollideInfo->collideType & 0x8) != 0)
	{
		for (i = 16; i < 32; i++)
		{
			instance = (struct _Instance*)instanceList->group[i].next;

			while (instance)
			{
				if (!(instance->flags2 & 0x24000000))
				{
					COLLIDE_PointAndInstanceTrivialReject(pcollideInfo, instance);
				}

				instance = (struct _Instance*)instance->node.next;
			}
		}
	}
	else if ((pcollideInfo->collideType & 0x10) != 0)
	{
		if ((pcollideInfo->collideType & 0x2) != 0)
		{
			for (i = 0; i < 8; i++)
			{
				instance = (struct _Instance*)instanceList->group[stat_clddyna[i]].next;

				while (instance)
				{
					if (!(instance->flags2 & 0x24000000))
					{
						COLLIDE_PointAndInstanceTrivialReject(pcollideInfo, instance);
					}

					instance = (struct _Instance*)instance->node.next;
				}
			}
		}
		if ((pcollideInfo->collideType & 0x4) != 0)
		{
			for (i = 0; i < 8; i++)
			{
				instance = (struct _Instance*)instanceList->group[dyna_clddyna[i]].next;

				while (instance)
				{
					if (!(instance->flags2 & 0x24000000))
					{
						COLLIDE_PointAndInstanceTrivialReject(pcollideInfo, instance);
					}

					instance = (struct _Instance*)instance->node.next;
				}
			}
		}
	}
	else
	{
		if ((pcollideInfo->collideType & 0x4))
		{
			for (i = 0; i < 8; i++)
			{
				instance = (struct _Instance*)instanceList->group[dyna_cldstat[i]].next;

				while (instance)
				{
					if (!(instance->flags2 & 0x24000000))
					{
						COLLIDE_PointAndInstanceTrivialReject(pcollideInfo, instance);
					}

					instance = (struct _Instance*)instance->node.next;
				}
			}
		}
	}
}

// autogenerated function stub: 
// long /*$ra*/ COLLIDE_ClosestPointInBoxToPoint(_Position *boxPoint /*$a0*/, struct _HBox *hbox /*$a1*/, struct _SVector *point /*$a2*/)
long COLLIDE_ClosestPointInBoxToPoint(_Position *boxPoint, struct _HBox *hbox, struct _SVector *point)
{ // line 1893, offset 0x80020c4c
	/* begin block 1 */
		// Start line: 1895
		// Start offset: 0x80020C4C
		// Variables:
			long inside; // $a3
	/* end block 1 */
	// End offset: 0x80020D18
	// End Line: 1941

	/* begin block 2 */
		// Start line: 4803
	/* end block 2 */
	// End Line: 4804

	/* begin block 3 */
		// Start line: 4804
	/* end block 3 */
	// End Line: 4805

	/* begin block 4 */
		// Start line: 4806
	/* end block 4 */
	// End Line: 4807
			UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// long /*$ra*/ COLLIDE_SphereAndPoint(struct _Sphere *sphere /*$s1*/, struct _SVector *point /*$s2*/, struct _SVector *normal /*$s3*/)
long COLLIDE_SphereAndPoint(struct _Sphere *sphere, struct _SVector *point, struct _SVector *normal)
{ // line 1944, offset 0x80020d20
	/* begin block 1 */
		// Start line: 1945
		// Start offset: 0x80020D20
		// Variables:
			long len; // $a2
			struct _Vector *line; // $s0

		/* begin block 1.1 */
			// Start line: 1945
			// Start offset: 0x80020D20
			// Variables:
				long _x0; // $a2
				long _y0; // $a1
				long _z0; // $a0
				long _x1; // $v0
				long _y1; // $v0
				long _z1; // $v0
		/* end block 1.1 */
		// End offset: 0x80020D20
		// End Line: 1945

		/* begin block 1.2 */
			// Start line: 1953
			// Start offset: 0x80020DB4
			// Variables:
				long a; // stack offset -40
				long b; // stack offset -36
				long c; // stack offset -32
		/* end block 1.2 */
		// End offset: 0x80020DB4
		// End Line: 1953
	/* end block 1 */
	// End offset: 0x80020F48
	// End Line: 1970

	/* begin block 2 */
		// Start line: 4905
	/* end block 2 */
	// End Line: 4906
				UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// long /*$ra*/ COLLIDE_SphereAndHBox(struct _HBox *hbox /*$s4*/, struct _Sphere *sphere /*$s1*/, _Position *oldPos /*$s5*/, struct _SVector *normal /*$s3*/)
long COLLIDE_SphereAndHBox(struct _HBox *hbox, struct _Sphere *sphere, _Position *oldPos, struct _SVector *normal)
{ // line 1975, offset 0x80020f64
#if 0
	/* begin block 1 */
		// Start line: 1976
		// Start offset: 0x80020F64
		// Variables:
			struct _SVector point; // stack offset -96
			struct _SVector point0; // stack offset -88
			struct _SVector point1; // stack offset -80
			struct _SVector normal0; // stack offset -72
			struct _SVector normal1; // stack offset -64
			struct _SVector sphereNormal; // stack offset -56
			struct _SVector dv; // stack offset -48
			struct _SVector absdv; // stack offset -40

		/* begin block 1.1 */
			// Start line: 1989
			// Start offset: 0x80020FAC
			// Variables:
				long numIntersects; // $a2

			/* begin block 1.1.1 */
				// Start line: 1993
				// Start offset: 0x80020FDC
				// Variables:
					short _x1; // $v0
					short _y1; // $v1
					short _z1; // $a0
			/* end block 1.1.1 */
			// End offset: 0x80020FDC
			// End Line: 1993

			/* begin block 1.1.2 */
				// Start line: 1993
				// Start offset: 0x80020FDC
				// Variables:
					short _x1; // $a0
					short _y1; // $v1
					short _z1; // $a1
					struct _SVector *_v0; // $v0
			/* end block 1.1.2 */
			// End offset: 0x80020FDC
			// End Line: 1993

			/* begin block 1.1.3 */
				// Start line: 2000
				// Start offset: 0x8002107C
				// Variables:
					short _y0; // $v0
					short _z0; // $v1
					short _x1; // $v0
					short _y1; // $a2
					short _z1; // $a3
					struct _SVector *_v; // $a1
					struct _SVector *_v0; // $v1
			/* end block 1.1.3 */
			// End offset: 0x8002107C
			// End Line: 2000
		/* end block 1.1 */
		// End offset: 0x80021290
		// End Line: 2076

		/* begin block 1.2 */
			// Start line: 2080
			// Start offset: 0x80021290

			/* begin block 1.2.1 */
				// Start line: 2087
				// Start offset: 0x800212D4
				// Variables:
					short _x1; // $v0
					short _y1; // $v1
					short _z1; // $a0
					struct _SVector *_v0; // $v0
					struct _SVector *_v1; // $v0
			/* end block 1.2.1 */
			// End offset: 0x800212D4
			// End Line: 2087

			/* begin block 1.2.2 */
				// Start line: 2087
				// Start offset: 0x800212D4
				// Variables:
					short _x1; // $v1
					short _y1; // $a0
					short _z1; // $a1
			/* end block 1.2.2 */
			// End offset: 0x800212D4
			// End Line: 2087

			/* begin block 1.2.3 */
				// Start line: 2096
				// Start offset: 0x80021318
				// Variables:
					short _x1; // $v0
					short _y1; // $v1
					short _z1; // $a0
			/* end block 1.2.3 */
			// End offset: 0x80021318
			// End Line: 2096

			/* begin block 1.2.4 */
				// Start line: 2096
				// Start offset: 0x80021318
				// Variables:
					short _x1; // $a0
					short _y1; // $v1
					short _z1; // $a1
					struct _SVector *_v0; // $v0
			/* end block 1.2.4 */
			// End offset: 0x80021318
			// End Line: 2096
		/* end block 1.2 */
		// End offset: 0x800213B8
		// End Line: 2106
	/* end block 1 */
	// End offset: 0x800213B8
	// End Line: 2108

	/* begin block 2 */
		// Start line: 4981
	/* end block 2 */
	// End Line: 4982
#endif
					UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ COLLIDE_Instance1SpheresToInstance2(struct _Instance *instance1 /*stack 0*/, struct _Instance *instance2 /*$fp*/, long sphereToSphere /*$a2*/)
void COLLIDE_Instance1SpheresToInstance2(struct _Instance *instance1, struct _Instance *instance2, long sphereToSphere)
{ // line 2419, offset 0x800213dc
#if 0
	/* begin block 1 */
		// Start line: 2420
		// Start offset: 0x800213DC
		// Variables:
			MATRIX *wsTransform2; // stack offset -132
			MATRIX *oldWSTransform2; // stack offset -128
			_Position *spherePos; // stack offset -124
			struct _Vector *line; // $s7
			struct _Vector *offset; // stack offset -120
			struct _Vector *tempVec; // stack offset -116
			struct _Vector *sSpherePos1; // stack offset -112
			struct _Vector *oldSSpherePos1; // stack offset -108
			struct _SVector *svec; // stack offset -104
			struct _Sphere *sSphere1; // $s3
			struct _Sphere *oldSSphere1; // stack offset -100
			struct _Sphere *wSphere1; // stack offset -96
			struct _Sphere *wSphere2; // stack offset -92
			struct _Sphere *oldWSphere1; // stack offset -88
			struct _Sphere *oldWSphere2; // stack offset -84
			struct _CollideInfo *collideInfo; // $s2
			struct _HFaceInfo *hfaceInfo; // stack offset -80
			MATRIX *swTransform1; // stack offset -76
			MATRIX *swTransform2; // stack offset -72
			MATRIX *oldSWTransform1; // $s0
			MATRIX *oldSWTransform2; // $s4
			struct _HSphere *hsphere1; // $s6
			struct _HSphere *hsphere2; // $s0
			struct _HFace *hface; // $s0
			void (*collideFunc)(); // $a3
			long flag; // stack offset -152
			long edge; // stack offset -136
			struct _HModel *hmodel1; // $a0
			struct _HModel *hmodel2; // stack offset -68
			struct _HPrim *hprim1; // stack offset -64
			struct _HPrim *hprim2; // stack offset -60
			int i; // stack offset -56

		/* begin block 1.1 */
			// Start line: 2478
			// Start offset: 0x80021578
			// Variables:
				int i; // stack offset -52

			/* begin block 1.1.1 */
				// Start line: 2524
				// Start offset: 0x800217F0
				// Variables:
					long len; // $a3

				/* begin block 1.1.1.1 */
					// Start line: 2525
					// Start offset: 0x800217F0
					// Variables:
						long a; // stack offset -148
						long b; // stack offset -144
						long c; // stack offset -140
				/* end block 1.1.1.1 */
				// End offset: 0x800217F0
				// End Line: 2525

				/* begin block 1.1.1.2 */
					// Start line: 2531
					// Start offset: 0x80021888
					// Variables:
						short _x1; // $v1
						short _y1; // $a0
						short _z1; // $v0
						_Position *_v1; // $v0
				/* end block 1.1.1.2 */
				// End offset: 0x80021888
				// End Line: 2531

				/* begin block 1.1.1.3 */
					// Start line: 2536
					// Start offset: 0x800218B0
					// Variables:
						short _x1; // $v0
						short _y1; // $v1
						short _z1; // $a0
				/* end block 1.1.1.3 */
				// End offset: 0x800218B0
				// End Line: 2536
			/* end block 1.1.1 */
			// End offset: 0x800219E4
			// End Line: 2561

			/* begin block 1.1.2 */
				// Start line: 2577
				// Start offset: 0x80021A58
				// Variables:
					struct _Model *model2; // $s1

				/* begin block 1.1.2.1 */
					// Start line: 2676
					// Start offset: 0x80021E58
					// Variables:
						struct _HBox *hbox; // $s4
				/* end block 1.1.2.1 */
				// End offset: 0x800220B4
				// End Line: 2742
			/* end block 1.1.2 */
			// End offset: 0x800220B4
			// End Line: 2743
		/* end block 1.1 */
		// End offset: 0x800220D4
		// End Line: 2746
	/* end block 1 */
	// End offset: 0x800220FC
	// End Line: 2748

	/* begin block 2 */
		// Start line: 4838
	/* end block 2 */
	// End Line: 4839
#endif
						UNIMPLEMENTED();
}

void COLLIDE_Instances(struct _Instance* instance1, struct _Instance* instance2)//Matching - 89.57%
{
	long lx;
	long ly;
	long lz;
	long mrmr;

	if (instance1 != instance2 && !INSTANCE_Linked(instance1, instance2))
	{
		lx = (instance1->position.x - instance2->position.x) >> 1;
		ly = (instance1->position.y - instance2->position.y) >> 1;
		lz = (instance1->position.z - instance2->position.z) >> 1;

		mrmr = (instance1->object->modelList[instance1->currentModel]->maxRad + instance2->object->modelList[instance2->currentModel]->maxRad) >> 1;

		gte_ldsv_ext((short)lx, (short)ly, (short)lz);
		gte_sqr0();
		gte_stsv_ext(lx, ly, lz);

		if (lx + ly + lz < (long)(unsigned int)(mrmr * mrmr) && instance1->matrix != NULL && instance1->oldMatrix != NULL && instance2->matrix != NULL && instance2->oldMatrix != NULL)
		{
			COLLIDE_Instance1SpheresToInstance2(instance1, instance2, 1);
			COLLIDE_Instance1SpheresToInstance2(instance2, instance1, 0);
		}
	}
}

void COLLIDE_InstanceList(struct _InstanceList* instanceList)
{
	struct _Instance* instance;
	struct _Instance* instance2;
	struct _Instance* playerInstance;
	int i;
	int j;

	playerInstance = gameTrackerX.playerInstance;

	if (gameTrackerX.cheatMode != 0x1)
	{
		for (i = 16; i < 32; i++)
		{
			instance = (struct _Instance*)instanceList->group[i].next;
			instance2 = instance;

			while (instance2 != NULL)
			{
				if ((instance2->flags2 & 0x24040000) == 0)
				{
					COLLIDE_Instances(instance2, playerInstance);
				}

				instance2 = (struct _Instance*)instance2->node.next;
			}
		}
	}

	for (i = 0; i < 8; i++)
	{
		playerInstance = (struct _Instance*)instanceList->group[dyna_clddyna[i]].next;

		while (playerInstance != NULL)
		{
			if (!(playerInstance->flags2 & 0x24040000))
			{
				instance2 = (struct _Instance*)(playerInstance->node).next;

				while (instance2 != (struct _Instance*)0x0)
				{
					if (!(instance2->flags2 & 0x24040000))
					{
						COLLIDE_Instances(playerInstance, instance2);
					}

					instance2 = (struct _Instance*)instance2->node.next;
				}

				for (j = i + 1; j < 8; j++)
				{
					instance2 = (struct _Instance*)instanceList->group[dyna_clddyna[j]].next;
					while (instance2 != NULL)
					{
						if (!(instance2->flags2 & 0x24040000))
						{
							COLLIDE_Instances(playerInstance, instance2);
						}

						instance2 = (struct _Instance*)instance2->node.next;
					}
				}
			}

			playerInstance = (struct _Instance*)playerInstance->node.next;
		}
	}

	for (i = 0; i < 8; i++)
	{
		playerInstance = (struct _Instance*)instanceList->group[dyna_cldstat[i]].next;

		while (playerInstance != NULL)
		{
			if (!(playerInstance->flags2 & 0x24040000))
			{
				for (j = 0; j < 8; j++)
				{
					instance2 = (struct _Instance*)instanceList->group[stat_clddyna[j]].next;

					while (instance2 != NULL)
					{
						if (!(instance2->flags2 & 0x24040000))
						{
							COLLIDE_Instances(playerInstance, instance2);
						}

						instance2 = (struct _Instance*)instance2->node.next;
					}
				}
			}

			playerInstance = (struct _Instance*)playerInstance->node.next;
		}
	}

	return;
}

long COLLIDE_SphereAndHFace(struct _Sphere* sphere, struct _Position* oldPos, struct _HFaceInfo* hfaceInfo, struct _SVector* intersect, long* edge)//Matching - 60.48%
{
	struct _SVector* vertex0; // $a0
	int v10; // $s6
	int behind; // $v0
	int radius; // $v1
	unsigned long d0sq; // $s1
	unsigned long d1sq; // $a0
	int v46; // $a2
	long a; // [sp+28h] [-10h] BYREF
	long b; // [sp+2Ch] [-Ch] BYREF
	long c; // [sp+30h] [-8h] BYREF
	struct SandHFScratch* CSpad;
	long x;
	long y;
	long z;
	struct _SVector* _v1;
	struct _SVector* _v0;
	struct _Vector dv;
	short _x1; // $v0
	short _y1; // $v1
	short _z1; // $a0
	long _x0; // $a3
	long _y0; // $t0
	long _z0; // $t1
	long _x11; // $v1
	long _y11; // $v0
	long _z11; // $v1

	CSpad = (struct SandHFScratch*)getScratchAddr(21);
	vertex0 = (struct _SVector*)hfaceInfo->vertex0;
	v10 = 0;
	if ((hfaceInfo->hface->attr & 0x40) == 0)
	{
		*edge = 1;

		CSpad->posMatrix.m[0][0] = sphere->position.x - vertex0->x;
		CSpad->posMatrix.m[0][1] = sphere->position.y - vertex0->y;
		CSpad->posMatrix.m[0][2] = sphere->position.z - vertex0->z;
		CSpad->posMatrix.m[1][0] = oldPos->x - vertex0->x;
		CSpad->posMatrix.m[1][1] = oldPos->y - vertex0->y;
		CSpad->posMatrix.m[1][2] = oldPos->z - vertex0->z;
		CSpad->posMatrix.m[2][0] = vertex0->x;
		CSpad->posMatrix.m[2][1] = vertex0->y;
		CSpad->posMatrix.m[2][2] = vertex0->z;

		CSpad->normal = *(struct _SVector*)&hfaceInfo->normal;

		gte_SetRotMatrix(&CSpad->posMatrix);
		gte_ldv0(&CSpad->normal);
		gte_rtv0();
		gte_stlvnl(&CSpad->dpv);

		behind = 0;
		if (CSpad->dpv.y < CSpad->dpv.x)
			return behind;
		radius = sphere->radius;
		if (CSpad->dpv.x < radius)
		{
			behind = 0;
			if (CSpad->dpv.y < -radius)
				return behind;
			if (CSpad->dpv.x >= 0)
			{
				COLLIDE_NearestPointOnPlane_S(&CSpad->planePoint, &CSpad->normal, CSpad->dpv.z, &sphere->position);
			}
			else if (!COLLIDE_IntersectLineAndPlane_S(
				&CSpad->planePoint,
				oldPos,
				&sphere->position,
				&CSpad->normal,
				CSpad->dpv.z))
			{
				return 0;
			}
			if (COLLIDE_PointInTriangle(
				(struct _SVector*)hfaceInfo->vertex0,
				(struct _SVector*)hfaceInfo->vertex1,
				(struct _SVector*)hfaceInfo->vertex2,
				&CSpad->planePoint,
				&CSpad->normal))
			{
				_v0 = &CSpad->planePoint;

				_x1 = _v0->x;
				_y1 = _v0->y;
				_z1 = _v0->z;

				_v0 = &CSpad->triPoint;

				_v0->x = _x1;
				_v0->y = _y1;
				_v0->z = _z1;

				intersect->x = CSpad->triPoint.x;
				intersect->y = CSpad->triPoint.y;
				intersect->z = CSpad->triPoint.z;

				dv.x = (CSpad->normal.x * sphere->radius) >> 12;
				dv.y = (CSpad->normal.y * sphere->radius) >> 12;
				dv.z = (CSpad->normal.z * sphere->radius) >> 12;

				sphere->position.x = (short)(dv.x + CSpad->triPoint.x);
				sphere->position.y = (short)(dv.y + CSpad->triPoint.y);
				sphere->position.z = (short)(dv.z + CSpad->triPoint.z);

				behind = -1;
				*edge = 0;
				return behind;
			}
			COLLIDE_NearestPointOnLine_S(
				&CSpad->triPoint,
				(SVECTOR*)hfaceInfo->vertex0,
				(SVECTOR*)hfaceInfo->vertex1,
				&sphere->position);

			x = sphere->position.x - CSpad->triPoint.x;
			y = sphere->position.y - CSpad->triPoint.y;
			z = sphere->position.z - CSpad->triPoint.z;

			gte_ldsv_ext((short)x, (short)y, (short)z);
			gte_sqr0();
			gte_stsv_ext(x, y, z);

			d0sq = (x + y + z);
			if (d0sq < sphere->radiusSquared)
				goto LABEL_18;
			COLLIDE_NearestPointOnLine_S(
				&CSpad->planePoint,
				(SVECTOR*)hfaceInfo->vertex1,
				(SVECTOR*)hfaceInfo->vertex2,
				&sphere->position);

			x = sphere->position.x - CSpad->planePoint.x;
			y = sphere->position.y - CSpad->planePoint.y;
			z = sphere->position.z - CSpad->planePoint.z;

			gte_ldsv_ext((short)x, (short)y, (short)z);
			gte_sqr0();
			gte_stsv_ext(x, y, z);


			d1sq = (x + y + z);
			if (d1sq < d0sq)
			{
				CSpad->triPoint = CSpad->planePoint;
				d0sq = d1sq;
			}
			if (d0sq < sphere->radiusSquared)
				goto LABEL_18;
			COLLIDE_NearestPointOnLine_S(
				&CSpad->planePoint,
				(SVECTOR*)hfaceInfo->vertex2,
				(SVECTOR*)hfaceInfo->vertex0,
				&sphere->position);

			x = sphere->position.x - CSpad->planePoint.x;
			y = sphere->position.y - CSpad->planePoint.y;
			z = sphere->position.z - CSpad->planePoint.z;

			gte_ldsv_ext((short)x, (short)y, (short)z);
			gte_sqr0();
			gte_stsv_ext(x, y, z);

			d1sq = (x + y + z);
			if (d1sq < d0sq)
			{
				CSpad->triPoint = CSpad->planePoint;
				d0sq = d1sq;
			}
			if (d0sq < sphere->radiusSquared)
			{
			LABEL_18:
				_v1 = &CSpad->triPoint;

				_x0 = _v1->x;
				_y0 = _v1->y;
				_z0 = _v1->z;

				_x1 = sphere->position.x;
				_y1 = sphere->position.y;
				_z1 = sphere->position.z;

				dv.x = _x0 - _x1;
				dv.y = _y0 - _y1;
				dv.z = _z0 - _z1;

				a = (dv.x < 0) ? -(dv.x) : dv.x;
				c = (dv.y < 0) ? -(dv.y) : dv.y;
				b = (dv.z < 0) ? -(dv.z) : dv.z;

				MATH3D_Sort3VectorCoords(&a, &b, &c);

				intersect->x = CSpad->triPoint.x;
				intersect->y = CSpad->triPoint.y;
				intersect->z = CSpad->triPoint.z;

				v46 = 2 * (15 * c) + 12 * b + 9 * a;
				if (v46)
				{
					dv.x *= sphere->radius;
					dv.y *= sphere->radius;
					dv.z *= sphere->radius;

					dv.x = (32 * dv.x / v46);
					dv.y = (32 * dv.y / v46);
					dv.z = (32 * dv.z / v46);
				}
				else
				{
					dv.x = ((CSpad->normal.x * sphere->radius) >> 12);
					dv.y = ((CSpad->normal.y * sphere->radius) >> 12);
					dv.z = ((CSpad->normal.z * sphere->radius) >> 12);
				}

				sphere->position.x = (short)(dv.x + CSpad->triPoint.x);
				sphere->position.y = (short)(dv.y + CSpad->triPoint.y);
				sphere->position.z = (short)(dv.z + CSpad->triPoint.z);

				v10 = 1;
			}
		}
	}
	return v10;
}

typedef void (*collideFuncPtr)(struct _Instance*, struct GameTracker*);

long COLLIDE_SAndT(struct SCollideInfo* scollideInfo, struct Level* level)//Matching - 94.71%
{
	struct _Terrain* terrain; // $s5
	void** stack; // $s1
	int v4; // $t6
	int radiusSquared; // $t7
	SVECTOR* oldPos; // $v0
	int y; // $v0
	int z; // $v1
	int result; // $v0
	short* nrmlArray; // $a0
	struct BSPTree* bspTree; // $s4
	short flags; // $v1
	struct _BoundingBox* box;
	short _x0; // $v1 MAPDST
	short _y0; // $v1 MAPDST
	short _z0; // $a0 MAPDST
	struct _BSPNode* bspNode; // $a1
	struct _SVector* point;
	struct _TFace* tface; // $s3
	struct _TVertex* v41; // $a3
	char segment; // $a2
	struct _SVector* nrml;
	int plane_front_error; // $v0
	int plane_back_error; // $a3
	void* front; // $v0
	void* v55; // $v0
	void* back; // $v0
	short _x1; // $a2 MAPDST
	short _y1; // $a2 MAPDST
	short _z1; // $a3 MAPDST
	struct _Sphere* sphere; // $v0
	long a; // [sp+18h] [-10h] BYREF
	long b; // [sp+1Ch] [-Ch] BYREF
	long c; // [sp+20h] [-8h] BYREF
	struct SandTScratch* CSpad;
	struct _SVector* _v;
	struct _SVector* _v0;
	struct _Position* _v1;
	struct _Position* _v2;
	struct _Position* _v3;
	collideFuncPtr collideFunc;
	int t1;
	short normal;
	short* v36; // $v1
	short v37; // $v0
	int curTree; // [sp+24h] [-4h]
	short* sPtr;
	long l;

	CSpad = (struct SandTScratch*)getScratchAddr(114);
	terrain = level->terrain;
	stack = (void**)getScratchAddr(167);

	if (gameTrackerX.gameData.asmData.MorphTime != 1000)
	{
		CSpad->in_spectral = 2;
	}
	else
	{
		if (gameTrackerX.gameData.asmData.MorphType == 1)
		{
			CSpad->in_spectral = 1;
		}
		else
		{
			CSpad->in_spectral = 0;
		}
	}

	_v0 = &CSpad->oldPos;

	CSpad->normalList = (struct _HNormal*)terrain->normalList;
	CSpad->vertexList = terrain->vertexList;
	CSpad->collideFunc = scollideInfo->collideFunc;
	CSpad->instance = scollideInfo->instance;
	CSpad->prim = scollideInfo->prim;
	CSpad->sphere = *scollideInfo->sphere;

	CSpad->result = 0;
	CSpad->collide_ignoreAttr = collide_ignoreAttr;
	CSpad->collide_acceptAttr = collide_acceptAttr;

	oldPos = scollideInfo->oldPos;

	_x1 = oldPos->vx;
	_y1 = oldPos->vy;
	_z1 = oldPos->vz;

	_v0->x = _x1;
	_v0->y = _y1;
	_v0->z = _z1;

	CSpad->spherePos.x = CSpad->sphere.position.x;
	CSpad->spherePos.y = CSpad->sphere.position.y;
	CSpad->spherePos.z = CSpad->sphere.position.z;

	CSpad->midPoint.x = CSpad->spherePos.x - CSpad->oldPos.x;
	CSpad->midPoint.y = CSpad->spherePos.y - CSpad->oldPos.y;
	CSpad->midPoint.z = CSpad->spherePos.z - CSpad->oldPos.z;


	a = ABS(CSpad->midPoint.x);
	b = ABS(CSpad->midPoint.y);
	c = ABS(CSpad->midPoint.z);

	MATH3D_Sort3VectorCoords(&a, &b, &c);

	CSpad->midRadius = 30 * c + 12 * b + 9 * a;

	if (CSpad->midRadius == 0)
	{
		return 0;
	}
	CSpad->midPoint.x = (CSpad->spherePos.x + CSpad->oldPos.x) >> 1;
	CSpad->midPoint.y = (CSpad->spherePos.y + CSpad->oldPos.y) >> 1;
	CSpad->midPoint.z = (CSpad->spherePos.z + CSpad->oldPos.z) >> 1;

	CSpad->midRadius = CSpad->midRadius / 2 + (unsigned short)CSpad->sphere.radius;

	if (CSpad->in_spectral == 2)
	{
		CSpad->midRadius += 2048;
	}

	for (curTree = 0; curTree < terrain->numBSPTrees; curTree++)
	{
		_v0 = &CSpad->midPoint;
		_v1 = &CSpad->sphere.position;
		_v = &CSpad->oldPos;

		bspTree = &terrain->BSPTreeArray[curTree];

		if (bspTree->ID >= 0)
		{
			if ((bspTree->flags & 0x4000) == 0 || gameTrackerX.raziel_collide_override)
			{
				if (!(bspTree->flags & 0x2000) || gameTrackerX.monster_collide_override)
				{
					if (!(bspTree->flags & 0x102) || (bspTree->flags & 0xE0) != 0 && (INSTANCE_Query(CSpad->instance, 0x1) & 0x2))
					{
						CSpad->collideInfo.bspID = bspTree->ID;

						_x0 = _v->x;
						_y0 = _v->y;
						_z0 = _v->z;

						_x0 -= bspTree->globalOffset.x;
						_y0 -= bspTree->globalOffset.y;
						_z0 -= bspTree->globalOffset.z;

						_v->x = _x0;
						_v->y = _y0;
						_v->z = _z0;

						_x0 = _v0->x;
						_y0 = _v0->y;
						_z0 = _v0->z;

						_x0 -= bspTree->globalOffset.x;
						_y0 -= bspTree->globalOffset.y;
						_z0 -= bspTree->globalOffset.z;

						_v0->x = _x0;
						_v0->y = _y0;
						_v0->z = _z0;

						_x0 = _v1->x;
						_y0 = _v1->y;
						_z0 = _v1->z;

						_x0 -= bspTree->globalOffset.x;
						_y0 -= bspTree->globalOffset.y;
						_z0 -= bspTree->globalOffset.z;

						_v1->x = _x0;
						_v1->y = _y0;
						_v1->z = _z0;


						CSpad->posMatrix.m[0][0] = CSpad->sphere.position.x;
						CSpad->posMatrix.m[0][1] = CSpad->sphere.position.y;
						CSpad->posMatrix.m[0][2] = CSpad->sphere.position.z;
						CSpad->posMatrix.m[1][0] = CSpad->oldPos.x;
						CSpad->posMatrix.m[1][1] = CSpad->oldPos.y;
						CSpad->posMatrix.m[1][2] = CSpad->oldPos.z;

						*stack = stack;

						SetRotMatrix(&CSpad->posMatrix);

						*++stack = bspTree->bspRoot;

						while (*stack != stack)
						{
							bspNode = (struct _BSPNode*)*stack;
							if ((*((short*)*stack-- + 7) & 2))
							{
								box = (struct _BoundingBox*)&bspNode->d;
								point = (struct _SVector*)&CSpad->midPoint;

								if ((point->x - (short)CSpad->midRadius >= box->maxX
									|| point->x + (short)CSpad->midRadius <= box->minX
									|| point->y - (short)CSpad->midRadius >= box->maxY
									|| point->y + (short)CSpad->midRadius <= box->minY
									|| point->z - (short)CSpad->midRadius >= box->maxZ
									|| point->z + (short)CSpad->midRadius > box->minZ) &&
									(box->maxX >= point->x - (short)CSpad->midRadius
									|| box->minX <= point->x + (short)CSpad->midRadius
									|| box->maxY >= point->y - (short)CSpad->midRadius
									|| box->minY <= point->y + (short)CSpad->midRadius
									|| box->maxZ >= point->z - (short)CSpad->midRadius
									|| box->minZ < point->z + (short)CSpad->midRadius))
								{

									*(unsigned int*)&CSpad->posMatrix.m[0][0] = *(unsigned int*)&CSpad->sphere.position.x;
									CSpad->posMatrix.m[0][2] = CSpad->sphere.position.z;
									gte_ldsvrtrow0(&CSpad->posMatrix);
									CSpad->i = bspNode->c;
									tface = *(struct _TFace**)&bspNode->a;
									while (CSpad->i)
									{
										if (((tface->attr & CSpad->collide_ignoreAttr) == 0 || (tface->attr & CSpad->collide_acceptAttr) != 0)
											&& ((unsigned short)tface->textoff == 0xFFFF
												|| (*(short*)((char*)&terrain->StartTextureList->attr
													+ (unsigned short)tface->textoff) & 0x2000) == 0)
											&& !(tface->attr & 8))
										{
											if (CSpad->in_spectral == 2
												&& (unsigned short)tface->normal != *(short*)(1
													* ((-1431655765
														* ((char*)tface
															- (char*)terrain->faceList)) >> 2)
													+ terrain->morphNormalIdx))
											{
												COLLIDE_MakeNormal(terrain, (struct _TFace*)tface, &CSpad->normal);
											}
											else
											{
												normal = (short)tface->normal;
												nrmlArray = (short*)CSpad->normalList;
												nrml = &CSpad->normal;

												if (normal >= 0)
												{
													sPtr = &nrmlArray[normal * 3];
													nrml->x = *sPtr++ & 0x1FFF;
													nrml->y = *sPtr++;
													nrml->z = *sPtr;
												}
												else
												{
													sPtr = &nrmlArray[-normal * 3];
													nrml->x = -(*sPtr++ & 0x1FFF);
													nrml->y = -*sPtr++;
													nrml->z = -*sPtr;
												}
											}
											_v3 = (struct _Position*)&CSpad->vertexList[tface->face.v0];
											gte_ldv2_ext(_v3);
											gte_ldv0(&CSpad->normal);
											gte_rtv0();
											gte_stlvnl(&CSpad->dpv);
											if (CSpad->dpv.x <= CSpad->dpv.y
												&& CSpad->dpv.x - CSpad->dpv.z < CSpad->sphere.radius
												&& CSpad->dpv.y - CSpad->dpv.z >= -CSpad->sphere.radius)
											{
												CSpad->hfaceInfo.hface = (struct _HFace*)tface;

												CSpad->hfaceInfo.vertex0 = (struct _HVertex*)_v3;
												CSpad->hfaceInfo.vertex1 = (struct _HVertex*)&CSpad->vertexList[(short)tface->face.v1];
												CSpad->hfaceInfo.vertex2 = (struct _HVertex*)&CSpad->vertexList[(short)tface->face.v2];

												CSpad->hfaceInfo.normal = *(struct _SVector*)&CSpad->normal;

												if (COLLIDE_SphereAndHFace(
													&CSpad->sphere,
													(struct _Position*)&CSpad->oldPos,
													&CSpad->hfaceInfo,
													(struct _SVector*)&CSpad->collideInfo.point1,
													&CSpad->edge))
												{
													CSpad->collideInfo.flags = 0;
													if (CSpad->edge)
													{
														CSpad->collideInfo.flags = 4;
													}
													else
													{
														CSpad->collideInfo.flags = 8;
													}


													CSpad->collideInfo.type0 = 1;
													CSpad->collideInfo.type1 = 3;
													CSpad->collideInfo.inst1 = bspTree;
													CSpad->collideInfo.level = level;
													CSpad->collideInfo.inst0 = CSpad->instance;
													segment = (char)scollideInfo->segment;
													CSpad->collideInfo.prim0 = CSpad->prim;
													CSpad->collideInfo.offset.x = CSpad->sphere.position.x - CSpad->posMatrix.m[0][0];
													CSpad->collideInfo.offset.y = CSpad->sphere.position.y - CSpad->posMatrix.m[0][1];
													CSpad->collideInfo.offset.z = CSpad->sphere.position.z - CSpad->posMatrix.m[0][2];
													CSpad->collideInfo.prim1 = tface;
													CSpad->collideInfo.segment = segment;

													if (CSpad->instance)
													{
														CSpad->instance->collideInfo = &CSpad->collideInfo;

														if (CSpad->collideFunc)
														{
															collideFunc = CSpad->collideFunc;
															collideFunc(CSpad->instance, &gameTrackerX);
														}
													}

													CSpad->result = 1;
													*(unsigned int*)&CSpad->posMatrix.m[0][0] = *(unsigned int*)&CSpad->sphere.position.x;
													CSpad->posMatrix.m[0][2] = CSpad->sphere.position.z;

												}
												SetRotMatrix(&CSpad->posMatrix);
											}
										}

										CSpad->i--;
										tface++;
									}
									*(unsigned int*)&CSpad->posMatrix.m[0][0] = *(unsigned int*)&CSpad->sphere.position.x;
									CSpad->posMatrix.m[0][2] = CSpad->sphere.position.z;
									gte_ldsvrtrow0(&CSpad->posMatrix);
								}
							}
							else
							{
								gte_ldv0(&bspNode->a);
								gte_rtv0();
								gte_stlvnl(&CSpad->dpv);
								CSpad->dpv.x -= bspNode->d;
								CSpad->dpv.y -= bspNode->d;

								if (CSpad->in_spectral)
								{
									plane_front_error = bspNode->front_spectral_error;
									plane_back_error = bspNode->back_spectral_error;
								}

								else
								{
									plane_front_error = bspNode->front_material_error;
									plane_back_error = bspNode->back_material_error;
								}

								if (CSpad->sphere.radius + plane_front_error <= CSpad->dpv.y)
								{
									if (CSpad->sphere.radius + plane_front_error < CSpad->dpv.x)
									{
										if (bspNode->front)
											*++stack = bspNode->front;
									}
									else
									{
										if (bspNode->back)
											*++stack = bspNode->back;
										if (bspNode->front)
											*++stack = bspNode->front;
									}
								}
								else
								{
									if (plane_back_error - CSpad->sphere.radius >= CSpad->dpv.y)
									{
										if (plane_back_error - CSpad->sphere.radius > CSpad->dpv.x)
										{
											if (bspNode->back)
												*++stack = bspNode->back;

											if (bspNode->front)
												*++stack = bspNode->front;
										}
										else
										{
											if (bspNode->front)
												*++stack = bspNode->front;

											if (bspNode->back)
												*++stack = bspNode->back;
										}
									}
									else
									{
										if (CSpad->dpv.x >= CSpad->dpv.y)
										{
											if (bspNode->front)
												*++stack = bspNode->front;
											if (bspNode->back)
												*++stack = bspNode->back;
										}
										else
										{
											if (bspNode->back)
												*++stack = bspNode->back;
											if (bspNode->front)
												*++stack = bspNode->front;
										}
									}
								}
							}
						}

						_v2 = &bspTree->globalOffset;

						_x0 = _v->x;
						_y0 = _v->y;
						_z0 = _v->z;

						_x1 = _v2->x;
						_y1 = _v2->y;
						_z1 = _v2->z;

						_x0 += _x1;
						_y0 += _y1;
						_z0 += _z1;

						_v->x = _x0;
						_v->y = _y0;
						_v->z = _z0;

						_x0 = _v0->x;
						_y0 = _v0->y;
						_z0 = _v0->z;

						_x1 = _v2->x;
						_y1 = _v2->y;
						_z1 = _v2->z;

						_x0 += _x1;
						_y0 += _y1;
						_z0 += _z1;

						_v0->x = _x0;
						_v0->y = _y0;
						_v0->z = _z0;

						_x0 = _v1->x;
						_y0 = _v1->y;
						_z0 = _v1->z;

						_x1 = _v2->x;
						_y1 = _v2->y;
						_z1 = _v2->z;

						_x0 += _x1;
						_y0 += _y1;
						_z0 += _z1;

						_v1->x = _x0;
						_v1->y = _y0;
						_v1->z = _z0;
					}
				}
			}
		}
	}

	_v0 = (struct _SVector*)&CSpad->sphere.position;
	_v1 = &scollideInfo->sphere->position;

	_x0 = _v0->x;
	_y0 = _v0->y;
	_z0 = _v0->z;


	_v1->x = _x0;
	_v1->y = _y0;
	_v1->z = _z0;

	return CSpad->result;
}

long COLLIDE_SphereAndTerrain(struct SCollideInfo* scollideInfo, struct Level* level)//Matching - 97.67%
{
	int result;
	int d;
	int in_warpRoom;
	struct _StreamUnit* stream;

	result = COLLIDE_SAndT(scollideInfo, level);

	in_warpRoom = 0;
	
	stream = STREAM_GetStreamUnitWithID(level->streamUnitID);

	if (stream != NULL)
	{
		in_warpRoom = stream->flags & 0x1;
	}

	for (d = 0; d < 16; d++)
	{
		if ((StreamTracker.StreamList[d].used == 2) && (StreamTracker.StreamList[d].level != level) && ((in_warpRoom == 0) || !(StreamTracker.StreamList[d].flags & 0x1)) && (MEMPACK_MemoryValidFunc((char*)StreamTracker.StreamList[d].level) != 0))
		{
			result = COLLIDE_SAndT(scollideInfo, StreamTracker.StreamList[d].level);
		}
	}

	return result;
}

void COLLIDE_InstanceTerrain(struct _Instance* instance, struct Level* level)//Matching - 99.90%
{

	struct _Vector* newPosVec; // stack offset -60
	struct _Vector* oldPosVec; // stack offset -56
	struct _SVector* oldPos; // $fp
	struct SCollideInfo scollideInfoX; // stack offset -96
	struct SCollideInfo* scollideInfo; // $s3
	struct _Sphere* wSphere; // $s5
	MATRIX* swTransform; // $s0
	MATRIX* oldSWTransform; // $s1
	struct _HSphere* hsphere; // $s2
	long flags; // stack offset -64
	int i; // $s7
	struct _HModel* hmodel; // $v0
	struct _HPrim* hprim; // $s6
	int currentModel;
	unsigned char withFlags;
	void (*collideFunc)(struct _Instance* instance, struct GameTracker* gameTracker);

	newPosVec = (struct _Vector*)getScratchAddr(82);
	oldPosVec = (struct _Vector*)getScratchAddr(86);
	oldPos = (struct _SVector*)getScratchAddr(90);
	wSphere = (struct _Sphere*)getScratchAddr(110);

	scollideInfo = &scollideInfoX;


	if (instance->matrix != NULL && instance->oldMatrix != NULL)
	{
		if ((instance->object->oflags2 & 0x80000))
		{
			gameTrackerX.monster_collide_override = 1;
		}

		collideFunc = instance->collideFunc;
		currentModel = instance->currentModel;

		if (collideFunc != NULL)
		{
			hmodel = &instance->hModelList[currentModel];
			i = hmodel->numHPrims;
			hprim = hmodel->hPrimList;

			while (i)
			{
				if ((hprim->hpFlags & 0x1) && (withFlags = hprim->withFlags & 0x2) && hprim->type == 1)
				{
					swTransform = &instance->matrix[hprim->segment];

					oldSWTransform = &instance->oldMatrix[hprim->segment];

					hsphere = hprim->data.hsphere;

					SetRotMatrix(swTransform);

					SetTransMatrix(swTransform);

					RotTrans((SVECTOR*)&hsphere->position, (VECTOR*)newPosVec, &flags);

					SetRotMatrix(oldSWTransform);

					SetTransMatrix(oldSWTransform);

					RotTrans((SVECTOR*)&hsphere->position, (VECTOR*)oldPosVec, &flags);

					wSphere->position.x = (short)newPosVec->x;
					wSphere->position.y = (short)newPosVec->y;
					wSphere->position.z = (short)newPosVec->z;
					wSphere->radius = hsphere->radius;
					wSphere->radiusSquared = hsphere->radiusSquared;

					oldPos->x = (short)oldPosVec->x;
					oldPos->y = (short)oldPosVec->y;
					oldPos->z = (short)oldPosVec->z;


					scollideInfo->sphere = wSphere;

					scollideInfo->oldPos = (SVECTOR*)oldPos;

					scollideInfo->collideFunc = collideFunc;

					scollideInfo->instance = instance;

					scollideInfo->segment = hprim->segment;

					scollideInfo->id = hsphere->id;

					scollideInfo->prim = (void*)hsphere;

					COLLIDE_SphereAndTerrain(scollideInfo, level);
				}

				i--;
				hprim++;
			}
		}

		gameTrackerX.monster_collide_override = 0;
	}
}

long COLLIDE_LineWithSignals(struct _SVector* startPoint, struct _SVector* endPoint, struct _MultiSignal** signalList, long maxSignals, struct Level* level)
{
	struct IandTScratch* CSpad;
	struct _Terrain* terrain; // $s4
	void** stack; // $s2
	int v8; // $t3
	int numSignalsCollidedWith; // $fp
	int result; // $v0
	struct _Vector* p_dpv; // $t1
	int v12; // $s5
	struct _BSPNode* bspNode; // $a1
	int front_high; // $v1
	int d_low; // $v1
	int back_low; // $v1
	int d_high; // $v1
	int back_high; // $v1
	int front_low; // $v1
	struct _TFace* tface; // $s3
	unsigned short* p_textoff; // $s1
	int v27; // $v1
	short* v28; // $v1
	short v29; // $v0
	int v30; // $v0
	short* v31; // $v1
	short v32; // $v0
	struct _SVector* v33; // $a0
	int v37; // $v1
	int y; // $v0
	int v39; // $a1
	int v40; // $v0
	int v41; // $v0
	int front_spectral_error; // $v1
	int back_spectral_error; // $a0
	void* back; // $v0
	void* front; // $v0
	short _x1; // $a0 MAPDST
	short _y1; // $a0 MAPDST
	short _z1; // $a1 MAPDST
	int v52; // [sp+18h] [-10h]
	struct _Vector* v53; // [sp+20h] [-8h]
	struct _Vector* v54; // [sp+20h] [-8h]

	CSpad = (struct IandTScratch*)getScratchAddr(16);

	terrain = level->terrain;

	stack = (void**)getScratchAddr(41);

	CSpad->normalList = (short*)terrain->normalList;
	CSpad->vertexList = terrain->vertexList;

	CSpad->oldPos = *startPoint;
	CSpad->newPos = *endPoint;

	numSignalsCollidedWith = 0;

	if (gameTrackerX.gameData.asmData.MorphTime != 1000)
	{
		CSpad->in_spectral = 2;
	}
	else
	{
		if (gameTrackerX.gameData.asmData.MorphType == 1)
		{
			CSpad->in_spectral = 1;
		}
		else
		{
			CSpad->in_spectral = 0;
		}
	}

	CSpad->line.x = CSpad->oldPos.x - CSpad->newPos.x;
	CSpad->line.y = CSpad->oldPos.y - CSpad->newPos.y;
	CSpad->line.z = CSpad->oldPos.z - CSpad->newPos.z;

	result = 0;

	if (CSpad->line.x || CSpad->line.y || CSpad->line.z)
	{
		v52 = 0;
		if (terrain->numBSPTrees > 0)
		{
			v12 = 0;
			do
			{
				if (terrain->BSPTreeArray[v12].ID == -1)
				{
					*stack = stack;

					_x1 = terrain->BSPTreeArray[v12].globalOffset.x;
					_y1 = terrain->BSPTreeArray[v12].globalOffset.y;
					_z1 = terrain->BSPTreeArray[v12].globalOffset.z;

					CSpad->newPos.x -= _x1;
					CSpad->newPos.y -= _y1;
					CSpad->newPos.z -= _z1;

					_x1 = terrain->BSPTreeArray[v12].globalOffset.x;
					_y1 = terrain->BSPTreeArray[v12].globalOffset.y;
					_z1 = terrain->BSPTreeArray[v12].globalOffset.z;

					CSpad->oldPos.x -= _x1;
					CSpad->oldPos.y -= _y1;
					CSpad->oldPos.z -= _z1;

					*++stack = *(void**)(&terrain->BSPTreeArray[v12]);

					CSpad->posMatrix.m[0][0] = CSpad->newPos.x;
					CSpad->posMatrix.m[0][1] = CSpad->newPos.y;
					CSpad->posMatrix.m[0][2] = CSpad->newPos.z;
					CSpad->posMatrix.m[1][0] = CSpad->oldPos.x;
					CSpad->posMatrix.m[1][1] = CSpad->oldPos.y;
					CSpad->posMatrix.m[1][2] = CSpad->oldPos.z;

					SetRotMatrix(&CSpad->posMatrix);
					while (*stack != stack)
					{
						bspNode = (struct _BSPNode*)*stack;
						if (*((short*)*stack-- + 7) & 2)
						{
							front_high = ((short*)&bspNode->front)[1];
							if (front_high >= CSpad->newPos.x || front_high >= CSpad->oldPos.x)
							{
								d_low = ((short*)&bspNode->d)[0];
								if (CSpad->newPos.x >= d_low || CSpad->oldPos.x >= d_low)
								{
									back_low = ((short*)&bspNode->back)[0];
									if (back_low >= CSpad->newPos.y || back_low >= CSpad->oldPos.y)
									{
										d_high = ((short*)&bspNode->d)[1];
										if (CSpad->newPos.y >= d_high || CSpad->oldPos.y >= d_high)
										{
											back_high = ((short*)&bspNode->back)[1];
											if (back_high >= CSpad->newPos.z || back_high >= CSpad->oldPos.z)
											{
												front_low = ((short*)&bspNode->front)[0];
												if (CSpad->newPos.z >= front_low || CSpad->oldPos.z >= front_low)
												{
													CSpad->i = bspNode->c;
													tface = *(struct _TFace**)&bspNode->a;
													if (CSpad->i)
													{
														do
														{
															if ((((unsigned short*)&tface->attr)[0] & 0xC0) != 0 && tface->textoff != 0xFFFF)
															{
																v27 = (short)tface->normal;
																if (v27 >= 0)
																{
																	v28 = &CSpad->normalList[3 * v27];
																	v29 = *v28++;
																	CSpad->normal.x = v29 & 0x1FFF;
																	CSpad->normal.y = *v28;
																	v30 = v28[1];
																}
																else
																{
																	v31 = &CSpad->normalList[3 * -v27];
																	v32 = *v31++;
																	CSpad->normal.x = -(v32 & 0x1FFF);
																	CSpad->normal.y = -*v31;
																	v30 = -(unsigned short)v31[1];
																}
																CSpad->normal.z = v30;


																v33 = (struct _SVector*)&CSpad->vertexList[tface->face.v0];

																gte_ldv2_ext(v33);
																gte_ldv0(&CSpad->normal);
																gte_rtv0();
																gte_stlvnl(&CSpad->dpv);

																CSpad->dpv.x -= CSpad->dpv.z;
																CSpad->dpv.y -= CSpad->dpv.z;
																if (CSpad->dpv.x < 0 && CSpad->dpv.y - CSpad->dpv.z >= 0)
																{
																	v39 = (CSpad->dpv.x - CSpad->dpv.y != 0) ? (CSpad->dpv.y << 12) / (CSpad->dpv.x - CSpad->dpv.y) : 0;

																	CSpad->planePoint.x = CSpad->oldPos.x + ((CSpad->line.x * v39) >> 12);
																	CSpad->planePoint.y = CSpad->oldPos.y + ((CSpad->line.y * v39) >> 12);
																	CSpad->planePoint.z = CSpad->oldPos.z + ((CSpad->line.z * v39) >> 12);
																	v40 = COLLIDE_PointInTriangle(
																		(struct _SVector*)(v33),
																		(struct _SVector*)(&CSpad->vertexList[tface->face.v1]),
																		(struct _SVector*)(&CSpad->vertexList[tface->face.v2]),
																		&CSpad->planePoint,
																		&CSpad->normal);
																	if (v40)
																	{
																		v41 = numSignalsCollidedWith;
																		if (numSignalsCollidedWith < maxSignals)
																		{
																			++numSignalsCollidedWith;
																			signalList[v41] = (struct _MultiSignal*)((char*)terrain->signals + (unsigned short)tface->textoff);
																		}
																	}
																}
															}
															++tface;
														} while (CSpad->i-- != 1);
													}
												}
											}
										}
									}
								}
							}
						}
						else
						{

							gte_ldv0(&bspNode->a);
							gte_rtv0();
							gte_stlvnl(&CSpad->dpv);
							CSpad->dpv.x -= bspNode->d;
							CSpad->dpv.y -= bspNode->d;

							if (CSpad->in_spectral)
							{
								front_spectral_error = bspNode->front_spectral_error;
								back_spectral_error = bspNode->back_spectral_error;
							}
							else
							{
								front_spectral_error = bspNode->front_material_error;
								back_spectral_error = bspNode->back_material_error;
							}
							if (CSpad->dpv.x < front_spectral_error || CSpad->dpv.y < front_spectral_error)
							{
								if (back_spectral_error < CSpad->dpv.x || back_spectral_error < CSpad->dpv.y)
								{
									front = (void*)bspNode->front;
									if (front)
										*++stack = front;
								}
								back = (void*)bspNode->back;
							}
							else
							{
								back = (void*)bspNode->front;
							}
							if (back)
								*++stack = back;
						}
					}
					_x1 = terrain->BSPTreeArray[v12].globalOffset.x;
					_y1 = terrain->BSPTreeArray[v12].globalOffset.y;
					_z1 = terrain->BSPTreeArray[v12].globalOffset.z;

					CSpad->newPos.x += _x1;
					CSpad->newPos.y += _y1;
					CSpad->newPos.z += _z1;

					_x1 = terrain->BSPTreeArray[v12].globalOffset.x;
					_y1 = terrain->BSPTreeArray[v12].globalOffset.y;
					_z1 = terrain->BSPTreeArray[v12].globalOffset.z;

					CSpad->oldPos.x += _x1;
					CSpad->oldPos.y += _y1;
					CSpad->oldPos.z += _z1;
				}
				v12++;
				++v52;
			} while (v52 < terrain->numBSPTrees);
		}
		return numSignalsCollidedWith;
	}
	return result;
}

void COLLIDE_InstanceTerrainSignal(struct _Instance* instance, struct Level* level)//Matching - 98.95%
{
	struct _Model* model;
	int numSignals;
	int d;
	struct _MultiSignal* msignal;
	struct _SVector startPoint;
	struct _SVector endPoint;
	struct _MultiSignal* signalListArray[8];

	model = instance->object->modelList[instance->currentModel];

	if (instance->matrix != NULL && instance->oldMatrix != NULL)
	{
		if ((instance->object->oflags2 & 0x80000) && INSTANCE_Query(instance, 0x1) != 130)
		{
			startPoint = *(struct _SVector*)&instance->oldPos;
			startPoint.z += 100;
			endPoint = *(struct _SVector*)&instance->position;
			endPoint.z += 100;
		}
		else
		{
			if (model != NULL && model->numSegments >= 2)
			{
				startPoint.x = (short)instance->oldMatrix[1].t[0];
				startPoint.y = (short)instance->oldMatrix[1].t[1];
				startPoint.z = (short)instance->oldMatrix[1].t[2];

				endPoint.x = (short)instance->matrix[1].t[0];
				endPoint.y = (short)instance->matrix[1].t[1];
				endPoint.z = (short)instance->matrix[1].t[2];
			}
			else
			{
				startPoint = *(struct _SVector*)&instance->oldPos;
				endPoint = *(struct _SVector*)&instance->position;
			}
		}
		
		numSignals = COLLIDE_LineWithSignals(&startPoint, &endPoint, signalListArray, 8, level);

		for (d = 0; d < numSignals; d++)
		{
			msignal = signalListArray[d];
			if (instance == gameTrackerX.playerInstance)
			{
				msignal->flags |= 0x1;
			}

			SIGNAL_HandleSignal(instance, msignal->signalList, 0);
			EVENT_AddSignalToReset(msignal);
		}
	}
}

struct _StreamUnit* COLLIDE_CameraWithStreamSignals(struct Camera* camera)//Matching - 88.91%
{
	struct _SVector startPoint;
	struct _SVector endPoint;
	struct _Model* model;
	long numSignals;
	long i;
	long numStreamSignals;
	struct _MultiSignal(*signalListArray[8]);
	struct _StreamUnit(*streamSignalUnits[8]);
	struct _StreamUnit* playerStreamUnit;
	struct Level* level;
	long playerStreamUnitID;
	struct _Instance* instance;
	struct _Instance* af_instance;
	struct _MultiSignal* msignal;
	long isWarpGateSignal;
	struct _StreamUnit* cameraStreamUnit;
	long cameraStreamID;
	int number;

	instance = camera->focusInstance;
	af_instance = gameTrackerX.playerInstance;

	playerStreamUnitID = instance->currentStreamUnitID;

	if (instance == af_instance)
	{
		if (gameTrackerX.SwitchToNewStreamUnit != 0)
		{
			playerStreamUnitID = gameTrackerX.moveRazielToStreamID;
		}
	}

	playerStreamUnit = STREAM_GetStreamUnitWithID(playerStreamUnitID);

	endPoint = *(struct _SVector*)&camera->core.position;

	if (instance->matrix != NULL)
	{
		model = instance->object->modelList[instance->currentModel];

		if (model != NULL)
		{
			if (model->numSegments >= 2)
			{
				startPoint.x = (short)instance->matrix[1].t[0];
				startPoint.y = (short)instance->matrix[1].t[1];
				startPoint.z = (short)instance->matrix[1].t[2];
			}
			else
			{
				startPoint = *(struct _SVector*)&instance->position;
			}
		}
		else
		{
			startPoint = *(struct _SVector*)&instance->position;
		}
	}
	else
	{
		startPoint = *(struct _SVector*)&instance->position;
	}

	level = playerStreamUnit->level;
	numStreamSignals = 0;

	if (level != NULL)
	{
		numSignals = COLLIDE_LineWithSignals(&startPoint, &endPoint, signalListArray, 8, level);

		for (i = 0; i < numSignals; i++)
		{
			if (SIGNAL_IsStreamSignal(signalListArray[i]->signalList, &isWarpGateSignal) != 0)
			{
				if (isWarpGateSignal)
				{
					if (WARPGATE_IsWarpgateActive())
					{
						if (gameTrackerX.SwitchToNewWarpIndex == -1)
						{
							number = CurrentWarpNumber;
						}
						else
						{
							number = gameTrackerX.SwitchToNewWarpIndex;
						}

						cameraStreamID = WarpRoomArray[number].streamUnit->StreamUnitID;
					}
					else
					{
						cameraStreamID = 0;
					}
				}
				else
				{
					cameraStreamID = signalListArray[i]->signalList->data.StreamLevel.streamID;
				}

				if (cameraStreamID != 0)
				{
					cameraStreamUnit = STREAM_GetStreamUnitWithID(cameraStreamID);
				}
				else
				{
					cameraStreamUnit = NULL;
				}

				if (cameraStreamUnit != NULL)
				{
					streamSignalUnits[numStreamSignals++] = cameraStreamUnit;
				}
			}
		}
	}

	if (numStreamSignals == 0)
	{
		return 0;
	}
	else if (numStreamSignals != 1)
	{
		if (numStreamSignals > 0)
		{
			for (i = 0; i < numStreamSignals; i++)
			{
				if (streamSignalUnits[i]->StreamUnitID != playerStreamUnitID)
				{
					return streamSignalUnits[i];
				}
			}
		}
	}
	else
	{
		return streamSignalUnits[0];
	}

	return NULL;
}

void COLLIDE_InstanceListWithSignals(struct _InstanceList* instanceList)//Matching - 99.66%
{
	struct _Instance* instance;
	struct Level* level;

	instance = (struct _Instance*)instanceList->first;

	while (instance != NULL)
	{
		if (!(instance->flags2 & 0x24000000))
		{
			level = STREAM_GetLevelWithID(instance->currentStreamUnitID);
			
			if (level != NULL)
			{
				COLLIDE_InstanceTerrainSignal(instance, level);
			}
		}

		instance = instance->next;
	}
}

void COLLIDE_InstanceListTerrain(struct _InstanceList* instanceList)//Matching - 99.77%
{
	long i;
	struct _Instance* instance;
	struct Level* level;

	for (i = 1; i < 32; i += 2)
	{
		instance = (struct _Instance*)instanceList->group[i].next;

		while (instance != NULL)
		{
			if (instance->hModelList != NULL && !(instance->flags2 & 0x24040000))
			{
				level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

				if (level != NULL)
				{
					COLLIDE_InstanceTerrain(instance, level);
				}
			}

			instance = (struct _Instance*)instance->node.next;
		}
	}
}

void COLLIDE_SegmentCollisionOn(struct _Instance* instance, int segment)//Matching - 96.77%
{
	int i;
	struct _HModel* hmodel;
	struct _HPrim* hprim;

	if (instance->hModelList)
	{
		hmodel = &instance->hModelList[instance->currentModel];

		for (i = hmodel->numHPrims, hprim = hmodel->hPrimList; i != 0; i--, ++hprim)
		{
			if (hprim->segment == segment)
			{
				hprim->hpFlags |= 0x1;
			}
		}
		instance->flags2 &= ~0x40000;
	}
}

void COLLIDE_SegmentCollisionOff(struct _Instance* instance, int segment)
{
	int i;
	int enabled;
	struct _HModel* hmodel;
	struct _HPrim* hprim;

	if (instance->hModelList != NULL)
	{
		hmodel = &instance->hModelList[instance->currentModel];

		hprim = hmodel->hPrimList;

		enabled = 0;

		if (hmodel->numHPrims != 0)
		{
			for (i = hmodel->numHPrims; i != 0; i--)
			{
				if (hprim[i].segment == segment)
				{
					hprim[i].hpFlags &= 0xFE;
				}

				if (enabled == 0 && (hprim[i].hpFlags & 0x1))
				{
					enabled = 1;
				}
			}
		}

		if (enabled == 0)
		{
			instance->flags2 |= 0x40000;
		}
	}
}



// autogenerated function stub: 
// long /*$ra*/ COLLIDE_FindCollisionFaceNormal(struct _CollideInfo *collideInfo /*$a0*/, struct _Normal *normal /*$s2*/)
long COLLIDE_FindCollisionFaceNormal(struct _CollideInfo *collideInfo, struct _Normal *normal)
{ // line 5015, offset 0x80024964
#if 0
	/* begin block 1 */
		// Start line: 5016
		// Start offset: 0x80024964
		// Variables:
			long valid_normal; // $a2

		/* begin block 1.1 */
			// Start line: 5025
			// Start offset: 0x80024998
			// Variables:
				struct _TFace *tface; // $a1
		/* end block 1.1 */
		// End offset: 0x800249E0
		// End Line: 5029

		/* begin block 1.2 */
			// Start line: 5036
			// Start offset: 0x80024A18

			/* begin block 1.2.1 */
				// Start line: 5040
				// Start offset: 0x80024A18
				// Variables:
					SVECTOR*lNormal; // stack offset -24
					struct _Instance *inst1; // $s1
			/* end block 1.2.1 */
			// End offset: 0x80024A18
			// End Line: 5041
		/* end block 1.2 */
		// End offset: 0x80024A18
		// End Line: 5041

		/* begin block 1.3 */
			// Start line: 5054
			// Start offset: 0x80024A6C
			// Variables:
				struct _Instance *inst1; // $v0
		/* end block 1.3 */
		// End offset: 0x80024A84
		// End Line: 5058
	/* end block 1 */
	// End offset: 0x80024A84
	// End Line: 5060

	/* begin block 2 */
		// Start line: 10030
	/* end block 2 */
	// End Line: 10031
#endif
				UNIMPLEMENTED();
	return 0;
}

short* COLLIDE_GetBSPTreeFlag(struct _CollideInfo* collideInfo)
{
	struct Level* level;
	struct _Terrain* terrain;
	struct BSPTree* bspTree;

	level = (struct Level*)collideInfo->level;

	terrain = level->terrain;

	bspTree = &terrain->BSPTreeArray[collideInfo->bspID];

	return &bspTree->flags;
}

void COLLIDE_SetBSPTreeFlag(struct _CollideInfo* collideInfo, short flag)  // Matching - 100%
{
	short* bspTreeFlags;

	bspTreeFlags = COLLIDE_GetBSPTreeFlag(collideInfo);

	bspTreeFlags[0] |= flag;
}

int COLLIDE_PointAndTfaceFunc(struct _Terrain* terrain, struct BSPTree* bsp, struct _SVector* orgNewPos, struct _SVector* orgOldPos, struct _TFace* tface, long ignoreAttr, long flags)//Matching - 85.47%
{
	short _x0;
	short _y0; // $v1 MAPDST
	short _z0; // $a1 OVERLAPPED MAPDST
	short _x1; // $a2
	short _y1; // $t0
	short _z1; // $a3
	int normal; // $v1
	short* nrmlArray; // $a1
	short* sPtr; // $v1 MAPDST
	short v18; // $v0
	int v19; // $v0
	short v21; // $v0
	struct _SVector* vertex0; // $s3
	struct _SVector* vertex1; // $s4
	int result; // [sp+18h] [-8h]
	struct PandTFScratch* CSpad; // $s0
	struct _SVector* nrml;
	struct _SVector* _v;
	struct _SVector* _v1;
	struct _Position* _v2;
	struct _Position* _v3;

	CSpad = (struct PandTFScratch*)getScratchAddr(16);
	result = 0;
	if (!tface || (bsp->flags & 2) != 0)
		return 0;
	if (!((1 << (tface->attr & 0x1F)) & ignoreAttr))
	{
		_v = (struct _SVector*)getScratchAddr(26);
		_v1 = (struct _SVector*)getScratchAddr(28);
		_v2 = &bsp->globalOffset;

		_x0 = orgNewPos->x;
		_y0 = orgNewPos->y;
		_z0 = orgNewPos->z;

		_x1 = _v2->x;
		_y1 = _v2->y;
		_z1 = _v2->z;

		_x0 -= _x1;
		_y0 -= _y1;
		_z0 -= _z1;

		_v->x = _x0;
		_v->y = _y0;
		_v->z = _z0;

		_x0 = orgOldPos->x;
		_y0 = orgOldPos->y;
		_z0 = orgOldPos->z;

		_x1 = _v2->x;
		_y1 = _v2->y;
		_z1 = _v2->z;

		_x0 -= _x1;
		_y0 -= _y1;
		_z0 -= _z1;

		_v1->x = _x0;
		_v1->y = _y0;
		_v1->z = _z0;

		_x0 = CSpad->newPos.x;
		_y0 = CSpad->newPos.y;
		_z0 = CSpad->newPos.z;

		_x1 = CSpad->oldPos.x;
		_y1 = CSpad->oldPos.y;
		_z1 = CSpad->oldPos.z;

		CSpad->posMatrix.m[0][0] = _x0;
		CSpad->posMatrix.m[0][1] = _y0;
		CSpad->posMatrix.m[0][2] = _z0;
		CSpad->posMatrix.m[1][0] = _x1;
		CSpad->posMatrix.m[1][1] = _y1;
		CSpad->posMatrix.m[1][2] = _z1;

		SetRotMatrix(&CSpad->posMatrix);

		normal = (short)tface->normal;
		nrmlArray = (short*)terrain->normalList;
		nrml = &CSpad->normal;
		if (normal >= 0)
		{
			sPtr = &nrmlArray[3 * normal];
			nrml->x = *sPtr++ & 0x1FFF;
			nrml->y = *sPtr++;
			nrml->z = *sPtr;
		}
		else
		{
			sPtr = &nrmlArray[3 * -normal];
			nrml->x = -(*sPtr++ & 0x1FFF);
			nrml->y = -*sPtr++;
			nrml->z = -*sPtr;
		}

		vertex0 = (struct _SVector*)&terrain->vertexList[tface->face.v0];
		vertex1 = (struct _SVector*)&terrain->vertexList[tface->face.v1];

		gte_ldv2_ext(vertex0);
		gte_ldv0(&CSpad->normal);
		gte_rtv0();
		gte_stlvnl(&CSpad->dpv);

		CSpad->dpv.x -= CSpad->dpv.z;
		CSpad->dpv.y -= CSpad->dpv.z;

		if ((CSpad->dpv.x < 0 && CSpad->dpv.y >= 0) || ((flags & 1) != 0 && CSpad->dpv.x > 0 && CSpad->dpv.y <= 0))
		{
			if (COLLIDE_IntersectLineAndPlane_S(
				&CSpad->planePoint,
				(struct _Position*)&CSpad->oldPos,
				(struct _Position*)&CSpad->newPos,
				&CSpad->normal,
				CSpad->dpv.z))
			{
				if (COLLIDE_PointInTriangle(
					vertex0,
					vertex1,
					(struct _SVector*)&terrain->vertexList[tface->face.v2],
					&CSpad->planePoint,
					&CSpad->normal))
				{
					result = 1;
					_v2 = (struct _Position*)&CSpad->planePoint;

					_v3 = &bsp->globalOffset;
					_x0 = _v2->x;
					_y0 = _v2->y;
					_z0 = _v2->z;

					_x1 = _v3->x;
					_y1 = _v3->y;
					_z1 = _v3->z;

					_x0 += _x1;
					_y0 += _y1;
					_z0 += _z1;

					orgNewPos->x = _x0;
					orgNewPos->y = _y0;
					orgNewPos->z = _z0;
				}
			}
		}
	}
	return result;
}