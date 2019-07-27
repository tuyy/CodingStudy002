cmake_minimum_required(VERSION 3.14)
project(CodingStudy002)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -fpic -Wno-non-virtual-dtor -pthread")

add_executable(CodingStudy002 main.cpp)

# Chapter02
add_executable(SpinLockVsMutex Chapter02/_01SpinLockVsMutex.cpp)
add_executable(ConditionalVariable Chapter02/_02ConditionalVariable.cpp)
add_executable(AtomicEtc Chapter02/_03AtomicEtc.cpp)

# Chapter03
add_executable(CreateThread Chapter03/_01CreateThread.cpp)
add_executable(Mutex Chapter03/_02Mutex.cpp)
add_executable(UniqueLock Chapter03/_03UniqueLock.cpp)
add_executable(SharedLock Chapter03/_04SharedLock.cpp)
add_executable(OneceFlag Chapter03/_05OneceFlag.cpp)
add_executable(ThreadLocalData Chapter03/_06ThreadLocalData.cpp)
add_executable(ConVal Chapter03/_07ConVal.cpp)
add_executable(Task Chapter03/_08task.cpp)
