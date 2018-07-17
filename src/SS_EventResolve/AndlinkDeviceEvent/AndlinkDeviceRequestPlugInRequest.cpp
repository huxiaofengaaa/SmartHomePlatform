/*
 * AndlinkDeviceRequestPlugInRequest.cpp
 *
 *  Created on: 2018��7��3��
 *      Author: Administrator
 */
#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_requestPlugIn_request_msg(std::string msg, struct Interface56_RequestPlugIn_Req* req)
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
	cJSON* Key = cJSON_GetObjectItem(obj, "Key");
	cJSON* ServerAddr = cJSON_GetObjectItem(obj, "ServerAddr");
	cJSON* ServerPort = cJSON_GetObjectItem(obj, "ServerPort");
	if(RPCMethod && Key && ServerAddr && ServerPort)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->Key = Key->valuestring;
		req->ServerAddr = ServerAddr->valuestring;
		req->ServerPort = ServerPort->valuestring;
		cJSON_Delete(obj);
		if(req->RPCMethod != "RequestPlug-in")
		{
			return false;
		}
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string build_if56_requestPlugIn_request_msg(struct Interface56_RequestPlugIn_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", "RequestPlug-in");
	cJSON_AddStringToObject(regJs, "Key", req.Key.c_str());
	cJSON_AddStringToObject(regJs, "ServerAddr", req.ServerAddr.c_str());
	cJSON_AddStringToObject(regJs, "ServerPort", req.ServerPort.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	free(regch);
	cJSON_Delete(regJs);

	return l_result;
}



