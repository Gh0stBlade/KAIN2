set(KAIN2_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(Game)
if(NOT UNIX AND NOT EMSCRIPTEN AND NOT ANDROID AND NOT PLATFORM_PSP2)
#add_subdirectory(Editor)
endif()

if(DEFINED $ENV{APPVEYOR_PROJECT_ID})
add_subdirectory(Editor)
endif()