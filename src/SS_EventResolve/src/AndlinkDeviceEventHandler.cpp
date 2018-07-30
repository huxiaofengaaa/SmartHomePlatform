#include "AndlinkDeviceEventHandler.hpp"
#include "Aes.h"

std::string AndlinkDeviceEventHandler::run(
		std::string& p_rawData, std::string& p_host, int p_port, int p_sockfd, bool isTCP)
{
	struct Interface56_Register_Req l_registerReq;
	struct Interface56_Online_Req l_onlineReq;
	struct Interface56_Auth_Req l_authReq;
	struct Interface56_Heartbeat_Req l_heartbeatReq;
	struct Interface56_RequestPlugIn_Resp l_pluginResp;
	struct Interface56_Disconnect_Resp l_disconnectResp;

	std::string l_plainResp;
	if(true == resolve_if56_register_request_msg(p_rawData, &l_registerReq))
	{
		std::string l_deviceID = m_ueContextHolder->DeviceRegister(
				l_registerReq.deviceMac, l_registerReq.deviceType, l_registerReq.productToken);
		if(l_deviceID.empty() == false)
		{
			struct Interface56_Register_Resp l_registerResp;
			if(true == m_ueContextHolder->setRegisterResponse(l_deviceID, l_registerResp))
			{
				l_plainResp = build_if56_register_response_success_msg(l_registerResp);
			}
		}
	}
	else if(true == resolve_if56_online_request_msg(p_rawData, &l_onlineReq))
	{
		auto l_deviceid = l_onlineReq.deviceId;
		struct Interface56_Online_Resp l_onlineResp;
		bool l_response = m_ueContextHolder->DeviceOnline(l_onlineReq);
		m_ueContextHolder->setOnlineResponse(l_deviceid, l_onlineResp, l_response);
		if(true == l_response)
		{
			l_plainResp = build_if56_online_response_success_msg(l_onlineResp);
		}
		else
		{
			l_plainResp = build_if56_online_response_failed_msg(l_onlineResp);
		}
	}
	else if(true == resolve_if56_auth_request_msg(p_rawData, &l_authReq))
	{
		auto l_deviceMAC = l_authReq.MAC;
		auto l_deviceCheckSN = l_authReq.CheckSN;
		struct Interface56_Auth_Resp l_authResp;
		bool l_response = m_ueContextHolder->DeviceAuth(l_deviceMAC, l_deviceCheckSN);
		m_ueContextHolder->setAuthResponse(l_response, l_authResp);
		if(true == l_response)
		{
			m_ueContextHolder->updateNetAddress(l_deviceMAC, p_host, p_port, p_sockfd, isTCP);
		}
		l_plainResp = build_if56_auth_response_msg(l_authResp);
	}
	else if(true == resolve_if56_heartbeat_request_msg(p_rawData, &l_heartbeatReq))
	{
		struct Interface56_Heartbeat_Resp l_heartbeatResp;
		auto l_deviceid = l_heartbeatReq.deviceId;
		auto l_deviceMAC = l_heartbeatReq.MAC;
		auto l_deviceIPAddr = l_heartbeatReq.IPAddr;
		bool l_response = m_ueContextHolder->DeviceHeartbeat(l_deviceid, l_deviceMAC, l_deviceIPAddr);
		m_ueContextHolder->setHeartbeatResponse(l_response, l_heartbeatResp);
		l_plainResp = build_if56_heartbeat_response_msg(l_heartbeatResp);
	}
	else if(true == resolve_if56_requestPlugIn_response_msg(p_rawData, &l_pluginResp))
	{
		int l_respCode = l_pluginResp.respCode;
		std::string l_DevRND = l_pluginResp.DevRND;
		auto l_deviceID = m_ueContextHolder->getDeviceIDByUDPAddress(p_host, p_port);
		if(l_deviceID.empty() == true)
		{
			LOG(INFO) << "can find deviceID by UDP address, " << p_host << ":" << p_port;
		}
		m_ueContextHolder->pluginResetStatus(l_deviceID);
	}
	else if(true == resolve_if56_disconnect_response_msg(p_rawData, &l_disconnectResp))
	{

	}
	else
	{
		LOG(INFO) << "Unknown Type msg from " << p_host << ":" << p_port << ", sockfd:"
				<< p_sockfd << ", isTCP: " << isTCP ? "True" : "False";
	}

	return l_plainResp;
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

std::string AndlinkDeviceEventHandler::chiperDecrypt(std::string p_deviceID, std::string p_chiperText)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceID);
	if(!l_uecontext)
	{
		return std::string();
	}
	std::string l_sessionKey = l_uecontext->deviceSn;
	std::string l_initVector = std::string("0000000000000000");
	char* l_plainText = new char[p_chiperText.size()];
	if(!l_plainText)
	{
		return std::string();
	}
	memset(l_plainText, 0, p_chiperText.size());

	std::string l_resultString;
	if(AES_SUCCESS == AES_CBC_Decrypt(p_chiperText.c_str(), p_chiperText.size(),
			l_plainText, p_chiperText.size(), l_sessionKey.c_str(), l_initVector.c_str()))
	{
		l_resultString = std::string(l_plainText);
	}
	if(l_plainText)
	{
		delete[] l_plainText;
	}
	return l_resultString;
}

std::string AndlinkDeviceEventHandler::chiperDecrypt(std::string p_host, int p_port, std::string p_chiperText)
{
	/*
	 * If the corresponding device ID cannot be found based on the TCP IP address and port number,
	 * it indicates that the TCP has just been established, and the device has not been online yet,
	 * and is not associated with the device. Therefore the message should be in plaintext;
	 */
	auto l_deviceID = m_ueContextHolder->getDeviceIDByTCPAddress(p_host, p_port);
	if(l_deviceID.empty() == true)
	{
		return p_chiperText;
	}
	return chiperDecrypt(l_deviceID, p_chiperText);
}

std::string AndlinkDeviceEventHandler::plainEncrypt(std::string p_deviceID, std::string p_plainText)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceID);
	if(!l_uecontext)
	{
		return p_plainText;
	}
	std::string l_sessionKey = l_uecontext->deviceSn;
	std::string l_initVector = std::string("0000000000000000");
	char* l_chiperText = new char[p_plainText.size() + 16];
	if(!l_chiperText)
	{
		return std::string();
	}
	memset(l_chiperText, 0, p_plainText.size() + 16);

	std::string l_resultString;
	if(AES_SUCCESS == AES_CBC_Encrypt(p_plainText.c_str(), p_plainText.size(),
			l_chiperText, p_plainText.size() + 16, l_sessionKey.c_str(), l_initVector.c_str(), PKCS5Padding))
	{
		l_resultString = std::string(l_chiperText);
	}
	if(l_chiperText)
	{
		delete[] l_chiperText;
	}
	return l_resultString;
}
