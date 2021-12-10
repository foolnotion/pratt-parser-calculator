cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    pratt-parser
    VERSION 0.1.0
    DESCRIPTION "Pratt parser"
    HOMEPAGE_URL "https://github.com/foolnotion/pratt-parser-calculator"
    LANGUAGES CXX 
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

docs_early_return()

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Always export compile commands ----
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

# ---- Declare library ----

add_library(pratt-parser_pratt-parser INTERFACE)
add_library(pratt-parser::pratt-parser ALIAS pratt-parser_pratt-parser)

set_property(
    TARGET pratt-parser_pratt-parser PROPERTY
    EXPORT_NAME pratt-parser
)

target_include_directories(
    pratt-parser_pratt-parser ${pratt-parser_warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(pratt-parser_pratt-parser INTERFACE cxx_std_17)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${pratt-parser_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT pratt-parser_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of pratt-parser"
  )
endif()

include(cmake/dev-mode.cmake)
