#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_disconnect_request_msg(std::string msg, struct Interface56_Disconnect_Req* req)
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
	if(RPCMethod && ID)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->ID = ID->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string build_if56_disconnect_request_msg(struct Interface56_Disconnect_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", req.RPCMethod.c_str());
	cJSON_AddStringToObject(regJs, "ID", req.ID.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	free(regch);
	cJSON_Delete(regJs);

	return l_result;
}


