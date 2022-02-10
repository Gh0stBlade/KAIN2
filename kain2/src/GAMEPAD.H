#ifndef GAMEPAD_H
#define GAMEPAD_H


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


#endif
