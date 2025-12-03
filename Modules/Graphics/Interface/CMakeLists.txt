cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Graphics_Base)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

TP_add_interface_subdirectory(Types)
include("${TEAPACKET_ROOT}/CMakeCommons/InterfaceCommon.CMake")
target_link_libraries(TeaPacket_Graphics_Base INTERFACE TeaPacket_Types_Base)