/*
 * AndlinkDeviceWiFiStatsQueryResp.cpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceQueryEvent.hpp"

bool resolveAndlinkDeviceWiFiStatsQueryResp(std::string msg, struct Interface56_WiFiStats_Resp* resp)
{
	int i = 0;
	if(msg.empty() == true || resp == NULL)
	{
		return false;
	}

	cJSON* obj = cJSON_Parse(msg.c_str());
	if(obj == NULL)
	{
		return false;
	}

	// failed case
	cJSON* ID = cJSON_GetObjectItem(obj, "ID");
	cJSON* deviceId = cJSON_GetObjectItem(obj, "deviceId");
	cJSON* respCode = cJSON_GetObjectItem(obj, "respCode");
	cJSON* respCont = cJSON_GetObjectItem(obj, "respCont");
	if(ID && respCode && respCont)
	{
		resp->ID = ID->valuestring;
		resp->respCode = respCode->valueint;
		resp->respCont = respCont->valuestring;
		return true;
	}

	// success case
	if(ID && deviceId)
	{
		resp->ID = ID->valuestring;
		resp->deviceId = deviceId->valuestring;
		cJSON* data = cJSON_GetObjectItem(obj, "data");
		if(data == NULL)
		{
			cJSON_Delete(obj);
			return true;
		}

		cJSON* Status = cJSON_GetObjectItem(data, "Status");
		if(Status == NULL)
		{
			cJSON_Delete(obj);
			return true;
		}

		int l_StatusNumber = cJSON_GetArraySize(Status);
		resp->Status = new WiFiStats[l_StatusNumber];
		resp->StatusNumber = l_StatusNumber;
		for(i = 0 ; i < l_StatusNumber ; i++)
		{
			cJSON* l_status = cJSON_GetArrayItem(Status, i);
			if(l_status == NULL)
			{
				continue;
			}
			cJSON* Radio = cJSON_GetObjectItem(l_status, "Radio");
			cJSON* Index = cJSON_GetObjectItem(l_status, "Index");
			cJSON* SSID = cJSON_GetObjectItem(l_status, "SSID");
			cJSON* TotalBytesSent = cJSON_GetObjectItem(l_status, "TotalBytesSent");
			cJSON* TotalBytesReceived = cJSON_GetObjectItem(l_status, "TotalBytesReceived");
			cJSON* TotalPacketsSent = cJSON_GetObjectItem(l_status, "TotalPacketsSent");
			cJSON* TotalPacketsReceived = cJSON_GetObjectItem(l_status, "TotalPacketsReceived");
			cJSON* ErrorsSent = cJSON_GetObjectItem(l_status, "ErrorsSent");
			cJSON* ErrorsReceived = cJSON_GetObjectItem(l_status, "ErrorsReceived");
			cJSON* DiscardPacketsSent = cJSON_GetObjectItem(l_status, "DiscardPacketsSent");
			cJSON* DiscardPacketsReceived = cJSON_GetObjectItem(l_status, "DiscardPacketsReceived");
			if(Radio && Index && SSID && TotalBytesSent && TotalBytesReceived
					&& TotalPacketsSent && TotalPacketsReceived && ErrorsSent
					&& ErrorsReceived && DiscardPacketsSent && DiscardPacketsReceived)
			{
				resp->Status[i].Radio = Radio->valuestring;
				resp->Status[i].Index = Index->valueint;
				resp->Status[i].SSID = SSID->valuestring;
				resp->Status[i].TotalBytesSent = TotalBytesSent->valueint;
				resp->Status[i].TotalBytesReceived = TotalBytesReceived->valueint;
				resp->Status[i].TotalPacketsSent = TotalPacketsSent->valueint;
				resp->Status[i].TotalPacketsReceived = TotalPacketsReceived->valueint;
				resp->Status[i].ErrorsSent = ErrorsSent->valueint;
				resp->Status[i].ErrorsReceived = ErrorsReceived->valueint;
				resp->Status[i].DiscardPacketsSent = DiscardPacketsSent->valueint;
				resp->Status[i].DiscardPacketsReceived = DiscardPacketsReceived->valueint;
			}
		}
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceWiFiStatsQueryResp(struct Interface56_WiFiStats_Resp resp)
{
	int i = 0;
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}
	cJSON_AddStringToObject(regJs, "ID", resp.ID.c_str());
	cJSON_AddStringToObject(regJs, "deviceId", resp.deviceId.c_str());

	cJSON *data = cJSON_CreateObject();
	if(data == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(regJs, "data", data);

	cJSON* Status = cJSON_CreateArray();
	if(Status == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(data, "Status", Status);

	for(i = 0 ; i < resp.StatusNumber ; i++)
	{
		cJSON *l_status = cJSON_CreateObject();
		if(l_status == NULL)
		{
			cJSON_Delete(regJs);
			return l_result;
		}
		cJSON_AddItemToArray(Status, l_status);

		cJSON_AddStringToObject(l_status, "Radio", resp.Status[i].Radio.c_str());
		cJSON_AddNumberToObject(l_status, "Index", resp.Status[i].Index);
		cJSON_AddStringToObject(l_status, "SSID", resp.Status[i].SSID.c_str());
		cJSON_AddNumberToObject(l_status, "TotalBytesSent", resp.Status[i].TotalBytesSent);
		cJSON_AddNumberToObject(l_status, "TotalBytesReceived", resp.Status[i].TotalBytesReceived);
		cJSON_AddNumberToObject(l_status, "TotalPacketsSent", resp.Status[i].TotalPacketsSent);
		cJSON_AddNumberToObject(l_status, "TotalPacketsReceived", resp.Status[i].TotalPacketsReceived);
		cJSON_AddNumberToObject(l_status, "ErrorsSent", resp.Status[i].ErrorsSent);
		cJSON_AddNumberToObject(l_status, "ErrorsReceived", resp.Status[i].ErrorsReceived);
		cJSON_AddNumberToObject(l_status, "DiscardPacketsSent", resp.Status[i].DiscardPacketsSent);
		cJSON_AddNumberToObject(l_status, "DiscardPacketsReceived", resp.Status[i].DiscardPacketsReceived);
	}

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}


