cmake_minimum_required(VERSION 3.21)
project(Fei_compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Fei_compiler compiler/main.cpp compiler/WordAnalyse.h compiler/WordAnalyse.cpp compiler/LR0.h compiler/LR0.cpp)
