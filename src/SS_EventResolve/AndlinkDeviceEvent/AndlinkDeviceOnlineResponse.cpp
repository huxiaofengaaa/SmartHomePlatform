/*
 * AndlinkDeviceOnlineResponse.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */
#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_online_response_msg(std::string msg, struct Interface56_Online_Resp* resp)
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
	cJSON* timestamp = cJSON_GetObjectItem(obj, "timestamp");
	cJSON* encrypt = cJSON_GetObjectItem(obj, "encrypt");
	cJSON* ChallengeCode = cJSON_GetObjectItem(obj, "ChallengeCode");
	if(timestamp && encrypt && ChallengeCode && respCode)
	{
		resp->respCode = respCode->valueint;
		resp->timestamp = timestamp->valueint;
		resp->encrypt = encrypt->valueint;
		resp->ChallengeCode = ChallengeCode->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON* ServerIP = cJSON_GetObjectItem(obj, "ServerIP");
	if(respCode && ServerIP)
	{
		resp->respCode = respCode->valueint;
		resp->ServerIP = ServerIP->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string build_if56_online_response_success_msg(struct Interface56_Online_Resp resp)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddNumberToObject(regJs, "respCode", resp.respCode);
	cJSON_AddNumberToObject(regJs, "timestamp", resp.timestamp);
	cJSON_AddNumberToObject(regJs, "encrypt", resp.encrypt);
	cJSON_AddStringToObject(regJs, "ChallengeCode", resp.ChallengeCode.c_str());
	cJSON_AddStringToObject(regJs, "ServerIP", resp.ServerIP.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}

std::string build_if56_online_response_failed_msg(struct Interface56_Online_Resp resp)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddNumberToObject(regJs, "respCode", resp.respCode);
	cJSON_AddStringToObject(regJs, "ServerIP", resp.ServerIP.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}


