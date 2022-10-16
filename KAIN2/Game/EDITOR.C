#include "EDITOR.H"
#include "CORE.H"

#include "Game/STREAM.H"
#include "Game/GAMELOOP.H"
#include "Game/CAMERA.H"
#include "BSP.H"

#if defined(EDITOR)

RECT16 screen_clip;

struct DebugDrawIntro
{
	const char* groupName;
	CVECTOR groupColour;
};

struct DebugDrawIntro introGroups[3] = {
	{ "campath", 255, 0, 0, 0 },
	{ "soul", 0, 255, 0, 0 },
	{ "marker", 0, 0, 255, 0 },
};

struct _SVector g_relocationOffset;

SVECTOR cube_verts[] = {
	{ -100, -100, -100, 0 },
	{  100, -100, -100, 0 },
	{ -100,  100, -100, 0 },
	{  100,  100, -100, 0 },
	{  100, -100,  100, 0 },
	{ -100, -100,  100, 0 },
	{  100,  100,  100, 0 },
	{ -100,  100,  100, 0 }
};

SVECTOR cube_norms[] = {
	{ 0, 0, -ONE, 0 },
	{ 0, 0, ONE, 0 },
	{ 0, -ONE, 0, 0 },
	{ 0, ONE, 0, 0 },
	{ -ONE, 0, 0, 0 },
	{ ONE, 0, 0, 0 }
};

typedef struct {
	short v0, v1, v2, v3;
} INDEX;

/* Cube vertex indices */
INDEX cube_indices[] = {
	{ 0, 1, 2, 3 },
	{ 4, 5, 6, 7 },
	{ 5, 4, 0, 1 },
	{ 6, 7, 3, 2 },
	{ 0, 2, 5, 7 },
	{ 3, 1, 6, 4 }
};

extern struct Camera theCamera;

extern struct _Rotation overrideEditorRotation;
extern struct _Position overrideEditorPosition;

int test_clip(RECT16* clip, short x, short y) {

	// Tests which corners of the screen a point lies outside of
#define CLIP_LEFT	1
#define CLIP_RIGHT	2
#define CLIP_TOP	4
#define CLIP_BOTTOM	8


	int result = 0;

	if (x < clip->x) {
		result |= CLIP_LEFT;
	}

	if (x >= (clip->x + (clip->w - 1))) {
		result |= CLIP_RIGHT;
	}

	if (y < clip->y) {
		result |= CLIP_TOP;
	}

	if (y >= (clip->y + (clip->h - 1))) {
		result |= CLIP_BOTTOM;
	}

	return result;

}

int tri_clip(RECT16* clip, DVECTOR* v0, DVECTOR* v1, DVECTOR* v2) {

	// Returns non-zero if a triangle is outside the screen boundaries

	short c[3];

	c[0] = test_clip(clip, v0->vx, v0->vy);
	c[1] = test_clip(clip, v1->vx, v1->vy);
	c[2] = test_clip(clip, v2->vx, v2->vy);

	if ((c[0] & c[1]) == 0)
		return 0;
	if ((c[1] & c[2]) == 0)
		return 0;
	if ((c[2] & c[0]) == 0)
		return 0;

	return 1;
}

int quad_clip(RECT16* clip, DVECTOR* v0, DVECTOR* v1, DVECTOR* v2, DVECTOR* v3) {

	// Returns non-zero if a quad is outside the screen boundaries

	short c[4];

	c[0] = test_clip(clip, v0->vx, v0->vy);
	c[1] = test_clip(clip, v1->vx, v1->vy);
	c[2] = test_clip(clip, v2->vx, v2->vy);
	c[3] = test_clip(clip, v3->vx, v3->vy);

	if ((c[0] & c[1]) == 0)
		return 0;
	if ((c[1] & c[2]) == 0)
		return 0;
	if ((c[2] & c[3]) == 0)
		return 0;
	if ((c[3] & c[0]) == 0)
		return 0;
	if ((c[0] & c[2]) == 0)
		return 0;
	if ((c[1] & c[3]) == 0)
		return 0;

	return 1;
}

void Editor_BillboardSprite(struct _Position* position, struct _Rotation* rotation)
{
	VECTOR obj_pos;
	obj_pos.vx = position->x - theCamera.core.position.x;
	obj_pos.vy = position->y - theCamera.core.position.y;
	obj_pos.vz = position->z - theCamera.core.position.z;

	SVECTOR obj_rot;
	obj_rot.vx = rotation->x;
	obj_rot.vy = rotation->y;
	obj_rot.vz = rotation->z;

	MATRIX obj_mtx;
	MATRIX tmp_mtx;

	int p = 0;
	int sz = 0;

	RotMatrix(&obj_rot, &obj_mtx);
	TransMatrix(&obj_mtx, &obj_pos);

	SVECTOR tmp;
	tmp.vx = -theCamera.core.position.x >> 12;
	tmp.vy = -theCamera.core.position.y >> 12;
	tmp.vz = -theCamera.core.position.z >> 12;

	ApplyMatrix(theCamera.core.wcTransform, &tmp, (VECTOR*)&theCamera.core.wcTransform->t[0]);

	CompMatrixLV(theCamera.core.wcTransform, &obj_mtx, &tmp_mtx);

	SetRotMatrix(&tmp_mtx);
	SetTransMatrix(&tmp_mtx);

	SVECTOR verts[] = {
	{ -50, -50, -50, 0 },
	{  50, -50, -50, 0 },
	{ -50,  50, -50, 0 },
	{  50,  50, -50, 0 },
	{  50, -50,  50, 0 },
	{ -50, -50,  50, 0 },
	{  50,  50,  50, 0 },
	{ -50,  50,  50, 0 }
	};

	POLY_FT4* polyf4 = (POLY_FT4*)gameTrackerX.primPool->nextPrim;

	unsigned long** drawot = gameTrackerX.drawOT;

	for (int i = 0; i < 8; i++) 
	{
		gte_ldv0(&verts[i]);

		gte_rtps();

		gte_stsz(&p);

		if (p > 0) 
		{
			SVECTOR spos;
			
			gte_stsxy2(&spos);

			sz = (16 * (512 / 2)) / p;

			setPolyFT4(polyf4);

			setXY4(polyf4,
				spos.vx - sz, spos.vy - sz,
				spos.vx + sz, spos.vy - sz,
				spos.vx - sz, spos.vy + sz,
				spos.vx + sz, spos.vy + sz);

			setRGB0(polyf4, 128, 128, 128);

			setcode(polyf4, 0x2C);

			polyf4->tpage = 0;// getTPage(tim.mode & 0x8, 0, tim.prect->x, tim.prect->y);

			polyf4->clut = 0;// setClut(quad, tim.crect->x, tim.crect->y);

			setUVWH(polyf4, 0, 0, 64, 64);

			addPrim(drawot + ((p >> 2) * 2), polyf4);

			/* Advance to make another primitive */
			polyf4++;

		}
	}

	gameTrackerX.primPool->nextPrim = (unsigned long*)polyf4;
}

void Editor_DrawSouls()
{
	for (int i = 0; i < gameTrackerX.level->numIntros; i++)
	{
		struct Intro* intro = &gameTrackerX.level->introList[i];

		if (!strcmp(intro->name, "soul"))
		{
			Editor_BillboardSprite(&intro->position, &intro->rotation);
		}
	}
}

void Editor_DrawCameraSplines()
{
	LINE_F2* line = (LINE_F2*)gameTrackerX.primPool->nextPrim;

	unsigned long** drawot = gameTrackerX.drawOT;

	int p = 0;

	for (int i = 0; i < gameTrackerX.level->numIntros; i++)
	{
		struct Intro* intro = &gameTrackerX.level->introList[i];

		if (!strcmp(intro->name, "campath"))
		{
			VECTOR obj_pos;
			obj_pos.vx = intro->position.x - theCamera.core.position.x;
			obj_pos.vy = intro->position.y - theCamera.core.position.y;
			obj_pos.vz = intro->position.z - theCamera.core.position.z;

			SVECTOR obj_rot;
			obj_rot.vx = intro->rotation.x;
			obj_rot.vy = intro->rotation.y;
			obj_rot.vz = intro->rotation.z;

			MATRIX obj_mtx;

			MATRIX tmp_mtx;

			RotMatrix(&obj_rot, &obj_mtx);
			TransMatrix(&obj_mtx, &obj_pos);

			SVECTOR tmp;
			tmp.vx = -theCamera.core.position.x >> 12;
			tmp.vy = -theCamera.core.position.y >> 12;
			tmp.vz = -theCamera.core.position.z >> 12;

			ApplyMatrix(theCamera.core.wcTransform, &tmp, (VECTOR*)&theCamera.core.wcTransform->t[0]);

			CompMatrixLV(theCamera.core.wcTransform, &obj_mtx, &tmp_mtx);

			SetRotMatrix(&tmp_mtx);
			SetTransMatrix(&tmp_mtx);

			for (int j = 0; j < intro->multiSpline->positional->numkeys; j+=2)
			{
				SplineKey* sKey = &intro->multiSpline->positional->key[j];
				SplineKey* sKeyNext = &intro->multiSpline->positional->key[j + 1];

				gte_ldv0(&sKey->point.x);
				
				gte_rtps();

				//gte_nclip();

				//gte_stopz(&p);

				//if (p < 0)
				//	continue;

				//gte_avsz4();
				//gte_stotz(&p);

				//if ((p >> 2) > 3071)
				//	continue;

				setLineF2(line);

				gte_stsxy(&line->x1);

				//
				gte_ldv0(&sKeyNext->point.x);

				gte_rtps();

				/*gte_nclip();

				gte_stopz(&p);

				if (p < 0)
					continue;

				gte_avsz4();
				gte_stotz(&p);

				if ((p >> 2) > 3071)
					continue;
				*/
				gte_stsxy(&line->x0);
				
				setRGB0(line, 255, 255, 255);

				addPrim(drawot + ((p >> 2) * 2), line);

				line++;

				gameTrackerX.primPool->numPrims++;
			}
		}
	}

	gameTrackerX.primPool->nextPrim = (unsigned long*)line;
}

void Editor_DrawInstancesAsCubes()
{
	int p;

	POLY_F4* pol4;

	for (int i = 0; i < gameTrackerX.level->numIntros; i++)
	{
		struct Intro* intro = &gameTrackerX.level->introList[i];

		struct DebugDrawIntro* ddi = NULL;

		for (int j = 0; j < sizeof(introGroups) / sizeof(DebugDrawIntro); j++)
		{
			if (!strcmp(intro->name, introGroups[j].groupName))
			{
				ddi = &introGroups[j];
				break;
			}
			else
			{
				printf("Unknown type: %s\n", intro->name);
			}
		}

		VECTOR obj_pos;
		obj_pos.vx = intro->position.x - theCamera.core.position.x;
		obj_pos.vy = intro->position.y - theCamera.core.position.y;
		obj_pos.vz = intro->position.z - theCamera.core.position.z;

		SVECTOR obj_rot;
		obj_rot.vx = intro->rotation.x;
		obj_rot.vy = intro->rotation.y;
		obj_rot.vz = intro->rotation.z;
		
		MATRIX obj_mtx;

		MATRIX tmp_mtx;

		RotMatrix(&obj_rot, &obj_mtx);
		TransMatrix(&obj_mtx, &obj_pos);

		SVECTOR tmp;
		tmp.vx = -theCamera.core.position.x >> 12;
		tmp.vy = -theCamera.core.position.y >> 12;
		tmp.vz = -theCamera.core.position.z >> 12;

		ApplyMatrix(theCamera.core.wcTransform, &tmp, (VECTOR*)&theCamera.core.wcTransform->t[0]);

		CompMatrixLV(theCamera.core.wcTransform, &obj_mtx, &tmp_mtx);

		SetRotMatrix(&tmp_mtx);
		SetTransMatrix(&tmp_mtx);

		unsigned long** drawot;

		drawot = gameTrackerX.drawOT;

		pol4 = (POLY_F4*)gameTrackerX.primPool->nextPrim;

		for (int j = 0; j < 6; j++) {

			gte_ldv3(&cube_verts[cube_indices[j].v0], &cube_verts[cube_indices[j].v1], &cube_verts[cube_indices[j].v2]);

			gte_rtpt();

			gte_nclip();

			gte_stopz(&p);

			if (p < 0)
				continue;

			gte_avsz4();
			gte_stotz(&p);

			if (((p >> 2) * 2) > 3071)
				continue;

			setPolyF4(pol4);

			gte_stsxy0(&pol4->x0);
			gte_stsxy1(&pol4->x1);
			gte_stsxy2(&pol4->x2);

			gte_ldv0(&cube_verts[cube_indices[j].v3]);
			gte_rtps();
			gte_stsxy(&pol4->x3);

			if (quad_clip(&screen_clip, (DVECTOR*)&pol4->x0, (DVECTOR*)&pol4->x1, (DVECTOR*)&pol4->x2, (DVECTOR*)&pol4->x3))
			{
				continue;
			}

			setcode(pol4, 0x28);

			if (ddi != NULL)
			{
				setRGB0(pol4, ddi->groupColour.r, ddi->groupColour.g, ddi->groupColour.b);
			}
			else
			{
				setRGB0(pol4, 255, 255, 255);
			}

			addPrim(drawot + ((p >> 2) * 2), pol4);

			pol4++;

			gameTrackerX.primPool->numPrims++;
		}

		gameTrackerX.primPool->nextPrim = (unsigned long*)pol4;
	}
}

void Editor_DoDebug()
{
	setRECT16(&screen_clip, 0, 0, 512, 240);

	//Editor_DrawCameraSplines();

	Editor_DrawInstancesAsCubes();

	//Editor_DrawSouls();
}

#endif