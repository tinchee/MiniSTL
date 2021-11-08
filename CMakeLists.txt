cmake_minimum_required(VERSION 3.5)

project(test)

set(SOURCES
	test/test.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test ${SOURCES})