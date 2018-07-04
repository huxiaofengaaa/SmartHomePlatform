/*
 * AndlinkDeviceOnlineResponse.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */
#include "AndlinkDeviceEvent.hpp"

std::string build_online_response_success_msg(struct Interface56_Online_Resp resp)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddNumberToObject(regJs, "timestamp", resp.timestamp);
	cJSON_AddNumberToObject(regJs, "encrypt", resp.encrypt);
	cJSON_AddStringToObject(regJs, "ChallengeCode", resp.ChallengeCode.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}

std::string build_online_response_failed_msg(struct Interface56_Online_Resp resp)
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


