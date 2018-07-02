/*
 * AndlinkDeviceRegisterResponse.cpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */
#include "cJSON.h"
#include "AndlinkDeviceEvent.hpp"
#include <string>

std::string build_register_response_success_msg(struct Interface56_Register_Resp resp)
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

std::string build_register_response_failed_msg(struct Interface56_Register_Resp resp)
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
