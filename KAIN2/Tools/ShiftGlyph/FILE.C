#include "FILE.H"

void FILE_Reserve(FILE* f, int size)
{
	if (f != NULL)
	{
		char dummy = 0;
		fseek(f, size-1, SEEK_SET);
		fwrite(&dummy, sizeof(dummy), 1, f);
	}
}

FILE* FILE_OpenWrite(char* filePath)
{
	return fopen(filePath, "wb+");
}

void FILE_Close(FILE* f)
{
	if (f != NULL)
	{
		fclose(f);
		f = NULL;
	}
}

long FILE_SeekEnd(FILE* f)
{
	if (f != NULL)
	{
		fseek(f, 0, SEEK_END);
		return ftell(f);
	}
}

void FILE_SeekCur(FILE* f, long offset)
{
	if (f != NULL)
	{
		fseek(f, offset-1, SEEK_CUR);
		char dummy = 0;
		fwrite(&dummy, sizeof(dummy), 1, f);
	}
}

long FILE_GetOffset(FILE* f)
{
	if (f != NULL)
	{
		return ftell(f);
	}
	return -1;
}

void FILE_Seek(FILE* f, long offset, int origin)
{
	if (f != NULL)
	{
		fseek(f, offset, origin);
	}
}