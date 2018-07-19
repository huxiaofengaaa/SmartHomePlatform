/*
 * AndlinkDeviceDisconnectResponse.cpp
 *
 *  Created on: 2018��7��3��
 *      Author: Administrator
 */

#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_disconnect_response_msg(std::string msg, struct Interface56_Disconnect_Resp* resp)
{
	if(msg.empty() == true || resp == NULL)
	{
		return false;
	}

	cJSON* obj = cJSON_Parse(msg.c_str());
	if(obj == NULL)
	{
		return false;
	}

	cJSON* respCode = cJSON_GetObjectItem(obj, "respCode");
	cJSON* ID = cJSON_GetObjectItem(obj, "ID");
	if(respCode && ID)
	{
		resp->respCode = respCode->valueint;
		resp->ID = ID->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string build_if56_disconnect_response_msg(struct Interface56_Disconnect_Resp resp)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddNumberToObject(regJs, "respCode", resp.respCode);
	cJSON_AddStringToObject(regJs, "ID", resp.ID.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	free(regch);
	cJSON_Delete(regJs);

	return l_result;
}
