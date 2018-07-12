/*
 * AndlinkDeviceMacFilterReq.cpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceControlEvent.hpp"

bool resolveAndlinkDeviceMacFilterReq(std::string msg, struct Interface56_MacFilter_Req* req)
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
	cJSON* MacFilterEnable = cJSON_GetObjectItem(data, "MacFilterEnable");
	cJSON* MacFilterPolicy = cJSON_GetObjectItem(data, "MacFilterPolicy");
	cJSON* MacFilterEntries = cJSON_GetObjectItem(data, "MacFilterEntries");

	if(RPCMethod && ID && deviceId && MacFilterEnable && MacFilterPolicy && MacFilterEntries)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->ID = ID->valuestring;
		req->deviceId = deviceId->valuestring;
		req->MacFilterEnable = MacFilterEnable->valueint;
		req->MacFilterPolicy = MacFilterPolicy->valueint;
		req->MacFilterEntries = MacFilterEntries->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceMacFilterReq(struct Interface56_MacFilter_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", "MacFilter");
	cJSON_AddStringToObject(regJs, "ID", req.ID.c_str());
	cJSON_AddStringToObject(regJs, "deviceId", req.deviceId.c_str());

	cJSON *data = cJSON_CreateObject();
	if(data == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(regJs, "data", data);
	cJSON_AddNumberToObject(data, "MacFilterEnable", req.MacFilterEnable);
	cJSON_AddNumberToObject(data, "MacFilterPolicy", req.MacFilterPolicy);
	cJSON_AddStringToObject(data, "MacFilterEntries", req.MacFilterEntries.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}




