cmake_minimum_required(VERSION 3.22)
project(bnb_method C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(utils)
add_subdirectory(methods)

add_library(Solver STATIC solver.h solver.c n_knapsack_solver.c knapsack_solver.c)
target_link_libraries(Solver Utils Problem)
target_include_directories(Solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils ${CMAKE_CURRENT_SOURCE_DIR}/methods)


# Main executable
add_executable(bnb_method main.c)
target_link_libraries(bnb_method Solver)
