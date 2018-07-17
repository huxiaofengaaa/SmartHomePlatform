#include_directories(${CMAKE_BINARY_DIR}/library/glog/)
#include_directories(${CMAKE_SOURCE_DIR}/library/glog/src)
#include_directories(${CMAKE_SOURCE_DIR}/library/cJSON)
#include_directories(SS_Deployment/include)
#include_directories(SS_EventResolve/include)
#include_directories(SS_SysMechanish/include)
#include_directories(SS_TerminalCmd/include)
#include_directories(SS_UeContext/include)
                                 
#target_link_libraries(SmartHomePlatform terminalCmd eventResolve ueContext sysMechanish pthread glog cJSON)


include_directories(include)
add_executable(SmartHomeDevice SmartHomeClient.cpp
                               src/ExcutionUnitClient.cpp
                               src/ExcutionUnitFsmAction.cpp
                               src/UDPClient.cpp
                               src/TCPClient.cpp
                               src/FsmManager.cpp)
