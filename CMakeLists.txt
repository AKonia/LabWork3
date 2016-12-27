cmake_minimum_required(VERSION 2.8)

project(LabWork3_postfix_polish_notation)

set(exename "sampleapp")

set(CMAKE_CURRENT_BINARY_DIR /bin)
set(CMAKE_LIBRARY_PATH /lib)
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(BINARIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(LIBRARIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

include_directories("${INCLUDE_DIR}" ./gtest)

add_subdirectory(samples)
add_subdirectory(lib)
add_subdirectory(tests)
