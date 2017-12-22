cmake_minimum_required(VERSION 3.6)
project(exponential-backoff)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        System/Packet.cpp
        System/Packet.h
        MyRandom.cpp
        MyRandom.h
        System/System.cpp
        System/System.h
        AdvancedSystem/AdvancedSystem.cpp
        AdvancedSystem/AdvancedSystem.h
        System/Subscriber.cpp
        System/Subscriber.h
        log.h
        log.cpp)
add_executable(exponential-backoff ${SOURCE_FILES})
