cmake_minimum_required(VERSION 3.17)
project(tsp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
add_executable(tsp main.cpp)