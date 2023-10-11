cmake_minimum_required(VERSION 3.9)

project(matrix_mul C)

find_package(MPI REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_executable(02_matrix_mul 02_matrix_mul.c)
target_link_libraries(02_matrix_mul PUBLIC MPI::MPI_C)
