set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/src/SS_Deployment/include)
include_directories(${CMAKE_SOURCE_DIR}/library/cJSON)
include_directories(${CMAKE_BINARY_DIR}/library/glog/)


set(ANDLINK_CONTROL_SRCS AndlinkDeviceControlEvent/AndlinkDeviceWiFiParameterSyncReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceWPSReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceWiFiSwitchReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceRoamingConfigReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceRebootReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceRadioConfigReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceMacFilterReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceLEDControlReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceControlCommonResp.cpp)

set(ANDLINK_SRCS AndlinkDeviceEvent/AndlinkDeviceRegisterRequest.cpp
                 AndlinkDeviceEvent/AndlinkDeviceRegisterResponse.cpp
                 AndlinkDeviceEvent/AndlinkDeviceOnlineRequest.cpp
                 AndlinkDeviceEvent/AndlinkDeviceOnlineResponse.cpp
                 AndlinkDeviceEvent/AndlinkDeviceAuthRequest.cpp
                 AndlinkDeviceEvent/AndlinkDeviceAuthResponse.cpp
                 AndlinkDeviceEvent/AndlinkDeviceRequestPlugInRequest.cpp
                 AndlinkDeviceEvent/AndlinkDeviceRequestPlugInResponse.cpp
                 AndlinkDeviceEvent/AndlinkDeviceDisconnectRequest.cpp
                 AndlinkDeviceEvent/AndlinkDeviceDisconnectResponse.cpp
                 AndlinkDeviceEvent/AndlinkDeviceHeartbeatRequest.cpp
                 AndlinkDeviceEvent/AndlinkDeviceHeartbeatResponse.cpp)

add_library(eventResolve STATIC src/AndlinkDeviceEventHandler.cpp
                                ${ANDLINK_SRCS}
                                ${ANDLINK_CONTROL_SRCS})

