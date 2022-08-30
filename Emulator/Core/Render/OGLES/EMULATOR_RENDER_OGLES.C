#include <Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H>
#include <Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H>

#if defined(OGLES)

#if OGLES_VERSION == 2
const char* renderBackendName = "OpenGLES 2.0";
#elif OGLES_VERSION == 3
const char* renderBackendName = "OpenGLES 3.0";
#endif

GLuint dynamic_vertex_buffer;
GLuint dynamic_vertex_array;

#endif