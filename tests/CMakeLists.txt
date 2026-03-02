cmake_minimum_required(VERSION 3.20)

find_package(Catch2 3 REQUIRED)

add_executable(lexer_tests
    test_main.cpp
    ${CMAKE_SOURCE_DIR}/src/lexer/lexer.cpp
    ${CMAKE_SOURCE_DIR}/src/parser/parser.cpp
    ${CMAKE_SOURCE_DIR}/src/ast/ast.cpp
)

target_include_directories(lexer_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(lexer_tests PRIVATE Catch2::Catch2WithMain)

target_compile_features(lexer_tests PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(lexer_tests PRIVATE /W4)
else()
    target_compile_options(lexer_tests PRIVATE -Wall -Wextra -pedantic)
endif()

include(CTest)
include(Catch)
catch_discover_tests(lexer_tests)
