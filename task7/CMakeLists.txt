cmake_minimum_required(VERSION 3.3)
project(task7)


set(SOURCE_FILES lazy_string.cpp main.cpp)
set(HEADER_FILES lazy_string.h)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
#add_library(lazy_string ${SOURCE_FILES} ${HEADER_FILES})

add_executable(main ${SOURCE_FILES})
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
#install(TARGETS lazy_string DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
#install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

