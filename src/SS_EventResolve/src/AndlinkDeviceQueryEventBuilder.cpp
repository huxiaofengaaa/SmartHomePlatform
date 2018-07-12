/*
 * AndlinkDeviceQueryEventBuilder.cpp
 *
 *  Created on: 2018Äê7ÔÂ12ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceEventBuilder.hpp"

std::string AndlinkDeviceEventBuilder::buildApConfigInfoQueryRequest(std::string p_deviceid)
{
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	return buildAndlinkDeviceApConfigInfoQueryReq(ID);
}

std::string AndlinkDeviceEventBuilder::buildSTAInfoQueryRequest(std::string p_deviceid)
{
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	return buildAndlinkDeviceSTAInfoQueryReq(ID);
}

std::string AndlinkDeviceEventBuilder::buildUplinkStatusQueryRequest(std::string p_deviceid)
{
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	return buildAndlinkDeviceUplinkStatusQueryReq(ID);
}

std::string AndlinkDeviceEventBuilder::buildWiFiStatsQueryRequest(std::string p_deviceid)
{
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	return buildAndlinkDeviceWiFiStatsQueryReq(ID);
}

