cmake_minimum_required(VERSION 3.20.0)

project(Cpp-Heap
    DESCRIPTION "The objectively perfect (not object-oriented) programming language. "
    LANGUAGES CXX
    VERSION 0.0.0.1
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(My-Lang
    example/hello_world.cpp
    src/lexer.cpp
)

target_include_directories(
    My-Lang PRIVATE ${PROJECT_SOURCE_DIR}/src,
    My-Lang PRIVATE ${PROJECT_SOURCE_DIR}/include
)

