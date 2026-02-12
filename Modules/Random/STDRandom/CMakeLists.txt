cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Random_STDRandom)

set(CMAKE_CXX_STANDARD 20)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

TP_add_interface_subdirectory(Random)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_Random_STDRandom STATIC ${sources})
target_include_directories(TeaPacket_Random_STDRandom PRIVATE source)
#target_include_directories(TeaPacket_Time_STDChrono PUBLIC include)
target_link_libraries(TeaPacket_Random_STDRandom TeaPacket_Random_Base)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")