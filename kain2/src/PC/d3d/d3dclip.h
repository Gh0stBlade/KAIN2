#pragma once

void __cdecl D3D_SetClipRect(int left, int top, int right, int bottom);
void __cdecl D3D_GetClipRect(int* left, int* top, int* right, int* bottom);
void __cdecl D3D_GetClipRect_float(float* left, float* top, float* right, float* bottom);
void __cdecl D3D_CalcOutCode(MYTRI* p);
