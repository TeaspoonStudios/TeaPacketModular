cmake_minimum_required(VERSION 3.28)
project(TeaPacket_TestPrograms_MemTests)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

TP_add_interface_subdirectory(Types)

add_executable(TeaPacket_TestPrograms_MemTests main.cpp)
target_link_libraries(TeaPacket_TestPrograms_MemTests PUBLIC TeaPacket_Types_Base)
set_target_properties(TeaPacket_TestPrograms_MemTests PROPERTIES CXX_STANDARD 20)