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
	int total;		// max allocations of MYTRIs, -16 for some reason
	MYTRI* ptr;
} MATBUCKET;


#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

void D3D_InitBuckets();
void D3D_FreeBuckets();
void D3D_SetTexMorphPosition(float pos);
void D3D_ClearAllBuckets();
void D3D_ClearBucket(MATBUCKET* bucket);
MATBUCKET* D3D_GetBucket(int page);
void D3D_AddTriToBucket(MATBUCKET* bucket, MYTRI* tri);
void D3D_AddPolyToBucket(MATBUCKET* bucket, MYTRI* poly, int count);
void D3D_DrawAllBuckets();
void D3D_DrawBucket(MATBUCKET* bucket);
void D3D_DrawTransBucket();

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif
