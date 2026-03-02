cmake_minimum_required(VERSION 3.20)
project(lex VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lexc
    src/main.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/parser/validator.cpp
    src/ast/ast.cpp
    src/codegen/lua_backend.cpp
    src/codegen/json_backend.cpp
)

target_include_directories(lexc PRIVATE ${CMAKE_SOURCE_DIR}/src)

if(MSVC)
    target_compile_options(lexc PRIVATE /W4)
else()
    target_compile_options(lexc PRIVATE -Wall -Wextra -pedantic)
endif()

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
