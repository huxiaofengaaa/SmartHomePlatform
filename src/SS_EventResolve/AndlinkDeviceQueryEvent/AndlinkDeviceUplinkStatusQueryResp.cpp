/*
 * AndlinkDeviceUplinkStatusQueryResp.cpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceQueryEvent.hpp"

bool resolveAndlinkDeviceUplinkStatusQueryResp(std::string msg, struct Interface56_UplinkStatus_Resp* resp)
{
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

		cJSON* UplinkType = cJSON_GetObjectItem(data, "UplinkType");
		cJSON* MacAddress = cJSON_GetObjectItem(data, "MacAddress");
		cJSON* Radio = cJSON_GetObjectItem(data, "Radio");
		cJSON* SSID = cJSON_GetObjectItem(data, "SSID");
		cJSON* Channel = cJSON_GetObjectItem(data, "Channel");
		cJSON* Noise = cJSON_GetObjectItem(data, "Noise");
		cJSON* SNR = cJSON_GetObjectItem(data, "SNR");
		cJSON* RSSI = cJSON_GetObjectItem(data, "RSSI");
		cJSON* RxRate = cJSON_GetObjectItem(data, "RxRate");
		cJSON* TxRate = cJSON_GetObjectItem(data, "TxRate");
		if(UplinkType && MacAddress && Radio && SSID && Channel && Noise
				&& SNR && RSSI && RxRate && TxRate)
		{
			resp->UplinkType = UplinkType->valuestring;
			resp->MacAddress = MacAddress->valuestring;
			resp->Radio = Radio->valuestring;
			resp->SSID = SSID->valuestring;
			resp->Channel = Channel->valueint;
			resp->Noise = Noise->valuestring;
			resp->SNR = SNR->valuestring;
			resp->RSSI = RSSI->valuestring;
			resp->RxRate = RxRate->valuestring;
			resp->TxRate = TxRate->valuestring;
		}
		cJSON_Delete(obj);
		return true;
	}
	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceUplinkStatusQueryResp(struct Interface56_UplinkStatus_Resp resp)
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

	cJSON_AddStringToObject(data, "UplinkType", resp.UplinkType.c_str());
	cJSON_AddStringToObject(data, "MacAddress", resp.MacAddress.c_str());
	cJSON_AddStringToObject(data, "Radio", resp.Radio.c_str());
	cJSON_AddStringToObject(data, "SSID", resp.SSID.c_str());
	cJSON_AddNumberToObject(data, "Channel", resp.Channel);
	cJSON_AddStringToObject(data, "Noise", resp.Noise.c_str());
	cJSON_AddStringToObject(data, "SNR", resp.SNR.c_str());
	cJSON_AddStringToObject(data, "RSSI", resp.RSSI.c_str());
	cJSON_AddStringToObject(data, "RxRate", resp.RxRate.c_str());
	cJSON_AddStringToObject(data, "TxRate", resp.TxRate.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}
