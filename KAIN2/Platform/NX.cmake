if(DEFINED PLATFORM_NX)

target_compile_definitions(${PROJECT_NAME} PRIVATE NN_NINTENDO_SDK)

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/include")

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Common/Configs/Targets/${CMAKE_GENERATOR_PLATFORM}-v140/Include")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Externals/glew/lib/Release/${CMAKE_GENERATOR_PLATFORM}")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Libraries/${CMAKE_GENERATOR_PLATFORM}-v140/$<CONFIG>")

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build_NX/Emulator/$(Configuration))

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_USER_PROPS "$(NintendoSdkSamplePropertySheetDirectory)NintendoSdkSampleSpec_NX.props $(NintendoSdkSamplePropertySheetDirectory)NintendoSdkSampleBuildType_Debug.props $(NintendoSdkSamplePropertySheetDirectory)NintendoSdkSampleVcProjectSettings.props")

target_compile_definitions(${PROJECT_NAME} PRIVATE "PLATFORM_NX")

target_compile_definitions(${PROJECT_NAME} PRIVATE "NN_SDK_BUILD_DEBUG")

set(PSX_LIB Valkyrie.lib)

set(CMAKE_C_STANDARD_LIBRARIES "OpenGL32.lib glew32.lib libnn_htcTenv.lib libnn_tmagent.lib libnn_settings.lib libnn_fssrv.lib libnn_fssystem.lib Rpcrt4.lib shlwapi.lib libnn_vi.lib libnn_diagTextPublicEn.lib libnn_htc.lib libnn_htcs.lib  libnn_sf.lib libnn_util.lib libnn_cstd.lib libnn_result.lib libnn_diag.lib libnn_os.lib libnn_init.lib libnn_fs.lib libnn_fsSave.lib libnn_utilC.lib libnn_audio.lib libnn_hid.lib winmm.lib libnn_hws.lib libnn_gfx.lib libnn_socket.lib libnn_ssl.lib libnn_mem.lib libnn_crypto.lib libnn_lm.lib libnn_lmem.lib iphlpapi.lib ws2_32.lib libnn_xcd.lib hid.lib setupapi.lib libnn_account.lib libnn_accountImpl.lib libnn_arp.lib libnn_arpImpl.lib libnn_time.lib libnn_timesrv.lib libnn_nifm.lib wlanapi.lib libnn_nsd.lib libnn_pctl.lib libnn_pctlSrv.lib libnn_ntcsrv.lib libnn_erpt.lib libnn_err.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib")
set(CMAKE_CXX_STANDARD_LIBRARIES "OpenGL32.lib glew32.lib libnn_htcTenv.lib libnn_tmagent.lib libnn_settings.lib libnn_fssrv.lib libnn_fssystem.lib Rpcrt4.lib shlwapi.lib libnn_vi.lib libnn_diagTextPublicEn.lib libnn_htc.lib libnn_htcs.lib  libnn_sf.lib libnn_util.lib libnn_cstd.lib libnn_result.lib libnn_diag.lib libnn_os.lib libnn_init.lib libnn_fs.lib libnn_fsSave.lib libnn_utilC.lib libnn_audio.lib libnn_hid.lib winmm.lib libnn_hws.lib libnn_gfx.lib libnn_socket.lib libnn_ssl.lib libnn_mem.lib libnn_crypto.lib libnn_lm.lib libnn_lmem.lib iphlpapi.lib ws2_32.lib libnn_xcd.lib hid.lib setupapi.lib libnn_account.lib libnn_accountImpl.lib libnn_arp.lib libnn_arpImpl.lib libnn_time.lib libnn_timesrv.lib libnn_nifm.lib wlanapi.lib libnn_nsd.lib libnn_pctl.lib libnn_pctlSrv.lib libnn_ntcsrv.lib libnn_erpt.lib libnn_err.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib")

endif()