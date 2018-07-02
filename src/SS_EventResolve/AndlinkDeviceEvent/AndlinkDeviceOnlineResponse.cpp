/*
 * AndlinkDeviceOnlineResponse.cpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */
#include "cJSON.h"
#include "AndlinkDeviceEvent.hpp"

char* build_online_response_success_msg(struct Interface56_Online_Resp* resp)
{
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return NULL;
	}

	cJSON_AddNumberToObject(regJs, "timestamp", resp->timestamp);
	cJSON_AddNumberToObject(regJs, "encrypt", resp->encrypt);
	cJSON_AddStringToObject(regJs, "ChallengeCode", resp->ChallengeCode.c_str());

	char* regch = cJSON_Print(regJs);
	cJSON_Delete(regJs);

	return regch;
}

char* build_online_response_failed_msg(struct Interface56_Online_Resp* resp)
{
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return NULL;
	}

	cJSON_AddNumberToObject(regJs, "respCode", resp->respCode);
	cJSON_AddStringToObject(regJs, "ServerIP", resp->ServerIP.c_str());

	char* regch = cJSON_Print(regJs);
	cJSON_Delete(regJs);

	return regch;
}


