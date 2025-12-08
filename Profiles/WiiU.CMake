

#TP_add_impl_subdirectory(Assets STDFile)
#TP_add_impl_subdirectory(Graphics Direct3D11)
TP_add_impl_subdirectory(Logging WHBLogUDP)
TP_add_impl_subdirectory(System WiiU)
#TP_add_impl_subdirectory(Window Windows)

TP_add_interface_subdirectory(Math)
TP_add_interface_subdirectory(Types)

target_link_libraries(${CMAKE_PROJECT_NAME}
    #TeaPacket_Assets_STDFile
    #TeaPacket_Graphics_Direct3D11 
    TeaPacket_Logging_WHBLogUDP
    TeaPacket_Math_Base
    TeaPacket_Types_Base
    TeaPacket_System_WiiU
    #TeaPacket_Window_Windows
    )

if(DEFINED TEAPACKET_PLATFORM)
    if(TEAPACKET_PLATFORM STREQUAL "WiiU")
        message("TEAPACKET PROFILE: CREATING RPX AUTOMATICALLY")
        wut_create_rpx(${CMAKE_PROJECT_NAME})
        message("TEAPACKET PROFILE: ADDING ASSETS AUTOMATICALLY")
    endif ()
endif ()