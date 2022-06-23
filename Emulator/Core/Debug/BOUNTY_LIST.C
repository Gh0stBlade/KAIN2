#include "BOUNTY_LIST.H"

#include <iostream>
#include <array>

std::array<std::string, 4096> bountyList;
std::array<unsigned int, 4096> bountyListCalls;
unsigned int lastIndex = 0;

void Emulator_AddBounty(const char* functionName)
{
	int alreadyHasBounty = 0;
	int currentBountyIndex = lastIndex;

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
}

void Emulator_SaveBountyList()
{
	FILE* f = fopen("BOUNTY_LIST.TXT", "wb+");

	if (f != NULL)
	{
		for (int i = 0; i < lastIndex; i++)
		{
			fprintf(f, "%s Calls: %d\n", bountyList[i].c_str(), bountyListCalls[i]);
		}
		fclose(f);
	}
}
