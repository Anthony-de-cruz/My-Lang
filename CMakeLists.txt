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

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(CTest)

add_subdirectory(tests)

add_executable(My-Lang
    example/hello_world.cpp
    src/lexer.cpp
)

target_include_directories(
    My-Lang PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(My-Lang PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
