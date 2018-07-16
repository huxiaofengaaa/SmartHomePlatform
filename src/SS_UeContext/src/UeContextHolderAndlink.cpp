/*
 * UeContextHolderAndlink.cpp
 *
 *  Created on: 2018锟斤拷7锟斤拷4锟斤拷
 *      Author: Administrator
 */

#include "UeContextHolderAndlink.hpp"
#include "AndlinkDeviceEvent.hpp"
#include "RandomGenerator.hpp"
#include "CalendarClock.hpp"

std::vector<std::string> UeContextHolderAndlink::getDeviceList()
{
	return getAllKey();
}

std::string UeContextHolderAndlink::getDeviceIDByMAC(std::string p_deviceMac)
{
	for(auto l_key : getAllKey())
	{
		auto l_keyValue = getRef(l_key);
		if(l_keyValue->deviceMac == p_deviceMac)
		{
			return l_keyValue->deviceId;
		}
	}
	return std::string();
}

bool UeContextHolderAndlink::updateNetAddress(std::string p_deviceID,
		std::string p_host, int p_port, int p_sockfd, bool isTCP)
{
	auto l_uecontext = getRef(p_deviceID);
	if(l_uecontext)
	{
		if(isTCP == true)
		{
			l_uecontext->peerTCPHost = p_host;
			l_uecontext->peerTCPPort = p_port;
			l_uecontext->TCPSocketfd = p_sockfd;
		}
		else
		{
			l_uecontext->peerUDPHost = p_host;
			l_uecontext->peerUDPPort = p_port;
			l_uecontext->UDPSocketfd = p_sockfd;
		}
		return true;
	}
	return false;
}

bool UeContextHolderAndlink::setRegisterResponse(std::string p_deviceID,
		struct Interface56_Register_Resp& resp)
{
	auto l_uecontext = getRef(p_deviceID);
	if(l_uecontext)
	{
		resp.gwToken = l_uecontext->gwToken;
		resp.deviceId = l_uecontext->deviceId;
		resp.deviceToken = l_uecontext->deviceToken;
		resp.andlinkToken = l_uecontext->andlinkToken;
		return true;
	}
	return false;
}

bool UeContextHolderAndlink::setOnlineResponse(std::string p_deviceID,
		struct Interface56_Online_Resp& resp, bool isSuccess)
{
	auto l_uecontext = getRef(p_deviceID);
	if(l_uecontext)
	{
		if(true == isSuccess)
		{
			CalendarClock clock;
			resp.timestamp = clock.getTimeStamp();
			resp.encrypt = l_uecontext->encrypt;
			resp.ChallengeCode = l_uecontext->ChallengeCode;
		}
		else
		{
			resp.respCode = -5;
			resp.ServerIP = m_host + std::string(":") + std::to_string(m_port);
		}
		return true;
	}
	return false;
}

bool UeContextHolderAndlink::setAuthResponse(std::string p_deviceMAC, std::string p_deviceCheckSN,
		struct Interface56_Auth_Resp& resp)
{
	auto l_deviceID = getDeviceIDByMAC(p_deviceMAC);
	auto l_uecontext = getRef(l_deviceID);
	if(l_uecontext)
	{
		resp.respCode = 2;
		resp.heartBeatTime = 15;
		resp.MessageServer = m_host + std::string(":") + std::to_string(m_port);
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port+1);
		l_uecontext->CheckSN = p_deviceCheckSN;
		return true;
	}
	else
	{
		resp.respCode = -2;
		resp.MessageServer = m_host + std::string(":") + std::to_string(m_port);
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port+1);
	}
	return false;
}

bool UeContextHolderAndlink::setHeartbeatResponse(std::string p_deviceID,
		std::string p_deviceMAC, std::string p_deviceIPAddr,
		struct Interface56_Heartbeat_Resp& resp)
{
	bool l_result = true;
	auto l_uecontext = getRef(p_deviceID);
	if(!l_uecontext || l_uecontext->deviceMac != p_deviceMAC)
	{
		l_result = false;
	}
	if(l_result == true)
	{
		resp.respCode = 0;
		resp.heartBeatTime = 15;
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port+1);

		CalendarClock clock;
		l_uecontext->lastHeartbeat = clock.getTimeStamp();
		l_uecontext->ipAddress = p_deviceIPAddr;
	}
	else
	{
		resp.respCode = -5;
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port+1);
	}
	return false;
}

std::string UeContextHolderAndlink::DeviceRegister(std::string p_deviceMac,
		std::string p_deviceType, std::string p_productToken)
{
	auto l_currentDeviceID = getDeviceIDByMAC(p_deviceMac);
	if(l_currentDeviceID.empty() == true)
	{
		std::string deviceID = generatorDeviceID();
		auto l_obj = std::make_shared<UeContextAndlink>(deviceID);
		l_obj->deviceMac = p_deviceMac;
		l_obj->deviceType = p_deviceType;
		l_obj->productToken = p_productToken;
		l_obj->gwToken = generatorGwToken();
		l_obj->deviceToken = generatorDeviceToken();
		l_obj->andlinkToken = generatorAndlinkToken();

		l_obj->isRegister = true;
		l_obj->isOnline = false;
		l_obj->isAuth = false;
		l_obj->isPlugIn = false;
		add(deviceID, l_obj);
		LOG(INFO) << "create new UeContextAndlink " << deviceID;
		return deviceID;
	}
	else
	{
		auto l_uecontext = getRef(l_currentDeviceID);
		l_uecontext->deviceMac = p_deviceMac;
		l_uecontext->deviceType = p_deviceType;
		l_uecontext->productToken = p_productToken;
		l_uecontext->gwToken = generatorGwToken();
		l_uecontext->deviceToken = generatorDeviceToken();
		l_uecontext->andlinkToken = generatorAndlinkToken();

		l_uecontext->isRegister = true;
		l_uecontext->isOnline = false;
		l_uecontext->isAuth = false;
		l_uecontext->isPlugIn = false;
		return l_currentDeviceID;
	}
}

bool UeContextHolderAndlink::DeviceOnline(struct Interface56_Online_Req& p_onlinereq)
{
	RandomGenerator l_generator;
	auto l_deviceid = p_onlinereq.deviceId;
	auto l_uecontext = getRef(l_deviceid);
	if(!l_uecontext)
	{
		return false;
	}
	if(l_uecontext->deviceMac != p_onlinereq.deviceMac
			|| l_uecontext->deviceType != p_onlinereq.deviceType
			|| l_uecontext->isRegister == false)
	{
		return false;
	}

	l_uecontext->DevRND = p_onlinereq.DevRND;
	l_uecontext->firmwareVersion = p_onlinereq.firmwareVersion;
	l_uecontext->softwareVersion = p_onlinereq.softwareVersion;
	l_uecontext->ipAddress = p_onlinereq.ipAddress;
	l_uecontext->deviceVendor = p_onlinereq.deviceVendor;
	l_uecontext->deviceModel = p_onlinereq.deviceModel;
	l_uecontext->deviceSn = p_onlinereq.deviceSn;
	l_uecontext->apUplinkType = p_onlinereq.apUplinkType;
	l_uecontext->radio5 = p_onlinereq.radio5;
	l_uecontext->user_key = p_onlinereq.user_key;
	l_uecontext->SyncCode = p_onlinereq.SyncCode;

	l_uecontext->heartBeatTime = 15000;
	l_uecontext->encrypt = 0;
	l_uecontext->ChallengeCode = l_generator.generatorRandomNumberString(16);

	l_uecontext->isRegister = true;
	l_uecontext->isOnline = true;
	l_uecontext->isAuth = false;
	l_uecontext->isPlugIn = false;
	return true;
}

std::string UeContextHolderAndlink::generatorDeviceID()
{
	RandomGenerator l_generator;
	std::string deviceID;
	do
	{
		deviceID = "CMCC-" + l_generator.generatorRandomNumberString(5)
				+ "-" + l_generator.generatorRandomCharString(12);
	}
	while(true == isExist(deviceID));
	return deviceID;
}

std::string UeContextHolderAndlink::generatorDeviceToken()
{
	RandomGenerator l_generator;
	return l_generator.generatorRandomCharString(32);
}

std::string UeContextHolderAndlink::generatorAndlinkToken()
{
	RandomGenerator l_generator;
	return l_generator.generatorRandomCharString(32);
}

std::string UeContextHolderAndlink::generatorGwToken()
{
	RandomGenerator l_generator;
	return l_generator.generatorRandomCharString(32);
}
