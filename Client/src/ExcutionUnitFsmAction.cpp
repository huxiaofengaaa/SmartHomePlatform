#include "ExcutionUnitClient.hpp"
#include "AndlinkDeviceEvent.hpp"
#include <stdio.h>
#include <unistd.h>

bool ExcutionUnitClient::deviceRegister()
{
	struct Interface56_Register_Req l_registerReq =
	{
			m_deviceDataStore.getDeviceMAC(),
			m_deviceDataStore.getDeviceType(),
			m_deviceDataStore.getProductToken(),
			m_deviceDataStore.getTimestamps()
	};
	std::string l_req = build_if56_register_request_msg(l_registerReq);
	std::string l_resp = writeAndReadUDPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	struct Interface56_Register_Resp l_registerResp;
	if(false == resolve_if56_register_response_msg(l_resp, &l_registerResp))
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

bool ExcutionUnitClient::deviceOnline()
{
	struct Interface56_Online_Req l_onlineReq;
	l_onlineReq.deviceId = m_deviceDataStore.getDeviceID();
	l_onlineReq.deviceMac = m_deviceDataStore.getDeviceMAC();
	l_onlineReq.deviceType = m_deviceDataStore.getDeviceType();
	l_onlineReq.firmwareVersion = m_deviceDataStore.getFirmWareVersion();
	l_onlineReq.softwareVersion = m_deviceDataStore.getSoftWareVersion();
	l_onlineReq.ipAddress = m_deviceDataStore.getDeviceIPAddr();
	l_onlineReq.timestamp = m_deviceDataStore.getTimestamps();
	std::string l_req = build_if56_online_request_msg(l_onlineReq);
	std::string l_resp = writeAndReadUDPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	struct Interface56_Online_Resp l_onlineResp;
	if(false == resolve_if56_online_response_msg(l_resp, &l_onlineResp))
	{
		return false;
	}
	if(l_onlineResp.respCode != 0)
	{
		return false;
	}
	m_deviceDataStore.storeEnctypt(l_onlineResp.encrypt);
	m_deviceDataStore.storeChallengeCode(l_onlineResp.ChallengeCode);
	printf("deviceOnline success\n");
	return true;
}

bool ExcutionUnitClient::deviceAuth()
{
	struct Interface56_Auth_Req l_authReq;
	l_authReq.MAC = m_deviceDataStore.getDeviceMAC();
	l_authReq.CheckSN = m_deviceDataStore.getDeviceCheckSN();
	std::string l_req = build_if56_auth_request_msg(l_authReq);
	std::string l_resp = writeAndReadUDPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	struct Interface56_Auth_Resp l_authResp;
	if(false == resolve_if56_auth_response_msg(l_resp, &l_authResp))
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

bool ExcutionUnitClient::deviceHeartbeat()
{
	struct Interface56_Heartbeat_Req l_heartbeatReq;
	l_heartbeatReq.deviceId = m_deviceDataStore.getDeviceID();
	l_heartbeatReq.MAC = m_deviceDataStore.getDeviceMAC();
	l_heartbeatReq.IPAddr = m_deviceDataStore.getDeviceIPAddr();
	std::string l_req = build_if56_heartbeat_request_msg(l_heartbeatReq);
	std::string l_resp = writeAndReadUDPString(l_req);
	if(l_resp.empty() == true)
	{
		return false;
	}
	struct Interface56_Heartbeat_Resp l_heartbeatResp;
	if(false == resolve_if56_heartbeat_response_msg(l_resp, &l_heartbeatResp))
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

bool ExcutionUnitClient::deviceUDPDownlinkAction()
{
	std::string l_downlink = readUDPString(2);
	if(l_downlink.empty() == true)
	{
		return false;
	}
	struct Interface56_RequestPlugIn_Req l_pluginReq;
	if(false == resolve_if56_requestPlugIn_request_msg(l_downlink, &l_pluginReq))
	{
		return false;
	}

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
	printf("devicePluginOnline success\n");
	return true;
}

bool ExcutionUnitClient::devicePluginAuth()
{
	printf("devicePluginAuth success\n");
	return true;
}

bool ExcutionUnitClient::devicePluginHeartbeat()
{
	printf("devicePluginHeartbeat success\n");
	return true;
}

bool ExcutionUnitClient::deviceTCPDownlinkAction()
{
	return true;
}

bool ExcutionUnitClient::deviceDisconnect()
{
	printf("deviceDisconnect success\n");
	return true;
}



