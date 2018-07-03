/*
 * AndlinkDeviceEvent.hpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */

#pragma once

#include <iostream>

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

struct Interface56_heartbeat_Req
{
	std::string RPCMethod;
	std::string deviceId;
	std::string MAC;
	std::string IPAddr;
};

struct Interface56_heartbeat_Resp
{
	int respCode;
	long heartBeatTime;
	std::string ServerIP;
};

struct Interface56_requestPlugIn_Req
{
	std::string RPCMethod;
	std::string Key;
	std::string ServerAddr;
	std::string ServerPort;
};

struct Interface56_requestPlugIn_Resp
{
	int respCode;
	int DevRND;
};

bool resolve_if56_register_request_msg(std::string msg, struct Interface56_Register_Req* req);

std::string build_register_response_success_msg(struct Interface56_Register_Resp resp);
char* build_register_response_failed_msg(struct Interface56_Register_Resp* resp);

bool resolve_if56_online_request_msg(std::string msg, struct Interface56_Online_Req* req);

std::string build_online_response_success_msg(struct Interface56_Online_Resp resp);
std::string build_online_response_failed_msg(struct Interface56_Online_Resp resp);

bool resolve_if56_auth_request_msg(std::string msg, struct Interface56_Auth_Req* req);
std::string build_auth_response_msg(struct Interface56_Auth_Resp resp);

bool resolve_if56_heartbeat_request_msg(std::string msg, struct Interface56_heartbeat_Req* req);
std::string build_heartbeat_response_msg(struct Interface56_heartbeat_Resp resp);

std::string build_requestPlugIn_request_msg(struct Interface56_requestPlugIn_Req resp);
bool resolve_if56_requestPlugIn_Response_msg(std::string msg, struct Interface56_requestPlugIn_Resp* req);
