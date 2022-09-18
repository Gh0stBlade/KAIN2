#include "EMULATOR_INPUT_NVN.H"

#if defined(PLATFORM_NX)

int g_initialisedInputNVN = FALSE;

void Emulator_InitialiseInputNVN()
{
    nn::hid::InitializeTouchScreen();
}

void Emulator_UpdateInputNVN()
{
    if (g_initialisedInputNVN == FALSE)
    {
        Emulator_InitialiseInputNVN();

        g_initialisedInputNVN = TRUE;
    }
    
    nn::hid::TouchScreenState<1> touchScreenState;
    nn::hid::GetTouchScreenStates(&touchScreenState, 1);
    nn::hid::TouchState touchState = touchScreenState.touches[0];

    extern void Emulator_HandleTouchEvent(int x, int y);
    Emulator_HandleTouchEvent(touchState.x, touchState.y);
}



#endif