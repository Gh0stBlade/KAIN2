#include "MIDI.H"

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		MIDI_Open(argv[1]);
	}
	
	return 0;
}