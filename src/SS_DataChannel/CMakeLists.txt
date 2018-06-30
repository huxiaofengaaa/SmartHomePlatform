include_directories(include)
add_library(network STATIC src/ContainerTerminal.cpp
                           src/ContainerNetwork.cpp
                           src/SocketHandle.cpp)
                           