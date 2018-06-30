/*
 * message_build.hpp
 *
 *  Created on: Jun 25, 2018
 *      Author: xiaofenh
 */

#ifndef CJSON_DEMO_UPLINK_MSG_BUILD_HPP_
#define CJSON_DEMO_UPLINK_MSG_BUILD_HPP_

#include <iostream>
using namespace std;

typedef struct _DeviceRegisterReq
{
	string deviceMac;
	string deviceType;
	string productToken;
	long timestamp;
}DeviceRegisterReq;

typedef struct _DeviceOnlineReq
{
	string RPCMethod;
	string DevRND;
	string deviceId;
	string deviceMac;
	string deviceType;
	string firmwareVersion;
	string softwareVersion;
	string ipAddress;
	long timestamp;
	string deviceVendor;
	string deviceModel;
	string deviceSn;
	string apUplinkType;
	int radio5;
	string user_key;
	string SyncCode;
}DeviceOnlineReq;

typedef struct _DeviceAuthReq
{
	string RPCMethod;
	string MAC;
	string CheckSN;
}DeviceAuthReq;

typedef struct _DeviceHeartBeatReq
{
	string RPCMethod;
	string deviceId;
	string MAC;
	string IPAddr;
}DeviceHeartBeatReq;

typedef struct _DeviceRequestPlugResp
{
	int respCode;
	int DevRND;
}DeviceRequestPlugResp;

typedef struct _DeviceDisconnectResp
{
	int respCode;
	string ID;
}DeviceDisconnectResp;

//typedef struct _DeviceEventInformReq
//{
//	string RPCMethod;
//	string MAC;
//	string eventType;
//	long timestamp;
//	//TODO
//}DeviceEventInformReq;

void enable_debug_mode(bool p_enable);

string build_device_register_request_msg(DeviceRegisterReq p_msg);
string build_device_online_request_msg(DeviceOnlineReq p_msg);
string build_device_auth_request_msg(DeviceAuthReq p_msg);
string build_device_heartbeat_request_msg(DeviceHeartBeatReq p_msg);

#endif /* CJSON_DEMO_UPLINK_MSG_BUILD_HPP_ */
