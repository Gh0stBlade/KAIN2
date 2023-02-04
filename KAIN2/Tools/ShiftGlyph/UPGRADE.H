#ifndef MIDI_H
#define MIDI_H

struct RedirectList 
{
	long numPointers;
	long* data;
};

extern void UPGRADE_OpenDRM(char* drmFilePath);

#endif