/*
 * AndlinkDeviceWiFiParameterSyncReq.cpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceControlEvent.hpp"

bool resolveAndlinkDeviceWiFiParameterSyncReq(std::string msg, struct Interface56_WiFiParameterSync_Req* req)
{
	int i = 0;
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
	cJSON* SyncCode = cJSON_GetObjectItem(data, "SyncCode");
	cJSON* Configurations = cJSON_GetObjectItem(data, "Configurations");

	if(RPCMethod && ID && deviceId && data && SyncCode && Configurations
			&& cJSON_IsArray(Configurations))
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->ID = ID->valuestring;
		req->deviceId = deviceId->valuestring;
		req->SyncCode = SyncCode->valuestring;

		int l_configSize = cJSON_GetArraySize(Configurations);
		req->Configurations = new WiFiParameterSync[l_configSize];
		req->configSize = l_configSize;

		for(i = 0 ; l_configSize ; i++)
		{
			cJSON* config = cJSON_GetArrayItem(Configurations, i);
			if(config == NULL)
			{
				continue;
			}
			cJSON* Radio = cJSON_GetObjectItem(config, "Radio");
			cJSON* Index = cJSON_GetObjectItem(config, "Index");
			cJSON* Enable = cJSON_GetObjectItem(config, "Enable");
			cJSON* SSID = cJSON_GetObjectItem(config, "SSID");
			cJSON* SecurityMode = cJSON_GetObjectItem(config, "SecurityMode");
			cJSON* Pwd = cJSON_GetObjectItem(config, "Pwd");
			cJSON* MaxAssociateNum = cJSON_GetObjectItem(config, "MaxAssociateNum");
			cJSON* SSIDAdvertisementEnabled = cJSON_GetObjectItem(config, "SSIDAdvertisementEnabled");

			if(Radio && Index && Enable && SSID && SecurityMode && Pwd && MaxAssociateNum
					&& SSIDAdvertisementEnabled)
			{
				req->Configurations[i].Radio = Radio->valuestring;
				req->Configurations[i].Index = Index->valuestring;
				req->Configurations[i].Enable = Enable->valuestring;
				req->Configurations[i].SSID = SSID->valuestring;
				req->Configurations[i].SecurityMode = SecurityMode->valuestring;
				req->Configurations[i].Pwd = Pwd->valuestring;
				req->Configurations[i].MaxAssociateNum = MaxAssociateNum->valuestring;
				req->Configurations[i].SSIDAdvertisementEnabled = SSIDAdvertisementEnabled->valuestring;
			}
		}
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceWiFiParameterSyncReq(struct Interface56_WiFiParameterSync_Req req)
{
	std::string l_result;
	int i = 0;
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
	cJSON_AddStringToObject(data, "SyncCode", req.SyncCode.c_str());

	cJSON* Configurations = cJSON_CreateArray();
	if(Configurations == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(data, "Configurations", Configurations);

	for(i = 0 ; i < req.configSize ; i++)
	{
		cJSON *config = cJSON_CreateObject();
		if(config == NULL)
		{
			cJSON_Delete(regJs);
			return l_result;
		}
		cJSON_AddItemToArray(Configurations, config);

		cJSON_AddStringToObject(config, "Radio", req.Configurations[i].Radio.c_str());
		cJSON_AddStringToObject(config, "Index", req.Configurations[i].Index.c_str());
		cJSON_AddStringToObject(config, "Enable", req.Configurations[i].Enable.c_str());
		cJSON_AddStringToObject(config, "SSID", req.Configurations[i].SSID.c_str());
		cJSON_AddStringToObject(config, "SecurityMode", req.Configurations[i].SecurityMode.c_str());
		cJSON_AddStringToObject(config, "Pwd", req.Configurations[i].Pwd.c_str());
		cJSON_AddStringToObject(config, "MaxAssociateNum", req.Configurations[i].MaxAssociateNum.c_str());
		cJSON_AddStringToObject(config, "SSIDAdvertisementEnabled", req.Configurations[i].SSIDAdvertisementEnabled.c_str());
	}

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}
