#include "Game/CORE.H"
#include "QUATVM.H"
#include "Game/PSX/COLLIDES.H"
#include "Game/MATH3D.H"

void G2Quat_Slerp_VM(long ratio, struct _G2Quat_Type* quatA, struct _G2Quat_Type* quatB, struct _G2Quat_Type* quatOut, int spin)
{
	long beta;
	short theta;
	short cos_t;
	long bflip;
	long foo[4];
	long cosTemp1;
	long cosTemp2;

	gte_ldsv(quatA);

	gte_ldv0sv(quatB);

	gte_rtir();

	cosTemp1 = (quatA->w * quatB->w) >> 12;

	gte_stlvnl0(&cosTemp2);

	cos_t = ((unsigned short*)&cosTemp2)[0] + cosTemp1;

	if (cos_t < 0)
	{
		cos_t = -cos_t;

		bflip = 1;
	}
	else
	{
		bflip = 0;
	}

	if ((4096 - cos_t) > 0)
	{
		beta = 4096 - ratio;

		theta = MATH3D_racos_S(cos_t);

		beta = theta;

		theta += (spin << 12);

		beta = (rsin(beta - ((ratio * theta))) << 12) / (theta = rsin(beta));

		ratio = (rsin(ratio) << 12) / theta;
	}
	else
	{
		beta = 4096 - ratio;
	}

	if (bflip != 0)
	{
		ratio = -ratio;
	}

	gte_ldsv(quatA);

	gte_lddp(beta);

	gte_gpf12();

	foo[3] = (beta * quatA->w) >> 12;

	gte_ldsv(quatB);

	gte_lddp(ratio);

	gte_gpl12();

	foo[3] += ((ratio * quatB->w) >> 12);

	gte_stlvl(&foo);

	cosTemp1 = 0x1000000 / MATH3D_FastSqrt((((foo[3] * foo[3]) + (foo[2] * foo[2]) + (foo[1] * foo[1]) + (foo[0] * foo[0])) + 2048) >> 12);

	gte_lddp(cosTemp1);

	gte_gpf12();

	quatOut->w = (foo[3] * cosTemp1) >> 12;

	gte_stsv(quatOut);
}

