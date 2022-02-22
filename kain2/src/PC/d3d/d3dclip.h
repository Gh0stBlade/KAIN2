#pragma once

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void D3D_SetClipRect(int left, int top, int right, int bottom);
extern void D3D_GetClipRect(int* left, int* top, int* right, int* bottom);
extern void D3D_GetClipRect_float(float* left, float* top, float* right, float* bottom);
extern void D3D_CalcOutCode(MYTRI* p);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif
