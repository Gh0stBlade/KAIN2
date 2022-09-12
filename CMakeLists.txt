cmake_minimum_required (VERSION 3.0)

project ("Soul Reaver")

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

#Set root path
set(ROOT_DIR ${ROOT_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")

#Stores root external lib folder.
set(EXTERN_LIB_PATH ${EXTERN_LIB_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/External")

#Extract target architecture (MSVC only)
if (MSVC OR DEFINED PLATFORM_PSP2)
	if(${PLATFORM_DURANGO})
        set(TARGET_ARCH "Durango64")
	elseif(${PLATFORM_PSP2})
        set(TARGET_ARCH "PSP2")
	elseif(${PLATFORM_NX})
        set(TARGET_ARCH "NX")
    elseif(CMAKE_CL_64)
        set(TARGET_ARCH "Win64")
    else()
        set(TARGET_ARCH "Win32")
    endif()
endif()

if (ANDROID)
	set(TARGET_ARCH "Androidx86")
endif()

if (EMSCRIPTEN)
	set(TARGET_ARCH "Web")
endif()


#We want to be able to group projects in folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if(ANDROID)
add_subdirectory(${ROOT_DIR}/External/SDL/ EXCLUDE_FROM_ALL)
endif()
add_subdirectory(Emulator)
add_subdirectory(KAIN2)
add_subdirectory(Tools)