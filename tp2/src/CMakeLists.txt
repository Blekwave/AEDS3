include_directories(${TP2_SOURCE_DIR}/src)

add_library(tsp tsp.c)
add_library(amgraph amgraph.c)

add_executable(TP2 main.c)

target_link_libraries(TP2 tsp amgraph)
