cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Window_Base)

TP_add_interface_subdirectory(MacroUtils)

include("${TEAPACKET_ROOT}/CMakeCommons/InterfaceCommon.CMake")

target_link_libraries(TeaPacket_Window_Base INTERFACE TeaPacket_MacroUtils_Base)