cmake_minimum_required(VERSION 3.1)
project(Vision)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES
	vision.h
	camera.h
	matrix.h
	contours.h
)

set(SOURCE_FILES
	main.cpp
	camera.cpp
	matrix.cpp
	contours.cpp
)

add_executable(Vision ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Vision ${OpenCV_LIBRARIES})