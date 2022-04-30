cmake_minimum_required (VERSION 3.0)

project (KAIN2_${TARGET_ARCH})

#Enable 32-bit address for tag
OPTIONAL_DEFINE(USE_32_BIT_ADDR "Enable/Disable 32-bit address for tag" ON)
OPTIONAL_DEFINE(NO_CD "Enable/Disable ISO mode for Emulator" ON)

file(GLOB_RECURSE KAIN2_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cur ${CMAKE_CURRENT_SOURCE_DIR}/*.rc)
list(FILTER KAIN2_SRCS EXCLUDE REGEX "/PC/*")

set(RESOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/KAIN2.RC")
list(APPEND KAIN2_SRCS ${RESOURCE_PATH})

if(ANDROID)
#Recursively locate the source files for Emulator.
file(GLOB_RECURSE EMULATOR_SPECIFIC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../Emulator/*.C ${CMAKE_CURRENT_SOURCE_DIR}/../Emulator/*.H)
list(FILTER EMULATOR_SPECIFIC_SRCS EXCLUDE REGEX "/Platform/Android/*")
endif()

if(ANDROID)
include_directories(${EMULATOR_INCLUDE_DIR}/Core/)

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

if(MSVC AND NOT WINDOWS_STORE)
set_target_properties(
${PROJECT_NAME} PROPERTIES OUTPUT_NAME "KAIN2" SUFFIX ".EXE"
)
endif()

set_target_properties(
${PROJECT_NAME} PROPERTIES FOLDER "Build Targets/PlayStation"
)

set(TARGET_BINARY_DIR "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${EMULATOR_PUBLIC_INCLUDE_DIR})
include_directories(${EMULATOR_INCLUDE_DIR})

#Include all platform specific cmake files.
include("Platform/Windows.cmake")
include("Platform/Mingw.cmake")
include("Platform/Linux.cmake")
include("Platform/Emscripten.cmake")
include("Platform/Android.cmake")
include("Platform/WindowsStore.cmake")

BuildPreProcessorDefinitions()

target_link_libraries(
${PROJECT_NAME}
${PSX_LIB}
${SDL2_LIBRARY}
${OPENGL_gl_LIBRARY}
${GLEW_LIBRARY}
${OPENAL_LIBRARY}
)

add_dependencies(${PROJECT_NAME} ${EMULATOR_TARGET_NAME})

if(MSVC)
	set_target_properties(
	${PROJECT_NAME}
	PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
	PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE"
	PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE"
	PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:CONSOLE"
	PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${TARGET_BINARY_DIR}
	)
endif()

add_definitions(-DPSXPC_VERSION)
add_definitions(-DPSX_VERSION)