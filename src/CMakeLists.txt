set(CMAKE_CXX_FLAGS "-g -std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR}/library/glog/src/)
include_directories(SS_Deployment/include)
include_directories(SS_DataChannel/include)
include_directories(SS_EventResolve/include)
include_directories(SS_SysMechanish/include)

add_subdirectory(SS_DataChannel)
add_subdirectory(SS_EventResolve)
add_subdirectory(SS_SysMechanish)

add_executable(SmartHomePlatform SmartHomeMain.cpp
                                 SS_Deployment/src/SmartHomeDeployment.cpp
                                 SS_Deployment/src/ExcutionUnitUDPNetwork.cpp
                                 SS_Deployment/src/ExcutionUnitTerminal.cpp)
                                 
target_link_libraries(SmartHomePlatform network eventResolve pthread  sysMechanish glog cJSON)

#add_executable(SmartHomeDevice SmartHomeClient.cpp)
