cmake_minimum_required (VERSION 3.0)

project (EDITOR_${TARGET_ARCH})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set (CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\msvc2019\\")

find_package(Qt5 COMPONENTS Widgets Gui Core REQUIRED)

#Disable MinSizeRel and RelWithDebInfo add Final.
#set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Final")

#SET(CMAKE_CXX_FLAGS_FINAL ${CMAKE_CXX_FLAGS_RELEASE} CACHE STRING "Flags used by the C++ compiler during Final builds." FORCE)

#SET(CMAKE_C_FLAGS_FINAL ${CMAKE_C_FLAGS_RELEASE} CACHE STRING "Flags used by the C compiler during Final builds." FORCE)

#SET(CMAKE_EXE_LINKER_FLAGS_FINAL ${CMAKE_EXE_LINKER_FLAGS_RELEASE} CACHE STRING "Flags used by the linker during Final builds." FORCE)

#SET(CMAKE_SHARED_LINKER_FLAGS_FINAL ${CMAKE_SHARED_LINKER_FLAGS_RELEASE} CACHE STRING "Flags used by the shared linker during Final builds." FORCE)

#MARK_AS_ADVANCED(CMAKE_CXX_FLAGS_FINAL CMAKE_C_FLAGS_FINAL CMAKE_EXE_LINKER_FLAGS_FINAL CMAKE_SHARED_LINKER_FLAGS_FINAL)

file(GLOB_RECURSE EDITOR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cur ${CMAKE_CURRENT_SOURCE_DIR}/*.rc ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc ${CMAKE_CURRENT_SOURCE_DIR}/*.qss)
file(GLOB_RECURSE KAIN2_SRCS ${KAIN2_ROOT}/*.C ${KAIN2_ROOT}/*.H)
file(GLOB_RECURSE EMULATOR_SPECIFIC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../../Emulator/*.C ${CMAKE_CURRENT_SOURCE_DIR}/../../Emulator/*.H)
#Exclude Android temporary files folder.
if(NOT ANDROID)
list(FILTER EMULATOR_SPECIFIC_SRCS EXCLUDE REGEX "/Platform/Android/*")
endif()

list(FILTER KAIN2_SRCS EXCLUDE REGEX "/PC/*")

set(RESOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/EDITOR.RC")
list(APPEND EDITOR_SRCS ${RESOURCE_PATH})

if(ANDROID)
#Recursively locate the source files for Emulator.
list(FILTER EMULATOR_SPECIFIC_SRCS EXCLUDE REGEX "/Platform/Android/*")
endif()

if(ANDROID)
include_directories(${EMULATOR_INCLUDE_DIR}/Core/)
include_directories(${EMULATOR_INCLUDE_DIR}/Library/)

add_library(
${PROJECT_NAME}
SHARED
${EMULATOR_SPECIFIC_SRCS}
${EDITOR_SRCS}
)
else()
add_executable(
${PROJECT_NAME}
WIN32
${EDITOR_SRCS}
${EMULATOR_SPECIFIC_SRCS}
${KAIN2_SRCS}
)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SRCS})
source_group(TREE ${KAIN2_ROOT} FILES ${KAIN2_SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../../ FILES ${EMULATOR_SPECIFIC_SRCS})


if(MSVC)
set_target_properties(
${PROJECT_NAME} PROPERTIES OUTPUT_NAME "EDITOR" SUFFIX ".exe"
)

endif()

set_target_properties(
${PROJECT_NAME} PROPERTIES FOLDER "Build Targets/PlayStation"
)

set(TARGET_BINARY_DIR "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")

#Still used by OpenGL version of Editor.
add_dependencies(${PROJECT_NAME} SDL2)

include_directories(${KAIN2_ROOT})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${EMULATOR_PUBLIC_INCLUDE_DIR})
include_directories(${EMULATOR_INCLUDE_DIR})
include_directories(${EMULATOR_INCLUDE_DIR}/Library)
include_directories(${KAIN2_ROOT}/Game)

#Include all platform specific cmake files.
include("Platform/Windows.cmake")
include("Platform/Mingw.cmake")
include("Platform/Linux.cmake")
include("Platform/Emscripten.cmake")
include("Platform/Android.cmake")
include("Platform/WindowsStore.cmake")
include("Platform/Durango.cmake")

#Enable 32-bit address for tag
OPTIONAL_DEFINE(USE_32_BIT_ADDR "Enable/Disable 32-bit address for tag" ON)

OPTIONAL_DEFINE(NO_CD "Enable/Disable ISO mode for Emulator" ON)

OPTIONAL_DEFINE(EDITOR "Enable/Disable ISO mode for Emulator" ON)

#Enable KAIN2 mode
OPTIONAL_DEFINE(KAIN2 "Enable/Disable KAIN2 code" ON)

BuildPreProcessorDefinitions()

target_link_libraries(
${PROJECT_NAME}
SDL2-static
SDL2main
${OPENGL_gl_LIBRARY}
Qt5::Widgets
Qt5::Core
Qt5::Gui
Qt5::GuiPrivate
)

#For "Final"
if(NOT EMSCRIPTEN AND NOT PLATFORM_PSP2 AND NOT PLATFORM_NX_ARM AND NOT ANDROID)
set(${CMAKE_EXE_LINKER_FLAGS_FINAL} CACHE STRING ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
endif()

if(MSVC AND NOT WINDOWS_STORE)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${ROOT_DIR}/Binaries/$(Configuration)")

	set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/Binaries")	
	set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
	set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_${TARGET_ARCH}_Debug")
	set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_${TARGET_ARCH}_Release")
	
	target_link_libraries(
	${PROJECT_NAME}
	d3d9.lib
	d3d11.lib
	d3d12.lib
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

add_custom_target("EDITOR_AUTOGEN" ALL COMMAND ${CMAKE_COMMAND} -P ${ROOT_DIR}/CMake/EngineVersion.cmake)
add_dependencies(${PROJECT_NAME} "EDITOR_AUTOGEN")

get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)
execute_process(COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX RESULT_VARIABLE return_code OUTPUT_VARIABLE qt5_install_prefix OUTPUT_STRIP_TRAILING_WHITESPACE)
set(WINDEPLOYQT_EXECUTABLE "${qt5_install_prefix}/bin/windeployqt.exe")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND "${WINDEPLOYQT_EXECUTABLE}" --verbose 1 --no-translations --no-svg --no-opengl --no-opengl-sw --no-compiler-runtime --no-system-d3d-compiler --${CMAKE_BUILD_TYPE} \"$<TARGET_FILE:${PROJECT_NAME}>\" COMMENT "Deploying Qt...")

add_definitions(-DPSXPC_VERSION)
add_definitions(-DPSX_VERSION)