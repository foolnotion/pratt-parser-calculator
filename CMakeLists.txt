cmake_minimum_required(VERSION 3.12)

include(FetchContent)

project(
    expression-parser
    VERSION 1.0.0.0
    DESCRIPTION "An operator precedence parser based on the Pratt algorithm."
    LANGUAGES CXX
)

FetchContent_Declare(
    doctest
    URL https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h
    DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/tests/
    DOWNLOAD_NO_EXTRACT 1
)

FetchContent_MakeAvailable(doctest)

# Export compile commands
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

add_library(expression-parser INTERFACE)
target_include_directories(expression-parser INTERFACE include/)
target_compile_features(expression-parser INTERFACE cxx_std_17)

add_executable(calculator src/calculator.cpp)
target_link_libraries(calculator expression-parser)

add_executable(sexpr src/sexpr.cpp)
target_link_libraries(sexpr expression-parser)

add_executable(expression-parser-test tests/calculator.cpp)
target_include_directories(expression-parser-test PRIVATE src/)
target_link_libraries(expression-parser-test expression-parser)
