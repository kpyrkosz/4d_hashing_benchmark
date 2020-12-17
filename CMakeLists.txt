cmake_minimum_required(VERSION 3.0)

project(hash_bench)

find_package(benchmark CONFIG REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark benchmark::benchmark_main)
