cmake_minimum_required(VERSION 3.24)

project(
    DemoLivecoding
    VERSION 1.0
    DESCRIPTION "C++ Year 2025 DemoLivecoding1"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} main.cpp)