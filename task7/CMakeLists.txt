cmake_minimum_required(VERSION 3.3)
project(lazy_string)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES lazy_string.cpp)
set(HEADER_FILES lazy_string.h)
add_library(lazy_string ${SOURCE_FILES} ${HEADER_FILES})

install(TARGETS lazy_string DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)