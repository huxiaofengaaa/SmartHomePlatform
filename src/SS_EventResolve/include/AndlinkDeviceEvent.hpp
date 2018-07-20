/*
 * AndlinkDeviceEvent.hpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */

#pragma once

#include <iostream>
#include <string.h>
#include "cJSON.h"

enum class ANDLINKDeviceDataType
{
	E_ANDLINK_DEVICE_REGISTER_REQ,
	E_ANDLINK_DEVICE_REGISTER_RESP,
	E_ANDLINK_DEVICE_ONLINE_REQ,
	E_ANDLINK_DEVICE_ONLINE_RESP,
	E_ANDLINK_DEVICE_AUTH_REQ,
	E_ANDLINK_DEVICE_AUTH_RESP,
	E_ANDLINK_DEVICE_HEARTBEAT_REQ,
	E_ANDLINK_DEVICE_HEARTBEAT_RESP,
	E_ANDLINK_DEVICE_REQUESTPLUGIN_REQ,
	E_ANDLINK_DEVICE_REQUESTPLUGIN_RESP
};

struct Interface56_Register_Req
{
	std::string deviceMac;
	std::string deviceType;
	std::string productToken;
	long timestamp;
};

struct Interface56_Register_Resp
{
	// success
	std::string gwToken;
	std::string deviceId;
	std::string deviceToken;
	std::string andlinkToken;
	// failed
	long respCode;
	std::string respCont;
};

struct Interface56_Online_Req
{
	std::string RPCMethod;
	std::string DevRND;
	std::string deviceId;
	std::string deviceMac;
	std::string deviceType;
	std::string firmwareVersion;
	std::string softwareVersion;
	std::string ipAddress;
	long timestamp;
	std::string deviceVendor;
	std::string deviceModel;
	std::string deviceSn;
	std::string apUplinkType;
	int radio5;
	std::string user_key;
	std::string SyncCode;
};

struct Interface56_Online_Resp
{
	// success
	long timestamp;
	int encrypt;
	std::string ChallengeCode;
	// failed
	int respCode;
	std::string ServerIP;
};

struct Interface56_Auth_Req
{
	std::string RPCMethod;
	std::string MAC;
	std::string CheckSN;
};

struct Interface56_Auth_Resp
{
	int respCode;
	long heartBeatTime;
	std::string MessageServer;
	std::string ServerIP;
};

struct Interface56_Heartbeat_Req
{
	std::string RPCMethod;
	std::string deviceId;
	std::string MAC;
	std::string IPAddr;
};

struct Interface56_Heartbeat_Resp
{
	int respCode;
	long heartBeatTime;
	std::string ServerIP;
};

struct Interface56_RequestPlugIn_Req
{
	std::string RPCMethod;
	std::string Key;
	std::string ServerAddr;
	std::string ServerPort;
};

struct Interface56_RequestPlugIn_Resp
{
	int respCode;
	std::string DevRND;
};

struct Interface56_Disconnect_Req
{
	std::string RPCMethod;
	std::string ID;
};

struct Interface56_Disconnect_Resp
{
	int respCode;
	std::string ID;
};

// resolve request
bool resolve_if56_register_request_msg(std::string msg, struct Interface56_Register_Req* req);
bool resolve_if56_online_request_msg(std::string msg, struct Interface56_Online_Req* req);
bool resolve_if56_auth_request_msg(std::string msg, struct Interface56_Auth_Req* req);
bool resolve_if56_heartbeat_request_msg(std::string msg, struct Interface56_Heartbeat_Req* req);
bool resolve_if56_requestPlugIn_request_msg(std::string msg, struct Interface56_RequestPlugIn_Req* req);
bool resolve_if56_disconnect_request_msg(std::string msg, struct Interface56_Disconnect_Req* req);

// resolve response
bool resolve_if56_register_response_msg(std::string msg, struct Interface56_Register_Resp* resp);
bool resolve_if56_online_response_msg(std::string msg, struct Interface56_Online_Resp* resp);
bool resolve_if56_auth_response_msg(std::string msg, struct Interface56_Auth_Resp* resp);
bool resolve_if56_heartbeat_response_msg(std::string msg, struct Interface56_Heartbeat_Resp* resp);
bool resolve_if56_requestPlugIn_response_msg(std::string msg, struct Interface56_RequestPlugIn_Resp* resp);
bool resolve_if56_disconnect_response_msg(std::string msg, struct Interface56_Disconnect_Resp* resp);

// build request
std::string build_if56_register_request_msg(struct Interface56_Register_Req msg);
std::string build_if56_online_request_msg(struct Interface56_Online_Req msg);
std::string build_if56_auth_request_msg(struct Interface56_Auth_Req msg);
std::string build_if56_heartbeat_request_msg(struct Interface56_Heartbeat_Req msg);
std::string build_if56_requestPlugIn_request_msg(struct Interface56_RequestPlugIn_Req msg);
std::string build_if56_disconnect_request_msg(struct Interface56_Disconnect_Req msg);

// build response
std::string build_if56_register_response_success_msg(struct Interface56_Register_Resp msg);
std::string build_if56_register_response_failed_msg(struct Interface56_Register_Resp msg);
std::string build_if56_online_response_success_msg(struct Interface56_Online_Resp msg);
std::string build_if56_online_response_failed_msg(struct Interface56_Online_Resp msg);
std::string build_if56_auth_response_msg(struct Interface56_Auth_Resp msg);
std::string build_if56_heartbeat_response_msg(struct Interface56_Heartbeat_Resp msg);
std::string build_if56_requestPlugIn_response_msg(struct Interface56_RequestPlugIn_Resp msg);
std::string build_if56_disconnect_response_msg(struct Interface56_Disconnect_Resp msg);
