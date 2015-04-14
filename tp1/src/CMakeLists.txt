include_directories(${TP1_SOURCE_DIR}/src)

add_library(parser parser.c)

add_executable(TP1 main.c)

target_link_libraries(TP1 parser)


