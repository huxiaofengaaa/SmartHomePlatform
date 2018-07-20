/*
 * AndlinkDeviceEventBuilder.cpp
 *
 *  Created on: 2018Äê7ÔÂ11ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceEventBuilder.hpp"
#include "RandomGenerator.hpp"
#include "MD5.h"

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

	std::string l_devSN = l_uecontext->deviceSn;
	std::string l_devRND = l_uecontext->DevRND;
	char l_md5Result[64] = { 0 };
	char l_srcString[256] = { 0 };
	snprintf(l_srcString, sizeof(l_srcString), "%s%s", l_devRND.c_str(), l_devSN.c_str());
	Compute_string_md5(l_srcString, l_md5Result);

	req.Key = std::string(l_md5Result);
	return build_if56_requestPlugIn_request_msg(req);
}

std::string AndlinkDeviceEventBuilder::buildDisconnectRequest(std::string p_deviceid)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	struct Interface56_Disconnect_Req req;
	return build_if56_disconnect_request_msg(req);
}



