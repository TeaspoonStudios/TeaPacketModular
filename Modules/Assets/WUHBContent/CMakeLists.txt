cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Assets_WUHBContent)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

set(CMAKE_CXX_STANDARD 20)

TP_add_interface_subdirectory(Assets)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_Assets_WUHBContent STATIC ${sources})
target_include_directories(TeaPacket_Assets_WUHBContent PRIVATE source)
#target_include_directories(TeaPacket_Assets_STDFile PUBLIC include)
target_link_libraries(TeaPacket_Assets_WUHBContent TeaPacket_Assets_Base)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")