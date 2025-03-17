cmake_minimum_required(VERSION 3.30)
project(cfam C)

set(CMAKE_C_COMPILER "/usr/bin/clang")

add_executable(cfam main.c histogram.c wordlengths.c anagram.c)
