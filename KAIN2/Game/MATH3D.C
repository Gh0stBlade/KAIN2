#include "CORE.H"
#include "MATH3D.H"
#include "Game/PSX/COLLIDES.H"
#include "Game/G2/QUATG2.H"

void MATH3D_Sort3VectorCoords(long* a, long* b, long* c)//Matching - 100%
{
	long a1;
	long b1;
	long c1;

	a1 = *a;
	b1 = *b;
	c1 = *c;

	if (a1 < b1)
	{
		if (c1 < a1)
		{
			*c = b1;
			*b = a1;
			*a = c1;
		}
		else if (c1 < b1)
		{
			*c = b1;
			*b = c1;
		}
	}
	else if (c1 < b1)
	{
		*a = c1;
		*c = a1;
	}
	else if (c1 < a1)
	{
		*a = b1;
		*b = c1;
		*c = a1;
	}
	else
	{
		*a = b1;
		*b = a1;
	}
}

long MATH3D_LengthXYZ(long x, long y, long z) // Matching - 100%
{
	long t;

	x = abs(x);
	y = abs(y);
	z = abs(z);

	if (x < y)
	{
		if (z < x)
		{
			t = x;
			x = z;
			z = y;
			y = t;
		}
		else if (z < y)
		{
			t = y;
			y = z;
			z = t;
		}
	}
	else if (z < y)
	{
		t = x;
		x = z;
		z = t;
	}
	else if (z < x)
	{
		t = x;
		x = y;
		y = z;
		z = t;
	}
	else
	{
		t = x;
		x = y;
		y = t;
	}

	t = z * 30 + y * 12 + x * 9;
	if (t < 0)
	{
		t += 31;
	}
	return t >> 5;
}

long MATH3D_LengthXY(long x, long y) // Matching - 98.75%
{
	long x1;
	long y1;

	x = abs(x);
	y = abs(y);

	y1 = y * 16;

	if (y < x)
	{
		x1 = x ^ y;
		y1 ^= x ^ y;
		x = x1 ^ y;
		y1 = y1 * 16;
	}

	return ((y1 - y) * 2 + 12 * x) / 32;
}

void MATH3D_Normalize(struct _Normal* normal)//Matching - 100%
{
	long length;

	length = MATH3D_LengthXYZ(normal->x * 4, normal->y * 4, normal->z * 4);

	if (length)
	{
		normal->x = (short)((normal->x << 14) / length);
		normal->y = (short)((normal->y << 14) / length);
		normal->z = (short)((normal->z << 14) / length);
	}
}

short MATH3D_FastAtan2(long y, long x) // Matching - 100%
{
	long ax;
	long ay;

	if (x == 0)
	{
		x = 1;
	}

	if (y == 0)
	{
		return (x < 1) * 2048;
	}

	ax = abs(-x);

	ay = abs(-y);

	if (x > 0)
	{
		if (y > 0)
		{
			if (ax < ay)
			{
				return (short)(1024 - ((ax * 512) / ay));
			}
			else
			{
				return (short)((ay * 512) / ax);
			}
		}
		else
		{
			if (ay < ax)
			{
				return (short)(4096 - ((ay * 512) / ax));
			}
			else
			{
				return (short)(((ax * 512) / ay) + 3072);
			}
		}
	}

	if (y > 0)
	{
		if (ax < ay)
		{
			return (short)(((ax * 512) / ay) + 1024);
		}
		else
		{
			return (short)(2048 - ((ay * 512) / ax));
		}
	}

	if (ay < ax)
	{
		return (short)(((ay * 512) / ax) + 2048);
	}
	else
	{
		return (short)(3072 - ((ax * 512) / ay));
	}
}

long MATH3D_FastSqrt(long square) // Matching - 100%
{
	unsigned long result;
	long remainder;
	long mask;
	long shift;
	long mask_squared;
	long result_shift;

	shift = 0x1F;

	if (square != 0)
	{
		mask = 0x80000000;

		if (square >= 0)
		{
			do
			{
				mask >>= 1;
				shift--;
			} while ((mask & square) == 0);
		}

		shift = shift >> 1;
		result = 1 << (shift + 6);
		mask = result;
		result_shift = 1 << (shift << 1);
		mask_squared = result_shift;
		shift = shift - 1;
		square = square - result_shift;

		while (shift != -1)
		{
			mask_squared >>= 2;

			remainder = result_shift + mask_squared;
			remainder = square - remainder;

			mask >>= 1;

			if (remainder < 0)
			{
				result_shift >>= 1;
			}
			else
			{
				square = remainder;

				remainder = result_shift >> 1;
				result_shift = remainder + mask_squared;

				result |= mask;
			}

			shift--;
		}

		mask_squared >>= 2;
		square <<= 12;
		result_shift <<= 12;
		mask_squared = 4096;
		mask >>= 1;

		while (mask != 0)
		{
			mask_squared >>= 2;
			remainder = result_shift + mask_squared;
			remainder = square - remainder;

			if (remainder < 0)
			{
				result_shift >>= 1;
			}
			else
			{
				square = remainder;
				remainder = result_shift >> 1;
				result_shift = remainder + mask_squared;
				result |= mask;
			}
			mask >>= 1;
		}

		return result;
	}

	return 0;
}

long MATH3D_FastSqrt0(long square)
{
	unsigned long result;
	long remainder;
	long mask;
	long shift;
	long mask_squared;
	long result_shift;

	if (square != 0)
	{
		shift = 0x1F;
		mask = 0x80000000;
		
		if (square >= 0)
		{
			do
			{
				mask >>= 1;
				
				remainder = mask & square;
			
				shift--;
		
			} while (remainder == 0);
		}
		
		shift >>= 1;

		result = 1 << shift;
		
		mask = result;

		result_shift = shift << 1;
		result_shift = 1 << result_shift;
		
		mask_squared = result_shift;
		
		square -= result_shift;

		while (--shift != -1)
		{
			mask >>= 1;

			mask_squared >>= 2;

			remainder = square - result_shift;
			remainder -= mask_squared;
			
			result_shift >>= 1;

			if (remainder >= 0)
			{
				square = remainder;
			
				result_shift += mask_squared;
				
				result |= mask;
			}
		}

		return result;
	}

	return 0;
}

long MATH3D_DistanceBetweenPositions(struct _Position* pos1, struct _Position* pos2) // Matching - 100%
{
	return MATH3D_FastSqrt0(MATH3D_SquareLength((pos2->x - pos1->x), (pos2->y - pos1->y), (pos2->z - pos1->z)));
}

short MATH3D_AngleBetweenVectors(struct _SVector* vector1, struct _SVector* vector2) // Matching - 100%
{
	long projection_length;

	if (vector1->x == vector2->x)
	{
		if ((vector1->y == vector2->y) && (vector1->z == vector2->z))
		{
			return 0;
		}
	}

	projection_length = (((vector1->x * vector2->x) + (vector1->y * vector2->y) + (vector1->z * vector2->z)) + 2048) >> 12;

	if (projection_length >= 4097)
	{
		projection_length = 4096;
	}
	else if (projection_length < -4096)
	{
		projection_length = -4096;
	}

	return (short)ratan2(MATH3D_FastSqrt0(16777216 - projection_length * projection_length), projection_length);
}

void MATH3D_RotMatAboutVec(struct _SVector* vec, MATRIX* mat, short angle) // Matching - 100%
{
	long length;
	SVECTOR rot_angs;
	MATRIX mat1;
	MATRIX mat2;

	if (angle != 0)
	{
		length = MATH3D_FastSqrt0(MATH3D_SquareLength(0, (int)vec->y, (int)vec->z) + 2048);
		rot_angs.vx = -(short)ratan2(vec->y, vec->z);
		rot_angs.vy = (short)ratan2(vec->x, length);
		rot_angs.vz = 0;
		RotMatrix(&rot_angs, &mat1);
		TransposeMatrix(&mat1, &mat2);
		MulMatrix2(&mat2, mat);
		RotMatrixZ(angle, mat);
		MulMatrix2(&mat1, mat);
	}
}

void MATH3D_SetUnityMatrix(MATRIX *mat)
{
	((unsigned int*)&mat->m[0][0])[0] = ONE;
	((unsigned int*)&mat->m[0][2])[0] = 0;
	((unsigned int*)&mat->m[1][1])[0] = ONE;
	((unsigned int*)&mat->m[2][0])[0] = 0;
	mat->m[2][2] = ONE;
}

void AngleMoveToward(short* current_ptr, short destination, short step)
{
	long diff; // $a0
	short current; // $s0

	current = *current_ptr;
	diff = (short)AngleDiff(*current_ptr, destination);

	if (diff != 0)
	{
		if (ABS(diff) < step)
		{
			*current_ptr = destination;
		}
		else
		{
			if (diff > 0)
			{
				*current_ptr = (current + step) & 0xFFF;
			}
			else
			{
				if (diff < 0)
				{
					*current_ptr = (current - step) & 0xFFF;
				}
				else
				{
					*current_ptr = (current & 0xFFF);
				}
			}
		}
	}
	else
	{
		*current_ptr = destination;
	}
}

short AngleDiff(short current, short destination)//Matching - 100%
{
	current = (destination - current) & 0xFFF;

	if (current >= 0x801)
	{
		current |= 0xF000;
	}

	return current;
}

short MATH3D_AngleFromPosToPos(struct _Position* from, struct _Position* to)//Matching - 100%
{
	return (ratan2(from->y - to->y, from->x - to->x) + 3072) & 0xFFF;
}


void MATH3D_ZYXtoXYZ(struct _Rotation* rot)//Matching - 99.57%
{
	MATRIX tempMat;
	struct _G2EulerAngles_Type ea;

	RotMatrixZYX((SVECTOR*)rot, &tempMat);

	G2EulerAngles_FromMatrix(&ea, (struct _G2Matrix_Type*)&tempMat, 21);

	rot->x = ea.x;
	rot->y = ea.y;
	rot->z = ea.z;
}

short  MATH3D_ElevationFromPosToPos(struct _Position* from, struct _Position* to)//Matching - 99.85%
{
	int dx;
	int dy;

	dx = from->x - to->x;
	dy = from->y - to->y;

	return -ratan2(to->z - from->z, (short)MATH3D_FastSqrt0(dx * dx + dy * dy)) & 0xFFF;
}

void MATH3D_RotationFromPosToPos(struct _Position* from, struct _Position* to, struct _Rotation* rot) // Matching - 100%
{
	rot->x = MATH3D_ElevationFromPosToPos(from, to);
	rot->y = 0;
	rot->z = MATH3D_AngleFromPosToPos(from, to);
}

int MATH3D_veclen2(int ix, int iy)
{
	int t;
	int v1 = ix;

	if (v1 < 0)
	{
		v1 = -v1;
	}

	if (iy < 0)
	{
		iy = -iy;
	}

	ix = iy >> 1;

	if (v1 < iy)
	{
		v1 ^= iy;
		iy ^= v1;
		v1 ^= iy;

		ix = iy >> 1;
	}

	ix = iy + ix;

	return ((v1 - (v1 >> 5)) - (v1 >> 7)) + (ix >> 2) + (ix >> 6);
}

void MATH3D_RotateAxisToVector(MATRIX* dest, MATRIX* src, struct _SVector* vec, enum MATH3D_AXIS axis)//Matching - 80.73%
{
	MATRIX xform;
	struct _G2Quat_Type rot;
	long len;
	int theta;
	int sintheta;
	int px;
	int py;
	int pz;

	if ((unsigned int)axis >= 3)
	{
		axis = (enum MATH3D_AXIS)((unsigned int)axis - 3);
		px = -src->m[0][(unsigned int)axis];
		py = -src->m[1][(unsigned int)axis];
		pz = -src->m[2][(unsigned int)axis];
	}
	else
	{
		px = src->m[0][(unsigned int)axis];
		py = src->m[1][(unsigned int)axis];
		pz = src->m[2][(unsigned int)axis];
	}

	rot.x = (py * vec->z - pz * vec->y) / 4096;
	rot.y = (pz * vec->x - px * vec->z) / 4096;
	rot.z = (px * vec->y - py * vec->x) / 4096;
	sintheta = MATH3D_racos_S((px * vec->x + py * vec->y + pz * vec->z) / 4096);
	theta = (int)((short)sintheta + ((unsigned int)((short)sintheta) >> 0x1F)) >> 1;
	len = MATH3D_SquareLength(rot.x, rot.y, rot.z);

	if (len <= 0)
	{
		len = 4096;
	}
	else
	{
		len = MATH3D_FastSqrt0(len);
	}

	sintheta = rsin(theta);
	
	rot.x = (short)(rot.x * sintheta / len);
	rot.y = (short)(rot.y * sintheta / len);
	rot.z = (short)(rot.z * sintheta / len);
	rot.w = rcos(theta);
	
	G2Quat_ToMatrix_S(&rot, (struct _G2Matrix_Type*)&xform);

	MulMatrix0(src, &xform, dest);
}

int MATH3D_ConeDetect(struct _SVector* pos, int arc, int elevation)//Matching - 93.39%
{
	long x;
	long y;

	x = pos->x;
	y = -pos->y;

	if ((short)MATH3D_FastAtan2(ABS(x), y) < arc)
	{
		if ((short)MATH3D_FastAtan2(ABS(pos->z), MATH3D_LengthXY(x, y)) < elevation)
		{
			return 1;
		}
	}
	return 0;
}

void MATH3D_CrossProduct(struct _SVector* t, struct _SVector* r, struct _SVector* s)//Matching - 100%
{
	t->x = (r->y * s->z - r->z * s->y) >> 12;
	t->y = -((r->x * s->z - r->z * s->x) >> 12);
	t->z = (r->x * s->y - r->y * s->x) >> 12;
}

unsigned long MATH3D_SquareLength(long x, long y, long z)
{
	long v[3];
	long r[3];

	v[0] = x;
	v[1] = y;
	v[2] = z;

	gte_ldlvl(&v);
	gte_sqr0();
	gte_stlvnl(r);

	return r[0] + r[1] + r[2];
}