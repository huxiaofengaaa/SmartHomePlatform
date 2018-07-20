set(CMAKE_CXX_FLAGS "-g -std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}")

include_directories(include)
include_directories(AndlinkDeviceInclude)
include_directories(cJSON)
include_directories(MD5)

set(ANDLINK_CONTROL_SRCS AndlinkDeviceControlEvent/AndlinkDeviceWiFiParameterSyncReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceWPSReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceWiFiSwitchReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceRoamingConfigReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceRebootReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceRadioConfigReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceMacFilterReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceLEDControlReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceUnbindReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceUpgradeReq.cpp
                         AndlinkDeviceControlEvent/AndlinkDeviceControlCommonResp.cpp)

set(ANDLINK_QUERY_SRCS AndlinkDeviceQueryEvent/AndlinkDeviceApConfigInfoQueryResp.cpp
                       AndlinkDeviceQueryEvent/AndlinkDeviceApConfigInfoQueryReq.cpp
                       AndlinkDeviceQueryEvent/AndlinkDeviceSTAInfoQueryResp.cpp
                       AndlinkDeviceQueryEvent/AndlinkDeviceSTAInfoQueryReq.cpp
                       AndlinkDeviceQueryEvent/AndlinkDeviceUplinkStatusQueryResp.cpp
                       AndlinkDeviceQueryEvent/AndlinkDeviceUplinkStatusQueryReq.cpp
                       AndlinkDeviceQueryEvent/AndlinkDeviceWiFiStatsQueryResp.cpp
                       AndlinkDeviceQueryEvent/AndlinkDeviceWiFiStatsQueryReq.cpp
                       AndlinkDeviceQueryEvent/AndlinkDeviceDataReportReq.cpp
                       AndlinkDeviceQueryEvent/AndlinkDeviceDataReportResp.cpp)

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

set(EXTERNAL_SOURCE cJSON/cJSON.c
                    MD5/MD5.c)

add_executable(SmartHomeDevice SmartHomeClient.cpp
                               src/RandomGenerator.cpp
                               src/ExcutionUnitClient.cpp
                               src/ExcutionUnitFsmAction.cpp
                               src/ExcutionUnitQueryChecker.cpp
                               src/ExcutionUnitControlChecker.cpp
                               src/UDPClient.cpp
                               src/TCPClient.cpp
                               src/FsmManager.cpp
                               src/DeviceDataStore.cpp
                               src/DeviceDataRunTime.cpp
                               src/DeviceDataReadOnly.cpp
                               ${EXTERNAL_SOURCE}
                               ${ANDLINK_SRCS}
                               ${ANDLINK_CONTROL_SRCS}
                               ${ANDLINK_QUERY_SRCS})

#target_link_libraries(SmartHomeDevice eventResolve pthread )