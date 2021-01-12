cmake_minimum_required(VERSION 3.17)
project(2sat)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(2sat main.cpp)