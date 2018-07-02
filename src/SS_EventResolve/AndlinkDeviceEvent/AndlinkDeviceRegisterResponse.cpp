/*
 * AndlinkDeviceRegisterResponse.cpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */
#include "cJSON.h"
#include "AndlinkDeviceEvent.hpp"

char* build_register_response_success_msg(struct Interface56_Register_Resp* resp)
{
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return NULL;
	}

	cJSON_AddStringToObject(regJs, "gwToken", resp->gwToken.c_str());
	cJSON_AddStringToObject(regJs, "deviceId", resp->deviceId.c_str());
	cJSON_AddStringToObject(regJs, "deviceToken", resp->deviceToken.c_str());
	cJSON_AddStringToObject(regJs, "andlinkToken", resp->andlinkToken.c_str());

	char* regch = cJSON_Print(regJs);
	cJSON_Delete(regJs);

	return regch;
}

char* build_register_response_failed_msg(struct Interface56_Register_Resp* resp)
{
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return NULL;
	}

	cJSON_AddNumberToObject(regJs, "respCode", resp->respCode);
	cJSON_AddStringToObject(regJs, "respCont", resp->respCont.c_str());

	char* regch = cJSON_Print(regJs);
	cJSON_Delete(regJs);

	return regch;
}
