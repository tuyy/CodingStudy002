cmake_minimum_required(VERSION 3.14)
project(CodingStudy002)

set(CMAKE_CXX_STANDARD 17)

add_executable(CodingStudy002 main.cpp main.cpp)

# Chapter02
add_executable(SpinLockVsMutex Chapter02/_01SpinLockVsMutex.cpp)
add_executable(ConditionalVariable Chapter02/_02ConditionalVariable.cpp)
add_executable(AtomicEtc Chapter02/_03AtomicEtc.cpp)
