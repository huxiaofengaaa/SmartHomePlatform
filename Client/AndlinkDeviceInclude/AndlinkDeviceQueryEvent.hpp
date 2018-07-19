/*
 * AndlinkDeviceQueryEvent.hpp
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: Administrator
 */

#pragma once
#include <iostream>
#include <string.h>
#include "cJSON.h"

struct RadioInformation
{
	std::string Radio;
	int Enable;
	std::string TransmitPower;
	int Channel;
};

struct RadioConfigurations
{
	std::string Radio;
	int Index;
	int Enable;
	std::string SSID;
	std::string SecurityMode;
	std::string Pwd;
	int MaxAssociateNum;
	int SSIDAdvertisementEnabled;
};

struct DeviceStatus
{
	std::string MacAddress;
	std::string VMacAddress;
	std::string UpTime;
	std::string Radio;
	std::string SSID;
	std::string RSSI;
	std::string RxRate;
	std::string TxRate;
};

struct WiFiStats
{
	std::string Radio;
	int Index;
	std::string SSID;
	int TotalBytesSent;
	int TotalBytesReceived;
	int TotalPacketsSent;
	int TotalPacketsReceived;
	int ErrorsSent;
	int ErrorsReceived;
	int DiscardPacketsSent;
	int DiscardPacketsReceived;
};

struct Interface56_CommonQuery_Req
{
	std::string RPCMethod;
	std::string ID;
};

struct Interface56_ApConfigInfo_Resp
{
	Interface56_ApConfigInfo_Resp()
	{
		Radios = NULL;
		RadioNumber = 0;
		Configurations = NULL;
		ConfigurationNumber = 0;
	}
	std::string ID;
	std::string deviceId;
	int respCode;
	std::string respCont;

	std::string hardwareVersion;
	std::string softwareVersion;
	int WorkingMode;
	std::string UpTime;
	int MacFilterEnable;
	int MacFilterPolicy;
	std::string MacFilterEntries;
	int LEDOnOff;
	int RoamingSwitch;
	int LowRSSI24G;
	int LowRSSI5G;
	std::string SyncCode;

	struct RadioInformation* Radios;
	int RadioNumber;

	struct RadioConfigurations* Configurations;
	int ConfigurationNumber;
};

struct Interface56_UplinkStatus_Resp
{
	std::string ID;
	std::string deviceId;
	int respCode;
	std::string respCont;

	std::string UplinkType;
	std::string MacAddress;
	std::string Radio;
	std::string SSID;
	int Channel;
	std::string Noise;
	std::string SNR;
	std::string RSSI;
	std::string RxRate;
	std::string TxRate;
};

struct Interface56_STAInfo_Resp
{
	Interface56_STAInfo_Resp()
	{
		Devices = NULL;
		DevicesNumber = 0;
	}
	std::string ID;
	std::string deviceId;
	int respCode;
	std::string respCont;

	struct DeviceStatus* Devices;
	int DevicesNumber;
};

struct Interface56_WiFiStats_Resp
{
	Interface56_WiFiStats_Resp()
	{
		Status = NULL;
		StatusNumber = 0;
	}
	std::string ID;
	std::string deviceId;
	int respCode;
	std::string respCont;

	struct WiFiStats* Status;
	int StatusNumber;
};

struct Interface56_DataReport_Req
{
	std::string RPCMethod;
	std::string MAC;
	std::string eventType;
	long timestamp;
	int respCode;
	std::string restCont;
};

struct Interface56_DataReport_Resp
{
	int respCode;
};

bool resolveAndlinkDeviceApConfigInfoQueryReq(std::string msg, struct Interface56_CommonQuery_Req* req);
std::string buildAndlinkDeviceApConfigInfoQueryReq(std::string ID);
bool resolveAndlinkDeviceApConfigInfoQueryResp(std::string msg, struct Interface56_ApConfigInfo_Resp* resp);
std::string buildAndlinkDeviceApConfigInfoQueryResp(struct Interface56_ApConfigInfo_Resp resp);

bool resolveAndlinkDeviceSTAInfoQueryReq(std::string msg, struct Interface56_CommonQuery_Req* req);
std::string buildAndlinkDeviceSTAInfoQueryReq(std::string ID);
bool resolveAndlinkDeviceSTAInfoQueryResp(std::string msg, struct Interface56_STAInfo_Resp* resp);
std::string buildAndlinkDeviceSTAInfoQueryResp(struct Interface56_STAInfo_Resp resp);

bool resolveAndlinkDeviceUplinkStatusQueryReq(std::string msg, struct Interface56_CommonQuery_Req* req);
std::string buildAndlinkDeviceUplinkStatusQueryReq(std::string ID);
bool resolveAndlinkDeviceUplinkStatusQueryResp(std::string msg, struct Interface56_UplinkStatus_Resp* resp);
std::string buildAndlinkDeviceUplinkStatusQueryResp(struct Interface56_UplinkStatus_Resp resp);

bool resolveAndlinkDeviceWiFiStatsQueryReq(std::string msg, struct Interface56_CommonQuery_Req* req);
std::string buildAndlinkDeviceWiFiStatsQueryReq(std::string ID);
bool resolveAndlinkDeviceWiFiStatsQueryResp(std::string msg, struct Interface56_WiFiStats_Resp* resp);
std::string buildAndlinkDeviceWiFiStatsQueryResp(struct Interface56_WiFiStats_Resp resp);

bool resolveAndlinkDeviceDataReportReq(std::string msg, struct Interface56_DataReport_Req* req);
std::string buildAndlinkDeviceDataReportReq(struct Interface56_DataReport_Req req);
bool resolveAndlinkDeviceDataReportResp(std::string msg, struct Interface56_DataReport_Resp* resp);
std::string buildAndlinkDeviceDataReportResp(struct Interface56_DataReport_Resp resp);
