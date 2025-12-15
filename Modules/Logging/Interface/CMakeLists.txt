cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Logging_Base)

TP_add_interface_subdirectory(Core)
include("${TEAPACKET_ROOT}/CMakeCommons/InterfaceCommon.CMake")
target_link_libraries(TeaPacket_Logging_Base INTERFACE TeaPacket_Core_Base)