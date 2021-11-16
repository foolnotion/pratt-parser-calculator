cmake_minimum_required(VERSION 3.14)

project(pratt-parserTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/windows-set-path.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(pratt-parser REQUIRED)
  enable_testing()
endif()

add_executable(pratt-parser_test source/pratt-parser_test.cpp)
target_link_libraries(pratt-parser_test PRIVATE pratt-parser::pratt-parser)
target_compile_features(pratt-parser_test PRIVATE cxx_std_17)

add_test(NAME pratt-parser_test COMMAND pratt-parser_test)
