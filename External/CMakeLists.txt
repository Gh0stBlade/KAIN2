add_subdirectory(libValkyrie)

if(NOT EMSCRIPTEN AND NOT PLATFORM_NX AND NOT PLATFORM_NX_ARM)
if(ANDROID)
add_subdirectory(SDL EXCLUDE_FROM_ALL)
else()
add_subdirectory(SDL)
endif()

if(NOT ANDROID)
set_target_properties(SDL2 PROPERTIES FOLDER "Libs/3rdParty/SDL2")
set_target_properties(SDL2-static PROPERTIES FOLDER "Libs/3rdParty/SDL2")

if(NOT WINDOWS_STORE)
set_target_properties(SDL2main PROPERTIES FOLDER "Libs/3rdParty/SDL2")
endif()
set_target_properties(SDL2_test PROPERTIES FOLDER "Libs/3rdParty/SDL2")
set_target_properties(uninstall PROPERTIES FOLDER "Libs/3rdParty/SDL2")
set_target_properties(sdl_headers_copy PROPERTIES FOLDER "Libs/3rdParty/SDL2")

set_target_properties(SDL2 PROPERTIES EXCLUDE_FROM_ALL True)
set_target_properties(SDL2_test PROPERTIES EXCLUDE_FROM_ALL True)
set_target_properties(uninstall PROPERTIES EXCLUDE_FROM_ALL True)

set_target_properties(SDL2 PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)
set_target_properties(SDL2_test PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)
set_target_properties(uninstall PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)
endif()
endif()