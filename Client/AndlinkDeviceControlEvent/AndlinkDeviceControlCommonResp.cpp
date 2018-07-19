/*
 * AndlinkDeviceControlCommonResp.cpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceControlEvent.hpp"

bool resolveAndlinkDeviceControlCommonResp(std::string msg, struct Interface56_ControlCommon_Resp* req)
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

	cJSON* ID = cJSON_GetObjectItem(obj, "ID");
	cJSON* respCode = cJSON_GetObjectItem(obj, "respCode");
	cJSON* restCont = cJSON_GetObjectItem(obj, "restCont");
	if(ID && respCode && restCont)
	{
		req->ID = ID->valuestring;
		req->respCode = respCode->valueint;
		req->restCont = restCont->valuestring;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceControlCommonResp(struct Interface56_ControlCommon_Resp req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "ID", req.ID.c_str());
	cJSON_AddNumberToObject(regJs, "respCode", req.respCode);
	cJSON_AddStringToObject(regJs, "restCont", req.restCont.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}




