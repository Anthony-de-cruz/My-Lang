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

# --- LLVM ---
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)
# --- !LLVM ---

add_executable(My-Lang
    example/interactive_parser.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/llvm_components.cpp
)

target_link_libraries(My-Lang ${llvm_libs})

target_include_directories(
    My-Lang PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(My-Lang PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

