/*
 * AndlinkDeviceQueryEventBuilder.cpp
 *
 *  Created on: 2018Äê7ÔÂ12ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceEventBuilder.hpp"
#include "RandomGenerator.hpp"

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildApConfigInfoQueryRequest(std::string& p_deviceid)
{
	RandomGenerator l_generator;
	std::string ID = l_generator.generatorRandomNumberString(16);
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceApConfigInfoQueryReq(ID));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildSTAInfoQueryRequest(std::string& p_deviceid)
{
	RandomGenerator l_generator;
	std::string ID = l_generator.generatorRandomNumberString(16);
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceSTAInfoQueryReq(ID));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildUplinkStatusQueryRequest(std::string& p_deviceid)
{
	RandomGenerator l_generator;
	std::string ID = l_generator.generatorRandomNumberString(16);
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceUplinkStatusQueryReq(ID));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildWiFiStatsQueryRequest(std::string& p_deviceid)
{
	RandomGenerator l_generator;
	std::string ID = l_generator.generatorRandomNumberString(16);
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceWiFiStatsQueryReq(ID));
}

