#pragma once

#include <iostream>

extern FILE* g_fileHandle;

extern void FILE_Reserve(int size);
extern FILE* FILE_OpenWrite(char* filePath);
extern void FILE_Close(FILE* f);
extern long FILE_SeekEnd(FILE* f);