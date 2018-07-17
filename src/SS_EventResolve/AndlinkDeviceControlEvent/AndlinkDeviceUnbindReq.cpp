/*
 * AndlinkDeviceUnbindReq.cpp
 *
 *  Created on: 2018Äê7ÔÂ10ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceControlEvent.hpp"

bool resolveAndlinkDeviceUnbindReq(std::string msg, struct Interface56_Unbind_Req* req)
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

	cJSON* RPCMethod = cJSON_GetObjectItem(obj, "RPCMethod");
	cJSON* ID = cJSON_GetObjectItem(obj, "ID");
	cJSON* deviceId = cJSON_GetObjectItem(obj, "deviceId");
	cJSON* childDeviceId = cJSON_GetObjectItem(obj, "childDeviceId");

	if(RPCMethod && ID && deviceId && childDeviceId)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->ID = ID->valuestring;
		req->deviceId = deviceId->valuestring;
		req->childDeviceId = childDeviceId->valuestring;
		cJSON_Delete(obj);
		if(req->RPCMethod != "unbind")
		{
			return false;
		}
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceUnbindReq(struct Interface56_Unbind_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", "unbind");
	cJSON_AddStringToObject(regJs, "ID", req.ID.c_str());
	cJSON_AddStringToObject(regJs, "deviceId", req.deviceId.c_str());
	cJSON_AddStringToObject(regJs, "childDeviceId", req.childDeviceId.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}

