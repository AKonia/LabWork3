cmake_minimum_required(VERSION 2.8)

include_directories(../include)

file(GLOB sources "*.cpp")
file(GLOB headers "*.h*") 

if(UNIX) 
	add_executable(../bin/${exename}  "${sources}" "${headers}")
else()
	add_executable(${exename}  "${sources}" "${headers}")
endif()
