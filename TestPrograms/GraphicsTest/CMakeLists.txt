cmake_minimum_required(VERSION 3.28)
project(TeaPacket_TestPrograms_Graphics)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

add_executable(TeaPacket_TestPrograms_Graphics main.cpp)
TP_use_profile(Windows)
set_target_properties(TeaPacket_TestPrograms_Graphics PROPERTIES CXX_STANDARD 20)