/*
 * AndlinkDeviceSTAInfoQueryReq.cpp
 *
 *  Created on: 2018Äê7ÔÂ10ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceQueryEvent.hpp"

bool resolveAndlinkDeviceSTAInfoQueryReq(std::string msg, struct Interface56_CommonQuery_Req* req)
{
	int i = 0;
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
	if(RPCMethod && ID && RPCMethod->valuestring == "STAInfo")
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->ID = ID->valuestring;
		cJSON_Delete(obj);
		return true;
	}
	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceSTAInfoQueryReq(std::string ID)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}
	cJSON_AddStringToObject(regJs, "RPCMethod", "STAInfo");
	cJSON_AddStringToObject(regJs, "ID", ID.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}

