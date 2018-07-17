
cmake_minimum_required(VERSION 3.9)
project(SmartHomePlatform)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "-g -std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}")

add_subdirectory(library)
add_subdirectory(src)
add_subdirectory(Client)

