#include "Game/CORE.H"
#include "QUATG2.H"
#include "Game/COLLIDE.H"
#include "Game/MATH3D.H"
#include "Game/PSX/COLLIDES.H"


#define EulSafe "\000\001\002\000"
#define EulNext "\001\002\000\001"
#define EulFrmR	1
#define EulParOdd 1

void G2Quat_ToEuler(struct _G2Quat_Type* quat, struct _G2EulerAngles_Type* euler, int order) // Matching - 100%
{
	struct _G2Matrix_Type tempMatrix;

	G2Quat_ToMatrix_S(quat, &tempMatrix);
	
	G2EulerAngles_FromMatrix(euler, &tempMatrix, order);
}

void G2EulerAngles_FromMatrix(struct _G2EulerAngles_Type* euler, struct _G2Matrix_Type* matrix, long order) // Matching - 99.82%
{
    long i;
    long j;
    long k;
    long n;
    long s;
    long f;
    long g; // Not exist in sysdump

    unsigned int o;
    o = order;
    g = o & 1;
    o >>= 1;
    i = o & 1;
    o >>= 1;
    f = o & 1;
    o >>= 1;
    n = EulSafe[o & 3];
    s = EulNext[n + f];
    k = EulNext[n + 1 - f];

    if (i == 1)
    {
        long sy;
        i = matrix->rotScale[n][s];
        j = matrix->rotScale[n][k];
        sy = MATH3D_FastSqrt((i * i + j * j) >> 12);
        if (sy > 16)
        {
            euler->x = (short)ratan2(matrix->rotScale[n][s], matrix->rotScale[n][k]);
            euler->y = (short)ratan2(sy, matrix->rotScale[n][n]);
            euler->z = (short)ratan2(matrix->rotScale[s][n], -matrix->rotScale[k][n]);
        }
        else
        {
            euler->x = (short)ratan2(-matrix->rotScale[s][k], matrix->rotScale[s][s]);
            euler->y = (short)ratan2(sy, matrix->rotScale[n][n]);
            euler->z = 0;
        }
    }
    else
    {
        long cy;
        i = matrix->rotScale[n][n];
        j = matrix->rotScale[s][n];
        cy = MATH3D_FastSqrt((i * i + j * j) >> 12);
        if (cy > 16)
        {
            euler->x = (short)ratan2(matrix->rotScale[k][s], matrix->rotScale[k][k]);
            euler->y = (short)ratan2(-matrix->rotScale[k][n], cy);
            euler->z = (short)ratan2(matrix->rotScale[s][n], matrix->rotScale[n][n]);
        }
        else
        {
            euler->x = (short)ratan2(-matrix->rotScale[s][k], matrix->rotScale[s][s]);
            euler->y = (short)ratan2(-matrix->rotScale[k][n], cy);
            euler->z = 0;
        }
    }

    if (f == EulParOdd)
    {
        euler->x = -euler->x;
        euler->y = -euler->y;
        euler->z = -euler->z;
    }
    if (g == EulFrmR)
    {
        short t;
        t = euler->x;
        euler->x = euler->z;
        euler->z = t;
    }
    euler->order = (short)order;
}