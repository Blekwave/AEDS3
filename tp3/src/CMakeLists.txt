include_directories(${TP3_SOURCE_DIR}/src)

add_library(solver_bf solver_bf.c)

add_executable(BF main.c)

target_link_libraries(BF solver_bf)
