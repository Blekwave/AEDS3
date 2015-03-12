include_directories(${TP0_SOURCE_DIR}/src)

add_library(treap treap.c)

add_executable(TP0 main.c)

target_link_libraries(TP0 treap)


