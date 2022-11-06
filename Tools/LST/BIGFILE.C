#include "BIGFILE.H"
#include "FILELIST.H"

#include <stdio.h>
#include <string>
#include <assert.h>
#include <map>
#if defined _MSC_VER
#include <Windows.h>
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif

std::map<unsigned int, const char*> finalListing;

char HashExtensions[7][4] = { "drm", "crm", "tim", "smp", "snd", "smf", "snf" };

unsigned int BIGFILE_HashName(char* string)
{
	long sum;
	long _xor;	// visual studio doesn't like 'xor' for whatever reason
	long length;
	long ext;
	char c;
	long strl;
	long endPos;
	long i;
	char* pos;

	sum = 0;
	_xor = 0;
	length = 0;
	ext = 0;

	strl = strlen(string) - 1;
	pos = strchr(string, '.');
	endPos = 0;

	if (pos != NULL)
	{
		pos++;

		for (i = 0; i < 7; i++)
		{
#if defined(PSXPC_VERSION)
			if (_strcmpi(pos, &HashExtensions[i][0]) == 0)
#else
			if (strcmpi(pos, &HashExtensions[i][0]) == 0)
#endif
			{
				ext = i;
				break;
			}
		}

		if (i < 7)
		{
			strl -= 4;
		}

		if (strl >= endPos)
		{
			for (; strl >= endPos; strl--)
			{
				c = string[strl];

				if (c != '\\')
				{
					if ((unsigned)(c - 0x61) < 0x1A)
					{
						c &= 0xDF;
					}

					c = (c - 0x1A) & 0xFF;
					sum = sum + c;
					_xor = _xor ^ (c * length++);
				}
			}
		}
	}

	return (length << 27) | (sum << 15) | (_xor << 3) | ext;
}

void BIGFILE_AddToList(unsigned int hash, char* fileName)
{
	if (!finalListing.count(hash))
	{
		finalListing.insert({ hash, fileName });
	}
}

char* BIGFILE_GetFolder(char* folderName, char* fileName)
{
	int length = strlen(fileName) - 1;
	char* p = &fileName[length];

	while(*p != '\\')
	{
		p--;
	}

	for (int i = 0; i != p - fileName + 1; i++)
	{
		folderName[i] = fileName[i];
	}

	return folderName;
}

void BIGFILE_CreateDirectories(char* filePath)
{
	char name[256];
	char* p = filePath;
	char* q = name;

	while (*p)
	{
		if (('\\' == *p) || ('/' == *p))
		{
			if (':' != *(p - 1))
			{
#if defined(_WIN32)
				CreateDirectory(name, NULL);
#endif
			}
		}
		*q++ = *p++;
		*q = '\0';
	}
#if defined(_WIN32)
	CreateDirectory(name, NULL);
#endif
}

void BIGFILE_WriteFile(char* fileName, unsigned int offset, unsigned int size)
{
#if defined(_WIN32)
	char folderName[MAX_PATH];
	memset(folderName, 0, sizeof(folderName));
	BIGFILE_GetFolder(folderName, fileName);

	char executablePath[MAX_PATH];
	memset(executablePath, 0, sizeof(executablePath));

	char fullPath[MAX_PATH];
	memset(fullPath, 0, sizeof(fullPath));


	GetModuleFileName(NULL, executablePath, MAX_PATH);
	BIGFILE_GetFolder(fullPath, executablePath);
	strcat(fullPath, folderName);
	BIGFILE_CreateDirectories(fullPath);

#else 
	mkdir(BIGFILE_GetFolder(folderName, fileName), 0700);
#endif

	FILE* f = fopen("BIGFILE.DAT", "rb");

	char* buff = new char[size];

	if (f != NULL)
	{
		fseek(f, offset, SEEK_SET);
		fread(buff, size, 1, f);
		fclose(f);
		f = NULL;
	}


	f = fopen(fileName, "wb+");
	if (f != NULL)
	{
		fwrite(buff, size, 1, f);
		fclose(f);
	}

	delete[] buff;
}

void BIGFILE_HashToName(unsigned int hash, unsigned int offset, unsigned int size)
{
	for (int i = 0; i < sizeof(fileList) / sizeof(char*); i++)
	{
		unsigned int fileHash = BIGFILE_HashName(fileList[i]);

		if (fileHash == hash)
		{
			BIGFILE_AddToList(fileHash, fileList[i]);
			
			BIGFILE_WriteFile(fileList[i], offset, size);

			break;
		}
	}
}

void BIGFILE_ProcessFiles(FILE* f, int numFiles)
{
	if (f != NULL)
	{
		struct BigFileEntry* bigFileEntries = new struct BigFileEntry[numFiles];

		fread(bigFileEntries, numFiles * sizeof(struct BigFileEntry), 1, f);

		for (int i = 0; i < numFiles; i++)
		{
			BIGFILE_HashToName(bigFileEntries[i].m_hash, bigFileEntries[i].m_offset, bigFileEntries[i].m_size);
		}

		delete[] bigFileEntries;
	}
}

void BIGFILE_ProcessDirectory(FILE* f, int numSubDirectories)
{
	if (f != NULL)
	{
		struct BigFileDirectory* bigFileDirectories = new struct BigFileDirectory[numSubDirectories];

		fread(bigFileDirectories, numSubDirectories * sizeof(struct BigFileDirectory), 1, f);

		for (int i = 0; i < numSubDirectories; i++)
		{
			fseek(f, bigFileDirectories[i].m_offset, SEEK_SET);
			struct BigFileEntryHeader entryHeader;

			fread(&entryHeader, sizeof(struct BigFileEntryHeader), 1, f);

			BIGFILE_ProcessFiles(f, entryHeader.m_numFiles);
		}
		delete[] bigFileDirectories;
	}
}

void BIGFILE_SaveListing()
{
	FILE* f = fopen("bigfile.lst", "wb+");
	char sep = 0;

	if (f != NULL)
	{
		for (auto const& lst : finalListing)
		{
			fprintf(f, "%x", lst.first);
			fwrite(&sep, sizeof(char), 1, f);
			fprintf(f, "%s", lst.second);
			fwrite(&sep, sizeof(char), 1, f);
		}

		fclose(f);
	}
}

void BIGFILE_Open(char* filePath)
{
	FILE* f = fopen(filePath, "rb");

	if (f != NULL)
	{
		struct BigFileDirectoryHeader bigFile;

		fread(&bigFile, sizeof(struct BigFileDirectoryHeader), 1, f);

		BIGFILE_ProcessDirectory(f, bigFile.m_numSubDirectories);

		fclose(f);
	}

	BIGFILE_SaveListing();
}
