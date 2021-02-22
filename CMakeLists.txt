cmake_minimum_required(VERSION 3.17)
project(cy)

set(CMAKE_CXX_STANDARD 17)

add_executable(cy main.cpp scanner.cpp scanner.h utils.cpp utils.h config.cpp config.h console.cpp console.h)