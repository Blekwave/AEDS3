include_directories(${TP2_SOURCE_DIR}/src)

# add_library(amgraph amgraph.c)

add_executable(TP3 main.c)

target_link_libraries(TP3 m)
