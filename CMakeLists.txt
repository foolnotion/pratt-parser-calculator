cmake_minimum_required(VERSION 3.12)

project(
    expression-parser
    VERSION 1.0.0.0
    DESCRIPTION "An operator precedence parser based on the Pratt algorithm."
    LANGUAGES CXX
)

include(CTest)
include(FetchContent)

FetchContent_Declare(
    doctest
    URL https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h
    DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/tests/
    DOWNLOAD_NO_EXTRACT 1
)

FetchContent_Declare(
    fast_float
    GIT_REPOSITORY https://github.com/fastfloat/fast_float
    GIT_TAG main
    DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/thirdparty/fast_float
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/fast_float
)

FetchContent_Declare(
    robin_hood
    URL https://raw.githubusercontent.com/martinus/robin-hood-hashing/master/src/include/robin_hood.h
    DOWNLOAD_NO_EXTRACT 1
    DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/thirdparty/robin_hood
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/robin_hood
)

FetchContent_MakeAvailable(doctest fast_float robin_hood)

set(THIRDPARTY_INCLUDE_DIRS
    "${fast_float_SOURCE_DIR}/include"
    "${robin_hood_SOURCE_DIR}")

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(PRATT_PARSER_INCLUDE_DIRS
        "${PROJECT_SOURCE_DIR}/include"
        "${THIRDPARTY_INCLUDE_DIRS}" PARENT_SCOPE)
endif()

# Export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

add_library(expression-parser INTERFACE)
target_include_directories(expression-parser INTERFACE ./include ${THIRDPARTY_INCLUDE_DIRS})
target_compile_features(expression-parser INTERFACE cxx_std_17)

add_executable(calculator src/calculator.cpp)
target_link_libraries(calculator expression-parser)

add_executable(sexpr src/sexpr.cpp)
target_link_libraries(sexpr expression-parser)

add_executable(expression-parser-test tests/calculator.cpp)
target_include_directories(expression-parser-test PRIVATE src/)
target_link_libraries(expression-parser-test expression-parser)

add_test(NAME expression-parser-test COMMAND expression-parser-test)
enable_testing()
