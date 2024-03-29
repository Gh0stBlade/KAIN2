#include "MAINVM.H"
#include "Game/PSX/MAIN.H"

#if defined(PSXPC_VERSION)
#include "Game/CORE.H"
#if (defined(SDL2) || defined(__ANDROID__)) && defined(UWP_SDL2) || defined(_WIN32) && !defined(SN_TARGET_PSP2)  && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM) && !defined(_XBOX)
#undef R13
#undef R12
#undef R11
#include <SDL_main.h>
#endif
#endif

struct _G2AppDataVM_Type _appDataVM;

EMULATOR_THREAD_DEF

#if !defined(EDITOR)

#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
extern "C" void nnMain()
#elif (defined(PSXPC_VERSION) || defined(__ANDROID__))
int main(int argc, char* argv[])
#elif defined(UWP)
int main(Platform::Array<Platform::String^>^ args)
#else
int main()
#endif
{ 
#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
    MainG2(&_appDataVM);
#else
    return MainG2(&_appDataVM);
#endif
}
#endif

#if defined(UWP_SDL2)
#include <wrl.h>

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    if (FAILED(Windows::Foundation::Initialize(RO_INIT_MULTITHREADED)))
    {
        return 1;
    }

    SDL_WinRTRunApp(SDL_main, NULL);
    return 0;
}
#endif
