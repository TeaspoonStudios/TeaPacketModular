cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Logging_WHBLogUDP)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

set(CMAKE_CXX_STANDARD 20)

TP_add_interface_subdirectory(Logging)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_Logging_WHBLogUDP STATIC ${sources})
target_include_directories(TeaPacket_Logging_WHBLogUDP PRIVATE source)
target_link_libraries(TeaPacket_Logging_WHBLogUDP TeaPacket_Logging_Base)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")