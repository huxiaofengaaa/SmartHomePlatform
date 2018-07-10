/*
 * AndlinkDeviceControlEvent.hpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */
#pragma once
#include <iostream>
#include <string.h>
#include "cJSON.h"


struct WiFiParameterSync
{
	std::string Radio;
	std::string Index;
	std::string Enable;
	std::string SSID;
	std::string SecurityMode;
	std::string Pwd;
	std::string MaxAssociateNum;
	std::string SSIDAdvertisementEnabled;
};

struct Interface56_WiFiParameterSync_Req
{
	~Interface56_WiFiParameterSync_Req()
	{
		if(Configurations != NULL)
		{
			delete[] Configurations;
			Configurations = NULL;
		}
	}
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;
	std::string data;
	std::string SyncCode;
	struct WiFiParameterSync* Configurations;
	int configSize;
};

struct Interface56_WiFiSwitch_Req
{
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;
	std::string Radio;
	int Enable;
};

struct Interface56_RadioConfig_Req
{
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;
	std::string Radio;
	std::string TransmitPower;
};

struct Interface56_WPS_Req
{
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;
	std::string Radio;
};

struct Interface56_MacFilter_Req
{
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;
	int MacFilterEnable;
	int MacFilterPolicy;
	std::string MacFilterEntries;
};

struct Interface56_Reboot_Req
{
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;
	std::string ControlType;
};

struct Interface56_LEDControl_Req
{
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;
	int LEDOnOff;
};

struct Interface56_RoamingConfig_Req
{
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;
	int RoamingSwitch;
	int LowRSSI24G;
	int LowRSSI5G;
};

struct Interface56_ControlCommon_Resp
{
	std::string ID;
	int respCode;
	std::string restCont;
};

struct Interface56_Unbind_Req
{
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;
	std::string childDeviceId;
};

struct Interface56_Upgrade_Req
{
	std::string RPCMethod;
	std::string ID;
	std::string deviceId;

	std::string childDeviceId;
	std::string fileMode;
	std::string fileType;
	std::string fileUrl;
	int downMode;
};

bool resolveAndlinkDeviceControlCommonResp(std::string msg, struct Interface56_ControlCommon_Resp* req);
std::string buildAndlinkDeviceControlCommonResp(struct Interface56_ControlCommon_Resp req);

bool resolveAndlinkDeviceLEDControlReq(std::string msg, struct Interface56_LEDControl_Req* req);
std::string buildAndlinkDeviceLEDControlReq(struct Interface56_LEDControl_Req req);

bool resolveAndlinkDeviceMacFilterReq(std::string msg, struct Interface56_MacFilter_Req* req);
std::string buildAndlinkDeviceMacFilterReq(struct Interface56_MacFilter_Req req);

bool resolveAndlinkDeviceRadioConfigReq(std::string msg, struct Interface56_RadioConfig_Req* req);
std::string buildAndlinkDeviceRadioConfigReq(struct Interface56_RadioConfig_Req req);

bool resolveAndlinkDeviceRebootReq(std::string msg, struct Interface56_Reboot_Req* req);
std::string buildAndlinkDeviceRebootReq(struct Interface56_Reboot_Req req);

bool resolveAndlinkDeviceRoamingConfigReq(std::string msg, struct Interface56_RoamingConfig_Req* req);
std::string buildAndlinkDeviceRoamingConfigReq(struct Interface56_RoamingConfig_Req req);

bool resolveAndlinkDeviceWiFiParameterSyncReq(std::string msg, struct Interface56_WiFiParameterSync_Req* req);
std::string buildAndlinkDeviceWiFiParameterSyncReq(struct Interface56_WiFiParameterSync_Req req);

bool resolveAndlinkDeviceWiFiSwitchReq(std::string msg, struct Interface56_WiFiSwitch_Req* req);
std::string buildAndlinkDeviceWiFiSwitchReq(struct Interface56_WiFiSwitch_Req req);

bool resolveAndlinkDeviceWPSReq(std::string msg, struct Interface56_WPS_Req* req);
std::string buildAndlinkDeviceWPSReq(struct Interface56_WPS_Req req);

bool resolveAndlinkDeviceUnbindReq(std::string msg, struct Interface56_Unbind_Req* req);
std::string buildAndlinkDeviceUnbindReq(struct Interface56_Unbind_Req req);

bool resolveAndlinkDeviceUpgradeReq(std::string msg, struct Interface56_Upgrade_Req* req);
std::string buildAndlinkDeviceUpgradeReq(struct Interface56_Upgrade_Req req);
