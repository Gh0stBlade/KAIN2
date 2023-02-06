#include "UPGRADE.H"

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		UPGRADE_OpenDRM(argv[1]);
	}
	
	return 0;
}