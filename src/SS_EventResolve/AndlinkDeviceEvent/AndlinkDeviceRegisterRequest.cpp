/*
 * AndlinkDeviceRegisterRequest.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */
#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_register_request_msg(std::string msg, struct Interface56_Register_Req* req)
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


