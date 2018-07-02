set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/src/SS_Deployment/include)
include_directories(${CMAKE_SOURCE_DIR}/library/cJSON)
add_library(eventResolve STATIC src/EventTypeStruct.cpp
                                src/EventTerminal.cpp
                                src/AndlinkDeviceEventHandler.cpp
                                AndlinkDeviceEvent/AndlinkDeviceRegisterRequest.cpp
                                AndlinkDeviceEvent/AndlinkDeviceRegisterResponse.cpp
                                AndlinkDeviceEvent/AndlinkDeviceOnlineRequest.cpp
                                AndlinkDeviceEvent/AndlinkDeviceOnlineResponse.cpp
                                AndlinkDeviceEvent/AndlinkDeviceAuthRequest.cpp
                                AndlinkDeviceEvent/AndlinkDeviceAuthResponse.cpp)
                           