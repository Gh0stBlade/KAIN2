if(DEFINED PLATFORM_NX_ARM)

target_compile_definitions(${PROJECT_NAME} PRIVATE NN_NINTENDO_SDK)

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/include")

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Common/Configs/Targets/NX-NXFP2-a32/Include")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Externals/glew/lib/Release/${CMAKE_GENERATOR_PLATFORM}")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Libraries/NX-NXFP2-a32/Release")

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build_NX_Arm/Emulator)

target_compile_definitions(${PROJECT_NAME} PRIVATE "PLATFORM_NX_ARM")

target_compile_definitions(${PROJECT_NAME} PRIVATE "NN_SDK_BUILD_RELEASE")

target_compile_definitions(${PROJECT_NAME} PRIVATE "NN_NINTENDO_SDK")

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_USER_PROPS "${NX_PROPS}")

set(PSX_LIB Valkyrie.lib)

set(CMAKE_C_STANDARD_LIBRARIES "libnn_gfx.lib libnn_diag.lib libnn_hws.lib libnn_util.lib libnn_cstd.lib libnn_fs.lib libnn_os.lib libnn_fssystem.lib libnn_fssrv.lib libnn_vi.lib libnn_htc.lib libnn_hid.lib libnn_crypto.lib libnn_hws.lib libnn_sf.lib libnn_htcTenv.lib libnn_gfxUtil.lib libnn_mem.lib libnn_lmem.lib libnn_result.lib opengl32.lib User32.lib Gdi32.lib libnn_init.lib")
set(CMAKE_CXX_STANDARD_LIBRARIES "libnn_gfx.lib libnn_diag.lib libnn_hws.lib libnn_util.lib libnn_cstd.lib libnn_fs.lib libnn_os.lib libnn_fssystem.lib libnn_fssrv.lib libnn_vi.lib libnn_htc.lib libnn_hid.lib libnn_crypto.lib libnn_hws.lib libnn_sf.lib libnn_htcTenv.lib libnn_gfxUtil.lib libnn_mem.lib libnn_lmem.lib libnn_result.lib opengl32.lib User32.lib Gdi32.lib libnn_init.lib")

endif()