cmake_minimum_required(VERSION 3.20)

# Fetch Catch2 if not found via vcpkg
find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND)
    message(STATUS "Catch2 not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.3
    )
    FetchContent_MakeAvailable(Catch2)
endif()

add_executable(lexer_tests
    test_main.cpp
    ${CMAKE_SOURCE_DIR}/src/lexer/lexer.cpp
    ${CMAKE_SOURCE_DIR}/src/parser/parser.cpp
    ${CMAKE_SOURCE_DIR}/src/parser/validator.cpp
    ${CMAKE_SOURCE_DIR}/src/ast/ast.cpp
    ${CMAKE_SOURCE_DIR}/src/semantic/type_checker.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/lua_backend.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/json_backend.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/godot_backend.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/unity_backend.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/love2d_backend.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/defold_backend.cpp
    ${CMAKE_SOURCE_DIR}/src/schema/schema.cpp
    ${CMAKE_SOURCE_DIR}/src/codegen/backend_registry.cpp
    ${CMAKE_SOURCE_DIR}/src/compiler/pass.cpp
    ${CMAKE_SOURCE_DIR}/src/compiler/pipeline.cpp
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
