cmake_minimum_required(VERSION 3.16)
project(DataGenerator)

set(CMAKE_CXX_STANDARD 14)

add_executable(DataGenerator main.cpp ToolFunction.cpp ToolFunction.h)
