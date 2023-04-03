set(EMULATOR_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")

add_subdirectory(D3D9)
add_subdirectory(D3D11)
add_subdirectory(D3D12)
add_subdirectory(Vulkan)