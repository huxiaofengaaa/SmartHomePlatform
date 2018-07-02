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


bool resolve_if56_register_request_msg(std::string msg, struct Interface56_Register_Req* req);

std::string build_register_response_success_msg(struct Interface56_Register_Resp resp);
char* build_register_response_failed_msg(struct Interface56_Register_Resp* resp);

bool resolve_if56_online_request_msg(std::string msg, struct Interface56_Online_Req* req);

std::string build_online_response_success_msg(struct Interface56_Online_Resp resp);
std::string build_online_response_failed_msg(struct Interface56_Online_Resp resp);


