cmake_minimum_required(VERSION 3.28)
project(TeaPacket_System_Base)

TP_add_interface_subdirectory(MacroUtils)
TP_add_interface_subdirectory(Core)

include("${TEAPACKET_ROOT}/CMakeCommons/InterfaceCommon.CMake")
target_link_libraries(TeaPacket_System_Base INTERFACE TeaPacket_MacroUtils_Base TeaPacket_Core_Base)