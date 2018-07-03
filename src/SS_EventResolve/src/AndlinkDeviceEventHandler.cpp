/*
 * AndlinkDeviceEventHandler.cpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */

#include "AndlinkDeviceEventHandler.hpp"
#include "AndlinkDeviceEvent.hpp"
#include "cJSON.h"
#include <string>

std::string AndlinkDeviceEventHandler::run(std::shared_ptr<EventTypeUDPClientDataObject> p_event)
{
	std::string l_rawData = p_event->m_rawData;

	struct Interface56_Register_Req l_registerReq;
	struct Interface56_Online_Req l_onlineReq;
	struct Interface56_Auth_Req l_authReq;
	struct Interface56_heartbeat_Req l_heartbeatReq;

	if(true == resolve_if56_register_request_msg(l_rawData, &l_registerReq))
	{
		struct Interface56_Register_Resp l_registerResp;
		return build_register_response_success_msg(l_registerResp);
	}
	else if(true == resolve_if56_online_request_msg(l_rawData, &l_onlineReq))
	{
		struct Interface56_Online_Resp l_onlineResp;
		return build_online_response_success_msg(l_onlineResp);
	}
	else if(true == resolve_if56_auth_request_msg(l_rawData, &l_authReq))
	{
		struct Interface56_Auth_Resp l_authResp;
		return build_auth_response_msg(l_authResp);
	}
	else if(true == resolve_if56_heartbeat_request_msg(l_rawData, &l_heartbeatReq))
	{
		struct Interface56_heartbeat_Resp l_heartbeatResp;
		return build_heartbeat_response_msg(l_heartbeatResp);
	}

	return std::string();
}


