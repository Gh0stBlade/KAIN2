#ifndef MAING2_H
#define MAING2_H

struct _PVertex 
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	long otz; // size=0, offset=4
};

struct _VertexPool 
{
	struct _PVertex vertex[528]; // size=4224, offset=0
	//struct CVECTOR color[528]; // size=2112, offset=4224
	CVECTOR color[528]; // size=2112, offset=4224
};

extern enum _G2Bool_Enum MainG2_InitEngine(void *appData, unsigned int screenWidth, unsigned int screenHeight, char *filesystemName); // 0x80095D8C
extern void MainG2_ShutDownEngine(void *appData); // 0x80095D94

#endif
