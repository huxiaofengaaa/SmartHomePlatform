/*
 * AndlinkDeviceEventHandler.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */

#include "AndlinkDeviceEventHandler.hpp"


std::string AndlinkDeviceEventHandler::run(std::shared_ptr<EventTypeUDPClientDataObject> p_event)
{
	if(!p_event)
	{
		return std::string();
	}

	std::string l_rawData = p_event->m_rawData;

	struct Interface56_Register_Req l_registerReq;
	struct Interface56_Online_Req l_onlineReq;
	struct Interface56_Auth_Req l_authReq;
	struct Interface56_Heartbeat_Req l_heartbeatReq;

	if(true == resolve_if56_register_request_msg(l_rawData, &l_registerReq))
	{
		std::string l_deviceID = m_ueContextHolder->DeviceRegister(
				l_registerReq.deviceMac, l_registerReq.deviceType, l_registerReq.productToken);

		auto l_uecontext = m_ueContextHolder->getRef(l_deviceID);
		l_uecontext->host = p_event->m_host;
		l_uecontext->port = p_event->m_port;

		struct Interface56_Register_Resp l_registerResp =
		{
				l_uecontext->gwToken,
				l_uecontext->deviceId,
				l_uecontext->deviceToken,
				l_uecontext->andlinkToken
		};
		l_registerResp.deviceId = l_deviceID;
		return build_if56_register_response_success_msg(l_registerResp);
	}
	else if(true == resolve_if56_online_request_msg(l_rawData, &l_onlineReq))
	{
		if(true == m_ueContextHolder->DeviceOnline(l_onlineReq))
		{
			auto l_deviceid = l_onlineReq.deviceId;
			auto l_uecontext = m_ueContextHolder->getRef(l_deviceid);

			struct Interface56_Online_Resp l_onlineResp =
			{
					0, l_uecontext->encrypt, l_uecontext->ChallengeCode,
					0, ""
			};
			return build_if56_online_response_success_msg(l_onlineResp);
		}
		else
		{
			struct Interface56_Online_Resp l_onlineResp;
			return build_if56_online_response_failed_msg(l_onlineResp);
		}
	}
	else if(true == resolve_if56_auth_request_msg(l_rawData, &l_authReq))
	{
		struct Interface56_Auth_Resp l_authResp;
		return build_if56_auth_response_msg(l_authResp);
	}
	else if(true == resolve_if56_heartbeat_request_msg(l_rawData, &l_heartbeatReq))
	{
		struct Interface56_Heartbeat_Resp l_heartbeatResp;
		return build_if56_heartbeat_response_msg(l_heartbeatResp);
	}

	return std::string();
}

std::string AndlinkDeviceEventHandler::buildPlugIuRequest(std::string p_deviceid)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	struct Interface56_RequestPlugIn_Req req;
	req.ServerAddr = "127.0.0.1";
	req.ServerPort = "6888";
	return build_if56_requestPlugIn_request_msg(req);
}

std::string AndlinkDeviceEventHandler::buildDisconnectRequest(std::string p_deviceid)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	struct Interface56_Disconnect_Req req;
	return build_if56_disconnect_request_msg(req);
}

