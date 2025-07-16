cmake_minimum_required(VERSION 4.1)
project(fsis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_subdirectory(src/engine)
add_subdirectory(src/fsis)