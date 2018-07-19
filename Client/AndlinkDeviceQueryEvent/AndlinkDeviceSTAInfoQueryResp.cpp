/*
 * AndlinkDeviceSTAInfoQueryResp.cpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceQueryEvent.hpp"

bool resolveAndlinkDeviceSTAInfoQueryResp(std::string msg, struct Interface56_STAInfo_Resp* resp)
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

		cJSON* Devices = cJSON_GetObjectItem(data, "Devices");
		if(Devices == NULL)
		{
			cJSON_Delete(obj);
			return true;
		}

		int l_DevicesNumber = cJSON_GetArraySize(Devices);
		resp->Devices = new DeviceStatus[l_DevicesNumber];
		resp->DevicesNumber = l_DevicesNumber;
		for(i = 0 ; i < l_DevicesNumber ; i++)
		{
			cJSON* l_device = cJSON_GetArrayItem(Devices, i);
			if(l_device == NULL)
			{
				continue;
			}

			cJSON* MacAddress = cJSON_GetObjectItem(l_device, "MacAddress");
			cJSON* VMacAddress = cJSON_GetObjectItem(l_device, "VMacAddress");
			cJSON* UpTime = cJSON_GetObjectItem(l_device, "UpTime");
			cJSON* Radio = cJSON_GetObjectItem(l_device, "Radio");
			cJSON* SSID = cJSON_GetObjectItem(l_device, "SSID");
			cJSON* RSSI = cJSON_GetObjectItem(l_device, "RSSI");
			cJSON* RxRate = cJSON_GetObjectItem(l_device, "RxRate");
			cJSON* TxRate = cJSON_GetObjectItem(l_device, "TxRate");
			if(MacAddress && VMacAddress && UpTime && Radio && SSID && RSSI && RxRate && TxRate)
			{
				resp->Devices[i].MacAddress = MacAddress->valuestring;
				resp->Devices[i].VMacAddress = VMacAddress->valuestring;
				resp->Devices[i].UpTime = UpTime->valuestring;
				resp->Devices[i].Radio = Radio->valuestring;
				resp->Devices[i].SSID = SSID->valuestring;
				resp->Devices[i].RSSI = RSSI->valuestring;
				resp->Devices[i].RxRate = RxRate->valuestring;
				resp->Devices[i].TxRate = TxRate->valuestring;
			}
		}
		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceSTAInfoQueryResp(struct Interface56_STAInfo_Resp resp)
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

	cJSON* Devices = cJSON_CreateArray();
	if(Devices == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(data, "Devices", Devices);

	for(i = 0 ; i < resp.DevicesNumber ; i++)
	{
		cJSON *l_Devices = cJSON_CreateObject();
		if(l_Devices == NULL)
		{
			cJSON_Delete(regJs);
			return l_result;
		}
		cJSON_AddItemToArray(Devices, l_Devices);

		cJSON_AddStringToObject(l_Devices, "MacAddress", resp.Devices[i].MacAddress.c_str());
		cJSON_AddStringToObject(l_Devices, "VMacAddress", resp.Devices[i].VMacAddress.c_str());
		cJSON_AddStringToObject(l_Devices, "UpTime", resp.Devices[i].UpTime.c_str());
		cJSON_AddStringToObject(l_Devices, "Radio", resp.Devices[i].Radio.c_str());
		cJSON_AddStringToObject(l_Devices, "SSID", resp.Devices[i].SSID.c_str());
		cJSON_AddStringToObject(l_Devices, "RSSI", resp.Devices[i].RSSI.c_str());
		cJSON_AddStringToObject(l_Devices, "RxRate", resp.Devices[i].RxRate.c_str());
		cJSON_AddStringToObject(l_Devices, "TxRate", resp.Devices[i].TxRate.c_str());
	}

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}

