#pragma once

struct _AadDynSfxFileHdr64
{
	unsigned long snfID;
	unsigned short snfVersion;
	unsigned short uniqueID;
	unsigned short handle;
	unsigned short numSfxInFile;
	union
	{
		struct _AadDynSfxFileHdr* prevDynSfxFile;
		long long prevDynSfxFile_;
	};
	union
	{
		struct _AadDynSfxFileHdr* nextDynSfxFile;
		long long nextDynSfxFile_;
	};
};

struct _AadDynSfxFileHdr
{
	unsigned long snfID;
	unsigned short snfVersion;
	unsigned short uniqueID;
	unsigned short handle;
	unsigned short numSfxInFile;
	struct _AadDynSfxFileHdr* prevDynSfxFile;
	struct _AadDynSfxFileHdr* nextDynSfxFile;
};