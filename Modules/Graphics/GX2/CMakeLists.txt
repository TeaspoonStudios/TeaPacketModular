cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Graphics_GX2)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

set(CMAKE_CXX_STANDARD 20)


TP_add_interface_subdirectory(Graphics)
TP_add_interface_subdirectory(MacroUtils)
TP_add_interface_subdirectory(Math)
TP_add_interface_subdirectory(Logging)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_Graphics_GX2 STATIC ${sources})
target_include_directories(TeaPacket_Graphics_GX2 PRIVATE source)
target_include_directories(TeaPacket_Graphics_GX2 PUBLIC include)
target_link_libraries(TeaPacket_Graphics_GX2 TeaPacket_Graphics_Base TeaPacket_Logging_Base)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")