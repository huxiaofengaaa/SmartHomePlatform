/*
 * AndlinkDeviceApConfigInfoQueryResp.cpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceQueryEvent.hpp"

bool resolveAndlinkDeviceApConfigInfoQueryResp(std::string msg, struct Interface56_ApConfigInfo_Resp* resp)
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

		cJSON* hardwareVersion = cJSON_GetObjectItem(data, "hardwareVersion");
		cJSON* softwareVersion = cJSON_GetObjectItem(data, "softwareVersion");
		cJSON* WorkingMode = cJSON_GetObjectItem(data, "WorkingMode");
		cJSON* UpTime = cJSON_GetObjectItem(data, "UpTime");
		cJSON* MacFilterEnable = cJSON_GetObjectItem(data, "MacFilterEnable");
		cJSON* MacFilterPolicy = cJSON_GetObjectItem(data, "MacFilterPolicy");
		cJSON* MacFilterEntries = cJSON_GetObjectItem(data, "MacFilterEntries");
		cJSON* LEDOnOff = cJSON_GetObjectItem(data, "LEDOnOff");
		cJSON* RoamingSwitch = cJSON_GetObjectItem(data, "RoamingSwitch");
		cJSON* LowRSSI24G = cJSON_GetObjectItem(data, "LowRSSI24G");
		cJSON* LowRSSI5G = cJSON_GetObjectItem(data, "LowRSSI5G");
		cJSON* SyncCode = cJSON_GetObjectItem(data, "SyncCode");

		if(hardwareVersion && softwareVersion && WorkingMode && UpTime && MacFilterEnable
				&& MacFilterPolicy && MacFilterEntries && LEDOnOff && RoamingSwitch
				&& LowRSSI24G && LowRSSI5G && SyncCode)
		{
			resp->hardwareVersion = hardwareVersion->valuestring;
			resp->softwareVersion = softwareVersion->valuestring;
			resp->WorkingMode = WorkingMode->valueint;
			resp->UpTime = UpTime->valuestring;
			resp->MacFilterEnable = MacFilterEnable->valueint;
			resp->MacFilterPolicy = MacFilterPolicy->valueint;
			resp->MacFilterEntries = MacFilterEntries->valuestring;
			resp->LEDOnOff = LEDOnOff->valueint;
			resp->RoamingSwitch = RoamingSwitch->valueint;
			resp->LowRSSI24G = LowRSSI24G->valueint;
			resp->LowRSSI5G = LowRSSI5G->valueint;
			resp->SyncCode = SyncCode->valuestring;
		}

		cJSON* Radios = cJSON_GetObjectItem(data, "Radios");
		if(Radios == NULL)
		{
			cJSON_Delete(obj);
			return true;
		}

		int l_RadiosNumber = cJSON_GetArraySize(Radios);
		resp->Radios = new RadioInformation[l_RadiosNumber];
		resp->RadioNumber = l_RadiosNumber;
		for(i = 0 ; i < l_RadiosNumber ; i++)
		{
			cJSON* l_radio = cJSON_GetArrayItem(Radios, i);
			if(l_radio == NULL)
			{
				continue;
			}

			cJSON* Radio = cJSON_GetObjectItem(l_radio, "Radio");
			cJSON* Enable = cJSON_GetObjectItem(l_radio, "Enable");
			cJSON* TransmitPower = cJSON_GetObjectItem(l_radio, "TransmitPower");
			cJSON* Channel = cJSON_GetObjectItem(l_radio, "Channel");
			if(Radio && Enable && TransmitPower && Channel)
			{
				resp->Radios[i].Radio = Radio->valuestring;
				resp->Radios[i].Enable = Enable->valueint;
				resp->Radios[i].TransmitPower = TransmitPower->valuestring;
				resp->Radios[i].Channel = Channel->valueint;
			}
		}

		cJSON* Configurations = cJSON_GetObjectItem(data, "Configurations");
		if(Configurations == NULL)
		{
			cJSON_Delete(obj);
			return true;
		}

		int l_ConfigurationsNumber = cJSON_GetArraySize(Configurations);
		resp->Configurations = new RadioConfigurations[l_ConfigurationsNumber];
		resp->ConfigurationNumber = l_ConfigurationsNumber;
		for(i = 0 ; i < l_ConfigurationsNumber ; i++)
		{
			cJSON* l_Configuration = cJSON_GetArrayItem(Configurations, i);
			if(l_Configuration == NULL)
			{
				continue;
			}

			cJSON* Radio = cJSON_GetObjectItem(l_Configuration, "Radio");
			cJSON* Index = cJSON_GetObjectItem(l_Configuration, "Index");
			cJSON* Enable = cJSON_GetObjectItem(l_Configuration, "Enable");
			cJSON* SSID = cJSON_GetObjectItem(l_Configuration, "SSID");
			cJSON* SecurityMode = cJSON_GetObjectItem(l_Configuration, "SecurityMode");
			cJSON* Pwd = cJSON_GetObjectItem(l_Configuration, "Pwd");
			cJSON* MaxAssociateNum = cJSON_GetObjectItem(l_Configuration, "MaxAssociateNum");
			cJSON* SSIDAdvertisementEnabled = cJSON_GetObjectItem(l_Configuration,
					"SSIDAdvertisementEnabled");
			if(Radio && Index && Enable && SSID && SecurityMode && Pwd && MaxAssociateNum
					&& SSIDAdvertisementEnabled)
			{
				resp->Configurations[i].Radio = Radio->valuestring;
				resp->Configurations[i].Index = Index->valueint;
				resp->Configurations[i].Enable = Enable->valueint;
				resp->Configurations[i].SSID = SSID->valuestring;
				resp->Configurations[i].SecurityMode = SecurityMode->valuestring;
				resp->Configurations[i].Pwd = Pwd->valuestring;
				resp->Configurations[i].MaxAssociateNum = MaxAssociateNum->valueint;
				resp->Configurations[i].SSIDAdvertisementEnabled = SSIDAdvertisementEnabled->valueint;
			}
		}

		cJSON_Delete(obj);
		return true;
	}

	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceApConfigInfoQueryResp(struct Interface56_ApConfigInfo_Resp resp)
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

	cJSON_AddStringToObject(data, "hardwareVersion", resp.hardwareVersion.c_str());
	cJSON_AddStringToObject(data, "softwareVersion", resp.softwareVersion.c_str());
	cJSON_AddNumberToObject(data, "WorkingMode", resp.WorkingMode);
	cJSON_AddStringToObject(data, "UpTime", resp.UpTime.c_str());
	cJSON_AddNumberToObject(data, "MacFilterEnable", resp.MacFilterEnable);
	cJSON_AddNumberToObject(data, "MacFilterPolicy", resp.MacFilterPolicy);
	cJSON_AddStringToObject(data, "MacFilterEntries", resp.MacFilterEntries.c_str());
	cJSON_AddNumberToObject(data, "LEDOnOff", resp.LEDOnOff);
	cJSON_AddNumberToObject(data, "RoamingSwitch", resp.RoamingSwitch);
	cJSON_AddNumberToObject(data, "LowRSSI24G", resp.LowRSSI24G);
	cJSON_AddNumberToObject(data, "LowRSSI5G", resp.LowRSSI5G);
	cJSON_AddStringToObject(data, "SyncCode", resp.SyncCode.c_str());

	cJSON* Radios = cJSON_CreateArray();
	if(Radios == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(data, "Radios", Radios);
	for(i = 0 ; i < resp.RadioNumber ; i++)
	{
		cJSON *l_Radios = cJSON_CreateObject();
		if(l_Radios == NULL)
		{
			cJSON_Delete(regJs);
			return l_result;
		}
		cJSON_AddItemToArray(Radios, l_Radios);

		cJSON_AddStringToObject(l_Radios, "Radio", resp.Radios[i].Radio.c_str());
		cJSON_AddNumberToObject(l_Radios, "Enable", resp.Radios[i].Enable);
		cJSON_AddStringToObject(l_Radios, "TransmitPower", resp.Radios[i].TransmitPower.c_str());
		cJSON_AddNumberToObject(l_Radios, "Channel", resp.Radios[i].Channel);
	}

	cJSON* Configurations = cJSON_CreateArray();
	if(Configurations == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(data, "Configurations", Configurations);
	for(i = 0 ; i < resp.ConfigurationNumber ; i++)
	{
		cJSON *l_Configurations = cJSON_CreateObject();
		if(l_Configurations == NULL)
		{
			cJSON_Delete(regJs);
			return l_result;
		}
		cJSON_AddItemToArray(Configurations, l_Configurations);

		cJSON_AddStringToObject(l_Configurations, "Radio", resp.Configurations[i].Radio.c_str());
		cJSON_AddNumberToObject(l_Configurations, "Index", resp.Configurations[i].Index);
		cJSON_AddNumberToObject(l_Configurations, "Enable", resp.Configurations[i].Enable);
		cJSON_AddStringToObject(l_Configurations, "SSID", resp.Configurations[i].SSID.c_str());
		cJSON_AddStringToObject(l_Configurations, "SecurityMode",
				resp.Configurations[i].SecurityMode.c_str());
		cJSON_AddStringToObject(l_Configurations, "Pwd",
				resp.Configurations[i].Pwd.c_str());
		cJSON_AddNumberToObject(l_Configurations, "MaxAssociateNum",
				resp.Configurations[i].MaxAssociateNum);
		cJSON_AddNumberToObject(l_Configurations, "SSIDAdvertisementEnabled",
				resp.Configurations[i].SSIDAdvertisementEnabled);
	}

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}



