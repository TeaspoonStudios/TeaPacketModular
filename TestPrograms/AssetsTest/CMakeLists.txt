cmake_minimum_required(VERSION 3.28)
project(TeaPacket_TestPrograms_Assets)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")


add_executable(TeaPacket_TestPrograms_Assets main.cpp)
TP_use_profile(${TEAPACKET_PLATFORM})
set_target_properties(TeaPacket_TestPrograms_Assets PROPERTIES CXX_STANDARD 20)