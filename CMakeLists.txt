cmake_minimum_required (VERSION 3.0)

project(lw-network CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES ./src/*.cc)
file(GLOB_RECURSE HEADERS ./src/*.h)

# this is the "object library" target: compiles the sources only once
add_library(LIB_OBJECT OBJECT ${SOURCES} ${HEADERS})

# shared libraries need PIC
set_property(TARGET ${objlib} PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(lw-network-shared SHARED $<TARGET_OBJECTS:LIB_OBJECT>)
add_library(lw-network-static STATIC $<TARGET_OBJECTS:LIB_OBJECT>)

set_target_properties(lw-network-static PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(lw-network-shared PROPERTIES LINKER_LANGUAGE CXX)
