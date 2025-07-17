cmake_minimum_required(VERSION 4.1)
project(fsis LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/engine)
add_subdirectory(src/fsis)