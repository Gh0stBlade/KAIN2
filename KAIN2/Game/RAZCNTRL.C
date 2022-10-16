#include "CORE.H"
#include "RAZCNTRL.H"

long RazielCommands[10];
int Pending;
int Up;
int Down;

void ProcessRazControl(long* command)
{
#if defined(PSX_VERSION)

	if ((command[0] & RazielCommands[2]))
	{
		Up = 0;
	
		Down++;
	}
	else
	{
		Up++;

		if (Down != 0)
		{
			Pending = Down;
		}
		
		Down = 0;
	}

	if (Down >= 6)
	{
		Pending = Down;
	}

	if (Up >= 3)
	{
		Pending = 0;
	}

#elif defined(PC_VERSION)
	int v1; // eax

	if ((dword_4FAD98 & *command) != 0)
	{
		dword_C55180 = 0;
		v1 = ++dword_C55184;
	}
	else
	{
		++dword_C55180;
		if (dword_C55184)
			dword_C5517C = dword_C55184;
		v1 = 0;
		dword_C55184 = 0;
	}
	if (v1 > 5)
		dword_C5517C = v1;
	if (dword_C55180 > 2)
		dword_C5517C = 0;
#endif
}
