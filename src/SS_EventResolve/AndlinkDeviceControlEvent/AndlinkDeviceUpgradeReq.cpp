/*
 * AndlinkDeviceUpgradeReq.cpp
 *
 *  Created on: 2018Äê7ÔÂ10ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceControlEvent.hpp"

bool resolveAndlinkDeviceUpgradeReq(std::string msg, struct Interface56_Upgrade_Req* req)
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
	cJSON* deviceId = cJSON_GetObjectItem(obj, "deviceId");

	cJSON* data = cJSON_GetObjectItem(obj, "data");
	if(data == NULL)
	{
		return false;
	}
	cJSON* childDeviceId = cJSON_GetObjectItem(data, "childDeviceId");
	cJSON* fileMode = cJSON_GetObjectItem(data, "fileMode");
	cJSON* fileType = cJSON_GetObjectItem(data, "fileType");
	cJSON* fileUrl = cJSON_GetObjectItem(data, "fileUrl");
	cJSON* downMode = cJSON_GetObjectItem(data, "downMode");

	if(RPCMethod && ID && deviceId && childDeviceId && fileMode && fileType
			&& fileUrl && downMode)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->ID = ID->valuestring;
		req->deviceId = deviceId->valuestring;
		req->childDeviceId = childDeviceId->valuestring;
		req->fileMode = fileMode->valuestring;
		req->fileType = fileType->valuestring;
		req->fileUrl = fileUrl->valuestring;
		req->downMode = downMode->valueint;
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceUpgradeReq(struct Interface56_Upgrade_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", req.RPCMethod.c_str());
	cJSON_AddStringToObject(regJs, "ID", req.ID.c_str());
	cJSON_AddStringToObject(regJs, "deviceId", req.deviceId.c_str());

	cJSON *data = cJSON_CreateObject();
	if(data == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(regJs, "data", data);
	cJSON_AddStringToObject(data, "childDeviceId", req.childDeviceId.c_str());
	cJSON_AddStringToObject(data, "fileMode", req.fileMode.c_str());
	cJSON_AddStringToObject(data, "fileType", req.fileType.c_str());
	cJSON_AddStringToObject(data, "fileUrl", req.fileUrl.c_str());
	cJSON_AddNumberToObject(data, "downMode", req.downMode);

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}




