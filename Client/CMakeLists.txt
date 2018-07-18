include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/src/SS_EventResolve/include)
include_directories(${CMAKE_SOURCE_DIR}/library/cJSON)
include_directories(${CMAKE_SOURCE_DIR}/src/SS_SysMechanish/include)

set(ANDLINK_CONTROL_SRCS ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceWiFiParameterSyncReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceWPSReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceWiFiSwitchReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceRoamingConfigReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceRebootReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceRadioConfigReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceMacFilterReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceLEDControlReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceUnbindReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceUpgradeReq.cpp
                         ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceControlEvent/AndlinkDeviceControlCommonResp.cpp)

set(ANDLINK_QUERY_SRCS ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceApConfigInfoQueryResp.cpp
                       ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceApConfigInfoQueryReq.cpp
                       ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceSTAInfoQueryResp.cpp
                       ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceSTAInfoQueryReq.cpp
                       ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceUplinkStatusQueryResp.cpp
                       ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceUplinkStatusQueryReq.cpp
                       ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceWiFiStatsQueryResp.cpp
                       ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceWiFiStatsQueryReq.cpp
                       ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceDataReportReq.cpp
                       ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceQueryEvent/AndlinkDeviceDataReportResp.cpp)

set(ANDLINK_SRCS ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceRegisterRequest.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceRegisterResponse.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceOnlineRequest.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceOnlineResponse.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceAuthRequest.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceAuthResponse.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceRequestPlugInRequest.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceRequestPlugInResponse.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceDisconnectRequest.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceDisconnectResponse.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceHeartbeatRequest.cpp
                 ${CMAKE_SOURCE_DIR}/src/SS_EventResolve/AndlinkDeviceEvent/AndlinkDeviceHeartbeatResponse.cpp)

set(EXTERNAL_SOURCE ${CMAKE_SOURCE_DIR}/src/SS_SysMechanish/src/RandomGenerator.cpp
                    ${CMAKE_SOURCE_DIR}/library/cJSON/cJSON.c)

add_executable(SmartHomeDevice SmartHomeClient.cpp
                               src/ExcutionUnitClient.cpp
                               src/ExcutionUnitFsmAction.cpp
                               src/UDPClient.cpp
                               src/TCPClient.cpp
                               src/FsmManager.cpp
                               src/DeviceDataStore.cpp
                               ${EXTERNAL_SOURCE}
                               ${ANDLINK_SRCS})

#target_link_libraries(SmartHomeDevice eventResolve pthread )