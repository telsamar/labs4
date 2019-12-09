cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
        HUNTER_CACHE_SERVERS
        "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
        CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
       URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
       SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)

project(source)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_library(header STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/Experiment.cpp
)

add_library(Cli STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/Cli.cpp
)

add_executable(main
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
)

add_executable(tests
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
)



target_include_directories(tests 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(header 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(Cli 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tests header GTest::main)
target_link_libraries(tests Cli GTest::main)
target_link_libraries(Cli header)
target_link_libraries(main header)
target_link_libraries(main Cli)

enable_testing()
add_test(NAME unit_tests COMMAND tests)

if (BUILD_COVERAGE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(codecov)
    add_coverage(tests)
    add_coverage(main)
    list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
    coverage_evaluate()
endif ()
