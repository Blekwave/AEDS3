include_directories(${TP3_SOURCE_DIR}/src)

add_library(solver_bf solver_bf.c)
add_library(solver_bf_p solver_bf_p.c)
add_library(solver_dp solver_dp.c)

add_executable(BF main.c)
add_executable(PBF main.c)
add_executable(DP main.c)

target_link_libraries(BF solver_bf)
target_link_libraries(PBF solver_bf_p pthread)
target_link_libraries(DP solver_dp)
