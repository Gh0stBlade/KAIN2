#include "CORE.H"
#include "RAZCNTRL.H"

long RazielCommands[10] = { 0x8000, 0x40, 0x20, 0x80, 0x400, 0x800, 0x100, 0x200, 0xC00, 0x10 };
int Pending;
int Up;
int Down;

void ProcessRazControl(long* command) // Matching - 100%
{
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
}
