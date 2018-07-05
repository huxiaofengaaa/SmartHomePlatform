/*
 * UeContextAndlink.hpp
 *
 *  Created on: 2018��7��4��
 *      Author: Administrator
 */

#pragma once

#include "UeContext.hpp"

struct UeContextAndlink
{
	UeContextAndlink() = default;
	~UeContextAndlink() = default;

	std::string host;
	int port;

	std::string deviceMac;         // device, register req
	std::string deviceType;        // device, register req
	std::string productToken;      // device, register req

	std::string gwToken;           // server generat, register resp
	std::string deviceId;          // server generat, register resp
	std::string deviceToken;       // server generat, register resp
	std::string andlinkToken;      // server generat, register resp

	std::string DevRND;            // device, online req
	std::string firmwareVersion;
	std::string softwareVersion;
	std::string ipAddress;
	std::string deviceVendor;
	std::string deviceModel;
	std::string deviceSn;
	std::string apUplinkType;
	bool radio5;
	std::string user_key;
	std::string SyncCode;

	int heartBeatTime;             // server setting,
	bool  encrypt;                 // server setting
	std::string ChallengeCode;     // server setting, 16 bytes
	std::string ServerIP;          // server setting, 0.0.0.0:1234

};
