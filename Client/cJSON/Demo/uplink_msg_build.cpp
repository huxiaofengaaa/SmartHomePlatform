
#include "uplink_msg_build.hpp"

#include "cJSON.h"
#include <iostream>
using namespace std;

static bool g_debug_mode = false;

void enable_debug_mode(bool p_enable)
{
	g_debug_mode = p_enable;
}

string build_device_register_request_msg(DeviceRegisterReq p_msg)
{
	cJSON* obj = cJSON_CreateObject();
	{
		cJSON_AddItemToObject(obj, "deviceMac", cJSON_CreateString(p_msg.deviceMac.c_str()));
		cJSON_AddItemToObject(obj, "deviceType", cJSON_CreateString(p_msg.deviceType.c_str()));
		cJSON_AddItemToObject(obj, "productToken", cJSON_CreateString(p_msg.productToken.c_str()));
		cJSON_AddItemToObject(obj, "timestamp", cJSON_CreateNumber(p_msg.timestamp));
	}

	if(g_debug_mode == true)
	{
		cout << cJSON_Print(obj) << endl;
	}

	string result = cJSON_PrintUnformatted(obj);
	cJSON_Delete(obj);
	return result;
}

string build_device_online_request_msg(DeviceOnlineReq p_msg)
{
	cJSON* obj = cJSON_CreateObject();
	{
		cJSON_AddItemToObject(obj, "RPCMethod", cJSON_CreateString("boot"));
		cJSON_AddItemToObject(obj, "DevRND", cJSON_CreateString(p_msg.DevRND.c_str()));
		cJSON_AddItemToObject(obj, "deviceId", cJSON_CreateString(p_msg.deviceId.c_str()));
		cJSON_AddItemToObject(obj, "deviceMac", cJSON_CreateString(p_msg.deviceMac.c_str()));
		cJSON_AddItemToObject(obj, "deviceType", cJSON_CreateString(p_msg.deviceType.c_str()));
		cJSON_AddItemToObject(obj, "firmwareVersion", cJSON_CreateString(p_msg.firmwareVersion.c_str()));
		cJSON_AddItemToObject(obj, "softwareVersion", cJSON_CreateString(p_msg.softwareVersion.c_str()));
		cJSON_AddItemToObject(obj, "ipAddress", cJSON_CreateString(p_msg.ipAddress.c_str()));
		cJSON_AddItemToObject(obj, "timestamp", cJSON_CreateNumber(p_msg.timestamp));
	}

	cJSON* subobj = cJSON_CreateObject();
	{
		cJSON_AddItemToObject(subobj, "deviceVendor", cJSON_CreateString(p_msg.deviceVendor.c_str()));
		cJSON_AddItemToObject(subobj, "deviceModel", cJSON_CreateString(p_msg.deviceModel.c_str()));
		cJSON_AddItemToObject(subobj, "deviceSn", cJSON_CreateString(p_msg.deviceSn.c_str()));
		cJSON_AddItemToObject(subobj, "apUplinkType", cJSON_CreateString(p_msg.apUplinkType.c_str()));
		cJSON_AddItemToObject(subobj, "radio5", cJSON_CreateNumber(p_msg.radio5));
		cJSON_AddItemToObject(subobj, "user_key", cJSON_CreateString(p_msg.user_key.c_str()));
		cJSON_AddItemToObject(subobj, "SyncCode", cJSON_CreateString(p_msg.SyncCode.c_str()));
	}

	cJSON_AddItemToObject(obj, "XData", subobj);

	if(g_debug_mode == true)
	{
		cout << cJSON_Print(obj) << endl;
	}

	string result = cJSON_PrintUnformatted(obj);
	cJSON_Delete(obj);
	return result;
}

string build_device_auth_request_msg(DeviceAuthReq p_msg)
{
	cJSON* obj = cJSON_CreateObject();
	{
		cJSON_AddItemToObject(obj, "RPCMethod", cJSON_CreateString("Register"));
		cJSON_AddItemToObject(obj, "MAC", cJSON_CreateString(p_msg.MAC.c_str()));
		cJSON_AddItemToObject(obj, "CheckSN", cJSON_CreateString(p_msg.CheckSN.c_str()));
	}

	if(g_debug_mode == true)
	{
		cout << cJSON_Print(obj) << endl;
	}

	string result = cJSON_PrintUnformatted(obj);
	cJSON_Delete(obj);
	return result;
}

string build_device_heartbeat_request_msg(DeviceHeartBeatReq p_msg)
{
	cJSON* obj = cJSON_CreateObject();
	{
		cJSON_AddItemToObject(obj, "RPCMethod", cJSON_CreateString("heartbeat"));
		cJSON_AddItemToObject(obj, "deviceId", cJSON_CreateString(p_msg.deviceId.c_str()));
		cJSON_AddItemToObject(obj, "MAC", cJSON_CreateString(p_msg.MAC.c_str()));
		cJSON_AddItemToObject(obj, "IPAddr", cJSON_CreateString(p_msg.IPAddr.c_str()));
	}

	if(g_debug_mode == true)
	{
		cout << cJSON_Print(obj) << endl;
	}

	string result = cJSON_PrintUnformatted(obj);
	cJSON_Delete(obj);
	return result;
}

int main(int argc, char** argv)
{
	enable_debug_mode(true);

	DeviceRegisterReq registerReq;
	cout << build_device_register_request_msg(registerReq) << endl;

	DeviceOnlineReq onlineReq;
	cout << build_device_online_request_msg(onlineReq) << endl;

	DeviceAuthReq authReq;
	cout << build_device_auth_request_msg(authReq) << endl;

	DeviceHeartBeatReq heartbeatReq;
	cout << build_device_heartbeat_request_msg(heartbeatReq) << endl;
	return 0;
}
