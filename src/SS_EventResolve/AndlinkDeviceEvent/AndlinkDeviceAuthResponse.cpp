/*
 * AndlinkDeviceAuthResponse.cpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */

#include "cJSON.h"
#include "AndlinkDeviceEvent.hpp"
#include <string>

std::string build_auth_response_msg(struct Interface56_Auth_Resp resp)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	int respCode;
	long heartBeatTime;
	std::string MessageServer;
	std::string ServerIP;

	cJSON_AddNumberToObject(regJs, "respCode", resp.respCode);
	cJSON_AddNumberToObject(regJs, "heartBeatTime", resp.heartBeatTime);
	cJSON_AddStringToObject(regJs, "MessageServer", resp.MessageServer.c_str());
	cJSON_AddStringToObject(regJs, "ServerIP", resp.ServerIP.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	free(regch);
	cJSON_Delete(regJs);

	return l_result;
}

