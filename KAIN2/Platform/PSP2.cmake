if(MSVC AND NOT WINDOWS_STORE)

set(GLEW_LIBRARY "KERNEL32.LIB")

target_compile_definitions(${PROJECT_NAME} PRIVATE SN_TARGET_PSP2 NDEBUG __SNC__)

target_link_directories(${PROJECT_NAME} PRIVATE -I%$(SCE_PSP2_SDK_DIR)/target/lib%)

target_include_directories(${PROJECT_NAME} PRIVATE -I%$(SCE_PSP2_SDK_DIR)/host_tools/build/include%)
target_include_directories(${PROJECT_NAME} PRIVATE -I%$(SCE_PSP2_SDK_DIR)/target/include%)
target_include_directories(${PROJECT_NAME} PRIVATE -I%$(SCE_PSP2_SDK_DIR)/target/include_common%)

endif()