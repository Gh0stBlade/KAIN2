cmake_minimum_required (VERSION 3.0)

project (KAIN2_${TARGET_ARCH})

#Disable MinSizeRel and RelWithDebInfo add Final.
#set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Final")

#SET(CMAKE_CXX_FLAGS_FINAL ${CMAKE_CXX_FLAGS_RELEASE} CACHE STRING "Flags used by the C++ compiler during Final builds." FORCE)

#SET(CMAKE_C_FLAGS_FINAL ${CMAKE_C_FLAGS_RELEASE} CACHE STRING "Flags used by the C compiler during Final builds." FORCE)

#SET(CMAKE_EXE_LINKER_FLAGS_FINAL ${CMAKE_EXE_LINKER_FLAGS_RELEASE} CACHE STRING "Flags used by the linker during Final builds." FORCE)

#SET(CMAKE_SHARED_LINKER_FLAGS_FINAL ${CMAKE_SHARED_LINKER_FLAGS_RELEASE} CACHE STRING "Flags used by the shared linker during Final builds." FORCE)

#MARK_AS_ADVANCED(CMAKE_CXX_FLAGS_FINAL CMAKE_C_FLAGS_FINAL CMAKE_EXE_LINKER_FLAGS_FINAL CMAKE_SHARED_LINKER_FLAGS_FINAL)

if(NOT UNIX)
file(GLOB_RECURSE KAIN2_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cur ${CMAKE_CURRENT_SOURCE_DIR}/*.rc)
else()
file(GLOB_RECURSE KAIN2_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.C ${CMAKE_CURRENT_SOURCE_DIR}/*.H)
endif()

list(FILTER KAIN2_SRCS EXCLUDE REGEX "/PC/*")

set(RESOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/KAIN2.RC")

list(APPEND KAIN2_SRCS ${RESOURCE_PATH})

if(ANDROID)

#Recursively locate the source files for Emulator.
file(GLOB_RECURSE EMULATOR_SPECIFIC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../../Emulator/*.C ${CMAKE_CURRENT_SOURCE_DIR}/../../Emulator/*.H)
list(FILTER EMULATOR_SPECIFIC_SRCS EXCLUDE REGEX "/Platform/Android/*")
endif()

include_directories(${KAIN2_ROOT})
include_directories(${EMULATOR_PUBLIC_INCLUDE_DIR})
include_directories(${EMULATOR_INCLUDE_DIR})
include_directories(${EMULATOR_INCLUDE_DIR}/Library)

if(ANDROID)
add_library(
${PROJECT_NAME}
SHARED
${EMULATOR_SPECIFIC_SRCS}
${KAIN2_SRCS}
)
else()
add_executable(
${PROJECT_NAME}
WIN32
${KAIN2_SRCS}
)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${KAIN2_SRCS})

if(MSVC)
set_target_properties(
${PROJECT_NAME} PROPERTIES OUTPUT_NAME "KAIN2" SUFFIX ".exe"
)
endif()

set_target_properties(
${PROJECT_NAME} PROPERTIES FOLDER "Build Targets/PlayStation"
)

set(TARGET_BINARY_DIR "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")

if(NOT EMSCRIPTEN)
add_dependencies(${PROJECT_NAME} SDL2)
endif()

#Include all platform specific cmake files.
include("Platform/Win32.cmake")
include("Platform/Win64.cmake")
include("Platform/Mingw.cmake")
include("Platform/Linux.cmake")
include("Platform/Emscripten.cmake")
include("Platform/Android.cmake")
include("Platform/WindowsStore.cmake")
include("Platform/Durango.cmake")

#Enable 32-bit address for tag
OPTIONAL_DEFINE(USE_32_BIT_ADDR "Enable/Disable 32-bit address for tag" ON)

OPTIONAL_DEFINE(NO_CD "Enable/Disable ISO mode for Emulator" ON)

#Enable KAIN2 mode
OPTIONAL_DEFINE(KAIN2 "Enable/Disable KAIN2 code" ON)

BuildPreProcessorDefinitions()

if(EMSCRIPTEN)
target_link_libraries(
${PROJECT_NAME}
${PSX_LIB}
${OPENGL_gl_LIBRARY}
)
elseif(ANDROID)
target_link_libraries(
${PROJECT_NAME}
${PSX_LIB}
)
elseif(WINDOWS_STORE)
target_link_libraries(
${PROJECT_NAME}
${PSX_LIB}
SDL2::SDL2-static
)
else()
target_link_libraries(
${PROJECT_NAME}
${PSX_LIB}
SDL2::SDL2-static
SDL2::SDL2main
${OPENGL_gl_LIBRARY}
)
endif()

#For "Final"
if(NOT UNIX AND NOT EMSCRIPTEN AND NOT PLATFORM_PSP2 AND NOT PLATFORM_NX_ARM AND NOT ANDROID)
set(${CMAKE_EXE_LINKER_FLAGS_FINAL} CACHE STRING ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
endif()

if(MSVC AND NOT WINDOWS_STORE)
	add_dependencies(${PROJECT_NAME} ${EMULATOR_TARGET_NAME})

	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/Binaries")	
	set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_${TARGET_ARCH}_Debug")
	set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_${TARGET_ARCH}_Release")
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${ROOT_DIR}/Binaries/$(Configuration)")
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME})
	
	target_link_libraries(
	${PROJECT_NAME}
	d3d9.lib
	d3d11.lib
	d3d12.lib
	dxgi.lib
	dxguid.lib
	)
endif()

if(WINDOWS_STORE)
	set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_${TARGET_ARCH}")
	set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_${TARGET_ARCH}")
endif()

if(DEFINED PLATFORM_PSP2)
	set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/Binaries/PSP2")	
	set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "EBOOT")
	set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".BIN")
endif()

add_definitions(-DPSXPC_VERSION)
add_definitions(-DPSX_VERSION)