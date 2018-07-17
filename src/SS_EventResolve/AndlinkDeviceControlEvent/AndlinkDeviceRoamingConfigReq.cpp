/*
 * AndlinkDeviceRoamingConfigReq.cpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceControlEvent.hpp"

bool resolveAndlinkDeviceRoamingConfigReq(std::string msg, struct Interface56_RoamingConfig_Req* req)
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

	cJSON* data = cJSON_GetObjectItem(obj, "data");
	if(data == NULL)
	{
		return false;
	}
	cJSON* RoamingSwitch = cJSON_GetObjectItem(data, "RoamingSwitch");
	cJSON* LowRSSI24G = cJSON_GetObjectItem(data, "LowRSSI2.4G");
	cJSON* LowRSSI5G = cJSON_GetObjectItem(obj, "LowRSSI5G");

	if(RPCMethod && ID && deviceId && RoamingSwitch && LowRSSI24G && LowRSSI5G)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->ID = ID->valuestring;
		req->deviceId = deviceId->valuestring;
		req->RoamingSwitch = RoamingSwitch->valueint;
		req->LowRSSI24G = LowRSSI24G->valueint;
		req->LowRSSI5G = LowRSSI5G->valueint;
		cJSON_Delete(obj);
		if(req->RPCMethod != "RoamingConfig")
		{
			return false;
		}
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceRoamingConfigReq(struct Interface56_RoamingConfig_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", "RoamingConfig");
	cJSON_AddStringToObject(regJs, "ID", req.ID.c_str());
	cJSON_AddStringToObject(regJs, "deviceId", req.deviceId.c_str());

	cJSON *data = cJSON_CreateObject();
	if(data == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(regJs, "data", data);
	cJSON_AddNumberToObject(data, "RoamingSwitch", req.RoamingSwitch);
	if(req.RoamingSwitch == 1)
	{
		cJSON_AddNumberToObject(data, "LowRSSI2.4G", req.LowRSSI24G);
		cJSON_AddNumberToObject(data, "LowRSSI5G", req.LowRSSI5G);
	}

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}




