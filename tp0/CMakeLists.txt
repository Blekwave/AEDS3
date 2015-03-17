# CMakeLists.txt
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(TP0 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pg -Wall -std=c99")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${TP0_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${TP0_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TP0_BINARY_DIR}/bin")

add_subdirectory(${TP0_SOURCE_DIR}/src)