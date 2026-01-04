cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Input_Base)

TP_add_interface_subdirectory(Core)
TP_add_interface_subdirectory(Types)
include("${TEAPACKET_ROOT}/CMakeCommons/InterfaceCommon.CMake")
target_link_libraries(TeaPacket_Input_Base INTERFACE TeaPacket_Core_Base TeaPacket_Types_Base)