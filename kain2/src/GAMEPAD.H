#ifndef GAMEPAD_H
#define GAMEPAD_H

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

	typedef unsigned short PadData;

	typedef struct MouseData
	{
		unsigned short buttons;
		signed char xOffset;
		signed char yOffset;
	}
	MouseData;

	typedef struct NegconData
	{
		unsigned short digitalButtons;               /* Bit mask of plain keys. */
		char centralTwist;                           /* Analogue twisting thing.*/
		char buttonI;                                /* The I analogue button.  */
		char buttonII;                               /* The II analogue button. */
		char topLeft;                                /* The analogue tl button. */
	}
	NegconData;

	typedef struct AnalogjoyData
	{
		unsigned short digitalButtons;               /* Bit mask of plain keys. */
		unsigned char  right_x;
		unsigned char  right_y;
		unsigned char  left_x;
		unsigned char  left_y;
	} AnalogjoyData;

	typedef struct TapCtrllerData
	{
		unsigned char transStatus;  /* 0xff = no pad, bad pad, bad transmission */
		unsigned char dataFormat;            /* Top 4 bits = type of controller */

		union                                         /* Controller data union. */                   /* Controller data union. */
		{
			PadData    pad;                                 /* Plain pad.       */
			NegconData negcon;                              /* Namco controller.*/
		}
		data;
	}
	TapCtrllerData;

	typedef struct MultiTapData
	{
		TapCtrllerData ctrllers[4];               /* Just 4 controller packets. */
	}
	MultiTapData;

	typedef struct ControllerPacket
	{
		unsigned char transStatus;  /* 0xff = no pad, bad pad, bad transmission */
		unsigned char dataFormat;            /* Top 4 bits = type of controller */
										   /* Bottom 4 == shorts of data written */
		union                                         /* Controller data union. */
		{
			PadData       pad;                     /* Plain pad.       */
			MouseData     mouse;                   /* Mouse.           */
			NegconData    negcon;                  /* Namco controller.*/
			AnalogjoyData joystick;				/* Analog Joystick  */
			MultiTapData  tap;                     /* 4-way multi-tap. */
		}
		data;
	} ControllerPacket;

extern long gDummyCommand[2][2]; // offset 0x800D0D60

extern struct ControllerPacket readGPBuffer1; // offset 0x800D0CF4

extern struct ControllerPacket readGPBuffer2; // offset 0x800D0D3C

extern struct ControllerPacket gpbuffer1; // offset 0x800D0CD0

extern struct ControllerPacket gpbuffer2; // offset 0x800D0D18

extern void GAMEPAD_Commands(long (*command)[5], long (*data)[5], long pad); // 0x80030F68
extern int GAMEPAD_ControllerIsDualShock(); // 0x80031324
extern int GAMEPAD_DualShockEnabled(); // 0x80031330
extern void GAMEPAD_DisableDualShock(); // 0x8003133C
extern void GAMEPAD_EnableDualShock(); // 0x8003137C
extern void GAMEPAD_HandleDualShock(); // 0x80031390
extern void GAMEPAD_Shock(int motor0_speed, int motor0_time, int motor1_speed, int motor1_time); // 0x80031488
extern void GAMEPAD_Shock0(int motor0_speed, int motor0_time); // 0x800314CC
extern void GAMEPAD_Shock1(int motor1_speed, int motor1_time); // 0x80031508
extern void GAMEPAD_Detect(); // 0x80031544
extern void GAMEPAD_Init(); // 0x8003164C
extern void PSXPAD_TranslateData(long *data, unsigned short padData, unsigned short lastData); // 0x800316D4
extern unsigned short GAMEPAD_RemapAnalogueButtons(unsigned short in); // 0x800317F8
extern void GAMEPAD_DetectInit(); // 0x80031844
extern void GAMEPAD_GetData(long (*data)[5]); // 0x8003188C
extern void GAMEPAD_DisplayControllerStatus(int msgY); // 0x80031CC8
extern void GAMEPAD_Process(struct GameTracker *gameTracker); // 0x80031D28
extern void GAMEPAD_SaveControllers(); // 0x80031D80
extern void GAMEPAD_RestoreControllers(); // 0x80031E1C

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
