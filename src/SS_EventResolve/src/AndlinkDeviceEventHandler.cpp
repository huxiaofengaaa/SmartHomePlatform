/*
 * AndlinkDeviceEventHandler.cpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */

#include "AndlinkDeviceEventHandler.hpp"

bool AndlinkDeviceEventHandler::updateNetAddress(std::string p_deviceID,
		std::string p_host, int p_port, int p_sockfd, bool isTCP)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceID);
	if(l_uecontext)
	{
		if(isTCP == true)
		{
			l_uecontext->peerTCPHost = p_host;
			l_uecontext->peerTCPPort = p_port;
			l_uecontext->TCPSocketfd = p_sockfd;
		}
		else
		{
			l_uecontext->peerUDPHost = p_host;
			l_uecontext->peerUDPPort = p_port;
			l_uecontext->UDPSocketfd = p_sockfd;
		}
		return true;
	}
	return false;
}

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
		auto l_uecontext = m_ueContextHolder->getRef(l_deviceID);

		updateNetAddress(l_deviceID, p_host, p_port, p_sockfd, isTCP);
		struct Interface56_Register_Resp l_registerResp =
		{
				l_uecontext->gwToken,
				l_uecontext->deviceId,
				l_uecontext->deviceToken,
				l_uecontext->andlinkToken
		};
		return build_if56_register_response_success_msg(l_registerResp);
	}
	else if(true == resolve_if56_online_request_msg(p_rawData, &l_onlineReq))
	{
		if(true == m_ueContextHolder->DeviceOnline(l_onlineReq))
		{
			auto l_deviceid = l_onlineReq.deviceId;
			auto l_uecontext = m_ueContextHolder->getRef(l_deviceid);
			updateNetAddress(l_deviceid, p_host, p_port, p_sockfd, isTCP);
			struct Interface56_Online_Resp l_onlineResp =
			{
					0, l_uecontext->encrypt, l_uecontext->ChallengeCode,
					0, ""
			};
			return build_if56_online_response_success_msg(l_onlineResp);
		}
		else
		{
			struct Interface56_Online_Resp l_onlineResp;
			return build_if56_online_response_failed_msg(l_onlineResp);
		}
	}
	else if(true == resolve_if56_auth_request_msg(p_rawData, &l_authReq))
	{
		struct Interface56_Auth_Resp l_authResp;
		return build_if56_auth_response_msg(l_authResp);
	}
	else if(true == resolve_if56_heartbeat_request_msg(p_rawData, &l_heartbeatReq))
	{
		struct Interface56_Heartbeat_Resp l_heartbeatResp;
		auto l_deviceid = l_heartbeatReq.deviceId;
		auto l_uecontext = m_ueContextHolder->getRef(l_deviceid);
		if(!l_uecontext || l_uecontext->deviceMac != l_heartbeatReq.MAC)
		{
			l_heartbeatResp.respCode = -5;
		}
		else
		{
			updateNetAddress(l_deviceid, p_host, p_port, p_sockfd, isTCP);
			l_uecontext->lastHeartbeat = time(NULL);
			l_heartbeatResp.respCode = 0;
			l_heartbeatResp.heartBeatTime = 15;
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
