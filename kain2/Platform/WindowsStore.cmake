if(WINDOWS_STORE)

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build_UWP/Emulator/$(Configuration))

set(PSX_LIB PSX.lib)

endif()