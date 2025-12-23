TP_add_impl_subdirectory(Assets STDFile)
TP_add_impl_subdirectory(Graphics Direct3D11)
TP_add_impl_subdirectory(Input GameInput)
TP_add_impl_subdirectory(Logging STDIO)
TP_add_impl_subdirectory(System Windows)
TP_add_impl_subdirectory(Window Windows)

TP_add_interface_subdirectory(Math)
TP_add_interface_subdirectory(Types)

target_link_libraries(${CMAKE_PROJECT_NAME}
    TeaPacket_Assets_STDFile
    TeaPacket_Graphics_Direct3D11
    TeaPacket_Input_GameInput
    TeaPacket_Logging_STDIO
    TeaPacket_Math_Base
    TeaPacket_Types_Base
    TeaPacket_System_Windows
    TeaPacket_Window_Windows
    )