cmake_minimum_required(VERSION 3.26)

project(miniaudio)

add_library(miniaudio miniaudio.c miniaudio.h)

target_include_directories(miniaudio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
