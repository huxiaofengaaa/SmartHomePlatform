/*
 * AndlinkDeviceOnlineRequest.cpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */

#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "AndlinkDeviceEvent.hpp"

bool resolve_if56_online_request_msg(const char* msg, struct Interface56_Online_Req* req)
{
	if(msg == NULL || req == NULL)
	{
		return false;
	}

	cJSON* obj = cJSON_Parse(msg);
	if(obj == NULL)
	{
		return false;
	}

	memset(req, 0, sizeof(struct Interface56_Online_Req));

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
		return true;
	}

	cJSON_Delete(obj);
	return false;
}



