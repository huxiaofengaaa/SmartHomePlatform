/*
 * AndlinkDeviceEventHandler.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */

#include "AndlinkDeviceEventHandler.hpp"

std::string AndlinkDeviceEventHandler::run(
		std::string& p_rawData, std::string& p_host, int p_port, int p_sockfd, bool isTCP)
{
	struct Interface56_Register_Req l_registerReq;
	struct Interface56_Online_Req l_onlineReq;
	struct Interface56_Auth_Req l_authReq;
	struct Interface56_Heartbeat_Req l_heartbeatReq;
	struct Interface56_RequestPlugIn_Resp l_pluginResp;
	struct Interface56_Disconnect_Resp l_disconnectResp;

	if(true == resolve_if56_register_request_msg(p_rawData, &l_registerReq))
	{
		std::string l_deviceID = m_ueContextHolder->DeviceRegister(
				l_registerReq.deviceMac, l_registerReq.deviceType, l_registerReq.productToken);
		if(l_deviceID.empty() == false)
		{
			m_ueContextHolder->updateNetAddress(l_deviceID, p_host, p_port, p_sockfd, isTCP);
			struct Interface56_Register_Resp l_registerResp;
			if(true == m_ueContextHolder->setRegisterResponse(l_deviceID, l_registerResp))
			{
				return build_if56_register_response_success_msg(l_registerResp);
			}
		}
		return std::string();
	}
	else if(true == resolve_if56_online_request_msg(p_rawData, &l_onlineReq))
	{
		auto l_deviceid = l_onlineReq.deviceId;
		struct Interface56_Online_Resp l_onlineResp;
		bool l_response = m_ueContextHolder->DeviceOnline(l_onlineReq);
		if(true == l_response)
		{
			m_ueContextHolder->updateNetAddress(l_deviceid, p_host, p_port, p_sockfd, isTCP);
		}
		m_ueContextHolder->setOnlineResponse(l_deviceid, l_onlineResp, l_response);
		return build_if56_online_response_failed_msg(l_onlineResp);
	}
	else if(true == resolve_if56_auth_request_msg(p_rawData, &l_authReq))
	{
		auto l_deviceMAC = l_authReq.MAC;
		auto l_deviceCheckSN = l_authReq.CheckSN;
		struct Interface56_Auth_Resp l_authResp;
		bool l_response = m_ueContextHolder->DeviceAuth(l_deviceMAC, l_deviceCheckSN);
		m_ueContextHolder->setAuthResponse(l_response, l_authResp);
		return build_if56_auth_response_msg(l_authResp);
	}
	else if(true == resolve_if56_heartbeat_request_msg(p_rawData, &l_heartbeatReq))
	{
		struct Interface56_Heartbeat_Resp l_heartbeatResp;
		auto l_deviceid = l_heartbeatReq.deviceId;
		auto l_deviceMAC = l_heartbeatReq.MAC;
		auto l_deviceIPAddr = l_heartbeatReq.IPAddr;
		bool l_response = m_ueContextHolder->DeviceHeartbeat(l_deviceid, l_deviceMAC, l_deviceIPAddr);
		if(true == l_response)
		{
			m_ueContextHolder->updateNetAddress(l_deviceid, p_host, p_port, p_sockfd, isTCP);
		}
		m_ueContextHolder->setHeartbeatResponse(l_response, l_heartbeatResp);
		return build_if56_heartbeat_response_msg(l_heartbeatResp);
	}
	else if(true == resolve_if56_requestPlugIn_response_msg(p_rawData, &l_pluginResp))
	{
		int l_respCode = l_pluginResp.respCode;
		std::string l_DevRND = l_pluginResp.DevRND;
	}
	else if(true == resolve_if56_disconnect_response_msg(p_rawData, &l_disconnectResp))
	{

	}

	return std::string();
}

std::string AndlinkDeviceEventHandler::run(std::shared_ptr<EventTypeUDPClientDataObject> p_event)
{
	if(!p_event)
	{
		return std::string();
	}
	return run(p_event->m_rawData, p_event->m_host, p_event->m_port, p_event->m_serverSocketFd, false);
}

std::string AndlinkDeviceEventHandler::run(std::shared_ptr<EventTypeTCPClientDataObject> p_event)
{
	if(!p_event)
	{
		return std::string();
	}
	return run(p_event->m_rawData, p_event->m_host, p_event->m_port, p_event->m_socketfd, true);
}
