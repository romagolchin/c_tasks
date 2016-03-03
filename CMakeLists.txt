cmake_minimum_required(VERSION 3.3)
project(hw2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_C_FLAGS "-std=c99")

#set_property(TARGET hw2 PROPERTY C_STANDARD 99)

set(SOURCE_FILES
    #demo.c
    main.c
    #main1.c
    phonebook
    #test
        )

add_executable(hw2 ${SOURCE_FILES})