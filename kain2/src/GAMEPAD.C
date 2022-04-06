#include "CORE.H"
#include "GAMELOOP.H"
#include "GAMEPAD.H"
#include "LOCAL/LOCALSTR.H"
#include "FONT.H"
#include "MENU/MENU.H"

#if defined(PSXPC_VERSION)
#include "EMULATOR_PRIVATE.H"
#endif

long dualshock0_time; // offset 0x800d3000

long dualshock1_time; // offset 0x800d3004

short align_flag;

int dualShock;

short dualshock_onflag;

int ignoreFind;

unsigned short lastData[2];

unsigned short psxData[2];

long gDummyCommand[2][2]; // offset 0x800D0D60

struct ControllerPacket readGPBuffer1; // offset 0x800D0CF4

struct ControllerPacket readGPBuffer2; // offset 0x800D0D3C

struct ControllerPacket gpbuffer1; // offset 0x800D0CD0

struct ControllerPacket gpbuffer2; // offset 0x800D0D18

int gamePadControllerOut;

unsigned char controllerType[2];

void GAMEPAD_Commands(long (*command)[5], long (*data)[5], long pad)
{
#if defined(PSX_VERSION)
	long analogX;
	long analogY;
	static short lastPad[2];

	command[pad][2] = command[pad][0] & ~data[pad][0];
	
	analogX = data[pad][3];
	analogY = data[pad][4];

	if (!(data[pad][0] & 0x1))
	{
		if ((data[pad][0] & 0x2))
		{
			analogY = 128;
		}
	}
	else
	{
		analogY = -128;
	}

	if (!(data[pad][0] & 0x4))
	{
		if ((data[pad][0] & 0x8))
		{
			analogX = 128;
		}
	}
	else
	{
		analogX = -128;
	}

	if (analogY < -55)
	{
		data[pad][0] |= 0x1;
	}
	else if(analogY > 55)
	{
		data[pad][0] |= 0x2;
	}
	
	if (analogX < -55)
	{
		data[pad][0] |= 0x4;
	}
	else if (analogX > 55)
	{
		data[pad][0] |= 0x8;
	}

	data[pad][1] = ~lastPad[pad] & data[pad][0];
	lastPad[pad] = data[pad][0];

	command[pad][3] = analogX;
	command[pad][4] = analogY;

	if ((gameTrackerX.gameFlags & 0x10))
	{
		command[pad][1] = ~command[pad][0] & gameTrackerX.overrideData[pad][0];
		command[pad][0] = gameTrackerX.overrideData[pad][0];
		
		data[pad][3] = gameTrackerX.overrideData[pad][3];
		data[pad][4] = gameTrackerX.overrideData[pad][4];

		if (!(gameTrackerX.overrideData[pad][0] & 0x1))
		{
			if ((gameTrackerX.overrideData[pad][0] & 0x2))
			{
				data[pad][4] = 128;
			}
		}
		else
		{
			data[pad][4] = -128;
		}
		
		if (!(gameTrackerX.overrideData[pad][0] & 0x4))
		{
			if ((gameTrackerX.overrideData[pad][0] & 0x8))
			{
				data[pad][3] = 128;
			}
		}
		else
		{
			data[pad][3] = -128;
		}
	}
	
	if ((gameTrackerX.gameFlags & 0x1))
	{
		memset(gameTrackerX.controlCommand, 0, sizeof(gameTrackerX.controlCommand));
		memset(gameTrackerX.controlData, 0, sizeof(gameTrackerX.controlData));
	}
	else
	{
		command[pad][1] = ~command[pad][0] & data[pad][0];
		command[pad][0] = data[pad][0];

		if (!(data[pad][0] & 0x1))
		{
			if ((data[pad][0] & 0x2))
			{
				analogY = 128;
			}
		}
		else
		{
			analogY = -128;
		}

		if (!(data[pad][0] & 0x4))
		{
			if ((data[pad][0] & 0x8))
			{
				analogX = 128;
			}
		}
		else
		{
			analogX = -128;
		}

		if (analogX != 0 || analogY != 0)
		{
			data[pad][3] = analogX;
			data[pad][4] = analogY;
		}
		
		if (!(gameTrackerX.debugFlags & 0x40000))
		{
			if ((gameTrackerX.debugFlags2 & 0x2000000))
			{
				if ((data[pad][0] & 0x300) == 0x300)
				{
					command[pad][0] &= 0xFFFFFBFF;
					command[pad][0] &= 0xFFFFF7FF;
					command[pad][0] |= 0x40000000;

					if ((command[pad][1] & 0xC00))
					{
						command[pad][1] |= 0x40000000;
					}
	
					command[pad][1] &= 0xFFFFFBFF;
					command[pad][1] &= 0xFFFFF7FF;
				}
			}
		}
	}

#elif defined(PC_VERSION)
	unsigned int v4; // eax
	int v5; // edx
	int v6; // ebp
	int v7; // ebx
	int v8; // edx
	int v9; // edx
	int* v10; // edi
	int v11; // edx
	int v12; // edx
	int v13; // ebx
	int v14; // ebp
	int v15; // edx
	int v16; // ecx
	int v17; // ecx
	int v18; // eax
	int pada; // [esp+1Ch] [ebp+Ch]

	v4 = 20 * pad;
	(*command)[5 * pad + 2] = (*command)[5 * pad] & ~(*data)[5 * pad];
	v5 = (*data)[5 * pad];
	v6 = (*data)[5 * pad + 3];
	v7 = (*data)[5 * pad + 4];
	pada = v6;
	if ((v5 & 1) != 0)
	{
		v7 = -128;
	}
	else if ((v5 & 2) != 0)
	{
		v7 = 128;
	}
	if ((v5 & 4) != 0)
	{
		v6 = -128;
	}
	else
	{
		if ((v5 & 8) == 0)
			goto LABEL_10;
		v6 = 128;
	}
	pada = v6;
LABEL_10:
	if (v7 >= -55)
	{
		if (v7 <= 55)
			goto LABEL_15;
		v8 = v5 | 2;
	}
	else
	{
		v8 = v5 | 1;
	}
	(*data)[v4 / 4] = v8;
LABEL_15:
	if (v6 >= -55)
	{
		if (v6 <= 55)
			goto LABEL_20;
		v9 = (*data)[v4 / 4] | 8;
	}
	else
	{
		v9 = (*data)[v4 / 4] | 4;
	}
	(*data)[v4 / 4] = v9;
LABEL_20:
	(*data)[v4 / 4 + 1] = ~word_C54FA8[pad] & (*data)[v4 / 4];
	word_C54FA8[pad] = (*data)[v4 / 4];
	(*command)[v4 / 4 + 3] = pada;
	(*command)[v4 / 4 + 4] = v7;
	if ((gameTrackerX.gameFlags & 0x10) != 0)
	{
		v10 = &(*command)[v4 / 4 + 1];
		*v10 = gameTrackerX.overrideData[v4 / 0x14][0] & ~(*command)[v4 / 4];
		(*command)[v4 / 4] = gameTrackerX.overrideData[v4 / 0x14][0];
		(*data)[v4 / 4 + 3] = gameTrackerX.overrideData[v4 / 0x14][3];
		(*data)[v4 / 4 + 4] = gameTrackerX.overrideData[v4 / 0x14][4];
		v11 = gameTrackerX.overrideData[v4 / 0x14][0];
		if ((v11 & 1) != 0)
		{
			(*data)[v4 / 4 + 4] = -128;
		}
		else if ((v11 & 2) != 0)
		{
			(*data)[v4 / 4 + 4] = 128;
		}
		v12 = gameTrackerX.overrideData[v4 / 0x14][0];
		if ((v12 & 4) != 0)
		{
			(*data)[v4 / 4 + 3] = -128;
		}
		else if ((v12 & 8) != 0)
		{
			(*data)[v4 / 4 + 3] = 128;
		}
	}
	else
	{
		if ((gameTrackerX.gameFlags & 1) != 0)
		{
			memset(gameTrackerX.controlCommand, 0, sizeof(gameTrackerX.controlCommand));
			memset(gameTrackerX.controlData, 0, sizeof(gameTrackerX.controlData));
			return;
		}
		v10 = &(*command)[v4 / 4 + 1];
		v13 = 0;
		v14 = 0;
		*v10 = (*data)[v4 / 4] & ~(*command)[v4 / 4];
		(*command)[v4 / 4] = (*data)[v4 / 4];
		v15 = (*data)[v4 / 4];
		if ((v15 & 1) != 0)
		{
			v14 = -128;
		}
		else if ((v15 & 2) != 0)
		{
			v14 = 128;
		}
		if ((v15 & 4) != 0)
		{
			v13 = -128;
		}
		else if ((v15 & 8) != 0)
		{
			v13 = 128;
		}
		if (v13 || v14)
		{
			(*data)[v4 / 4 + 3] = v13;
			(*data)[v4 / 4 + 4] = v14;
		}
	}
	if ((gameTrackerX.debugFlags & 0x40000) == 0 && (gameTrackerX.debugFlags2 & 0x2000000) != 0)
	{
		v16 = (*data)[v4 / 4];
		if ((v16 & 0x100) != 0 && (v16 & 0x200) != 0)
		{
			v17 = (*command)[v4 / 4];
			v17 &= ~0xC00;
			(*command)[v4 / 4] = v17 | 0x40000000;
			if ((*v10 & 0xC00) != 0)
				*v10 |= 0x40000000u;
			v18 = *v10;
			v18 = *v10 & ~0xC00;
			*v10 = v18;
		}
	}
#endif
}

int GAMEPAD_ControllerIsDualShock()
{
	return dualShock;
}

int GAMEPAD_DualShockEnabled()
{ 
	return dualshock_onflag;
}


// autogenerated function stub: 
// void /*$ra*/ GAMEPAD_DisableDualShock()
void GAMEPAD_DisableDualShock()
{ // line 268, offset 0x8003133c
	/* begin block 1 */
		// Start line: 552
	/* end block 1 */
	// End Line: 553

}

void GAMEPAD_EnableDualShock()
{
	dualshock_onflag = 1;
	align_flag = 0;
}


// autogenerated function stub: 
// void /*$ra*/ GAMEPAD_HandleDualShock()
void GAMEPAD_HandleDualShock()
{ // line 285, offset 0x80031390
	/* begin block 1 */
		// Start line: 286
		// Start offset: 0x80031390
		// Variables:
			int decrement_amount; // $s3

		/* begin block 1.1 */
			// Start line: 295
			// Start offset: 0x800313D4
			// Variables:
				int timeout; // $s0
		/* end block 1.1 */
		// End offset: 0x80031410
		// End Line: 302
	/* end block 1 */
	// End offset: 0x8003146C
	// End Line: 327

	/* begin block 2 */
		// Start line: 591
	/* end block 2 */
	// End Line: 592

}


// autogenerated function stub: 
// void /*$ra*/ GAMEPAD_Shock(int motor0_speed /*$a0*/, int motor0_time /*$a1*/, int motor1_speed /*$a2*/, int motor1_time /*$a3*/)
void GAMEPAD_Shock(int motor0_speed, int motor0_time, int motor1_speed, int motor1_time)
{ // line 333, offset 0x80031488
	/* begin block 1 */
		// Start line: 697
	/* end block 1 */
	// End Line: 698

	/* begin block 2 */
		// Start line: 699
	/* end block 2 */
	// End Line: 700

}


// autogenerated function stub: 
// void /*$ra*/ GAMEPAD_Shock0(int motor0_speed /*$a0*/, int motor0_time /*$a1*/)
void GAMEPAD_Shock0(int motor0_speed, int motor0_time)
{ // line 349, offset 0x800314cc
	/* begin block 1 */
		// Start line: 736
	/* end block 1 */
	// End Line: 737

	/* begin block 2 */
		// Start line: 738
	/* end block 2 */
	// End Line: 739

}


// autogenerated function stub: 
// void /*$ra*/ GAMEPAD_Shock1(int motor1_speed /*$a0*/, int motor1_time /*$a1*/)
void GAMEPAD_Shock1(int motor1_speed, int motor1_time)
{ // line 363, offset 0x80031508
	/* begin block 1 */
		// Start line: 766
	/* end block 1 */
	// End Line: 767

	/* begin block 2 */
		// Start line: 768
	/* end block 2 */
	// End Line: 769

}

void GAMEPAD_Detect()
{ 
	int padState;
	int count;
	int count1;
	
	count1 = 0;
	
	dualshock0_time = 0;
	dualshock1_time = 0;

	align_flag = 0;
	dualshock_onflag = 0;
	ignoreFind = 0;
	dualShock = 0;

	do
	{
		count = 0;

		do
		{
			VSync(0);

			padState = PadGetState(0);

			if (padState != PadStateDiscon)
			{
				break;
			}

		} while (++count < 5);

		if (padState == PadStateReqInfo)
		{
			dualShock = 1;
		}

		count1++;

		if (count1 >= 31)
		{
			ignoreFind = 1;
			break;
		}

	} while (padState == PadStateFindPad || padState == PadStateReqInfo || padState == PadStateExecCmd);

	count = 0;
	
	if (dualShock != 0)
	{
		do
		{
			VSync(0);

			if (PadGetState(0) == PadStateStable)
			{
				break;
			}

		} while (++count < 31);
	}

	VSync(3);
}


void GAMEPAD_Init()
{
#if defined(PSX_VERSION)
	PadInitDirect(&readGPBuffer1.transStatus, &readGPBuffer2.transStatus);
	PadStartCom();
	GAMEPAD_Detect();

	memset(&gDummyCommand, 0, sizeof(gDummyCommand));
	memset(&readGPBuffer1, 0, sizeof(readGPBuffer1));
	memset(&readGPBuffer2, 0, sizeof(readGPBuffer2));
	
	readGPBuffer1.data.pad = -1;
	readGPBuffer1.transStatus = 0;

	readGPBuffer2.data.pad = -1;
	readGPBuffer2.transStatus = 0;

	gpbuffer1.data.pad = -1;
	gpbuffer1.transStatus = 0;

	gpbuffer2.data.pad = -1;
	gpbuffer2.transStatus = 0;
#else
	InitPAD((char*)&readGPBuffer1, 34, (char*)&readGPBuffer2, 34);
	StartPAD();
	CTRLCNFG_InitValues();
	gDummyCommand = 0;
	dword_C60FD4 = 0;
	dword_C60FD8 = 0;
	dword_C60FDC = 0;
	memset(&readGPBuffer1, 0, 0x20u);
	*(_WORD*)&readGPBuffer1.data.tap.ctrllers[3].data.negcon.buttonII = 0;
	memset(&readGPBuffer2, 0, 0x20u);
	*(_WORD*)&readGPBuffer2.data.tap.ctrllers[3].data.negcon.buttonII = 0;
	readGPBuffer1.data.pad = -1;
	readGPBuffer1.transStatus = 0;
	readGPBuffer2.data.pad = -1;
	readGPBuffer2.transStatus = 0;
	gpbuffer1.data.pad = -1;
	gpbuffer1.transStatus = 0;
	word_C60FA2 = -1;
	gpbuffer2 = 0;
#endif
}

#if defined(PC_VERSION)
struct JoypadMappingStruct joymap_tbl0[16] =
{
	0x10, 0x1,
	0x40, 0x2,
	0x80, 0x4,
	0x20, 0x8,
	0x8, 0x4000,
	0x1000, 0x10,
	0x4000, 0x80,
	0x8000, 0x20,
	0x2000, 0x40,
	0x400, 0x100,
	0x100, 0x400,
	0x800, 0x200,
	0x200, 0x800,
	0x2, 0x1000,
	0x4, 0x2000,
	0x1, 0x8000,
};
struct JoypadMappingStruct joymap_tbl1[16] =
{
	0x10, 0x1,
	0x40, 0x2,
	0x80, 0x4,
	0x20, 0x8,
	0x8, 0x4000,
	0x1000, 0x10,
	0x4000, 0x80,
	0x8000, 0x20,
	0x2000, 0x40,
	0x400, 0x100,
	0x100, 0x400,
	0x800, 0x200,
	0x200, 0x800,
	0x2, 0x1000,
	0x4, 0x2000,
	0x1, 0x8000
};
#endif

void PSXPAD_TranslateData(long *data, unsigned short padData, unsigned short lastData)
{
#if defined(PSX_VERSION)
	int i;
	struct JoypadMappingStruct table[16] = { 0x0010, 0x0001,
											 0x0040, 0x0002,
											 0x0080, 0x0004,
											 0x0020, 0x0008,
											 0x1000, 0x0010,
											 0x4000, 0x0080,
											 0x8000, 0x0020,
											 0x2000, 0x0040,
											 0x0400, 0x0100,
											 0x0100, 0x0400,
											 0x0800, 0x0200,
											 0x0200, 0x0800,
											 0x0002, 0x1000,
											 0x0004, 0x2000,
											 0x0008, 0x4000,
											 0x0001, 0x8000 };
	unsigned short padButton;
	unsigned short logicalButton;

	for (i = 0; i < 16; i++)
	{
		padButton = table[i].psxData;
		logicalButton = table[i].translation;

		if (!(padData & padButton))
		{
			data[0] |= logicalButton;
	
			if ((lastData & padButton))
			{
				data[1] |= logicalButton;
			}
		}
		else if (!(lastData & padButton))
		{
			data[2] |= logicalButton;
		}
	}

#elif defined(PC_VERSION)
	PAIR16 psxData; // eax
	struct JoypadMappingStruct* table; // esi
	int v5; // ebx
	int translation; // ecx

	if (MainTracker_IsGamePlaying() && (psxData.y = gApplyGamepadRemapping.y, *(DWORD*)&gApplyGamepadRemapping))
		table = joymap_tbl1;
	else
		table = joymap_tbl0;
	v5 = 16;
	do
	{
		psxData.x = table->psxData;
		if ((padData & table->psxData) != 0)
		{
			psxData = (PAIR16)(lastData & *(unsigned int*)&psxData);
			if (!psxData.x)
			{
				psxData = (PAIR16)(table->translation | data[2]);
				data[2] = (int)psxData;
			}
		}
		else
		{
			translation = table->translation;
			psxData = (PAIR16)(lastData & *(unsigned int*)&psxData);
			*data |= translation;
			if (psxData.x)
				data[1] |= translation;
		}
		++table;
		--v5;
	} while (v5);
#endif
}


// autogenerated function stub: 
// unsigned short /*$ra*/ GAMEPAD_RemapAnalogueButtons(unsigned short in /*$a0*/)
unsigned short GAMEPAD_RemapAnalogueButtons(unsigned short in)
{ // line 553, offset 0x800317f8
	/* begin block 1 */
		// Start line: 555
		// Start offset: 0x800317F8
	/* end block 1 */
	// End offset: 0x800317F8
	// End Line: 557

	/* begin block 2 */
		// Start line: 1228
	/* end block 2 */
	// End Line: 1229

	/* begin block 3 */
		// Start line: 1229
	/* end block 3 */
	// End Line: 1230

	/* begin block 4 */
		// Start line: 1231
	/* end block 4 */
	// End Line: 1232

	return 0;
}

void GAMEPAD_DetectInit()
{
	int orgdualshock_onflag;

	orgdualshock_onflag = dualshock_onflag;

	GAMEPAD_Detect();

	if (dualShock != 0 && orgdualshock_onflag != 0)
	{
		GAMEPAD_EnableDualShock();
	}
}

void GAMEPAD_GetData(long(*data)[5])
{
#if defined(PSX_VERSION)
	long analogue_x;
	long analogue_y;
	int padState;

	data[0][2] = 0;
	data[0][1] = 0;
	data[0][0] = 0;
	data[1][2] = 0;
	data[1][1] = 0;
	data[1][0] = 0;

	psxData[0] = 0;
	psxData[1] = 0;

	data[0][3] = 0;
	data[0][4] = 0;
	data[1][3] = 0;
	data[1][4] = 0;

	if (ignoreFind != 0)
	{
		memcpy(&gpbuffer1, &readGPBuffer1, sizeof(gpbuffer1));
		memcpy(&gpbuffer2, &readGPBuffer2, sizeof(gpbuffer2));
	}
	else
	{
		padState = PadGetState(0);

		if (padState != PadStateFindPad)
		{
			if (padState != PadStateReqInfo)
			{
				if (padState != PadStateExecCmd)
				{
					GAMEPAD_HandleDualShock();

					memcpy(&gpbuffer1, &readGPBuffer1, sizeof(gpbuffer1));
					memcpy(&gpbuffer2, &readGPBuffer2, sizeof(gpbuffer2));
				}
			}

			if (padState == 1)
			{
				GAMEPAD_DetectInit();
			}
		}
		else
		{
			GAMEPAD_DetectInit();
		}
	}

	if (gpbuffer1.transStatus != 0xFF)
	{
		if (gamePadControllerOut >= 6)
		{
			GAMEPAD_DetectInit();
		}
		
		psxData[0] = gpbuffer1.data.pad;
		gamePadControllerOut = 0;

		if (controllerType[0] == 0x53)
		{
			psxData[0] = GAMEPAD_RemapAnalogueButtons(psxData[0]);
		}

		PSXPAD_TranslateData((long*)data, psxData[0], lastData[0]);

		controllerType[0] = gpbuffer1.dataFormat;
		lastData[0] = psxData[0];

		if ((gpbuffer1.dataFormat & 0xFF) == 115 || (gpbuffer1.dataFormat & 0xFF) == 83)
		{
			analogue_x = gpbuffer1.data.analogue.xL;
			analogue_y = gpbuffer1.data.analogue.yL;
			
			if (analogue_x - 74 >= 109 && analogue_y < 74 && analogue_x - 128 < 183)
			{
				analogue_x = 128;
				analogue_y = 128;
			}
	
			data[0][3] = analogue_x - 128;
			data[0][4] = analogue_y - 128;
		}
	}
	else
	{
		gamePadControllerOut++;
	}
#elif defined(PC_VERSION)
	bool v2; // zf
	int v3; // eax
	unsigned __int16 v4; // si
	unsigned __int16 v5; // di
	JoypadMappingStruct* v6; // edx
	ushort psxData; // cx
	int translation; // eax
	int left_x; // eax
	int left_y; // ecx
	int dataa; // [esp+14h] [ebp+4h]

	(*data)[2] = 0;
	(*data)[1] = 0;
	(*data)[0] = 0;
	(*data)[7] = 0;
	(*data)[6] = 0;
	(*data)[5] = 0;
	::psxData = 0;
	(*data)[3] = 0;
	(*data)[4] = 0;
	(*data)[8] = 0;
	(*data)[9] = 0;
	memcpy(&gpbuffer1, &readGPBuffer1, sizeof(gpbuffer1));
	v2 = readGPBuffer1.transStatus == 0xFF;
	memcpy(&gpbuffer2, &readGPBuffer2, 0x20u);
	v3 = gamePadControllerOut;
	*((_WORD*)&gpbuffer2 + 16) = *(_WORD*)&readGPBuffer2.data.tap.ctrllers[3].data.negcon.buttonII;
	if (v2)
	{
		gamePadControllerOut = v3 + 1;
	}
	else
	{
		if (v3 > 5 && dword_C55018 && word_C55014)
		{
			word_C55014 = 1;
			word_C55010 = 0;
		}
		gamePadControllerOut = 0;
		LOWORD(::psxData) = readGPBuffer1.data.pad;
		if (controllerType == 83)
		{
			LOWORD((*data)[0]) = readGPBuffer1.data.pad;
			LOWORD(::psxData) = ~(~readGPBuffer1.data.pad & 0x61F9 | ((unsigned __int16)(~readGPBuffer1.data.pad & 0x1000 | (~(unsigned int)data >> 5) & 0x400) >> 1) | (2 * (~readGPBuffer1.data.pad & 0xA00 | (16 * (~readGPBuffer1.data.pad & 0xFC00)))));
		}
		v4 = lastData;
		v5 = ::psxData;
		if (MainTracker_IsGamePlaying() && *(DWORD*)&gApplyGamepadRemapping)
			v6 = joymap_tbl1;
		else
			v6 = joymap_tbl0;
		dataa = 16;
		do
		{
			psxData = v6->psxData;
			if ((v6->psxData & v5) != 0)
			{
				if ((psxData & v4) == 0)
					(*data)[2] |= v6->translation;
			}
			else
			{
				translation = v6->translation;
				(*data)[0] |= translation;
				if ((psxData & v4) != 0)
					(*data)[1] |= translation;
			}
			++v6;
			--dataa;
		} while (dataa);
		LOWORD(lastData) = ::psxData;
		controllerType = gpbuffer1.dataFormat;
		if (gpbuffer1.dataFormat == 115 || gpbuffer1.dataFormat == 83)
		{
			left_x = gpbuffer1.data.joystick.left_x;
			left_y = gpbuffer1.data.joystick.left_y;
			if (gpbuffer1.data.joystick.left_x > 73u
				&& gpbuffer1.data.joystick.left_x < 183u
				&& gpbuffer1.data.joystick.left_y > 73u
				&& gpbuffer1.data.joystick.left_y < 183u)
			{
				left_x = 128;
				left_y = 128;
			}
			(*data)[3] = left_x - 128;
			(*data)[4] = left_y - 128;
		}
	}

#endif
}

void GAMEPAD_DisplayControllerStatus(int msgY)
{
	char *noCtrlStr;

	if (gamePadControllerOut >= 6)
	{
		noCtrlStr = localstr_get(LOCALSTR_no_controller);
		FONT_FontPrintCentered(noCtrlStr, msgY);
		DisplayHintBox(FONT_GetStringWidth(noCtrlStr), msgY);
	}
}

void GAMEPAD_Process(struct GameTracker *gameTracker)
{ 
#if defined(PSXPC_VERSION)
	Emulator_UpdateInput();
#endif
	GAMEPAD_GetData(gameTracker->controlData);
	GAMEPAD_Commands(gameTracker->controlCommand, gameTracker->controlData, 0);
	GAMEPAD_Commands(gameTracker->controlCommand, gameTracker->controlData, 1);
}


// autogenerated function stub: 
// void /*$ra*/ GAMEPAD_SaveControllers()
void GAMEPAD_SaveControllers()
{ // line 797, offset 0x80031d80
	/* begin block 1 */
		// Start line: 1741
	/* end block 1 */
	// End Line: 1742

	/* begin block 2 */
		// Start line: 1742
	/* end block 2 */
	// End Line: 1743

}


// autogenerated function stub: 
// void /*$ra*/ GAMEPAD_RestoreControllers()
void GAMEPAD_RestoreControllers()
{ // line 804, offset 0x80031e1c
	/* begin block 1 */
		// Start line: 1755
	/* end block 1 */
	// End Line: 1756

	/* begin block 2 */
		// Start line: 1756
	/* end block 2 */
	// End Line: 1757

}




