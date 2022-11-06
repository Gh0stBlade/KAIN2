add_subdirectory(SDL)
set_target_properties(SDL2 PROPERTIES FOLDER "Libs/3rdParty/SDL2")
set_target_properties(SDL2-static PROPERTIES FOLDER "Libs/3rdParty/SDL2")
set_target_properties(SDL2main PROPERTIES FOLDER "Libs/3rdParty/SDL2")
set_target_properties(SDL2_test PROPERTIES FOLDER "Libs/3rdParty/SDL2")
set_target_properties(uninstall PROPERTIES FOLDER "Libs/3rdParty/SDL2")
set_target_properties(sdl_headers_copy PROPERTIES FOLDER "Libs/3rdParty/SDL2")

set_target_properties(SDL2 PROPERTIES EXCLUDE_FROM_ALL True)
set_target_properties(SDL2_test PROPERTIES EXCLUDE_FROM_ALL True)
set_target_properties(uninstall PROPERTIES EXCLUDE_FROM_ALL True)
set_target_properties(sdl_headers_copy PROPERTIES EXCLUDE_FROM_ALL True)

set_target_properties(SDL2 PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)
set_target_properties(SDL2_test PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)
set_target_properties(uninstall PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)
set_target_properties(sdl_headers_copy PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)

add_subdirectory(glew-cmake)
set_target_properties(libglew_static PROPERTIES FOLDER "Libs/3rdParty/glew")
set_target_properties(libglew_shared PROPERTIES FOLDER "Libs/3rdParty/glew")

set_target_properties(libglew_shared PROPERTIES EXCLUDE_FROM_ALL True)
set_target_properties(libglew_shared PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)