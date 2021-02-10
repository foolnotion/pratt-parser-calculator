cmake_minimum_required(VERSION 3.8)

include(ExternalProject)
find_package(Git REQUIRED)

project(
    expression-parser
    VERSION 1.0.0.0
    DESCRIPTION "An operator precedence parser based on the Pratt algorithm."
    LANGUAGES CXX
)

ExternalProject_Add(
    doctest
    PREFIX ${CMAKE_BINARY_DIR}/doctest
    GIT_REPOSITORY https://github.com/onqtam/doctest.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(doctest source_dir)
set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")

# Export compile commands
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

add_library(expression-parser INTERFACE)
target_include_directories(expression-parser INTERFACE include/)
target_compile_features(expression-parser INTERFACE cxx_std_17)

add_executable(expression-parser-test tests/main.cpp)
target_include_directories(expression-parser-test PRIVATE ${DOCTEST_INCLUDE_DIR})
target_link_libraries(expression-parser-test expression-parser)
