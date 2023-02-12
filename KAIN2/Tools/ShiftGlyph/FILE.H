#pragma once

#include <iostream>

extern FILE* g_fileHandle;

extern void FILE_Reserve(int size);
extern FILE* FILE_OpenWrite(char* filePath);
extern void FILE_Close(FILE* f);
extern long FILE_SeekEnd(FILE* f);
extern long FILE_GetOffset(FILE* f);
extern void FILE_Seek(FILE* f, long offset, int origin);