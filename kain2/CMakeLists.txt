cmake_minimum_required (VERSION 3.0)

project (KAIN2_PSX)

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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
	set_target_properties(
	${PROJECT_NAME}
	PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
	)
endif()

add_definitions(-DPSX_VERSION)