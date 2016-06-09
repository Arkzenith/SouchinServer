cmake_minimum_required(VERSION 3.5)
project(SouchinServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    add_definitions(-D__LINUX__)
    add_definitions(-fpermissive)
endif ()

if (UNIX)
    add_definitions(-D__UNIX__)
endif()

if (WIN32)
    add_definitions(-D__WIN32__)
endif()

set(SOURCE_FILES main.cpp SCServer.h Acception.h Procession.h ParamenterPtr.h)
add_executable(SouchinServer ${SOURCE_FILES})