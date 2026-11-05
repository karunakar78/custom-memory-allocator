cmake_minimum_required(VERSION 3.20)
project(CustomMemoryAllocator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(allocator
    src/main.cpp
    src/memory_pool.cpp
)

target_include_directories(allocator PRIVATE ${CMAKE_SOURCE_DIR}/include)
