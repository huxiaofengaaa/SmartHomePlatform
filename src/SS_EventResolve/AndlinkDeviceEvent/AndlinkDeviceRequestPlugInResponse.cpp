/*
 * AndlinkDeviceRequestPlugInResponse.cpp
 *
 *  Created on: 2018Äê7ÔÂ3ÈÕ
 *      Author: Administrator
 */

#include "cJSON.h"
#include "AndlinkDeviceEvent.hpp"
#include <string>

bool resolve_if56_requestPlugIn_Response_msg(std::string msg, struct Interface56_requestPlugIn_Resp* req)
{
	if(msg.empty() == true || req == NULL)
	{
		return false;
	}

	cJSON* obj = cJSON_Parse(msg.c_str());
	if(obj == NULL)
	{
		return false;
	}

	cJSON* respCode = cJSON_GetObjectItem(obj, "respCode");
	cJSON* DevRND = cJSON_GetObjectItem(obj, "DevRND");
	if(respCode && DevRND)
	{
		req->respCode = respCode->valueint;
		req->DevRND = DevRND->valueint;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}



