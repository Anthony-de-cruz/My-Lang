if (BUILD_TESTING)
    add_executable(tests
        main.cpp
        unit/lexer.cpp
        unit/parser.cpp
    )

    # --- LLVM ---
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

    include_directories(${LLVM_INCLUDE_DIRS})
    separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
    add_definitions(${LLVM_DEFINITIONS_LIST})

    # # Find the libraries that correspond to the LLVM components
    # # that we wish to use
    llvm_map_components_to_libnames(llvm_libs support core irreader)
    # --- !LLVM---

    target_link_libraries(tests PRIVATE GTest::gtest_main ${llvm_libs})
    target_sources(tests PRIVATE ${PROJECT_SOURCE_DIR}/src/lexer.cpp)
    target_sources(tests PRIVATE ${PROJECT_SOURCE_DIR}/src/parser.cpp)
    target_sources(tests PRIVATE ${PROJECT_SOURCE_DIR}/src/ir_builder.cpp)
    target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
    find_package(LLVM REQUIRED CONFIG)

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()
