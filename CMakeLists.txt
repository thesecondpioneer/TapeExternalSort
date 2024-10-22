cmake_minimum_required(VERSION 3.24)
project(TapeExternalSort)

set(CMAKE_CXX_STANDARD 17)

add_executable(TapeExternalSort src/main.cpp src/Tape.h src/Tape.cpp src/TapeDevice.h src/TapeDevice.cpp src/FileTapeDevice.h src/FileTapeDevice.cpp src/TapeSorter.h src/TapeSorter.cpp)
