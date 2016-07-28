cmake_minimum_required(VERSION 3.3)
project(format)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES format.cpp)
set(HEADER_FILES format.h)
add_library(format ${SOURCE_FILES} ${HEADER_FILES})

install(TARGETS format DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
