if(WINDOWS_STORE AND NOT DEFINED PLATFORM_DURANGO)

include_directories(${EXTERN_LIB_PATH}/SDL/Include)

set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} /ZW")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} /ZW")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /ZW")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} /ZW")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /ZW")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /ZW")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(PSX_LIB Valkyrie.lib)

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build/Build_UWP/External/libValkyrie/$(Configuration))

endif()