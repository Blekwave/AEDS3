# CMakeLists.txt
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(TP2 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -pg -lm")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${TP2_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${TP2_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TP2_BINARY_DIR}/bin")

add_subdirectory(${TP2_SOURCE_DIR}/src)
