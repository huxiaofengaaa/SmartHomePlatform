/*
 * AndlinkDeviceRegisterResponse.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */
#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_register_response_msg(std::string msg, struct Interface56_Register_Resp* resp)
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

	cJSON* gwToken = cJSON_GetObjectItem(obj, "gwToken");
	cJSON* deviceId = cJSON_GetObjectItem(obj, "deviceId");
	cJSON* deviceToken = cJSON_GetObjectItem(obj, "deviceToken");
	cJSON* andlinkToken = cJSON_GetObjectItem(obj, "andlinkToken");
	if(gwToken && deviceId && deviceToken && andlinkToken)
	{
		resp->gwToken = gwToken->valuestring;
		resp->deviceId = deviceId->valuestring;
		resp->deviceToken = deviceToken->valuestring;
		resp->andlinkToken = andlinkToken->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON* respCode = cJSON_GetObjectItem(obj, "respCode");
	cJSON* respCont = cJSON_GetObjectItem(obj, "respCont");
	if(respCode && respCont)
	{
		resp->respCode = respCode->valueint;
		resp->respCont = respCont->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string build_if56_register_response_success_msg(struct Interface56_Register_Resp resp)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "gwToken", resp.gwToken.c_str());
	cJSON_AddStringToObject(regJs, "deviceId", resp.deviceId.c_str());
	cJSON_AddStringToObject(regJs, "deviceToken", resp.deviceToken.c_str());
	cJSON_AddStringToObject(regJs, "andlinkToken", resp.andlinkToken.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	free(regch);
	cJSON_Delete(regJs);

	return l_result;
}

std::string build_if56_register_response_failed_msg(struct Interface56_Register_Resp resp)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddNumberToObject(regJs, "respCode", resp.respCode);
	cJSON_AddStringToObject(regJs, "respCont", resp.respCont.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	free(regch);
	cJSON_Delete(regJs);

	return l_result;
}
