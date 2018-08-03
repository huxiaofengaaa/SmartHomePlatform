/*
 * UeContextAndlink.hpp
 *
 *  Created on: 2018��7��4��
 *      Author: Administrator
 */

#pragma once
#include <string>
#include <mutex>
#include "UeContext.hpp"

struct UeContextAndlink
{
	UeContextAndlink(std::string p_deviceID):deviceId(p_deviceID)
	{

	}
	virtual ~UeContextAndlink() = default;

	bool isDeviceOnline()
	{
		return isRegister && isOnline && isAuth;
	}

	bool isRegister = false;
	bool isOnline = false;
	bool isAuth = false;
	bool isPlugIn = false;

	// device heartbeat req
	long lastHeartbeat = 0;

	std::string peerUDPHost;// socket peer address, not device address
	int peerUDPPort = -1;
	int UDPSocketfd = -1;

	std::string peerTCPHost;// socket peer address, not device address
	int peerTCPPort;
	int TCPSocketfd = -1;

	const std::string deviceId;          // server generat, register resp

	// deviec register req
	std::string deviceMac;         // device, register req
	std::string deviceType;        // device, register req
	std::string productToken;      // device, register req

	// device register resp
	std::string gwToken;           // server generat, register resp
	std::string deviceToken;       // server generat, register resp
	std::string andlinkToken;      // server generat, register resp

	// device online req
	std::string DevRND;            // device, online req
	std::string firmwareVersion;
	std::string softwareVersion;
	std::string ipAddress;
	std::string deviceVendor;
	std::string deviceModel;
	std::string deviceSn;
	std::string apUplinkType;
	int radio5 = 0;
	std::string user_key;
	std::string SyncCode;

	// device online resp
	int heartBeatTime;             // server setting,
	int encrypt = 0;               // server setting
	std::string ChallengeCode;     // server setting, 16 bytes

};
