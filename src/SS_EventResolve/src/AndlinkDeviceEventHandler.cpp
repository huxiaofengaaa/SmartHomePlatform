/*
 * AndlinkDeviceEventHandler.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */

#include "AndlinkDeviceEventHandler.hpp"

std::string AndlinkDeviceEventHandler::run(std::string& p_rawData,
		std::string& p_host, int p_port, int p_sockfd, bool isTCP)
{
	struct Interface56_Register_Req l_registerReq;
	struct Interface56_Online_Req l_onlineReq;
	struct Interface56_Auth_Req l_authReq;
	struct Interface56_Heartbeat_Req l_heartbeatReq;

	if(true == resolve_if56_register_request_msg(p_rawData, &l_registerReq))
	{
		std::string l_deviceID = m_ueContextHolder->DeviceRegister(
				l_registerReq.deviceMac, l_registerReq.deviceType, l_registerReq.productToken);
		m_ueContextHolder->updateNetAddress(l_deviceID, p_host, p_port, p_sockfd, isTCP);
		struct Interface56_Register_Resp l_registerResp;
		m_ueContextHolder->setRegisterResponse(l_deviceID, l_registerResp);
		return build_if56_register_response_success_msg(l_registerResp);
	}
	else if(true == resolve_if56_online_request_msg(p_rawData, &l_onlineReq))
	{
		if(true == m_ueContextHolder->DeviceOnline(l_onlineReq))
		{
			auto l_deviceid = l_onlineReq.deviceId;
			struct Interface56_Online_Resp l_onlineResp;
			m_ueContextHolder->setOnlineResponse(l_deviceid, l_onlineResp, true);
			m_ueContextHolder->updateNetAddress(l_deviceid, p_host, p_port, p_sockfd, isTCP);
			return build_if56_online_response_success_msg(l_onlineResp);
		}
		else
		{
			auto l_deviceid = l_onlineReq.deviceId;
			struct Interface56_Online_Resp l_onlineResp;
			m_ueContextHolder->setOnlineResponse(l_deviceid, l_onlineResp, false);
			return build_if56_online_response_failed_msg(l_onlineResp);
		}
	}
	else if(true == resolve_if56_auth_request_msg(p_rawData, &l_authReq))
	{
		auto l_deviceMAC = l_authReq.MAC;
		auto l_deviceCheckSN = l_authReq.CheckSN;
		struct Interface56_Auth_Resp l_authResp;
		m_ueContextHolder->setAuthResponse(l_deviceMAC, l_deviceCheckSN, l_authResp);
		return build_if56_auth_response_msg(l_authResp);
	}
	else if(true == resolve_if56_heartbeat_request_msg(p_rawData, &l_heartbeatReq))
	{
		struct Interface56_Heartbeat_Resp l_heartbeatResp;
		auto l_deviceid = l_heartbeatReq.deviceId;
		auto l_deviceMAC = l_heartbeatReq.MAC;
		auto l_deviceIPAddr = l_heartbeatReq.IPAddr;
		if(true == m_ueContextHolder->setHeartbeatResponse(l_deviceid, l_deviceMAC,
				l_deviceIPAddr, l_heartbeatResp))
		{
			m_ueContextHolder->updateNetAddress(l_deviceid, p_host, p_port, p_sockfd, isTCP);
		}
		return build_if56_heartbeat_response_msg(l_heartbeatResp);
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
