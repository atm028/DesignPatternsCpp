cmake_minimum_required(VERSION 3.1.1)

project(DesignPatternsCpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

add_subdirectory("src/dplib")

add_executable("tests" "tests/main.cpp")
add_library(libgtest STATIC IMPORTED)
SET_TARGET_PROPERTIES(libgtest PROPERTIES
    IMPORTED_LOCATION ./lib/libgtest.a)
target_link_libraries("tests" libgtest)
target_include_directories("tests" "PUBLIC" "./deps/googletest/googletest/include" "./tests" "src/dplib")

#add_executable("microservice" "src/microservice/service_main.cpp")
#add_library(libCppMicroServices STATIC IMPORTED)
#SET_TARGET_PROPERTIES(libCppMicroServices PROPERTIES
#    IMPORTED_LOCATION ./lib/libCppMicroServices.dylib)
#target_link_libraries("microservice" libCppMicroServices)
#target_include_directories("microservice" "PUBLIC" "./deps/CppMicroServices/include" "./tests" "src/microservice")


set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBRARY_OUTPUT_PATH "lib")

