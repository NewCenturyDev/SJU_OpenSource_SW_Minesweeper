cmake_minimum_required(VERSION 3.10.2)
project(Minesweeper)

include_directories(src)
aux_source_directory(src SRC)

add_compile_options(-std=c11 -g)
add_executable(main ${SRC})