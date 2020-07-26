cmake_minimum_required(VERSION "3.10")

project(Concurency_UT)

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../output/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../output/lib)

set (CMAKE_CXX_STANDARD 11)

set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

# Enable testing
enable_testing()

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/../../framework/googletest-master
	${CMAKE_CURRENT_SOURCE_DIR}/../../output/framework/googletest-master
	)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Concurency_UT
	thread_test.cpp
	${SOURCE_DIR}/my_thread.cpp
	)

target_link_libraries(Concurency_UT 
	gtest
	gtest_main
	)

add_test(NAME Concurency_UT COMMAND Concurency_UT)
