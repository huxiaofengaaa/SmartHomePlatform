/*
 * AndlinkDeviceEventBuilder.cpp
 *
 *  Created on: 2018Äê7ÔÂ11ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceEventBuilder.hpp"
#include "RandomGenerator.hpp"
#include "MD5.h"
#include "AES.h"

std::string AndlinkDeviceEventBuilder::buildPlugIuRequest(std::string p_deviceid,
		std::string p_host, int p_port)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	if(!l_uecontext)
	{
		return std::string();
	}
	RandomGenerator l_generator;
	struct Interface56_RequestPlugIn_Req req;
	req.ServerAddr = p_host;
	req.ServerPort = std::to_string(p_port);

	std::string l_devSN = l_uecontext->deviceSn;
	std::string l_devRND = l_uecontext->DevRND;
	char l_md5Result[64] = { 0 };
	char l_srcString[256] = { 0 };
	snprintf(l_srcString, sizeof(l_srcString), "%s%s", l_devRND.c_str(), l_devSN.c_str());
	Compute_string_md5(l_srcString, l_md5Result);

	req.Key = std::string(l_md5Result);
	return build_if56_requestPlugIn_request_msg(req);
}

std::string AndlinkDeviceEventBuilder::buildDisconnectRequest(std::string p_deviceid)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	struct Interface56_Disconnect_Req req;
	return build_if56_disconnect_request_msg(req);
}

std::string AndlinkDeviceEventBuilder::chiperDecrypt(std::string p_deviceID, std::string p_chiperText)
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

std::string AndlinkDeviceEventBuilder::plainEncrypt(std::string p_deviceID, std::string p_plainText)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceID);
	if(!l_uecontext)
	{
		return std::string();
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

