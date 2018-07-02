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

std::string AndlinkDeviceEventHandler::run(std::shared_ptr<EventTypeNetworkDataObject> p_event)
{
	std::string l_rawData = p_event->m_rawData;

	struct Interface56_Register_Req l_registerReq;
	struct Interface56_Online_Req l_onlineReq;

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

	return std::string();
}


