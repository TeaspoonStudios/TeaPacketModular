cmake_minimum_required(VERSION 3.28)
project(TeaPacket_System_Windows)

set(CMAKE_CXX_STANDARD 20)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

TP_add_interface_subdirectory(System)
TP_add_impl_subdirectory(Window Windows)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_System_Windows STATIC ${sources})
target_include_directories(TeaPacket_System_Windows PRIVATE source)
target_include_directories(TeaPacket_System_Windows PUBLIC include)
target_link_libraries(TeaPacket_System_Windows TeaPacket_System_Base TeaPacket_Window_Windows)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")