#ifndef QUATG2_H
#define QUATG2_H

struct _G2SVector3_Type 
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

struct _G2Quat_Type 
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short w; // size=0, offset=6
};

extern void G2Quat_ToEuler(struct _G2Quat_Type *quat, struct _G2EulerAngles_Type *euler, int order); // 0x80095ED8
extern void G2EulerAngles_FromMatrix(struct _G2EulerAngles_Type *euler, struct _G2Matrix_Type *matrix, long order); // 0x80095F1C

#endif
