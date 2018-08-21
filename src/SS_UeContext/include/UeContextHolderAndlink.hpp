/*
 * UeContextHolderAndlink.hpp
 *
 *  Created on: 2018��7��4��
 *      Author: Administrator
 */

#pragma once

#include <iostream>
#include "UeContextHolder.hpp"
#include "UeContextAndlink.hpp"
#include "glog/logging.h"
#include "ConfigParser.hpp"
#include "DataBaseStorage.hpp"

class UeContextHolderAndlink: public UeContextHolder<std::shared_ptr<UeContextAndlink>>,
                              public DataBaseStorage
{
public:
	UeContextHolderAndlink(std::string p_host, int p_port, std::shared_ptr<ConfigParser> p_config);
	~UeContextHolderAndlink(){ }

	std::string getDeviceIDByMAC(std::string p_deviceMac);
	std::string getDeviceIDByTCPAddress(std::string p_host, int p_port);
	std::string getDeviceIDByUDPAddress(std::string p_host, int p_port);

	int onlineSize();
	int pluginSize();

	bool shouldDeviceEncrypt(std::string p_deviceID);

	std::vector<std::string> getDeviceList();
	bool updateNetAddress(std::string p_deviceID, std::string p_host, int p_port, int p_sockfd, bool isTCP);
	bool pluginDisconnect(int p_sockfd, std::string p_host, int p_port);
	bool pluginResetStatus(std::string p_deviceID);

	std::string DeviceRegister(std::string deviceMac, std::string deviceType, std::string productToken);
	bool DeviceOnline(struct Interface56_Online_Req& onlinereq);
	bool DeviceAuth(std::string p_deviceMAC, std::string p_CheckSN);
	bool DeviceHeartbeat(std::string p_deviceId, std::string p_deviceMAC, std::string p_IPAddr);

	bool setRegisterResponse(std::string p_deviceID, struct Interface56_Register_Resp& resp);
	bool setOnlineResponse(std::string p_deviceID, struct Interface56_Online_Resp& resp, bool isSuccess);
	bool setAuthResponse(bool p_result, struct Interface56_Auth_Resp& resp);
	bool setHeartbeatResponse(bool p_result, struct Interface56_Heartbeat_Resp& resp);

private:
	std::string generatorDeviceID();
	std::string generatorGwToken();
	std::string generatorDeviceToken();
	std::string generatorAndlinkToken();

	bool recoveryAllDataFromRemoteServer()
	{
		//TODO
		return true;
	}

	int m_defaultEncryptType;

	const std::string m_host;
	const int m_port;
	std::shared_ptr<ConfigParser> m_config;
};
