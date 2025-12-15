cmake_minimum_required(VERSION 3.28)
project(TeaPacket_System_WiiU)

set(CMAKE_CXX_STANDARD 20)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

TP_add_interface_subdirectory(System)
TP_add_interface_subdirectory(Logging)

file(GLOB_RECURSE sources "${CMAKE_SOURCE_DIR}/source/*.c*")

add_library(TeaPacket_System_WiiU STATIC source/System.cpp)
target_include_directories(TeaPacket_System_WiiU PRIVATE "${CMAKE_SOURCE_DIR}/source")
#target_include_directories(TeaPacket_System_WiiU PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(TeaPacket_System_WiiU TeaPacket_System_Base TeaPacket_Logging_Base)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")