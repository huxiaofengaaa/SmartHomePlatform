set(CMAKE_CXX_FLAGS "-g -std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR}/library/glog/src/)
include_directories(SS_Deployment/include)
include_directories(SS_EventResolve/include)
include_directories(SS_SysMechanish/include)
include_directories(SS_TerminalCmd/include)

add_subdirectory(SS_DataChannel)
add_subdirectory(SS_EventResolve)
add_subdirectory(SS_SysMechanish)
add_subdirectory(SS_TerminalCmd)

add_dependencies(sysMechanish glog)
add_dependencies(terminalCmd glog)

add_executable(SmartHomePlatform SmartHomeMain.cpp
                                 SS_Deployment/src/SmartHomeDeployment.cpp
                                 SS_Deployment/src/SmartHomeDeploymentTerminalCmd.cpp
                                 SS_Deployment/src/ExcutionUnitAndlink.cpp
                                 SS_Deployment/src/ExcutionUnitTerminal.cpp)
                                 
target_link_libraries(SmartHomePlatform terminalCmd network eventResolve pthread  sysMechanish glog cJSON)

#add_executable(SmartHomeDevice SmartHomeClient.cpp)
