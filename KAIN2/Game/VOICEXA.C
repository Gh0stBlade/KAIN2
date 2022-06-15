#include "CORE.H"
#include "VOICEXA.H"
#include "GAMELOOP.H"
#include "PSX/AADLIB.H"

struct XAVoiceTracker voiceTracker; // offset 0x800D5AD4
struct XAVoiceListEntry* voiceList; // offset 0x800CF99C
enum language_t the_language; // offset 0x800D5BFC

typedef void (*voiceCmdFunc)(struct XAVoiceTracker* vt, short cmdParam);

voiceCmdFunc voiceCmdTbl[5] =
{
	voiceCmdPlay,
	voiceCmdStop,
	voiceCmdPause,
	voiceCmdResume,
	voiceCmdNull
};

void VOICEXA_Init()
{
	int i;
	CdlFILE fp;
	struct XAVoiceTracker* vt;
	char fileName[32];

	vt = &voiceTracker;

	if ((gameTrackerX.debugFlags & 0x80000))
	{
		vt->voiceStatus = 0;
		vt->cdStatus = 0;
		vt->reqIn = 0;
		vt->reqOut = 0;
		vt->reqsQueued = 0;
		vt->cdCmdIn = 0;
		vt->cdCmdOut = 0;
		vt->cdCmdsQueued = 0;
		vt->voiceCmdIn = 0;
		vt->voiceCmdOut = 0;
		vt->voiceCmdsQueued = 0;

		for (i = 0; i < 30; i++)
		{
			sprintf(&fileName[0], "\\VOICE\\VOICE%02d.XA;", i);
			
			if (CdSearchFile(&fp, &fileName[0]) == 0)
			{
				vt->xaFileInfo[i].startPos = 0;
			}
			else
			{
				vt->xaFileInfo[i].startPos = CdPosToInt(&fp.pos);
			}
		}
	}
}

void putCdCommand(struct XAVoiceTracker *vt, unsigned char cdCommand, int numParams, unsigned char *params)
{
	int i;

	vt->cdCmdQueue[vt->cdCmdIn].cdCommand = cdCommand;

	for (i = 0; i < numParams; i++)
	{
		vt->cdCmdQueue[vt->cdCmdIn + i].cdCmdParam[0] = params[i];
	}

	if (vt->cdCmdsQueued < 7)
	{
		vt->cdCmdsQueued++;
		vt->cdCmdIn++;

		if (vt->cdCmdIn == 8)
		{
			vt->cdCmdIn = 0;
		}
	}
}

void VOICEXA_CdSyncCallback(unsigned char status, unsigned char* result)
{
	struct XAVoiceTracker* vt;
	
	vt = &voiceTracker;

	if (status == 2)
	{
		vt->cdStatus = 0;
		
		if (++vt->cdCmdOut == 8)
		{
			vt->cdCmdOut = 0;
		}

		vt->cdCmdsQueued--;

		CdSyncCallback(vt->prevCallback);
	}
	else
	{
		vt->cdStatus = 2;
	}
}

void processCdCommands(struct XAVoiceTracker* vt)
{ 
	struct CdCommand* cmd;

	if (vt->cdStatus == 2)
	{
		vt->cdStatus = 1;
		
		cmd = &vt->cdCmdQueue[vt->cdCmdOut];
		
		CdControl(cmd->cdCommand, cmd->cdCmdParam, vt->cdResult);
	}
	else
	{
		if (vt->cdCmdsQueued != 0 && vt->cdStatus != 1)
		{
			vt->cdStatus = 1;
		
			cmd = &vt->cdCmdQueue[vt->cdCmdOut];
			
			vt->prevCallback = CdSyncCallback(VOICEXA_CdSyncCallback);

			CdControl(cmd->cdCommand, cmd->cdCmdParam, vt->cdResult);
		}
	}
}

void putVoiceCommand(struct XAVoiceTracker *vt, unsigned char voiceCmd, unsigned char nextVoiceStatus, int voiceCmdParam)
{ 
	vt->voiceCmdQueue[vt->voiceCmdIn].voiceCmd = voiceCmd;
	vt->voiceCmdQueue[vt->voiceCmdIn].nextVoiceStatus = nextVoiceStatus;
	vt->voiceCmdQueue[vt->voiceCmdIn].voiceCmdParam = voiceCmdParam;

	if (vt->voiceCmdsQueued < 15)
	{
		vt->voiceCmdsQueued++;
		vt->voiceCmdIn++;

		if (vt->voiceCmdIn >= 16)
		{
			vt->voiceCmdIn = 0;
		}
	}
}

void processVoiceCommands(struct XAVoiceTracker *vt)
{
	struct VoiceCommand *cmd;
	
	if (vt->voiceCmdsQueued != 0)
	{
		vt->voiceCmdsQueued--;

		cmd = &vt->voiceCmdQueue[vt->voiceCmdOut++];

		if (vt->voiceCmdOut == 16)
		{
			vt->voiceCmdOut = 0;
		}

		if (cmd->voiceCmd < 5)
		{
			voiceCmdTbl[cmd->voiceCmd](vt, cmd->voiceCmdParam);

			vt->voiceStatus = cmd->nextVoiceStatus;
		}
	}
}

void voiceCmdPlay(struct XAVoiceTracker *vt, short voiceIndex)
{ 
	CdlFILTER filter;
	CdlLOC pos;
	unsigned char mode;
	SpuCommonAttr spuattr;
	struct XAVoiceListEntry* voice;
	struct XAFileInfo* file;

	if (voiceList != NULL)
	{
		voice = &voiceList[voiceIndex];

		vt->fileNum = voiceIndex >> 4;

		file = &vt->xaFileInfo[voiceIndex >> 2];

		putCdCommand(vt, 9, 0, NULL);

		filter.file = 1;
		filter.chan = voiceIndex & 0xF;

		putCdCommand(vt, 13, 4, (unsigned char*)&filter);

		mode = 200;

		putCdCommand(vt, 14, 1, &mode);

		CdIntToPos(file->startPos, &vt->currentPos);

		vt->endSector = file->startPos + (voice->length - 150);

		CdIntToPos(file->startPos, &pos);

		putCdCommand(vt, 27, 4, (unsigned char*)&pos);

		spuattr.mask = 0x3FCF;
		spuattr.mvol.left = 0x3FFF;
		spuattr.mvol.right = 0x3FFF;
		spuattr.mvolmode.left = 0;
		spuattr.mvolmode.right = 0;
		spuattr.cd.reverb = 0;
		spuattr.cd.mix = 1;
		spuattr.ext.volume.left = 0x7FFF;
		spuattr.ext.volume.right = 0x7FFF;
		spuattr.ext.reverb = 0;
		spuattr.ext.mix = 1;
		spuattr.cd.volume.left = gameTrackerX.sound.gVoiceVol << 8;
		spuattr.cd.volume.right = gameTrackerX.sound.gVoiceVol << 8;

		SpuSetCommonAttr(&spuattr);

		if (gameTrackerX.sound.gMusicVol >= 60)
		{
			aadStartMusicMasterVolFade(60, -1, NULL);
		}
	}
}

void voiceCmdStop(struct XAVoiceTracker *vt, short cmdParam)
{
	SpuCommonAttr spuattr;

	if (vt->voiceStatus != 0)
	{
		putCdCommand(vt, 9, 0, NULL);
		
		spuattr.mask = 0x2200;
		spuattr.cd.mix = 0;
		spuattr.ext.mix = 0;
		
		SpuSetCommonAttr(&spuattr);

		aadStartMusicMasterVolFade(gameTrackerX.sound.gMusicVol, 1, NULL);
	}
}

void voiceCmdPause(struct XAVoiceTracker *vt, short cmdParam)
{ 
	if ((unsigned int)(vt->voiceStatus - 1) < 2)
	{
		putCdCommand(vt, 9, 0, NULL);
	}
}

void voiceCmdResume(struct XAVoiceTracker *vt, short cmdParam)
{ 
	if (vt->voiceStatus == 3)
	{
		putCdCommand(vt, 27, 4, (unsigned char*)&vt->currentPos);
	}
}

void voiceCmdNull(struct XAVoiceTracker *vt, short cmdParam)
{
}

void VOICEXA_Play(int voiceIndex, int queueRequests)
{ 
	struct XAVoiceTracker* vt;
	struct XAFileInfo* file;

	vt = &voiceTracker;
	file = &vt->xaFileInfo[voiceIndex];

	if ((gameTrackerX.debugFlags & 0x80000))
	{
		if (file->startPos != 0)
		{
			if (gameTrackerX.sound.gVoiceOn != 0)
			{
				if (queueRequests != 0)
				{
					if (vt->reqsQueued < 3)
					{
						vt->reqsQueued++;
						vt->reqIn++;
						
						if ((vt->reqIn & 0xFF) == 4)
						{
							vt->reqIn = 0;
						}
					}
				}
				else
				{
					putVoiceCommand(vt, 0, 1, voiceIndex);
				}
			}
		}
	}
}

int VOICEXA_FinalStatus(struct XAVoiceTracker* vt)
{
	int tailIndex;

	if (vt->voiceCmdsQueued == 0)
	{
		return vt->voiceStatus;
	}
	
	if (vt->voiceCmdIn == 0)
	{
		tailIndex = 15;
		return vt->voiceCmdQueue[tailIndex].nextVoiceStatus;
	}
	else
	{
		tailIndex = vt->voiceCmdIn - 1;
		return vt->voiceCmdQueue[tailIndex].nextVoiceStatus;
	}
}

void VOICEXA_Pause()
{
	struct XAVoiceTracker* vt;
	int finalStatus;

	vt = &voiceTracker;
	
	finalStatus = VOICEXA_FinalStatus(vt);

	if ((gameTrackerX.debugFlags & 0x80000))
	{
		if ((unsigned int)(finalStatus - 1) < 2)
		{
			putVoiceCommand(vt, 2, 3, 0);
		}
		else if (finalStatus == 0)
		{
			putVoiceCommand(vt, 4, 4, 0);
		}
	}
}

void VOICEXA_Resume()
{
	struct XAVoiceTracker* vt;
	int finalStatus;

	vt = &voiceTracker;

	finalStatus = VOICEXA_FinalStatus(vt);

	if ((gameTrackerX.debugFlags & 0x80000))
	{
		if (finalStatus == 3)
		{
			putVoiceCommand(vt, 3, 1, 0);
		}
		else if (finalStatus == 4)
		{
			putVoiceCommand(vt, 4, 0, 0);
		}
	}
}

void VOICEXA_Tick()
{
	struct XAVoiceTracker* vt;
	vt = &voiceTracker;

	if ((gameTrackerX.debugFlags & 0x80000))
	{
		processVoiceCommands(vt);
		processCdCommands(vt);

		if (vt->cdCmdsQueued == 0 && vt->voiceCmdsQueued == 0)
		{
			if (vt->voiceStatus >= 3 && vt->voiceStatus != 0)
			{
				CdControlB(CdlGetlocL, NULL, &voiceTracker.cdResult[0]);

				if ((vt->cdResult[3] & 0x2))
				{
					vt->voiceStatus = 2;
					vt->currentPos.track = 0;

					voiceTracker.currentPos.minute = vt->cdResult[0];
					
					vt->currentPos.second = vt->cdResult[1];
					vt->currentPos.sector = vt->cdResult[2];
					vt->currentSector = CdPosToInt(&vt->currentPos) - 150;

					if (vt->currentSector >= vt->endSector - 8)
					{
						putVoiceCommand(vt, 1, 0, 0);
					}
				}
			}
			else if (vt->voiceStatus == 0 && vt->reqsQueued != 0)
			{
				putVoiceCommand(vt, 0, 1, vt->requestQueue[vt->reqOut]);
				
				vt->reqOut++;
				vt->reqsQueued--;

				if (vt->reqOut == 4)
				{
					vt->reqOut = 0;
				}
			}
		}
	}
}

int VOICEXA_IsPlaying()
{ 
	struct XAVoiceTracker *vt = &voiceTracker;

	if (vt->voiceStatus == 2)
	{
		return 2;
	}

	if (vt->voiceStatus == 1)
	{
		return 1;
	}

	if (vt->cdStatus != 0)
	{
		return 1;
	}

	return 0;
}


// autogenerated function stub: 
// int /*$ra*/ VOICEXA_IsPlayingOrPaused()
int VOICEXA_IsPlayingOrPaused()
{ // line 559, offset 0x800b7768
	/* begin block 1 */
		// Start line: 561
		// Start offset: 0x800B7768
	/* end block 1 */
	// End offset: 0x800B7768
	// End Line: 564

	/* begin block 2 */
		// Start line: 1221
	/* end block 2 */
	// End Line: 1222

	/* begin block 3 */
		// Start line: 1222
	/* end block 3 */
	// End Line: 1223

	/* begin block 4 */
		// Start line: 1225
	/* end block 4 */
	// End Line: 1226
	UNIMPLEMENTED();
	return 0;
}




