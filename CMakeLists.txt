
cmake_minimum_required(VERSION 3.9)
project(SmartHomePlatform)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(library)
add_subdirectory(src)

