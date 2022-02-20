#pragma once

typedef struct MYTRI
{
	float x, y, z, w;
	DWORD spec, col;
	float u, v;
} MYTRI;

typedef struct TRANSTRI
{
	MYTRI* pri;
	int fogUnit;
	int page;
	int field_C;
} TRANSTRI;

typedef struct MATBUCKET
{
	int field_0;
	int page;
	int count;
	int total;
	MYTRI* ptr;
} MATBUCKET;

void __cdecl D3D_InitBuckets();
void __cdecl D3D_FreeBuckets();
void __cdecl D3D_SetTexMorphPosition(float pos);
void __cdecl D3D_ClearAllBuckets();
void __cdecl D3D_ClearBucket(MATBUCKET* bucket);
MATBUCKET* __cdecl D3D_GetBucket(int page);
void __cdecl D3D_AddTriToBucket(MATBUCKET* bucket, MYTRI* tri);
void __cdecl D3D_AddPolyToBucket(MATBUCKET* bucket, MYTRI* poly, int count);
void __cdecl D3D_DrawAllBuckets();
void __cdecl D3D_DrawBucket(MATBUCKET* bucket);
void __cdecl D3D_DrawTransBucket();
