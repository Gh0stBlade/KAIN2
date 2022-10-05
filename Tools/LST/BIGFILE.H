#ifndef BIGFILE_H
#define BIGFILE_H

struct BigFileEntry
{
	unsigned int m_hash;
	unsigned int m_size;
	unsigned int m_offset;
	unsigned int m_checksum;
};

struct BigFileEntryHeader
{
	unsigned int m_numFiles;
};

struct BigFileDirectory
{
	unsigned int m_hash;
	unsigned int m_offset;
};

struct BigFileDirectoryHeader
{
	unsigned int m_numSubDirectories;
};

extern void BIGFILE_Open(char* filePath);

#endif