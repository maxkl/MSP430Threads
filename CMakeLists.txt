cmake_minimum_required(VERSION 3.7)
project(MSP430Threads C ASM)

add_subdirectory("${CMAKE_SOURCE_DIR}/src/")

add_executable(MSP430Threads ${sources})
