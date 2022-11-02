#include "Game/CORE.H"
#include "LOCALSTR.H"
#include "Game/LOAD3D.H"
#include "Game/VOICEXA.H"

#include <stddef.h>

struct LocalizationHeader* LocalizationTable;
char** PTR_32 LocalStrings;

enum language_t localstr_get_language()
{ 
	return the_language;
}

#if defined(PSXPC_VERSION) && defined(NO_CD)
	#if defined(SN_TARGET_PSP2)
		#define LOCALS_TBL "ux0:app\\TEST12345\\locals.tbl"
	#else
		#define LOCALS_TBL "LOCALS.TBL"
	#endif
#else
	#define LOCALS_TBL "\\LOCALS.TBL;1"
#endif

void localstr_set_language(enum language_t lang)
{ 
	int i;

	LocalizationTable = (struct LocalizationHeader*)LOAD_ReadFileFromCD(LOCALS_TBL, 6);

	if (LocalizationTable != NULL)
	{
#if defined(_WIN64)
		LocalStrings = (char**)((uintptr_t)LocalizationTable + sizeof(struct LocalizationHeader));
#else
		LocalStrings = (char**)LocalizationTable + 4;
#endif
		voiceList = (struct XAVoiceListEntry*)(char*)(LocalizationTable + LocalizationTable->XATableOffset);

		if (LocalizationTable->numStrings > 0)
		{
			for (i = 0; i < LocalizationTable->numStrings; i++)
			{
#if defined(_WIN64)
				((unsigned int*)LocalStrings)[i] += (uintptr_t)LocalizationTable;
#else
				LocalStrings[i] = LocalStrings[i] + (unsigned int)LocalizationTable;
#endif
			}
		}

		the_language = LocalizationTable->language;
	}
}

char* PTR_32 localstr_get(enum localstr_t id)
{
	static char BlankStr[2] = { '.', 0x0 };

	if (LocalStrings == NULL)
	{
		return &BlankStr[0];
	}
	else
	{
#if defined(_WIN64)
		return (char* PTR_32)((unsigned int*)LocalStrings)[id];
#else
		return LocalStrings[id];
#endif
	}
}