include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/library/glog/src/)

add_library(sysMechanish STATIC src/SystemNotify.cpp
                                src/SignalRegister.cpp)
                           
