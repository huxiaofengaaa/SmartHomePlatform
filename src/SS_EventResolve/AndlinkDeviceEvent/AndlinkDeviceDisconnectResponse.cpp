/*
 * AndlinkDeviceDisconnectResponse.cpp
 *
 *  Created on: 2018Äê7ÔÂ3ÈÕ
 *      Author: Administrator
 */

#include "cJSON.h"
#include "AndlinkDeviceEvent.hpp"
#include <string>

bool resolve_if56_disconnect_Response_msg(std::string msg, struct Interface56_disconnect_resp* resp)
{
	if(msg.empty() == true || resp == NULL)
	{
		return false;
	}

	cJSON* obj = cJSON_Parse(msg.c_str());
	if(obj == NULL)
	{
		return false;
	}

	cJSON* respCode = cJSON_GetObjectItem(obj, "respCode");
	cJSON* ID = cJSON_GetObjectItem(obj, "ID");
	if(respCode && ID)
	{
		resp->respCode = respCode->valueint;
		resp->ID = ID->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}


