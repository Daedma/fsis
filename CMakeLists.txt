cmake_minimum_required(VERSION 3.30)
project(fsis LANGUAGES CXX VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/engine)
add_subdirectory(src/fsis)