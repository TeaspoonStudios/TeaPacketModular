cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Window_Windows)

set(CMAKE_CXX_STANDARD 20)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

TP_add_interface_subdirectory(Window)
TP_add_interface_subdirectory(System)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_Window_Windows STATIC ${sources})
target_include_directories(TeaPacket_Window_Windows PRIVATE source)
target_include_directories(TeaPacket_Window_Windows PUBLIC include)
target_link_libraries(TeaPacket_Window_Windows TeaPacket_Window_Base TeaPacket_System_Base)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")