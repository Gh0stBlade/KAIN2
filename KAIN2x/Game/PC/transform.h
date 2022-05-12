typedef struct TRANS_VEC
{
	float x;
	float y;
	float z;
	float w;
	float field_10;
	int flag;
} TRANS_VEC;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern void(*TRANS_DoTransform)(TRANS_VEC*, int x, int y, int z);
extern void(*SetTransformMatrix)();

extern void TRANS_Init();

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif