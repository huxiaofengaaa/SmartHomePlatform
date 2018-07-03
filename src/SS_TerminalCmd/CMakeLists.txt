set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/library/glog/)

add_library(terminalCmd STATIC src/TerminalCmdHelp.cpp)
                           
