#include "FILE.H"

FILE* g_fileHandle = NULL;
std::vector<int> g_currentDTPOffset;
int g_lastRelocIndex = 0;
int g_firstHack = 1;

void FILE_Reserve(int size)
{
	if (g_fileHandle != NULL)
	{
		char dummy = 0;
		fseek(g_fileHandle, size-1, SEEK_SET);
		fwrite(&dummy, sizeof(dummy), 1, g_fileHandle);
	}
}

void FILE_OpenWrite(char* filePath)
{
	g_fileHandle = fopen("TEMP.DRM", "wb+");

	if (g_fileHandle != NULL)
	{
		FILE_PushSeek(0);
	}
}

void FILE_Close()
{
	if (g_fileHandle != NULL)
	{
		fclose(g_fileHandle);
		g_fileHandle = NULL;
	}
}

void FILE_PushSeek(int offset)
{
	if (g_fileHandle != NULL)
	{
		fseek(g_fileHandle, offset, SEEK_SET);
		g_currentDTPOffset.push_back(offset);
	}
}

int FILE_PopSeek()
{
	int last = g_currentDTPOffset.back();

	g_currentDTPOffset.pop_back();

	if (g_fileHandle != NULL)
	{
		fseek(g_fileHandle, g_currentDTPOffset.back(), SEEK_SET);
	}

	return last;
}

long FILE_SeekEnd()
{
	if (g_fileHandle != NULL)
	{
		fseek(g_fileHandle, 0, SEEK_END);
		return ftell(g_fileHandle);
	}
}

void FILE_PushSeekEnd()
{
	if (g_fileHandle != NULL)
	{
		fseek(g_fileHandle, 0, SEEK_END);
		g_currentDTPOffset.push_back(ftell(g_fileHandle));
	}
}

void FILE_SeekCur(long offset)
{
	if (g_fileHandle != NULL)
	{
		fseek(g_fileHandle, offset-1, SEEK_CUR);
		char dummy = 0;
		fwrite(&dummy, sizeof(dummy), 1, g_fileHandle);
	}
}