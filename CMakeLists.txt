cmake_minimum_required (VERSION 3.0)

project (SoulReaver)

macro(OPTIONAL_DEFINE DEFNAME DESCRIPTION VALUE)
	set(${DEFNAME} ${VALUE})
	list(APPEND OPT_DEFINITIONS ${DEFNAME})
endmacro()

macro(BuildPreProcessorDefinitions)
	foreach(DEF ${OPT_DEFINITIONS})
		if(${${DEF}} STREQUAL "ON")
			target_compile_definitions(${PROJECT_NAME} PRIVATE ${DEF}=1)
		else()
			target_compile_definitions(${PROJECT_NAME} PRIVATE ${DEF}=0)
		endif()
		list(REMOVE_ITEM OPT_DEFINITIONS ${DEF})
	endforeach()	
endmacro()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

#Stores root external lib folder.
set(EXTERN_LIB_PATH ${EXTERN_LIB_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/External")

#Extract target architecture (MSVC only)
if (MSVC)
    if (CMAKE_CL_64)
        set(TARGET_ARCH "Win64")
    else()
        set(TARGET_ARCH "Win32")
    endif()
endif()

#We want to be able to group projects in folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(Emulator)
add_subdirectory(KAIN2)