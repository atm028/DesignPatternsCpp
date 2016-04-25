cmake_minimum_required(VERSION 3.1.1)

project(DesignPatternsCpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory("src/dplib")

add_executable("tests" "tests/main.cpp")
add_library(libgtest STATIC IMPORTED)
SET_TARGET_PROPERTIES(libgtest PROPERTIES
    IMPORTED_LOCATION ./lib/libgtest.a)
target_link_libraries("tests" libgtest)
target_include_directories("tests" "PUBLIC" "./deps/googletest/googletest/include" "./tests" "src/dplib")

set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBRARY_OUTPUT_PATH "lib")

