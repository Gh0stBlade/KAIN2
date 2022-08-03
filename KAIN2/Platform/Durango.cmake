if(WINDOWS_STORE AND DEFINED PLATFORM_DURANGO)

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build_Durango/Emulator/$(Configuration) ${EXTERN_LIB_PATH}/SDL_mixer/VisualC-WinRT/x64/Debug/SDL_mixer-UWP ${EXTERN_LIB_PATH}/SDL/VisualC-WinRT/x64/Debug/SDL-UWP)
target_link_libraries(${PROJECT_NAME} SDL2.lib SDL2_mixer.lib)

include_directories(${EXTERN_LIB_PATH}/SDL/include)
include_directories(${EXTERN_LIB_PATH}/SDL_mixer/include)

set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} /ZW")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} /ZW")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /ZW")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} /ZW")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /ZW")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /ZW")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(PSX_LIB Valkyrie.lib)

endif()