#include "ExcutionUnitClient.hpp"
#include "AndlinkDeviceEvent.hpp"
#include <stdio.h>
#include <unistd.h>

std::string ExcutionUnitClient::getRegisterReq()
{
	struct Interface56_Register_Req l_registerReq =
	{
			m_deviceDataStore.m_basicConfig.getDeviceMAC(),
			m_deviceDataStore.m_basicConfig.getDeviceType(),
			m_deviceDataStore.m_basicConfig.getProductToken(),
			m_deviceDataStore.getTimestamps()
	};
	return build_if56_register_request_msg(l_registerReq);
}

std::string ExcutionUnitClient::getOnlineReq()
{
	struct Interface56_Online_Req l_onlineReq;
	l_onlineReq.DevRND = m_deviceDataStore.generatorDevRND();
	l_onlineReq.deviceId = m_deviceDataStore.getDeviceID();
	l_onlineReq.deviceMac = m_deviceDataStore.m_basicConfig.getDeviceMAC();
	l_onlineReq.deviceType = m_deviceDataStore.m_basicConfig.getDeviceType();
	l_onlineReq.firmwareVersion = m_deviceDataStore.m_basicConfig.getFirmWareVersion();
	l_onlineReq.softwareVersion = m_deviceDataStore.m_basicConfig.getSoftWareVersion();
	l_onlineReq.ipAddress = m_deviceDataStore.getDeviceIPAddr();
	l_onlineReq.timestamp = m_deviceDataStore.getTimestamps();
	l_onlineReq.deviceVendor = m_deviceDataStore.m_basicConfig.getDeviceVendor();
	l_onlineReq.deviceModel = m_deviceDataStore.m_basicConfig.getDeviceModel();
	l_onlineReq.deviceSn = m_deviceDataStore.getDeviceSn();
	l_onlineReq.apUplinkType = m_deviceDataStore.m_uplinkInterface.getUplinkType();
	l_onlineReq.radio5 = m_deviceDataStore.m_radioConfig.get5GSupport();
	l_onlineReq.SyncCode = m_deviceDataStore.m_radioConfig.getSyncCode();
	return build_if56_online_request_msg(l_onlineReq);
}

std::string ExcutionUnitClient::getAuthReq()
{
	struct Interface56_Auth_Req l_authReq;
	l_authReq.MAC = m_deviceDataStore.m_basicConfig.getDeviceMAC();
	l_authReq.CheckSN = m_deviceDataStore.getDeviceCheckSN();
	return build_if56_auth_request_msg(l_authReq);
}

std::string ExcutionUnitClient::getHeartbeatReq()
{
	struct Interface56_Heartbeat_Req l_heartbeatReq;
	l_heartbeatReq.deviceId = m_deviceDataStore.getDeviceID();
	l_heartbeatReq.MAC = m_deviceDataStore.m_basicConfig.getDeviceMAC();
	l_heartbeatReq.IPAddr = m_deviceDataStore.getDeviceIPAddr();
	return build_if56_heartbeat_request_msg(l_heartbeatReq);
}

bool ExcutionUnitClient::checkerRegisterResp(std::string p_resp)
{
	struct Interface56_Register_Resp l_registerResp;
	if(false == resolve_if56_register_response_msg(p_resp, &l_registerResp))
	{
		return false;
	}
	m_deviceDataStore.storeDeviceGwToken(l_registerResp.gwToken);
	m_deviceDataStore.storeDeviceID(l_registerResp.deviceId);
	m_deviceDataStore.storeDeviceToken(l_registerResp.deviceToken);
	m_deviceDataStore.storeDeviceAndlinkToken(l_registerResp.andlinkToken);
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
	m_deviceDataStore.storeDevRND(m_deviceDataStore.getDevRND());
	m_deviceDataStore.storeEnctypt(l_onlineResp.encrypt);
	m_deviceDataStore.storeChallengeCode(l_onlineResp.ChallengeCode);
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
	m_deviceDataStore.storeHeartbeatInterval(l_authResp.heartBeatTime);
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

bool ExcutionUnitClient::deviceUDPDownlinkAction()
{
	std::string l_downlink = readUDPString(2);
	if(l_downlink.empty() == true)
	{
		return false;
	}
	struct Interface56_RequestPlugIn_Req l_pluginReq;

	if(true == resolve_if56_requestPlugIn_request_msg(l_downlink, &l_pluginReq))
	{
		struct Interface56_RequestPlugIn_Resp l_pluginResp;
		l_pluginResp.respCode = 0;
		std::string l_resp = build_if56_requestPlugIn_response_msg(l_pluginResp);
		if(l_resp.empty() == true)
		{
			return false;
		}
		if(writeUDPString(l_resp) <= 0)
		{
			return false;
		}
		return true;
	}
	return false;
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
	std::string l_resp = writeAndReadTCPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	return checkerHeartbeatResp(l_resp);
}

bool ExcutionUnitClient::deviceTCPDownlinkAction()
{
	std::string l_downlink = readTCPString(2);
	if(l_downlink.empty() == true)
	{
		return false;
	}
	struct Interface56_Disconnect_Req l_disconnectReq;

	if(true == resolve_if56_disconnect_request_msg(l_downlink, &l_disconnectReq))
	{
		struct Interface56_Disconnect_Resp l_disconnectResp;
		l_disconnectResp.respCode = 0;
		l_disconnectResp.ID = l_disconnectReq.ID;
		std::string l_resp = build_if56_disconnect_response_msg(l_disconnectResp);
		if(l_resp.empty() == true)
		{
			return false;
		}
		if(writeTCPString(l_resp) <= 0)
		{
			return false;
		}
		return true;
	}
	else if(true == deviceQueryChecker(l_downlink))
	{

	}
	else if(true == deviceControlChecker(l_downlink))
	{

	}
	else
	{
		printf("unknown downlink message\n");
	}
	return false;
}

bool ExcutionUnitClient::deviceDisconnect()
{
	shutDownTCPClient();
	printf("deviceDisconnect success\n");
	return true;
}



