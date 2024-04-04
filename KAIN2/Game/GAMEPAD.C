#include "CORE.H"
#include "GAMELOOP.H"
#include "GAMEPAD.H"
#include "LOCAL/LOCALSTR.H"
#include "FONT.H"
#include "MENU/MENU.H"

unsigned char dualshock_align[6];

unsigned char dualshock_motors[2];

long dualshock0_time; // offset 0x800d3000

long dualshock1_time; // offset 0x800d3004

short align_flag;

int dualShock;

short dualshock_onflag;

int ignoreFind;

unsigned short lastData[2];

unsigned short psxData[2];

long* PadData;

long gDummyCommand[2][2]; // offset 0x800D0D60

struct ControllerPacket readGPBuffer1; // offset 0x800D0CF4

struct ControllerPacket readGPBuffer2; // offset 0x800D0D3C

struct ControllerPacket gpbuffer1; // offset 0x800D0CD0

struct ControllerPacket gpbuffer2; // offset 0x800D0D18

int gamePadControllerOut;

unsigned char controllerType[2];

void GAMEPAD_Commands(long(*command)[5], long(*data)[5], long pad)//Matching - 95.96%
{
	long analogX;
	long analogY;
	static short lastPad[2];

	command[pad][2] = command[pad][0] & ~data[pad][0];

	analogX = data[pad][3];
	analogY = data[pad][4];

	if ((data[pad][0] & 0x1))
	{
		analogY = -128;
	}
	else if ((data[pad][0] & 0x2))
	{
		analogY = 128;
	}

	if ((data[pad][0] & 0x4))
	{
		analogX = -128;
	}
	else if ((data[pad][0] & 0x8))
	{
		analogX = 128;
	}

	if (analogY < -55)
	{
		data[pad][0] |= 0x1;
	}
	else if (analogY > 55)
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
	lastPad[pad] = (short)data[pad][0];

	command[pad][3] = analogX;
	command[pad][4] = analogY;

	if ((gameTrackerX.gameFlags & 0x10))
	{
		command[pad][1] = ~command[pad][0] & gameTrackerX.overrideData[pad][0];
		command[pad][0] = gameTrackerX.overrideData[pad][0];

		data[pad][3] = gameTrackerX.overrideData[pad][3];
		data[pad][4] = gameTrackerX.overrideData[pad][4];

		if ((gameTrackerX.overrideData[pad][0] & 0x1))
		{
			data[pad][4] = -128;
		}
		else if ((gameTrackerX.overrideData[pad][0] & 0x2))
		{
			data[pad][4] = 128;
		}

		if ((gameTrackerX.overrideData[pad][0] & 0x4))
		{
			data[pad][3] = -128;

		}
		else if ((gameTrackerX.overrideData[pad][0] & 0x8))
		{
			data[pad][3] = 128;
		}
	}
	else
	{
		if ((gameTrackerX.gameFlags & 0x1))
		{
			memset(gameTrackerX.controlCommand, 0, sizeof(gameTrackerX.controlCommand));
			memset(gameTrackerX.controlData, 0, sizeof(gameTrackerX.controlData));

			return;
		}
		else
		{
			command[pad][1] = ~command[pad][0] & data[pad][0];
			command[pad][0] = data[pad][0];

			if ((data[pad][0] & 0x1))
			{
				analogY = -128;
			}
			else if ((data[pad][0] & 0x2))
			{
				analogY = 128;
			}

			if ((data[pad][0] & 0x4))
			{
				analogX = -128;
			}
			else if ((data[pad][0] & 0x8))
			{
				analogX = 128;
			}

			if (analogX != 0 || analogY != 0)
			{
				data[pad][3] = analogX;
				data[pad][4] = analogY;
			}
		}
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

int GAMEPAD_ControllerIsDualShock()
{
	return dualShock;
}

int GAMEPAD_DualShockEnabled()
{ 
	return dualshock_onflag;
}

void GAMEPAD_DisableDualShock()
{ 
	dualshock_onflag = 0;

	dualshock_motors[0] = 0;
	dualshock_motors[1] = 0;

	dualshock0_time = 0;
	dualshock1_time = 0;

	PadSetAct(0, dualshock_motors, 2);
}

void GAMEPAD_EnableDualShock()
{
	dualshock_onflag = 1;
	align_flag = 0;
}

void GAMEPAD_HandleDualShock()
{ 
	int decrement_amount;

	int timeout;
	
	decrement_amount = gameTrackerX.timeMult;

	if (PadInfoMode(0, 2, 0) != 0)
	{
		timeout = 0;

		if (align_flag == 0)
		{
			PadSetAct(timeout, dualshock_motors, 2);

			while (1)
			{
				if (PadSetActAlign(0, dualshock_align) != 0)
				{
					break;
				}

				if (timeout++ >= 99999)
					break;

				timeout++;
			}

			align_flag = 1;
		}
	}
	else
	{
		align_flag = 0;
	}

	if (dualshock0_time > 0)
	{
		dualshock0_time -= decrement_amount;

		if (dualshock0_time <= 0)
		{
			dualshock0_time = 0;
			dualshock_motors[0] = 0;
		}

	}

	if (dualshock1_time > 0)
	{
		dualshock1_time -= decrement_amount;

		if (dualshock1_time <= 0)
		{
			dualshock1_time = 0;
			dualshock_motors[1] = 0;
		}
	}
}

void GAMEPAD_Shock(int motor0_speed, int motor0_time, int motor1_speed, int motor1_time)
{
	if (dualshock_onflag != 0)
	{
		dualshock0_time = motor0_time;
		dualshock_motors[0] = motor0_speed;
		
		dualshock1_time = motor1_time;
		dualshock_motors[1] = motor1_speed;

		PadSetAct(0, dualshock_motors, sizeof(dualshock_motors));
	}
}

void GAMEPAD_Shock0(int motor0_speed, int motor0_time)
{
	if (dualshock_onflag != 0)
	{
		dualshock0_time = motor0_time;
		dualshock_motors[0] = motor0_speed;
		PadSetAct(0, dualshock_motors, sizeof(dualshock_motors));
	}
}

void GAMEPAD_Shock1(int motor1_speed, int motor1_time)
{ 
	if (dualshock_onflag != 0)
	{
		dualshock1_time = motor1_time;
		dualshock_motors[1] = motor1_speed;
		PadSetAct(0, dualshock_motors, sizeof(dualshock_motors));
	}
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
}

void PSXPAD_TranslateData(long *data, unsigned short padData, unsigned short lastData)
{
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
}

unsigned short GAMEPAD_RemapAnalogueButtons(unsigned short in)//Matching - 100%
{
	in = ~in;
	return ~(((in & 0x800) << 1) | ((in & 0x400) << 5) | ((in & 0x200) << 1) | ((in & 0x8000) >> 6) | ((in & 0x1000) >> 1) & 0xFFFF | (in & 0x61F9));
}

void GAMEPAD_DetectInit() // Matching - 100%
{
	int orgdualshock_onflag;

	orgdualshock_onflag = dualshock_onflag;

	GAMEPAD_Detect();

	if (dualShock != 0 && orgdualshock_onflag != 0)
	{
		GAMEPAD_EnableDualShock();
	}
}

void GAMEPAD_GetData(long(*data)[5])//Matching - 87.92%
{
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

			if ((unsigned)(analogue_x - 74) < 109 && analogue_y >= 74 && analogue_y < 183)
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
}

void GAMEPAD_DisplayControllerStatus(int msgY) // Matching - 100%
{
	char *noCtrlStr;

	if (gamePadControllerOut >= 6)
	{
		noCtrlStr = localstr_get(LOCALSTR_no_controller);
		FONT_FontPrintCentered(noCtrlStr, msgY);
		DisplayHintBox(FONT_GetStringWidth(noCtrlStr), msgY);
	}
}

void GAMEPAD_Process(struct GameTracker *gameTracker) // Matching - 100%
{ 
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
	UNIMPLEMENTED();
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
	UNIMPLEMENTED();
}




