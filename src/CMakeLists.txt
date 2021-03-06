set(CMAKE_CXX_FLAGS "-g -std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_BINARY_DIR}/library/glog/)
include_directories(${CMAKE_SOURCE_DIR}/library/glog/src)
include_directories(${CMAKE_SOURCE_DIR}/library/cJSON)
include_directories(${CMAKE_SOURCE_DIR}/library/SecurityAlgorithm/include)
include_directories(${CMAKE_SOURCE_DIR}/library/ConfigParser)
include_directories(${CMAKE_SOURCE_DIR}/library/hiredis/include)
include_directories(SS_Deployment/include)
include_directories(SS_EventResolve/include)
include_directories(SS_SysMechanish/include)
include_directories(SS_TerminalCmd/include)
include_directories(SS_UeContext/include)

add_subdirectory(SS_EventResolve)
add_subdirectory(SS_SysMechanish)
add_subdirectory(SS_TerminalCmd)
add_subdirectory(SS_UeContext)

add_dependencies(sysMechanish glog)
add_dependencies(terminalCmd glog)
add_dependencies(eventResolve glog)
add_dependencies(eventResolve cJSON)
add_dependencies(ueContext glog)

add_executable(SmartHomePlatform SmartHomeMain.cpp
                                 SS_Deployment/src/SmartHomeDeployment.cpp
                                 SS_Deployment/src/ExcutionUnitAndlink.cpp
                                 SS_Deployment/src/ExcutionUnitAndlinkPlugIn.cpp
                                 SS_Deployment/src/ExcutionUnitAndlinkPlugInControl.cpp
                                 SS_Deployment/src/ExcutionUnitTerminal.cpp
                                 SS_Deployment/src/ExcutionUnitTerminalCmdCallback.cpp)
                                 
target_link_libraries(SmartHomePlatform terminalCmd eventResolve ueContext sysMechanish pthread 
                                        glog cJSON SecurityAlgorithm ConfigParser hiredis)

#add_executable(SmartHomeDevice SmartHomeClient.cpp)
