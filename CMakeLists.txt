cmake_minimum_required(VERSION 3.3)
project(rational)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES rational.cpp)
set(HEADER_FILES rational.h)
add_library(rational ${SOURCE_FILES} ${HEADER_FILES})
#add_executable(rational ${SOURCE_FILES})

