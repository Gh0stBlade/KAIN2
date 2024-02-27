#Store the path to our tools folder folder in "TOOL_ROOT_DIR".
set(TOOL_ROOT_DIR ${TOOL_ROOT_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

#Add each tool project
if(NOT(DEFINED ENV{TRAVIS} OR DEFINED ENV{APPVEYOR}) OR APPLE)
	add_subdirectory(ShiftMidi)
	add_subdirectory(ShiftList)
	#add_subdirectory(ShiftEditor)
endif()