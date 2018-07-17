/*
 * AndlinkDeviceWiFiSwitchReq.cpp
 *
 *  Created on: 2018��7��9��
 *      Author: Administrator
 */
#include "AndlinkDeviceControlEvent.hpp"

bool resolveAndlinkDeviceWiFiSwitchReq(std::string msg, struct Interface56_WiFiSwitch_Req* req)
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
	cJSON* Radio = cJSON_GetObjectItem(data, "Radio");
	cJSON* Enable = cJSON_GetObjectItem(obj, "Enable");

	if(RPCMethod && ID && deviceId && Radio && Enable)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->ID = ID->valuestring;
		req->deviceId = deviceId->valuestring;
		req->Radio = Radio->valuestring;
		req->Enable = Enable->valueint;
		cJSON_Delete(obj);
		if(req->RPCMethod != "WiFiSwitch")
		{
			return false;
		}
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceWiFiSwitchReq(struct Interface56_WiFiSwitch_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", "WiFiSwitch");
	cJSON_AddStringToObject(regJs, "ID", req.ID.c_str());
	cJSON_AddStringToObject(regJs, "deviceId", req.deviceId.c_str());

	cJSON *data = cJSON_CreateObject();
	if(data == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(regJs, "data", data);
	cJSON_AddStringToObject(data, "Radio", req.Radio.c_str());
	cJSON_AddNumberToObject(data, "Enable", req.Enable);

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}




