set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/library/glog/)

add_library(ueContext STATIC src/UeContextHolderAndlink.cpp
                             src/DataBaseStorage.cpp)
                           
