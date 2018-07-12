/*
 * UeContextHolderAndlink.cpp
 *
 *  Created on: 2018锟斤拷7锟斤拷4锟斤拷
 *      Author: Administrator
 */

#include "UeContextHolderAndlink.hpp"
#include "AndlinkDeviceEvent.hpp"
#include "RandomGenerator.hpp"

std::vector<std::string> UeContextHolderAndlink::getDeviceList()
{
	return getAllKey();
}

std::string UeContextHolderAndlink::DeviceRegister(std::string deviceMac, std::string deviceType, std::string productToken)
{
	bool l_result = false;
	std::shared_ptr<UeContextAndlink> l_currentUeContext;
	std::string l_currentKey;

	for(auto l_key : getAllKey())
	{
		auto l_keyValue = getRef(l_key);
		if(l_keyValue->deviceMac == deviceMac)
		{
			l_result = true;
			l_currentUeContext = l_keyValue;
			l_currentKey = l_key;
			break;
		}
	}

	if(l_result == false)
	{
		auto l_obj = std::make_shared<UeContextAndlink>();
		std::string deviceID = generatorDeviceID();
		l_obj->deviceMac = deviceMac;
		l_obj->deviceType = deviceType;
		l_obj->productToken = productToken;
		l_obj->gwToken = generatorGwToken();
		l_obj->deviceId = deviceID;
		l_obj->deviceToken = generatorDeviceToken();
		l_obj->andlinkToken = generatorAndlinkToken();

		add(deviceID, l_obj);
		LOG(INFO) << "create new UeContextAndlink " << deviceID;
		return deviceID;
	}
	else
	{
		auto l_uecontext = getRef(l_currentKey);
		l_uecontext->gwToken = generatorGwToken();
		l_uecontext->deviceToken = generatorDeviceToken();
		l_uecontext->andlinkToken = generatorAndlinkToken();

		l_uecontext->isRegister = true;
		l_uecontext->isOnline = false;
		l_uecontext->isAuth = false;
		//l_uecontext->isPlugIn = false;
		return l_currentKey;
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
			|| l_uecontext->deviceType != p_onlinereq.deviceType)
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
	l_uecontext->ServerIP = "127.0.0.1:6887";
	return true;
}

std::string UeContextHolderAndlink::generatorGwToken()
{
	RandomGenerator l_generator;
	return l_generator.generatorRandomCharString(32);
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
