if(MSVC AND NOT WINDOWS_STORE AND NOT PLATFORM_PSP2 AND NOT PLATFORM_NX AND NOT PLATFORM_NX_ARM)

#SET(USE_VULKAN TRUE)
#SET(USE_D3D9 TRUE)

#Find libs
if(USE_VULKAN)
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIR})
elseif(USE_D3D9)
find_package(D3D9 REQUIRED)
include_directories(${DIRECTX_INCLUDE_DIRS})
else()
find_package(OpenGL REQUIRED)
endif()

include_directories(${EXTERN_LIB_PATH}/glew-cmake/Include)
include_directories(${EXTERN_LIB_PATH}/SDL/Include)

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build/Build_Win32/Emulator/$(Configuration))

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build/Build_Win32/Emulator/$(Configuration))

set(PSX_LIB Valkyrie.lib)

endif()