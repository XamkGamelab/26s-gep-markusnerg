cmake_minimum_required(VERSION 3.28)

project(GEP26S
    VERSION 0.3.0
    DESCRIPTION "Game Engine Programming 26S"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ASSETS_INPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/assets")

add_subdirectory(vendor)
add_subdirectory(src)
