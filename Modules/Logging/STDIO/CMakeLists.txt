cmake_minimum_required(VERSION 3.31)
project(TeaPacket_Logging_STDIO)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

set(CMAKE_CXX_STANDARD 20)


TP_add_interface_subdirectory(Logging)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_Logging_STDIO STATIC ${sources})
target_include_directories(TeaPacket_Logging_STDIO PRIVATE source)
target_link_libraries(TeaPacket_Logging_STDIO TeaPacket_Logging_Base)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")