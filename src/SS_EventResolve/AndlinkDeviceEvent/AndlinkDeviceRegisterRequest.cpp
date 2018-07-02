/*
 * AndlinkDeviceRegisterRequest.cpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_register_request_msg(const char* msg, struct Interface56_Register_Req* req)
{
	if(msg == NULL || req == NULL)
	{
		return false;
	}

	cJSON* obj = cJSON_Parse(msg);
	if(obj == NULL)
	{
		return false;
	}

	memset(req, 0, sizeof(struct Interface56_Register_Req));

	cJSON* c_deviceMac = cJSON_GetObjectItem(obj, "deviceMac");
	cJSON* c_deviceType = cJSON_GetObjectItem(obj, "deviceType");
	cJSON* c_productToken = cJSON_GetObjectItem(obj, "productToken");
	cJSON* c_timestamp = cJSON_GetObjectItem(obj, "timestamp");
	if(c_deviceMac && c_deviceType && c_productToken && c_timestamp)
	{
		req->deviceMac = c_deviceMac->valuestring;
		req->deviceType = c_deviceType->valuestring;
		req->productToken = c_productToken->valuestring;
		req->timestamp = c_timestamp->valueint;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}


