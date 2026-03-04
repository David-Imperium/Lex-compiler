cmake_minimum_required(VERSION 3.20)
project(lex VERSION 0.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Lex Library
# ============================================================================

add_library(lex STATIC
    src/lex/lex.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/parser/validator.cpp
    src/ast/ast.cpp
    src/semantic/type_checker.cpp
    src/codegen/lua_backend.cpp
    src/codegen/json_backend.cpp
    src/codegen/godot_backend.cpp
    src/codegen/unity_backend.cpp
    src/codegen/love2d_backend.cpp
    src/codegen/defold_backend.cpp
    src/schema/schema.cpp
    src/context/context_generator.cpp
)

target_include_directories(lex
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(lex PRIVATE /W4)
else()
    target_compile_options(lex PRIVATE -Wall -Wextra -pedantic)
endif()

# ============================================================================
# Lex CLI (uses the library)
# ============================================================================

add_executable(lexc src/cli/main.cpp)
target_link_libraries(lexc PRIVATE lex)

# ============================================================================
# Tests
# ============================================================================

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ============================================================================
# Installation
# ============================================================================

include(GNUInstallDirs)

install(TARGETS lex
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES src/lex/lex.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lex)
