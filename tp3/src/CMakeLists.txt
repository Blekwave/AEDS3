include_directories(${TP3_SOURCE_DIR}/src)

add_library(solver_bf solver_bf.c)
add_library(solver_dp solver_dp.c)
add_library(solver_dp_p solver_dp_p.c)

add_executable(BF main.c)
add_executable(DP main.c)
add_executable(PDP main.c)

target_link_libraries(BF solver_bf)
target_link_libraries(DP solver_dp)
target_link_libraries(PDP solver_dp_p pthread)
