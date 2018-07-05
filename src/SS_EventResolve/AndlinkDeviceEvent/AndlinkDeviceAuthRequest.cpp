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

std::string build_if56_auth_request_msg(struct Interface56_Auth_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", req.RPCMethod.c_str());
	cJSON_AddStringToObject(regJs, "MAC", req.MAC.c_str());
	cJSON_AddStringToObject(regJs, "CheckSN", req.CheckSN.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}
