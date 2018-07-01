#pragma once

#include <string>

struct EventClientRegisterReq
{
	std::string deviceMac;
	std::string deviceType;
	std::string productToken;
	long timestamp;
};

struct EventClientOnlineReq
{
	std::string RPCMethod;
	std::string DevRND;
	std::string deviceId;
	std::string deviceMac;
	std::string deviceType;
	std::string firmwareVersion;
	std::string softwareVersion;
	std::string ipAddress;
	long timestamp;
	std::string deviceVendor;
	std::string deviceModel;
	std::string deviceSn;
	std::string apUplinkType;
	int radio5;
	std::string user_key;
	std::string SyncCode;
};

struct EventClientAuthReq
{
	std::string RPCMethod;
	std::string MAC;
	std::string CheckSN;
};

struct EventClientHeartBeatReq
{
	std::string RPCMethod;
	std::string deviceId;
	std::string MAC;
	std::string IPAddr;
};

