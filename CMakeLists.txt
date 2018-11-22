cmake_minimum_required(VERSION 3.9)
project(libSSD1306 CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_library(SSD1306 STATIC 
	lib/oled.cpp
	lib/font.cpp
	lib/framebuffer.cpp
	lib/bitmap.cpp
	lib/graphics.cpp
	lib/complex/clock.cpp)

include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/lib/complex)

if(BUILD_SAMPLES)
	message("Building with samples.")

	add_executable(test samples/test.cpp)
	target_link_libraries(test SSD1306)

	add_executable(test_clock samples/test_clock.cpp)
	target_link_libraries(test_clock SSD1306)

	add_executable(test_font samples/test_font.cpp)
	target_link_libraries(test_font SSD1306)

	add_custom_command(
        TARGET test_font POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/lib/fonts/OledFont8x8.oledfont
                ${CMAKE_CURRENT_BINARY_DIR}/OledFont8x8.oledfont)
	add_custom_command(
        TARGET test_font POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/lib/fonts/OledFont8x16.oledfont
                ${CMAKE_CURRENT_BINARY_DIR}/OledFont8x16.oledfont)

	add_executable(test_bitmap samples/test_bitmap.cpp)
	target_link_libraries(test_bitmap SSD1306)

	add_custom_command(
        TARGET test_bitmap POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/lib/bitmaps/bitSplash.bitmap
                ${CMAKE_CURRENT_BINARY_DIR}/bitSplash.bitmap)
	add_custom_command(
        TARGET test_bitmap POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/lib/bitmaps/byteSplash.bitmap
                ${CMAKE_CURRENT_BINARY_DIR}/byteSplash.bitmap)
endif()