cmake_minimum_required(VERSION 3.16.0)

project(DISCRETE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

link_directories(${PROJECT_SOURCE_DIR}/build/lib)

add_subdirectory(src)
add_subdirectory(test)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)