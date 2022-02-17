cmake_minimum_required (VERSION 3.0)

project (KAIN2_${TARGET_ARCH})

file(GLOB_RECURSE KAIN2_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cur ${CMAKE_CURRENT_SOURCE_DIR}/*.rc)

add_executable(
${PROJECT_NAME}
WIN32
${KAIN2_SRCS}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${KAIN2_SRCS})

set_target_properties(
${PROJECT_NAME} PROPERTIES OUTPUT_NAME "KAIN2"
${PROJECT_NAME} PROPERTIES FOLDER "Build Targets/PlayStation"
)

set(TARGET_BINARY_DIR "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${ROOT_DIR}/Emulator)
link_directories(${ROOT_DIR}/Build/Emulator/${CMAKE_BUILD_TYPE})

#Include all platform specific cmake files.
include("Platform/Windows.cmake")
include("Platform/Mingw.cmake")
include("Platform/Linux.cmake")
include("Platform/Emscripten.cmake")
include("Platform/Android.cmake")

target_link_libraries(
${PROJECT_NAME}
${PSX_LIB}
${SDL2_LIBRARY}
${OPENGL_gl_LIBRARY}
${GLEW_LIBRARY}
)

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

add_definitions(-DPSX_VERSION)