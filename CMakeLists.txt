cmake_minimum_required(VERSION 3.20)
project(lex VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Dependencies
# ============================================================================

# Try vcpkg first, then FetchContent as fallback
find_package(CLI11 CONFIG QUIET)

if(NOT CLI11_FOUND)
    message(STATUS "CLI11 not found via vcpkg, using FetchContent")
    include(FetchContent)
    FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.4.2
    )
    FetchContent_MakeAvailable(CLI11)
endif()

# ============================================================================
# Packages (Premium Extensions)
# ============================================================================

# Auto-include any packages in the packages/ directory
# This allows Lex-Plus and other extensions to be added without modifying this file
set(LEX_PLUS_FOUND OFF)

if(EXISTS "${CMAKE_SOURCE_DIR}/packages")
    file(GLOB package_dirs "${CMAKE_SOURCE_DIR}/packages/*")
    foreach(pkg_dir ${package_dirs})
        if(IS_DIRECTORY ${pkg_dir})
            if(EXISTS "${pkg_dir}/CMakeLists.txt")
                get_filename_component(pkg_name ${pkg_dir} NAME)
                message(STATUS "Found package: ${pkg_name}")
                add_subdirectory(${pkg_dir})
            endif()
        endif()
    endforeach()
endif()

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
    src/codegen/backend_registry.cpp
    src/codegen/lua_backend.cpp
    src/codegen/json_backend.cpp
    src/codegen/godot_backend.cpp
    src/codegen/unity_backend.cpp
    src/codegen/love2d_backend.cpp
    src/codegen/defold_backend.cpp
    src/schema/schema.cpp
    src/context/context_generator.cpp
    src/context/query.cpp
    src/compiler/pass.cpp
    src/compiler/pipeline.cpp
    src/license/license.cpp
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
target_link_libraries(lexc PRIVATE lex CLI11::CLI11)

# Link premium package if found
if(LEX_PLUS_FOUND)
    target_link_libraries(lexc PRIVATE lex_plus)
    target_include_directories(lexc PRIVATE ${CMAKE_SOURCE_DIR}/packages/Lex-Plus/src)
    message(STATUS "Linking Lex-Plus to executable")
endif()

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
