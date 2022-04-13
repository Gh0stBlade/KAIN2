#include "EMULATOR_INPUT.H"

#include "EMULATOR.H"

SDL_GameController* padHandle[MAX_CONTROLLERS];
unsigned char* padRumbleData[MAX_CONTROLLERS];
unsigned char* padData[MAX_CONTROLLERS];
const unsigned char* keyboardState;

SDL_GameController* padHandleDebug[MAX_CONTROLLERS];
unsigned char* padDataDebug[MAX_CONTROLLERS];
const unsigned char* keyboardStateDebug;

int g_initialisedPadSubsystem = FALSE;

void Emulator_InitialiseSDLInput(SDL_GameController** pad, const unsigned char** kbState, int isDebugInput)
{
	if (isDebugInput == TRUE && g_initialisedPadSubsystem == TRUE)
	{
		return;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		eprinterr("Failed to initialise subsystem GAMECONTROLLER\n");
	}

	if (SDL_NumJoysticks() < 1)
	{
		eprinterr("Failed to locate a connected gamepad!\n");
	}
	else
	{
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (SDL_IsGameController(i) && i < MAX_CONTROLLERS)
			{
				pad[i] = SDL_GameControllerOpen(i);///@TODO close joysticks
			}
		}
	}

	kbState[0] = SDL_GetKeyboardState(NULL);
}

#if defined(SDL2)
unsigned short UpdateGameControllerInput(SDL_GameController* pad)
{
	unsigned short ret = 0xFFFF;


	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_X))//Square
	{
		ret &= ~0x8000;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_B))//Circle
	{
		ret &= ~0x2000;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_Y))//Triangle
	{
		ret &= ~0x1000;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_A))//Cross
	{
		ret &= ~0x4000;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))//L1
	{
		ret &= ~0x400;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))//R1
	{
		ret &= ~0x800;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_UP))//UP
	{
		ret &= ~0x10;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN))//DOWN
	{
		ret &= ~0x40;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT))//LEFT
	{
		ret &= ~0x80;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))//RIGHT
	{
		ret &= ~0x20;
	}

	if (SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT))//L2
	{
		ret &= ~0x100;
	}

	if (SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT))//R2
	{
		ret &= ~0x200;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSTICK))//L3
	{
		ret &= ~0x2;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSTICK))//R3
	{
		ret &= ~0x4;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_BACK))//SELECT
	{
		ret &= ~0x1;
	}

	if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_START))//START
	{
		ret &= ~0x8;
	}
	return ret;
}
#endif

void UpdateGameControllerAnalogInput(SDL_GameController* pad, void* analogR, void* analogL)
{

#define PSX_MIN 0
#define PSX_MAX 255

#define SDL_MIN -32768
#define SDL_MAX 32767

	///@FIXME 0 is not exactly 0x80 it's 0x7F!
#define TRANSLATE(x) ((PSX_MAX - PSX_MIN) * (x - SDL_MIN) / (SDL_MAX - SDL_MIN)) + PSX_MIN

	struct Analog
	{
		unsigned char x;
		unsigned char y;
	};

	Analog* ar = (Analog*)analogR;
	Analog* al = (Analog*)analogL;

	if (ar != NULL)
	{
		constexpr int test = TRANSLATE(1);

		ar->x = TRANSLATE(SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTX));
		ar->y = TRANSLATE(SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTY));
	}

	if (al != NULL)
	{
		al->x = TRANSLATE(SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX));
		al->y = TRANSLATE(SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY));
	}
}

unsigned short UpdateKeyboardInput()
{
	unsigned short ret = 0xFFFF;

#if defined(SDL2)
	//Not initialised yet
	if (keyboardState == NULL)
	{
		return ret;
	}

	SDL_PumpEvents();

	if (keyboardState[SDL_SCANCODE_X])//Square
	{
		ret &= ~0x8000;
	}

	if (keyboardState[SDL_SCANCODE_V])//Circle
	{
		ret &= ~0x2000;
	}

	if (keyboardState[SDL_SCANCODE_Z])//Triangle
	{
		ret &= ~0x1000;
	}

	if (keyboardState[SDL_SCANCODE_C])//Cross
	{
		ret &= ~0x4000;
	}

	if (keyboardState[SDL_SCANCODE_LSHIFT])//L1
	{
		ret &= ~0x400;
	}

	if (keyboardState[SDL_SCANCODE_RSHIFT])//R1
	{
		ret &= ~0x800;
	}

	if (keyboardState[SDL_SCANCODE_UP])//UP
	{
		ret &= ~0x10;
	}

	if (keyboardState[SDL_SCANCODE_DOWN])//DOWN
	{
		ret &= ~0x40;
	}

	if (keyboardState[SDL_SCANCODE_LEFT])//LEFT
	{
		ret &= ~0x80;
	}

	if (keyboardState[SDL_SCANCODE_RIGHT])//RIGHT
	{
		ret &= ~0x20;
	}

	if (keyboardState[SDL_SCANCODE_LCTRL])//L2
	{
		ret &= ~0x100;
	}

	if (keyboardState[SDL_SCANCODE_RCTRL])//R2
	{
		ret &= ~0x200;
	}

	if (keyboardState[SDL_SCANCODE_SPACE])//SELECT
	{
		ret &= ~0x1;
	}

	if (keyboardState[SDL_SCANCODE_RETURN])//START
	{
		ret &= ~0x8;
	}
#endif
	return ret;
}

unsigned short UpdateKeyboardInputDebug()
{
	unsigned short ret = 0xFFFF;

#if defined(SDL2)
	//Not initialised yet
	if (keyboardStateDebug == NULL)
	{
		return ret;
	}

	SDL_PumpEvents();

	if (keyboardStateDebug[SDL_SCANCODE_X])//Square
	{
		ret &= ~0x8000;
	}

	if (keyboardStateDebug[SDL_SCANCODE_V])//Circle
	{
		ret &= ~0x2000;
	}

	if (keyboardStateDebug[SDL_SCANCODE_Z])//Triangle
	{
		ret &= ~0x1000;
	}

	if (keyboardStateDebug[SDL_SCANCODE_C])//Cross
	{
		ret &= ~0x4000;
	}

	if (keyboardStateDebug[SDL_SCANCODE_LSHIFT])//L1
	{
		ret &= ~0x400;
	}

	if (keyboardStateDebug[SDL_SCANCODE_RSHIFT])//R1
	{
		ret &= ~0x800;
	}

	if (keyboardStateDebug[SDL_SCANCODE_UP])//UP
	{
		ret &= ~0x10;
	}

	if (keyboardStateDebug[SDL_SCANCODE_DOWN])//DOWN
	{
		ret &= ~0x40;
	}

	if (keyboardStateDebug[SDL_SCANCODE_LEFT])//LEFT
	{
		ret &= ~0x80;
	}

	if (keyboardStateDebug[SDL_SCANCODE_RIGHT])//RIGHT
	{
		ret &= ~0x20;
	}

	if (keyboardStateDebug[SDL_SCANCODE_LCTRL])//L2
	{
		ret &= ~0x100;
	}

	if (keyboardStateDebug[SDL_SCANCODE_RCTRL])//R2
	{
		ret &= ~0x200;
	}

	if (keyboardStateDebug[SDL_SCANCODE_SPACE])//SELECT
	{
		ret &= ~0x1;
	}

	if (keyboardStateDebug[SDL_SCANCODE_RETURN])//START
	{
		ret &= ~0x8;
	}
#endif
	return ret;
}
