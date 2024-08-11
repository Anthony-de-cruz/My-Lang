if (BUILD_TESTING)
    add_executable(tests
        main.cpp
        unit/lexer.cpp
        unit/parser.cpp
    )
    target_link_libraries(tests PRIVATE GTest::gtest_main)
    target_sources(tests PRIVATE ${PROJECT_SOURCE_DIR}/src/lexer.cpp)
    target_sources(tests PRIVATE ${PROJECT_SOURCE_DIR}/src/parser.cpp)
    target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()
