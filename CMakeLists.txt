cmake_minimum_required(VERSION 3.9)
project(libSSD1306 CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_library(SSD1306 STATIC 
	lib/oled.cpp
	lib/framebuffer.cpp
	lib/bitmap.cpp
	lib/graphics.cpp)

include_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(test samples/test.cpp)
target_link_libraries(test SSD1306)