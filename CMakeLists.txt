cmake_minimum_required(VERSION 3.5)
project(SouchinServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

if (__SYSTEM__ STREQUAL LINUX)
    add_definitions(-D__LINUX__)
endif ()

set(SOURCE_FILES main.cpp SCServer.h Acception.h Procession.h ParamenterPtr.h)
add_executable(SouchinServer ${SOURCE_FILES})