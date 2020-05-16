cmake_minimum_required(VERSION 3.14)
project(proj4_89)

set(CMAKE_CXX_STANDARD 14)

set(
        SOURCE_FILES
        src/main.c
        src/HashTable.c
        src/Vector.cpp

        tests/testhash.cpp
        tests/testvector.cpp

        include/HashTable.h
        include/Vector.h

)



add_executable(proj4_89 ${SOURCE_FILES})
