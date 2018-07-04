/*
 * AndlinkDeviceAuthRequest.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */

#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_auth_request_msg(std::string msg, struct Interface56_Auth_Req* req)
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
	cJSON* MAC = cJSON_GetObjectItem(obj, "MAC");
	cJSON* CheckSN = cJSON_GetObjectItem(obj, "CheckSN");
	if(RPCMethod && MAC && CheckSN)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->MAC = MAC->valuestring;
		req->CheckSN = CheckSN->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}


