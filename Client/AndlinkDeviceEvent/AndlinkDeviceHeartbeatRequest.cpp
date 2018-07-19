#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_heartbeat_request_msg(std::string msg, struct Interface56_Heartbeat_Req* req)
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
		if(req->RPCMethod != "heartbeat")
		{
			return false;
		}
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string build_if56_heartbeat_request_msg(struct Interface56_Heartbeat_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", "heartbeat");
	cJSON_AddStringToObject(regJs, "deviceId", req.deviceId.c_str());
	cJSON_AddStringToObject(regJs, "MAC", req.MAC.c_str());
	cJSON_AddStringToObject(regJs, "IPAddr", req.IPAddr.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}
