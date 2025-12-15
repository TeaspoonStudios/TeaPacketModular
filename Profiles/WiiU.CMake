

TP_add_impl_subdirectory(Assets WUHBContent)
TP_add_impl_subdirectory(Graphics GX2)
TP_add_impl_subdirectory(Logging WHBLogUDP)
TP_add_impl_subdirectory(System WiiU)

TP_add_interface_subdirectory(Math)
TP_add_interface_subdirectory(Types)

target_link_libraries(${CMAKE_PROJECT_NAME}
    TeaPacket_Assets_WUHBContent
    TeaPacket_Graphics_GX2
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
        add_custom_command(
                TARGET ${CMAKE_PROJECT_NAME}
                POST_BUILD
                COMMENT "TEAPACKET PROFILE: ADDING ASSETS AUTOMATICALLY"
                COMMAND wuhbtool
                "${CMAKE_PROJECT_NAME}.rpx"
                "${CMAKE_PROJECT_NAME}.wuhb"
                "--content=assets"
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif ()
endif ()