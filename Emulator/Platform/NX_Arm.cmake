if(DEFINED PLATFORM_NX_ARM)

target_compile_definitions(${PROJECT_NAME} PRIVATE NN_NINTENDO_SDK)

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/include")

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Common/Configs/Targets/NX-NXFP2-a32/Include")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Externals/glew/lib/Release/${CMAKE_GENERATOR_PLATFORM}")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Libraries/NX-NXFP2-a32/$<CONFIG>")

target_compile_definitions(${PROJECT_NAME} PRIVATE "NN_SDK_BUILD_RELEASE")

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_USER_PROPS "$(NintendoSdkSamplePropertySheetDirectory)NintendoSdkSampleSpec_NX.props $(NintendoSdkSamplePropertySheetDirectory)NintendoSdkSampleBuildType_Debug.props $(NintendoSdkSamplePropertySheetDirectory)NintendoSdkSampleVcProjectSettings.props")

target_compile_definitions(${PROJECT_NAME} PRIVATE "PLATFORM_NX_ARM")

target_compile_definitions(${PROJECT_NAME} PRIVATE "NN_NINTENDO_SDK")

endif()