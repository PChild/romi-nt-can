# Project's name
PROJECT ( romi-nt-can C CXX )

# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.1)

add_definitions(-std=c++20)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package (Threads REQUIRED)

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(romi-nt-can ${PROJECT_SOURCE_DIR}/romi-nt-can.cpp)

if (${CMAKE_LIBRARY_ARCHITECTURE} STREQUAL "arm-linux-gnueabihf") # Jetson TK1 / Pi
    set(LIBS_DIR ${CMAKE_SOURCE_DIR}/lib/arm32)
elseif (${CMAKE_LIBRARY_ARCHITECTURE} STREQUAL "aarch64-linux-gnu") # Jetson TX2
    set(LIBS_DIR ${CMAKE_SOURCE_DIR}/lib/arm64)
else()
    set(LIBS_DIR ${CMAKE_SOURCE_DIR}/lib/x86-64)
endif()

target_link_libraries(romi-nt-can ${LIBS_DIR}/libCTRE_Phoenix.so)
target_link_libraries(romi-nt-can ${LIBS_DIR}/libCTRE_PhoenixCCI.so)
target_link_libraries(romi-nt-can ${LIBS_DIR}/libCTRE_PhoenixTools.so)

target_link_libraries(romi-nt-can ${LIBS_DIR}/libntcore.so)
target_link_libraries(romi-nt-can ${LIBS_DIR}/libwpiutil.so)

target_link_libraries(romi-nt-can Threads::Threads)

add_custom_command(TARGET romi-nt-can POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${LIBS_DIR}
        $<TARGET_FILE_DIR:romi-nt-can>)
