# CMakeLists.txt
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(PA2 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pg -Wall -Og -std=c99")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PA2_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PA2_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PA2_BINARY_DIR}/bin")

add_subdirectory(${PA2_SOURCE_DIR}/src)