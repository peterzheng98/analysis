cmake_minimum_required(VERSION 3.17)
project(analysis)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
add_subdirectory(src)


add_executable(analysis main.cpp)
target_link_libraries(analysis project-src)