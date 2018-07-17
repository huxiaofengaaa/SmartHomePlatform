/*
 * AndlinkDeviceEventBuilder.cpp
 *
 *  Created on: 2018Äê7ÔÂ11ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceEventBuilder.hpp"
#include "RandomGenerator.hpp"

std::string AndlinkDeviceEventBuilder::buildPlugIuRequest(std::string p_deviceid,
		std::string p_host, int p_port)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	if(!l_uecontext)
	{
		return std::string();
	}
	RandomGenerator l_generator;
	struct Interface56_RequestPlugIn_Req req;
	req.ServerAddr = p_host;
	req.ServerPort = std::to_string(p_port);
	req.Key = l_generator.generatorRandomNumberString(32);
	return build_if56_requestPlugIn_request_msg(req);
}

std::string AndlinkDeviceEventBuilder::buildDisconnectRequest(std::string p_deviceid)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	struct Interface56_Disconnect_Req req;
	return build_if56_disconnect_request_msg(req);
}



