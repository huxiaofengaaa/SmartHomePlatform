/*
 * UeContextHolderAndlink.cpp
 *
 *  Created on: 2018年7月4日
 *      Author: Administrator
 */

#include "UeContextHolderAndlink.hpp"

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
		return l_currentKey;
	}
}

std::string UeContextHolderAndlink::generatorGwToken()
{
	return generatorRandomCharString(32);
}

std::string UeContextHolderAndlink::generatorDeviceID()
{
	std::string deviceID;
	do
	{
		deviceID = "CMCC-" + generatorRandomNumberString(5) + "-" + generatorRandomCharString(12);
	}
	while(true == isExist(deviceID));
	return deviceID;
}

std::string UeContextHolderAndlink::generatorDeviceToken()
{
	return generatorRandomCharString(32);
}

std::string UeContextHolderAndlink::generatorAndlinkToken()
{
	return generatorRandomCharString(32);
}

std::string UeContextHolderAndlink::generatorRandomCharString(int length)
{
	std::random_device rd;  // 将用于为随机数引擎获得种子
	std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
	std::uniform_int_distribution<> dis(0, 'Z'-'A');

	char* buf = new char[length];

	for (int n = 0; n < length; ++n)
	{
		buf[n] = dis(gen) + 'A';
	}
	buf[length] = '\0';

	std::string l_result = buf;
	delete[] buf;
	return l_result;
}

std::string UeContextHolderAndlink::generatorRandomNumberString(int length)
{
	std::random_device rd;  // 将用于为随机数引擎获得种子
	std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
	std::uniform_int_distribution<> dis(0, 9);

	char* buf = new char[length];

	for (int n = 0; n < length; ++n)
	{
		buf[n] = dis(gen) + '0';
	}
	buf[length] = '\0';

	std::string l_result = buf;
	delete[] buf;
	return l_result;
}


