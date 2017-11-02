cmake_minimum_required (VERSION 3.0)

project(lw_tcp_server CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE SOURCES ./src/*.cc)
file(GLOB_RECURSE HEADERS ./src/*.h)
file(GLOB_RECURSE TEST_SOURCES ./test/*.cc)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
        "${CMAKE_BINARY_DIR}/googletest-build")

include_directories("${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")

include_directories(./src)

# this is the "object library" target: compiles the sources only once
add_library(LIB_OBJECT OBJECT ${SOURCES} ${HEADERS})

# shared libraries need PIC
set_property(TARGET ${objlib} PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(lw_tcp_server_shared SHARED $<TARGET_OBJECTS:LIB_OBJECT>)
add_library(lw_tcp_server_static STATIC $<TARGET_OBJECTS:LIB_OBJECT>)

set_target_properties(lw_tcp_server_static PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(lw_tcp_server_shared PROPERTIES LINKER_LANGUAGE CXX)

enable_testing()
add_executable(lw_tcp_server_test ${TEST_SOURCES} ${SOURCES})
target_link_libraries(lw_tcp_server_test gmock_main)
add_test(NAME testing_lw_tcp_server_ COMMAND lw_tcp_server_test)