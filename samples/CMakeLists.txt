cmake_minimum_required(VERSION 2.8)

include_directories(../include)
find_package( OpenCV REQUIRED )

file(GLOB sources "*.cpp")
file(GLOB headers "*.h*") 

if(UNIX) 
	add_executable(../bin/${exename}  "${sources}" "${headers}")
    target_link_libraries(../bin/${exename} ${OpenCV_LIBS})
else()
	add_executable(${exename}  "${sources}" "${headers}")
    target_link_libraries(../bin/${exename} ${OpenCV_LIBS})
endif()
