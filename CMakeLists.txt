cmake_minimum_required (VERSION 3.0)

project(lw_tcp_server CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        ${PROJECT_SOURCE_DIR}/src/Socket.cc
        ${PROJECT_SOURCE_DIR}/src/Socket.h
        ${PROJECT_SOURCE_DIR}/src/ReactiveSocket.cc
        ${PROJECT_SOURCE_DIR}/src/ReactiveSocket.h
        ${PROJECT_SOURCE_DIR}/src/Acceptor.cc
        ${PROJECT_SOURCE_DIR}/src/Acceptor.h
        ${PROJECT_SOURCE_DIR}/src/FDSet.cc
        ${PROJECT_SOURCE_DIR}/src/FDSet.h
        ${PROJECT_SOURCE_DIR}/src/EndPoint.cc
        ${PROJECT_SOURCE_DIR}/src/EndPoint.h
        ${PROJECT_SOURCE_DIR}/src/Protocol.cc
        ${PROJECT_SOURCE_DIR}/src/Protocol.h
        ${PROJECT_SOURCE_DIR}/src/Reactor.cc
        ${PROJECT_SOURCE_DIR}/src/Reactor.h
        ${PROJECT_SOURCE_DIR}/src/Resolver.cc
        ${PROJECT_SOURCE_DIR}/src/Resolver.h
        ${PROJECT_SOURCE_DIR}/src/socket_operations.cc
        ${PROJECT_SOURCE_DIR}/src/socket_operations.h
        ${PROJECT_SOURCE_DIR}/src/WinSockInit.h
        ${PROJECT_SOURCE_DIR}/src/Operation.h
        ${PROJECT_SOURCE_DIR}/src/Buffer.cc
        ${PROJECT_SOURCE_DIR}/src/Buffer.h
        ${PROJECT_SOURCE_DIR}/src/lw_network_error.h)

set(TEST_SOURCES
        ${PROJECT_SOURCE_DIR}/test/resolver_test.cc)

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
add_library(LIB_OBJECT OBJECT ${SOURCES})

# shared libraries need PIC
set_property(TARGET ${objlib} PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(lw_tcp_server_shared SHARED $<TARGET_OBJECTS:LIB_OBJECT>)
add_library(lw_tcp_server_static STATIC $<TARGET_OBJECTS:LIB_OBJECT>)

set_target_properties(lw_tcp_server_static PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(lw_tcp_server_shared PROPERTIES LINKER_LANGUAGE CXX)

enable_testing()
add_executable(lw_tcp_server_test ${TEST_SOURCES})
target_sources(lw_tcp_server_test PRIVATE ${SOURCES})
target_link_libraries(lw_tcp_server_test gmock_main)
add_test(NAME testing_lw_tcp_server_ COMMAND lw_tcp_server_test)

set(EXAMPLE_SERVER_SOURCES
        example/server/Server.cc
        example/server/Server.h
        example/server/Connection.cc
        example/server/Connection.h
        example/server/ConnectionManager.cc
        example/server/ConnectionManager.h
        ${SOURCES})

set(EXAMPLE_CLIENT_SOURCES
        example/client/Client.cc
        example/client/Client.h
        ${SOURCES})

# example server
add_executable(server example/server/main.cc)
target_sources(server PRIVATE ${EXAMPLE_SERVER_SOURCES})
target_include_directories(server PRIVATE ./src)

#example client
add_executable(client example/client/main.cc)
target_sources(client PRIVATE ${EXAMPLE_CLIENT_SOURCES})
target_include_directories(client PRIVATE ./src)

# example udplistener
add_executable(udplistener example/udplistener/main.cc)
target_include_directories(udplistener PRIVATE ./src)
target_sources(udplistener PRIVATE ${SOURCES})

#example udpsender
add_executable(udpsender example/udpsender/main.cc)
target_include_directories(udpsender PRIVATE ./src)
target_sources(udpsender PRIVATE ${SOURCES})
