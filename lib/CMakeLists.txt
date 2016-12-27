cmake_minimum_required(VERSION 2.6)

file(GLOB sources "../gtest/*.c*")

include_directories(../include)
include_directories(../gtest)

add_library(gtest STATIC "${sources}")
