#include "BOUNTY_LIST.H"
#include "Core/Setup/Game/GAME_VERSION.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Debug/EMULATOR_ASSERT.H"

#if !defined(NO_BOUNTY_LIST_EXPORT)

#include <iostream>

#define MAX_NUM_BOUNTIES 4096

const char* bountyList[MAX_NUM_BOUNTIES];
unsigned int bountyListCalls[MAX_NUM_BOUNTIES];
unsigned int lastIndex = 0;
#endif

void Emulator_AddBounty(const char* functionName)
{
#if !defined(NO_BOUNTY_LIST_EXPORT)
	int alreadyHasBounty = 0;
	int currentBountyIndex = lastIndex;

	eassert(lastIndex < MAX_NUM_BOUNTIES);

	if (lastIndex > 0)
	{
		for (int i = lastIndex; i >= 0; i--)
		{
			if (bountyList[i] == functionName)
			{
				alreadyHasBounty = 1;
				currentBountyIndex = i;
				break;
			}
		}
	}

	if (alreadyHasBounty == 0)
	{
		bountyList[lastIndex++] = functionName;
	}

	bountyListCalls[currentBountyIndex]++;
#endif
}

void Emulator_SaveBountyList()
{
#if !defined(NO_BOUNTY_LIST_EXPORT)
	char fileName[128];
	sprintf(fileName, "VALKYRIE_RUNTIME_BOUNTY_LIST_%s.txt", SHORT_GAME_NAME);
	FILE* f = fopen(fileName, "wb+");

	eassert(lastIndex < 4096);

	if (f != NULL)
	{
		for (unsigned int i = 0; i < lastIndex; i++)
		{
			fprintf(f, "%s Calls: %d\n", bountyList[i], bountyListCalls[i]);
		}
		fclose(f);
	}
#endif
}