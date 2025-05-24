cmake_minimum_required(VERSION 3.10)

#nom sous projet
project(SDLUtils C)

set(CMAKE_C_FLAGS "-std=c99 -Wall")

#création de la librairie
add_library(SDLUtils SDLUtils.c SDLUtils.h SDLMenu.c SDLMenu.h )

