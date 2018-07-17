/*
 * AndlinkDeviceOnlineRequest.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */

#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_online_request_msg(std::string msg, struct Interface56_Online_Req* req)
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

	cJSON* xdata = cJSON_GetObjectItem(obj, "xdata");
	if(xdata == NULL)
	{
		cJSON_Delete(obj);
		return false;
	}

	cJSON* RPCMethod = cJSON_GetObjectItem(obj, "RPCMethod");
	cJSON* DevRND = cJSON_GetObjectItem(obj, "DevRND");
	cJSON* deviceId = cJSON_GetObjectItem(obj, "deviceId");
	cJSON* deviceMac = cJSON_GetObjectItem(obj, "deviceMac");
	cJSON* deviceType = cJSON_GetObjectItem(obj, "deviceType");
	cJSON* firmwareVersion = cJSON_GetObjectItem(obj, "firmwareVersion");
	cJSON* softwareVersion = cJSON_GetObjectItem(obj, "softwareVersion");
	cJSON* ipAddress = cJSON_GetObjectItem(obj, "ipAddress");
	cJSON* timestamp = cJSON_GetObjectItem(obj, "timestamp");

	cJSON* deviceVendor = cJSON_GetObjectItem(xdata, "deviceVendor");
	cJSON* deviceModel = cJSON_GetObjectItem(xdata, "deviceModel");
	cJSON* deviceSn = cJSON_GetObjectItem(xdata, "deviceSn");
	cJSON* apUplinkType = cJSON_GetObjectItem(xdata, "apUplinkType");
	cJSON* radio5 = cJSON_GetObjectItem(xdata, "radio5");
	cJSON* user_key = cJSON_GetObjectItem(xdata, "user_key");
	cJSON* SyncCode = cJSON_GetObjectItem(xdata, "SyncCode");

	if(RPCMethod && DevRND && deviceType && firmwareVersion && softwareVersion && ipAddress
		&& timestamp && xdata && deviceVendor && deviceModel && deviceSn && apUplinkType
		&& radio5 && SyncCode)
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->DevRND = DevRND->valuestring;
		if(deviceId)
		{
			req->deviceId = deviceId->valuestring;
		}
		if(deviceMac)
		{
			req->deviceMac = deviceMac->valuestring;
		}

		req->deviceType = deviceType->valuestring;
		req->firmwareVersion = firmwareVersion->valuestring;
		req->softwareVersion = softwareVersion->valuestring;
		req->ipAddress = ipAddress->valuestring;
		req->timestamp = timestamp->valueint;

		req->deviceVendor = deviceVendor->valuestring;
		req->deviceModel = deviceModel->valuestring;
		req->deviceSn = deviceSn->valuestring;
		req->apUplinkType = apUplinkType->valuestring;
		req->radio5 = radio5->valueint;
		if(user_key)
		{
			req->user_key = user_key->valuestring;
		}
		req->SyncCode = SyncCode->valuestring;

		cJSON_Delete(obj);
		if(req->RPCMethod != "boot")
		{
			return false;
		}
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string build_if56_online_request_msg(struct Interface56_Online_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", "boot");
	cJSON_AddStringToObject(regJs, "DevRND", req.DevRND.c_str());
	cJSON_AddStringToObject(regJs, "deviceId", req.deviceId.c_str());
	cJSON_AddStringToObject(regJs, "deviceMac", req.deviceMac.c_str());
	cJSON_AddStringToObject(regJs, "deviceType", req.deviceType.c_str());
	cJSON_AddStringToObject(regJs, "firmwareVersion", req.firmwareVersion.c_str());
	cJSON_AddStringToObject(regJs, "softwareVersion", req.softwareVersion.c_str());
	cJSON_AddStringToObject(regJs, "ipAddress", req.ipAddress.c_str());
	cJSON_AddNumberToObject(regJs, "timestamp", req.timestamp);

	cJSON *regJs_xdata = cJSON_CreateObject();
	if(regJs_xdata == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddStringToObject(regJs_xdata, "deviceVendor", req.deviceVendor.c_str());
	cJSON_AddStringToObject(regJs_xdata, "deviceModel", req.deviceModel.c_str());
	cJSON_AddStringToObject(regJs_xdata, "deviceSn", req.deviceSn.c_str());
	cJSON_AddStringToObject(regJs_xdata, "apUplinkType", req.apUplinkType.c_str());
	cJSON_AddNumberToObject(regJs_xdata, "radio5", req.radio5);
	cJSON_AddStringToObject(regJs_xdata, "user_key", req.user_key.c_str());
	cJSON_AddStringToObject(regJs_xdata, "SyncCode", req.SyncCode.c_str());
	cJSON_AddItemToObject(regJs, "XData", regJs_xdata);

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}

