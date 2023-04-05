if(("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64") AND MSVC AND NOT WINDOWS_STORE AND NOT PLATFORM_PSP2 AND NOT PLATFORM_NX AND NOT PLATFORM_NX_ARM)

#Find libs
find_package(OpenGL REQUIRED)

find_package(Vulkan)
if(Vulkan_FOUND)
include_directories(${Vulkan_INCLUDE_DIR})

target_link_libraries(
${PROJECT_NAME}
${Vulkan_LIBRARY}
)
endif()

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build/Build_Win64/External/libValkyrie/$(Configuration))

set(PSX_LIB Valkyrie.lib)

endif()