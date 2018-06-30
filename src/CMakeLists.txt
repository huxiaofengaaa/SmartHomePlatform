set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(SS_Deployment/include)
include_directories(SS_DataChannel/include)

add_subdirectory(SS_DataChannel)

add_executable(SmartHomePlatform SmartHomeMain.cpp
                                 SS_Deployment/src/SmartHomeDeployment.cpp
                                 SS_Deployment/src/ExcutionUnitUDPNetwork.cpp
                                 SS_Deployment/src/ExcutionUnitTerminal.cpp)
target_link_libraries(SmartHomePlatform cJSON network pthread)

add_executable(SmartHomeDevice SmartHomeClient.cpp)