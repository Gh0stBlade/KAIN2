#include "EDITOR.H"
#include "CORE.H"

#include "Game/STREAM.H"
#include "Game/GAMELOOP.H"
#include "Game/CAMERA.H"
#include "BSP.H"


#if defined(EDITOR)

extern struct Level* g_selectedUnit;

extern int g_overrideWidth;
extern int g_overrideHeight;

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

			//if (quad_clip(&screen_clip, (DVECTOR*)&pol4->x0, (DVECTOR*)&pol4->x1, (DVECTOR*)&pol4->x2, (DVECTOR*)&pol4->x3))
			{
			//	continue;
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

struct _BoundingBox Editor_CreateUnitBBOX()
{
	struct _Terrain* terrain = g_selectedUnit->terrain;
	struct _BoundingBox box;
	SVECTOR max;

	box.minX = 0;
	box.minY = 0;
	box.minZ = 0;

	box.maxX = 0;
	box.maxY = 0;
	box.maxZ = 0;

	for (int i = 0; i < terrain->numVertices; i++)
	{
		struct _TVertex* vertex = &terrain->vertexList[i];

		if (vertex->vertex.x < box.minX)
		{
			box.minX = vertex->vertex.x;
		}

		if (vertex->vertex.x > box.maxX)
		{
			box.maxX = vertex->vertex.x;
		}

		if (vertex->vertex.y < box.minY)
		{
			box.minY = vertex->vertex.y;
		}

		if (vertex->vertex.y > box.maxY)
		{
			box.maxY = vertex->vertex.y;
		}

		if (vertex->vertex.z < box.minZ)
		{
			box.minZ = vertex->vertex.z;
		}

		if (vertex->vertex.z > box.maxZ)
		{
			box.maxZ = vertex->vertex.z;
		}
	}

	return box;
}

void Editor_DrawCameraLine(int clickX, int clickY)
{
#if 0
	int mouseX = clickX / (g_overrideWidth * 0.5f) - 1.0f;
	int mouseY = clickY / (g_overrideHeight * 0.5f) - 1.0f;

	glm::mat4 proj = glm::perspective(FoV, AspectRatio, theCamera.core.nearPlane, theCamera.core.farPlane);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f), CameraDirection, CameraUpVector);

	glm::mat4 invVP = glm::inverse(proj * view);
	glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
	glm::vec4 worldPos = invVP * screenPos;

	glm::vec3 dir = glm::normalize(glm::vec3(worldPos));
#endif
}

void Editor_DrawUnitBBOX(struct _BoundingBox box, struct _Position offset)
{
	//TODO use leaves system
	//TODO use "DRAWBBOX" function at xyz location etc etc.
	//TODO use leaves box
	LINE_F2* line = (LINE_F2*)gameTrackerX.primPool->nextPrim;
	SVECTOR verts[24];

	verts[0].vx = box.minX;
	verts[0].vy = box.maxY;
	verts[0].vz = box.minZ;
	verts[1].vx = box.minX;
	verts[1].vy = box.maxY;
	verts[1].vz = box.maxZ;

	verts[2].vx = box.minX;
	verts[2].vy = box.maxY;
	verts[2].vz = box.maxZ;
	verts[3].vx = box.maxX;
	verts[3].vy = box.maxY;
	verts[3].vz = box.maxZ;

	verts[4].vx = box.maxX;
	verts[4].vy = box.maxY;
	verts[4].vz = box.maxZ;
	verts[5].vx = box.maxX;
	verts[5].vy = box.maxY;
	verts[5].vz = box.minZ;

	verts[6].vx = box.maxX;
	verts[6].vy = box.maxY;
	verts[6].vz = box.minZ;
	verts[7].vx = box.minX;
	verts[7].vy = box.maxY;
	verts[7].vz = box.minZ;

	verts[8].vx = box.minX;
	verts[8].vy = box.minY;
	verts[8].vz = box.minZ;
	verts[9].vx = box.minX;
	verts[9].vy = box.minY;
	verts[9].vz = box.maxZ;

	verts[10].vx = box.minX;
	verts[10].vy = box.minY;
	verts[10].vz = box.maxZ;
	verts[11].vx = box.maxX;
	verts[11].vy = box.minY;
	verts[11].vz = box.maxZ;

	verts[12].vx = box.maxX;
	verts[12].vy = box.minY;
	verts[12].vz = box.maxZ;
	verts[13].vx = box.maxX;
	verts[13].vy = box.minY;
	verts[13].vz = box.minZ;

	verts[14].vx = box.maxX;
	verts[14].vy = box.minY;
	verts[14].vz = box.minZ;
	verts[15].vx = box.minX;
	verts[15].vy = box.minY;
	verts[15].vz = box.minZ;

	verts[16].vx = box.minX;
	verts[16].vy = box.maxY;
	verts[16].vz = box.minZ;
	verts[17].vx = box.minX;
	verts[17].vy = box.minY;
	verts[17].vz = box.minZ;

	verts[18].vx = box.minX;
	verts[18].vy = box.maxY;
	verts[18].vz = box.maxZ;
	verts[19].vx = box.minX;
	verts[19].vy = box.minY;
	verts[19].vz = box.maxZ;

	verts[20].vx = box.maxX;
	verts[20].vy = box.maxY;
	verts[20].vz = box.maxZ;
	verts[21].vx = box.maxX;
	verts[21].vy = box.minY;
	verts[21].vz = box.maxZ;

	verts[22].vx = box.maxX;
	verts[22].vy = box.maxY;
	verts[22].vz = box.minZ;
	verts[23].vx = box.maxX;
	verts[23].vy = box.minY;
	verts[23].vz = box.minZ;

	unsigned long** drawot = gameTrackerX.drawOT;

	int p = 0;
	struct _Position cam_pos_save;
	MATRIX cam_mat_save;

	cam_pos_save.x = theCamera.core.position.x;
	cam_pos_save.y = theCamera.core.position.y;
	cam_pos_save.z = theCamera.core.position.z;

	cam_mat_save.m[0][0] = theCamera.core.wcTransform->m[0][0];
	cam_mat_save.m[0][1] = theCamera.core.wcTransform->m[0][1];
	cam_mat_save.m[0][2] = theCamera.core.wcTransform->m[0][2];
	cam_mat_save.m[1][0] = theCamera.core.wcTransform->m[1][0];
	cam_mat_save.m[1][1] = theCamera.core.wcTransform->m[1][1];
	cam_mat_save.m[1][2] = theCamera.core.wcTransform->m[1][2];
	cam_mat_save.m[2][0] = theCamera.core.wcTransform->m[2][0];
	cam_mat_save.m[2][1] = theCamera.core.wcTransform->m[2][1];
	cam_mat_save.m[2][2] = theCamera.core.wcTransform->m[2][2];

	cam_mat_save.t[0] = theCamera.core.wcTransform->t[0];
	cam_mat_save.t[1] = theCamera.core.wcTransform->t[1];
	cam_mat_save.t[2] = theCamera.core.wcTransform->t[2];

	theCamera.core.position.x = cam_pos_save.x - offset.x;
	theCamera.core.position.y = cam_pos_save.y - offset.y;
	theCamera.core.position.z = cam_pos_save.z - offset.z;

	SVECTOR tmp;
	tmp.vx = -(cam_pos_save.x - offset.x);
	tmp.vy = -(cam_pos_save.y - offset.y);
	tmp.vz = -(cam_pos_save.z - offset.z);

	ApplyMatrix(&cam_mat_save, &tmp, (VECTOR*)&theCamera.core.wcTransform->t[0]);

	SetRotMatrix(theCamera.core.wcTransform);
	SetTransMatrix(theCamera.core.wcTransform);

	for (int i = 0; i < 24; i+=2)
	{
		gte_ldv0(&verts[i].vx);
		gte_ldv1(&verts[i + 1].vx);

		gte_rtpt();

		gte_avsz3();
		gte_stotz(&p);

		if (((p >> 2) >= 3071) || ((p >> 2) <= 0))
			continue;

		setLineF2(line);
		
		gte_stsxy0(&line->x0);
		gte_stsxy1(&line->x1);

		setRGB0(line, 255, 255, 255);

		addPrim(drawot + ((p >> 2) * 2), line);

		line++;

		gameTrackerX.primPool->numPrims++;
	}

	gameTrackerX.primPool->nextPrim = (unsigned long*)line;

	theCamera.core.position.x = cam_pos_save.x;
	theCamera.core.position.y = cam_pos_save.y;
	theCamera.core.position.z = cam_pos_save.z;

	theCamera.core.wcTransform->m[0][0] = cam_mat_save.m[0][0];
	theCamera.core.wcTransform->m[0][1] = cam_mat_save.m[0][1];
	theCamera.core.wcTransform->m[0][2] = cam_mat_save.m[0][2];
	theCamera.core.wcTransform->m[1][0] = cam_mat_save.m[1][0];
	theCamera.core.wcTransform->m[1][1] = cam_mat_save.m[1][1];
	theCamera.core.wcTransform->m[1][2] = cam_mat_save.m[1][2];
	theCamera.core.wcTransform->m[2][0] = cam_mat_save.m[2][0];
	theCamera.core.wcTransform->m[2][1] = cam_mat_save.m[2][1];
	theCamera.core.wcTransform->m[2][2] = cam_mat_save.m[2][2];

	theCamera.core.wcTransform->t[0] = cam_mat_save.t[0];
	theCamera.core.wcTransform->t[1] = cam_mat_save.t[1];
	theCamera.core.wcTransform->t[2] = cam_mat_save.t[2];
}

void Editor_CreateAndDrawUnitBBOX()
{
	if (g_selectedUnit != NULL)
	{
		if (g_selectedUnit->terrain != NULL)
		{
			Editor_DrawUnitBBOX(Editor_CreateUnitBBOX(), g_selectedUnit->terrain->BSPTreeArray->globalOffset);
		}
	}
}

void Editor_DoDebug()
{
	setRECT16(&screen_clip, 0, 0, 512, 240);

	//Editor_DrawCameraLine(g_selectedUnit->terrain->BSPTreeArray->globalOffset);

	Editor_CreateAndDrawUnitBBOX();

	//Editor_DrawCameraSplines();

	//Editor_DrawInstancesAsCubes();

	//Editor_DrawSouls();
}

#endif