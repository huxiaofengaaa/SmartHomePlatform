#include "ExcutionUnitClient.hpp"
#include "AndlinkDeviceEvent.hpp"
#include "MD5.h"
#include <stdio.h>
#include <unistd.h>

std::string ExcutionUnitClient::getRegisterReq()
{
	struct Interface56_Register_Req l_registerReq =
	{
			m_deviceDataStore.m_readOnlyData.getDeviceMAC(),
			m_deviceDataStore.m_readOnlyData.getDeviceType(),
			m_deviceDataStore.m_readOnlyData.getProductToken(),
			m_deviceDataStore.m_runTimeData.getTimestamps()
	};
	return build_if56_register_request_msg(l_registerReq);
}

std::string ExcutionUnitClient::getOnlineReq()
{
	struct Interface56_Online_Req l_onlineReq;
	l_onlineReq.DevRND = m_deviceDataStore.m_runTimeData.getDevRND();
	l_onlineReq.deviceId = m_deviceDataStore.m_runTimeData.getDeviceID();
	l_onlineReq.deviceMac = m_deviceDataStore.m_readOnlyData.getDeviceMAC();
	l_onlineReq.deviceType = m_deviceDataStore.m_readOnlyData.getDeviceType();
	l_onlineReq.firmwareVersion = m_deviceDataStore.m_readOnlyData.getFirmWareVersion();
	l_onlineReq.softwareVersion = m_deviceDataStore.m_readOnlyData.getSoftWareVersion();
	l_onlineReq.ipAddress = m_deviceDataStore.m_runTimeData.getDeviceIPAddr();
	l_onlineReq.timestamp = m_deviceDataStore.m_runTimeData.getTimestamps();
	l_onlineReq.deviceVendor = m_deviceDataStore.m_readOnlyData.getDeviceVendor();
	l_onlineReq.deviceModel = m_deviceDataStore.m_readOnlyData.getDeviceModel();
	l_onlineReq.deviceSn = m_deviceDataStore.m_readOnlyData.getDeviceSn();
	l_onlineReq.apUplinkType = m_deviceDataStore.m_uplinkInterface.getUplinkType();
	l_onlineReq.radio5 = m_deviceDataStore.m_radioConfig.get5GSupport();
	l_onlineReq.user_key = m_deviceDataStore.m_runTimeData.getUserKey();
	l_onlineReq.SyncCode = m_deviceDataStore.m_runTimeData.getSyncCode();
	return build_if56_online_request_msg(l_onlineReq);
}

std::string ExcutionUnitClient::getAuthReq()
{
	struct Interface56_Auth_Req l_authReq;
	l_authReq.MAC = m_deviceDataStore.m_readOnlyData.getDeviceMAC();

	std::string l_Challenge = m_deviceDataStore.m_runTimeData.getChallengeCode();
	std::string l_DeviceSN = m_deviceDataStore.m_readOnlyData.getDeviceSn();

	char l_md5Result[64] = { 0 };
	char l_srcString[256] = { 0 };
	snprintf(l_srcString, sizeof(l_srcString), "%s%s", l_Challenge.c_str(), l_DeviceSN.c_str());
	Compute_string_md5(l_srcString, l_md5Result);

	l_authReq.CheckSN = l_md5Result;
	return build_if56_auth_request_msg(l_authReq);
}

std::string ExcutionUnitClient::getHeartbeatReq()
{
	struct Interface56_Heartbeat_Req l_heartbeatReq;
	l_heartbeatReq.deviceId = m_deviceDataStore.m_runTimeData.getDeviceID();
	l_heartbeatReq.MAC = m_deviceDataStore.m_readOnlyData.getDeviceMAC();
	l_heartbeatReq.IPAddr = m_deviceDataStore.m_runTimeData.getDeviceIPAddr();
	return build_if56_heartbeat_request_msg(l_heartbeatReq);
}

bool ExcutionUnitClient::checkerRegisterResp(std::string p_resp)
{
	struct Interface56_Register_Resp l_registerResp;
	if(false == resolve_if56_register_response_msg(p_resp, &l_registerResp))
	{
		return false;
	}
	m_deviceDataStore.m_runTimeData.storeDeviceGwToken(l_registerResp.gwToken);
	m_deviceDataStore.m_runTimeData.storeDeviceID(l_registerResp.deviceId);
	m_deviceDataStore.m_runTimeData.storeDeviceToken(l_registerResp.deviceToken);
	m_deviceDataStore.m_runTimeData.storeDeviceAndlinkToken(l_registerResp.andlinkToken);
	printf("deviceRegister success\n");
	return true;
}

bool ExcutionUnitClient::checkerOnlineResp(std::string p_resp)
{
	struct Interface56_Online_Resp l_onlineResp;
	if(false == resolve_if56_online_response_msg(p_resp, &l_onlineResp))
	{
		return false;
	}
	if(l_onlineResp.respCode != 0)
	{
		return false;
	}
	m_deviceDataStore.m_runTimeData.storeEnctypt(l_onlineResp.encrypt);
	m_deviceDataStore.m_runTimeData.storeChallengeCode(l_onlineResp.ChallengeCode);
	printf("deviceOnline success\n");
	return true;
}

bool ExcutionUnitClient::checkerAuthResp(std::string p_resp)
{
	struct Interface56_Auth_Resp l_authResp;
	if(false == resolve_if56_auth_response_msg(p_resp, &l_authResp))
	{
		return false;
	}
	if(l_authResp.respCode != 2)
	{
		return false;
	}
	m_deviceDataStore.m_runTimeData.storeHeartbeatInterval(l_authResp.heartBeatTime);
	printf("deviceAuth success\n");
	return true;
}

bool ExcutionUnitClient::checkerHeartbeatResp(std::string p_resp)
{
	struct Interface56_Heartbeat_Resp l_heartbeatResp;
	if(false == resolve_if56_heartbeat_response_msg(p_resp, &l_heartbeatResp))
	{
		return false;
	}
	if(l_heartbeatResp.respCode != 0)
	{
		return false;
	}
	printf("deviceHeartbeat success\n");
	return true;
}

bool ExcutionUnitClient::deviceRegister()
{
	std::string l_req = getRegisterReq();
	std::string l_resp = writeAndReadUDPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	return checkerRegisterResp(l_resp);
}

bool ExcutionUnitClient::deviceOnline()
{
	std::string l_req = getOnlineReq();
	std::string l_resp = writeAndReadUDPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	return checkerOnlineResp(l_resp);
}

bool ExcutionUnitClient::deviceAuth()
{
	std::string l_req = getAuthReq();
	std::string l_resp = writeAndReadUDPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	return checkerAuthResp(l_resp);
}

bool ExcutionUnitClient::deviceHeartbeat()
{
	std::string l_req = getHeartbeatReq();
	std::string l_resp = writeAndReadUDPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	return checkerHeartbeatResp(l_resp);
}

int ExcutionUnitClient::deviceUDPDownlinkAction()
{
	const int l_resultNoAction = 0;
	const int l_resultReturnToBoot = -1;
	const int l_resultShouldPlugin = 1;
	std::string l_downlink = readUDPString(2);
	if(l_downlink.empty() == true)
	{
		return l_resultNoAction;
	}
	struct Interface56_RequestPlugIn_Req l_pluginReq;

	if(true == resolve_if56_requestPlugIn_request_msg(l_downlink, &l_pluginReq))
	{

		int returnResult = l_resultNoAction;
		std::string l_key = l_pluginReq.Key;
		std::string l_LocalDevRND = m_deviceDataStore.m_runTimeData.getDevRND();
		std::string l_LocalSN = m_deviceDataStore.m_readOnlyData.getDeviceSn();
		std::string l_lastKey = m_deviceDataStore.m_runTimeData.getLastPluginKey();

		char l_md5Result[64] = { 0 };
		char l_srcString[256] = { 0 };
		snprintf(l_srcString, sizeof(l_srcString), "%s%s", l_LocalDevRND.c_str(), l_LocalSN.c_str());
		Compute_string_md5(l_srcString, l_md5Result);

		struct Interface56_RequestPlugIn_Resp l_pluginResp;
		if(l_lastKey == l_key)
		{
			l_pluginResp.respCode = 0;
			l_pluginResp.DevRND = l_LocalDevRND;
			returnResult = l_resultNoAction;
		}
		else
		{
			if(l_key == l_md5Result)
			{
				l_pluginResp.respCode = 0;
				m_deviceDataStore.m_runTimeData.storePluginKey(l_key);
				m_deviceDataStore.m_runTimeData.initDevRND();
				l_pluginResp.DevRND = m_deviceDataStore.m_runTimeData.getDevRND();
				returnResult = l_resultShouldPlugin;
			}
			else
			{
				l_pluginResp.respCode = -1;
				returnResult = l_resultReturnToBoot;
			}
		}

		std::string l_resp = build_if56_requestPlugIn_response_msg(l_pluginResp);
		if(l_resp.empty() == true)
		{
			return l_resultNoAction;
		}
		if(writeUDPString(l_resp) <= 0)
		{
			return l_resultNoAction;
		}
		return returnResult;
	}
	return l_resultNoAction;
}

bool ExcutionUnitClient::devicePlugin()
{
	bool l_devicePluginResult = startTCPClient();
	if(l_devicePluginResult == true)
	{
		printf("devicePlugin success\n");
		return true;
	}
	else
	{
		return false;
	}
}

bool ExcutionUnitClient::devicePluginOnline()
{
	std::string l_req = getOnlineReq();
	std::string l_resp = writeAndReadTCPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	return checkerOnlineResp(l_resp);
}

bool ExcutionUnitClient::devicePluginAuth()
{
	std::string l_req = getAuthReq();
	std::string l_resp = writeAndReadTCPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	return checkerAuthResp(l_resp);
}

bool ExcutionUnitClient::devicePluginHeartbeat()
{
	std::string l_req = getHeartbeatReq();
	std::string l_resp = writeAndReadUDPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	return checkerHeartbeatResp(l_resp);
}

bool ExcutionUnitClient::deviceTCPDownlinkAction()
{
	/*
	 * In the function deviceTCPDownlinkAction, you need to pay special
	 * attention to the return value. Currently in STATE_PLUGIN_HEARTBEAT
	 * state, if you need to switch state, it should return true, otherwise
	 * you should return false even if you process the message successfully;
	 * STATE_PLUGIN_HEARTBEAT next state is STATE_DISCONNECT, it will cause
	 * TCP disconnection, and re-establish TCP;
	 */
	std::string l_downlink = readTCPString(2);
	if(l_downlink.empty() == true)
	{
		return false;
	}
	std::string l_plainReq = l_downlink;
	int l_enctyptType = m_deviceDataStore.m_runTimeData.getEnctypt();
	if(false == is_jSON_data(l_downlink) &&  l_enctyptType == 1)
	{
		/*
		 * First, the message format is detected, and only when the message
		 * is not in the JSON format, the AES decryption is attempted;
		 */
		l_plainReq = chiperDecrypt(l_downlink);
	}
	printf("Recv Control Req Msg: %s\n", l_plainReq.c_str());

	std::string l_plainResp;
	struct Interface56_Disconnect_Req l_disconnectReq;
	if(true == resolve_if56_disconnect_request_msg(l_plainReq, &l_disconnectReq))
	{
		struct Interface56_Disconnect_Resp l_disconnectResp;
		l_disconnectResp.respCode = 0;
		l_disconnectResp.ID = l_disconnectReq.ID;
		l_plainResp = build_if56_disconnect_response_msg(l_disconnectResp);
		printf("Send Resp Msg, enctyptType:%d, size:%d, : %s\n",
				l_enctyptType, l_plainResp.size(), l_plainResp.c_str());
		if(l_enctyptType == 1)
		{
			l_plainResp = plainEncrypt(l_plainResp);
		}
		if(false == writeTCPString(l_plainResp))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}

	l_plainResp = deviceQueryChecker(l_plainReq);
	if(l_plainResp.empty() == false)
	{
		printf("Send Query Resp Msg, enctyptType:%d, size:%d, : %s\n",
				l_enctyptType, l_plainResp.size(), l_plainResp.c_str());
		if(l_enctyptType == 1)
		{
			l_plainResp = plainEncrypt(l_plainResp);
		}
		if(false == writeTCPString(l_plainResp))
		{
			printf("send interface5_6 msg failed\n");
		}
		return false;
	}

	l_plainResp == deviceControlChecker(l_plainReq);
	if(l_plainResp.empty() == false)
	{
		printf("Send Control Resp Msg, enctyptType:%d, size:%d, : %s\n",
				l_enctyptType, l_plainResp.size(), l_plainResp.c_str());
		if(l_enctyptType == 1)
		{
			l_plainResp = plainEncrypt(l_plainResp);
		}
		if(false == writeTCPString(l_plainResp))
		{
			printf("send interface5_6 msg failed\n");
		}
		return false;
	}

	printf("unknown downlink message\n");
	return false;
}

bool ExcutionUnitClient::deviceDisconnect()
{
	shutDownTCPClient();
	printf("deviceDisconnect success\n");
	return true;
}



