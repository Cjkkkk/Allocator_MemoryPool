cmake_minimum_required(VERSION 3.9)
project(final)

set(CMAKE_CXX_STANDARD 11)

add_executable(final main.cpp STL_allocator.cpp STL_allocator.h MemoryPool.cpp MemoryPool.h)