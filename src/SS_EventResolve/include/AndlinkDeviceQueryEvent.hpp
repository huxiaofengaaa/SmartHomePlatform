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
	std::string ID;
	std::string deviceId;
	int respCode;
	std::string respCont;

	struct DeviceStatus* Devices;
	int DeviceNumber;
};

struct Interface56_WiFiStats_Resp
{
	std::string ID;
	std::string deviceId;
	int respCode;
	std::string respCont;

	struct WiFiStats* Status;
	int StatusNumber;
};

