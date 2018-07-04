/*
 * AndlinkDeviceHeartbeatRequest.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */

#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_heartbeat_request_msg(std::string msg, struct Interface56_heartbeat_Req* req)
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
	cJSON* deviceId = cJSON_GetObjectItem(obj, "deviceId");
	cJSON* MAC = cJSON_GetObjectItem(obj, "MAC");
	cJSON* IPAddr = cJSON_GetObjectItem(obj, "IPAddr");
	if(RPCMethod && deviceId && MAC && IPAddr)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->deviceId = deviceId->valuestring;
		req->MAC = MAC->valuestring;
		req->IPAddr = IPAddr->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}


