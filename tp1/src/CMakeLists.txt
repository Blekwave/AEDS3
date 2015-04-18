include_directories(${TP1_SOURCE_DIR}/src)

add_library(parser parser.c)
add_library(sorting sorting.c)
add_library(attack attack.c)

add_executable(TP1 main.c)

target_link_libraries(TP1 parser sorting attack)


