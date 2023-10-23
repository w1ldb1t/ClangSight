cmake_minimum_required(VERSION 3.2)

# Project statement
project(
        ClangSight
        VERSION 1.0
        LANGUAGES CXX)

# Set the C++ version
set (CMAKE_CXX_STANDARD 20)

# Generate compile commands for intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the available build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Add dependencies
include(cmake/CPM.cmake)

# Add individual third party libraries
CPMAddPackage(
        NAME nlohmann_json
        VERSION 3.11.2
        GITHUB_REPOSITORY nlohmann/json
        OPTIONS
        "JSON_BuildTests OFF"
)
CPMAddPackage(
        NAME argparse
        GIT_TAG v2.9
        GITHUB_REPOSITORY p-ranav/argparse
)

# Core source files
set(SOURCES
        src/core/main.cc
        src/core/code_file_iterator.cc
        src/clangd/clangd_process.cc
        src/clangd/clangd_client.cc
        src/clangd/request_database.cc
        src/process/process_handle.cc
        src/ast/ast_misc.cc
        src/ast/function_declaration_ast_visitor.cc
        src/ast/struct_declaration_ast_visitor.cc
)

# Append platform-specific files on the SOURCES list
if(WIN32)
    list(APPEND SOURCES src/process/windows_process.cc)
else()
    list(APPEND SOURCES src/process/posix_process.cc)
endif()

# Adding something we can run - Output name matches target name
add_executable(${PROJECT_NAME} ${SOURCES})

# Add the root directory to includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link the third party libraries
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json argparse::argparse)

target_include_directories(${PROJECT_NAME} PUBLIC ${argparse_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${nlohmann_json_SOURCE_DIR}/include)

# Conditional compilation based on the build type
if(CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE IS_RELEASE)
    if(MSVC)
        string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
        target_compile_options(${PROJECT_NAME} PRIVATE /O2)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O2)
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE IS_DEBUG)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /Od /Zi)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g)
    endif()
endif()