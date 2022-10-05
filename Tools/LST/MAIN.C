#include "BIGFILE.H"

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		BIGFILE_Open(argv[1]);
	}
	
	return 0;
}