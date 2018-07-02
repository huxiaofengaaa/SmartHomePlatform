include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/src/SS_Deployment/include)
add_library(eventResolve STATIC src/EventTypeStruct.cpp
                                src/EventTerminal.cpp)
                           