/*
 * AndlinkDeviceEventBuilder.cpp
 *
 *  Created on: 2018��7��11��
 *      Author: Administrator
 */
#include "AndlinkDeviceEventBuilder.hpp"

std::string AndlinkDeviceEventBuilder::buildPlugIuRequest(std::string p_deviceid,
		std::string p_host, int p_port)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	struct Interface56_RequestPlugIn_Req req;
	req.ServerAddr = p_host;
	req.ServerPort = std::to_string(p_port);
	return build_if56_requestPlugIn_request_msg(req);
}

std::string AndlinkDeviceEventBuilder::buildDisconnectRequest(std::string p_deviceid)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	struct Interface56_Disconnect_Req req;
	return build_if56_disconnect_request_msg(req);
}

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
