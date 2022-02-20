#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include "d3dbuckt.h"
#include "d3dclip.h"

HGLOBAL hMem;
MATBUCKET MaterialBuckets[6], transbucket;
TRANSTRI transinfo[4096];
TRANSTRI transinfo2[4096];
MATBUCKET* dword_BBBCC8;
DWORD dword_BBBCCC, dword_BABC48, dword_BBBCD4;

extern DWORD D3D_CurFogUnit, D3D_AdaptivePerspec;

#define alloc_tri(count)	(MYTRI*)GlobalAlloc(GMEM_FIXED, sizeof(MYTRI) * (count))
#define free_tri(x)			GlobalFree((HGLOBAL)(x))

//0001:00067770       _D3D_InitBuckets           00468770 f   d3dbuckt.obj
void __cdecl D3D_InitBuckets()
{
	MATBUCKET* bucket; // esi

	bucket = MaterialBuckets;
	for (int i = 0; i < 6; i++)
	{
		bucket->field_0 = 0;
		bucket->count = 0;
		bucket->page = -1;
		if (dword_BBBCC8 == bucket)
			dword_BBBCCC = -1;
		bucket->ptr = alloc_tri(128);
		bucket->field_0 = 0;
		bucket->page = -1;
		bucket->count = 0;
		bucket->total = 112;
		++bucket;
	}

	transbucket.count = 0;
	dword_BABC48 = 0;
	dword_BBBCD4 = 0;
	transbucket.page = -1;
	dword_BBBCCC = -1;
	transbucket.ptr = alloc_tri(12288);
	transbucket.field_0 = 1;
	transbucket.total = 12272;
}
//0001:00067810       _D3D_FreeBuckets           00468810 f   d3dbuckt.obj
void __cdecl D3D_FreeBuckets()
{
	MATBUCKET* bucket;
	bool v1;

	bucket = MaterialBuckets;
	for(int i = 0; i < 6; i++)
	{
		free_tri(bucket->ptr);
		v1 = dword_BBBCC8 == bucket;
		bucket->field_0 = 0;
		bucket->count = 0;
		bucket->page = -1;
		if (v1)
			dword_BBBCCC = -1;
		++bucket;
	}
	free_tri(transbucket.ptr);
}

//0001:00067860       _D3D_SetTexMorphPosition   00468860 f   d3dbuckt.obj
float dword_B9BC40;
int dword_BBBCD0,
	dword_BBBCD8;

#define MAKE4(a,b,c,d)		(a) | ((b) << 8) | ((c) << 16) | ((d) << 24)

void __cdecl D3D_SetTexMorphPosition(float pos)
{
	if (pos <= 0.99000001f)
	{
		if (pos >= 0.0099999998f)
		{
			dword_BBBCD4 = 0;
			dword_B9BC40 = pos;
			dword_BABC48 = 0;

			int a = (int)(pos * 255.f);
			int b = (int)((1.f - pos) * 255.f);

			dword_BBBCD0 = MAKE4(a,a,a,a);
			dword_BBBCD8 = MAKE4(b,b,b,b);
		}
		else
		{
			dword_BABC48 = 0;
			dword_BBBCD4 = 0;
		}
	}
	else
	{
		dword_BABC48 = 0;
		dword_BBBCD4 = 1;
	}
}
//0001:00067910       _D3D_ClearAllBuckets       00468910 f   d3dbuckt.obj
void __cdecl D3D_ClearAllBuckets()
{
	MATBUCKET* v0; // ecx
	MATBUCKET* bucket; // eax

	v0 = dword_BBBCC8;
	dword_BBBCCC = -1;
	bucket = MaterialBuckets;
	for(int i = 0; i < 6; i++)
	{
		bucket->field_0 = 0;
		bucket->count = 0;
		bucket->page = -1;
		if (v0 == bucket)
			dword_BBBCCC = -1;
		++bucket;
	}
}
//0001:00067950       _D3D_ClearBucket           00468950 f   d3dbuckt.obj
void __cdecl D3D_ClearBucket(MATBUCKET* bucket)
{
	bucket->field_0 = 0;
	bucket->count = 0;
	bucket->page = -1;
	if (dword_BBBCC8 == bucket)
		dword_BBBCCC = -1;
}
//0001:00067980       _D3D_GetBucket             00468980 f   d3dbuckt.obj
MATBUCKET* __cdecl D3D_GetBucket(int page)
{
	int i; // ecx
	MATBUCKET* res = nullptr; // eax
	MATBUCKET* b; // eax
	MATBUCKET* bucket; // esi
	int count; // edx

	if (page == dword_BBBCCC)
		return dword_BBBCC8;
	dword_BBBCCC = page;
	if (!D3D_AdaptivePerspec)
		page &= ~0x10000u;
	if ((page & 0xE000) != 0)
	{
		dword_BBBCCC = -1;
		transbucket.page = page;
		return &transbucket;
	}
	else
	{
		i = 0;
		b = MaterialBuckets;
		for(int j = 0; j < 6; j++, i++, b++)
		{
			if (!b->field_0)
				break;
			if (b->page == page)
			{
				dword_BBBCC8 = &MaterialBuckets[i];
				return dword_BBBCC8;
			}
		}

		if (i == 6)
		{
			bucket = 0;
			count = 0;
			b = MaterialBuckets;
			for(int j = 0; j < 6; j++, b++)
			{
				if (b->count >= count)
				{
					count = b->count;
					bucket = b;
				}
			}
			D3D_DrawBucket(bucket);
			bucket->field_0 = 0;
			bucket->count = 0;
			bucket->page = -1;
			if (dword_BBBCC8 == bucket)
				dword_BBBCCC = -1;
			bucket->page = page;
			bucket->count = 0;
			bucket->field_0 = 1;
			dword_BBBCC8 = bucket;
			return bucket;
		}
		else
		{
			MaterialBuckets[i].page = page;
			MaterialBuckets[i].count = 0;
			res = &MaterialBuckets[i];
			dword_BBBCC8 = res;
			res->field_0 = 1;
		}
	}

	return res;
}
//0001:00067aa0       _D3D_AddTriToBucket        00468aa0 f   d3dbuckt.obj
void __cdecl D3D_AddTriToBucket(MATBUCKET* bucket, MYTRI* tri)
{
	if (bucket->count > bucket->total)
	{
		D3D_DrawBucket(bucket);
		bucket->count = 0;
	}
	if (bucket == &transbucket)
	{
		int pos = transbucket.count / 3;
		transinfo[pos].page = bucket->page;
		transinfo[pos].fogUnit = D3D_CurFogUnit;
	}
	memcpy(&bucket->ptr[bucket->count], tri, sizeof(*tri) * 3);
	bucket->count += 3;
}
//0001:00067b20       _D3D_AddPolyToBucket       00468b20 f   d3dbuckt.obj
void __cdecl D3D_AddPolyToBucket(MATBUCKET* bucket, MYTRI* poly, int count)
{
	int i;

	if (count - 1 > 1)
	{
		i = count - 2;
		do
		{
			if (bucket->count > bucket->total)
			{
				D3D_DrawBucket(bucket);
				bucket->count = 0;
			}
			if (bucket == &transbucket)
			{
				int pos = transbucket.count / 3;
				transinfo[pos].page = bucket->page;
				transinfo[pos].fogUnit = D3D_CurFogUnit;
			}
			memcpy(&bucket->ptr[bucket->count + 0], &poly[0], sizeof(MYTRI));
			memcpy(&bucket->ptr[bucket->count + 1], &poly[1], sizeof(MYTRI));
			memcpy(&bucket->ptr[bucket->count + 2], &poly[2], sizeof(MYTRI));
			bucket->count += 3;
			--i;
		} while (i);
	}
}
//0001:00067c00       _D3D_DrawAllBuckets        00468c00 f   d3dbuckt.obj
void __cdecl D3D_DrawAllBuckets()
{
	MATBUCKET* bucket; // esi

	bucket = MaterialBuckets;
	for(int i = 0; i< 6; i++)
		D3D_DrawBucket(bucket++);
}
//0001:00067c20       _D3D_DrawBucket            00468c20 f   d3dbuckt.obj
void __cdecl D3D_DrawBucket(MATBUCKET* bucket)
{}
//0001:00067ec0       _D3D_DrawTransBucket       00468ec0 f   d3dbuckt.obj
void __cdecl D3D_DrawTransBucket()
{}
