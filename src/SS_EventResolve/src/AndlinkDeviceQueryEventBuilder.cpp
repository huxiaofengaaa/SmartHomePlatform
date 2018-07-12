/*
 * AndlinkDeviceQueryEventBuilder.cpp
 *
 *  Created on: 2018Äê7ÔÂ12ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceEventBuilder.hpp"
#include "RandomGenerator.hpp"

std::string AndlinkDeviceEventBuilder::buildApConfigInfoQueryRequest(std::string p_deviceid)
{
	RandomGenerator l_generator;
	std::string ID = l_generator.generatorRandomNumberString(16);
	return buildAndlinkDeviceApConfigInfoQueryReq(ID);
}

std::string AndlinkDeviceEventBuilder::buildSTAInfoQueryRequest(std::string p_deviceid)
{
	RandomGenerator l_generator;
	std::string ID = l_generator.generatorRandomNumberString(16);
	return buildAndlinkDeviceSTAInfoQueryReq(ID);
}

std::string AndlinkDeviceEventBuilder::buildUplinkStatusQueryRequest(std::string p_deviceid)
{
	RandomGenerator l_generator;
	std::string ID = l_generator.generatorRandomNumberString(16);
	return buildAndlinkDeviceUplinkStatusQueryReq(ID);
}

std::string AndlinkDeviceEventBuilder::buildWiFiStatsQueryRequest(std::string p_deviceid)
{
	RandomGenerator l_generator;
	std::string ID = l_generator.generatorRandomNumberString(16);
	return buildAndlinkDeviceWiFiStatsQueryReq(ID);
}

