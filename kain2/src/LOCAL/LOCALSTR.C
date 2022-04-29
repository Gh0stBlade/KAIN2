#include "CORE.H"
#include "LOCALSTR.H"
#include "LOAD3D.H"
#include "VOICEXA.H"

#include <stddef.h>

struct LocalizationHeader* LocalizationTable;
char** LocalStrings;

enum language_t localstr_get_language()
{ 
	return the_language;
}

#if defined(PSXPC_VERSION) && defined(NO_CD)
	#define LOCALS_TBL "\\LOCALS.TBL"
#else
	#define LOCALS_TBL "\\LOCALS.TBL;1"
#endif

void localstr_set_language(enum language_t lang)
{ 
	int i;

	LocalizationTable = (struct LocalizationHeader*)LOAD_ReadFileFromCD(LOCALS_TBL, 6);

	if (LocalizationTable != NULL)
	{
		LocalStrings = (char**)LocalizationTable + 4;
		voiceList = (XAVoiceListEntry*)(char*)(LocalizationTable + LocalizationTable->XATableOffset);

		if (LocalizationTable->numStrings > 0)
		{
			for (i = 0; i < LocalizationTable->numStrings; i++)
			{
				LocalStrings[i] = LocalStrings[i] + (unsigned int)LocalizationTable;
			}
		}

		the_language = LocalizationTable->language;
	}
}

char* localstr_get(enum localstr_t id)
{
	static char BlankStr[2] = { '.', 0x0 };

	if (LocalStrings == NULL)
	{
		return &BlankStr[0];
	}
	else
	{
		return LocalStrings[id];
	}
}