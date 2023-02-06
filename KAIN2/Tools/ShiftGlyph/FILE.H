#pragma once

#include <iostream>
#include <vector>

struct UPGRADE_RedirectList
{
	long* pDTP;
	long* pDTPConvertedFile;
	unsigned int incr;
};

extern FILE* g_fileHandle;
extern std::vector<int> g_currentDTPOffset;
extern std::vector<UPGRADE_RedirectList> g_redirectList;
extern int g_lastRelocIndex;
extern int g_firstHack;
extern int g_numPointerTypes;
extern char* g_pCurr;
extern long* g_baseAddr;


extern void FILE_Reserve(int size);
extern void FILE_OpenWrite(char* filePath);
extern void FILE_PushSeek(int offset);
extern long FILE_SeekEnd();
extern void FILE_PushSeekEnd();
extern int FILE_PopSeek();
extern void FILE_SeekCur(long offset);