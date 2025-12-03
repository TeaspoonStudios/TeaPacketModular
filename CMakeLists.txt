cmake_minimum_required(VERSION 3.28)
project(TeaPacket)

set(CMAKE_CXX_STANDARD 20)
set(TEAPACKET_ROOT ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(TestPrograms)
add_subdirectory(Tests)