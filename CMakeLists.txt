cmake_minimum_required(VERSION 3.22)
project(arxml_tool)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

find_package(GTest REQUIRED)
find_package(TinyXML2 REQUIRED)

list(APPEND CMAKE_CXX_FLAGS "-Werror -Wpedantic")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${TINYXML2_INCLUDE_DIR})

add_subdirectory(library)
add_subdirectory(apps)
add_subdirectory(tests)
