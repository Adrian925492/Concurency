cmake_minimum_required(VERSION "3.10")

project(Concurency_)

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../output/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../output/lib)

set (CMAKE_CXX_STANDARD 11)

add_executable(Concurency_ 
	main.cpp 
	my_thread.cpp 
	)

