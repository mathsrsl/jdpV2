cmake_minimum_required(VERSION 3.26)
project(jdpV2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(jdpV2
        main.c
        menu.c
        menu.h)
