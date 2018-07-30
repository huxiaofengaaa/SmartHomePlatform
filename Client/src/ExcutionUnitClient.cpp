#include "ExcutionUnitClient.hpp"
#include <unistd.h>
#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "AES.h"

ExcutionUnitClient::ExcutionUnitClient():
	UDPClient(ANDLINK_SERVER_HOST, ANDLINK_SERVER_PORT),
	TCPClient(ANDLINK_SERVER_HOST, ANDLINK_SERVER_PORT+1),
	FsmManager(),
	m_exitFlag(false)
{

}

ExcutionUnitClient::~ExcutionUnitClient()
{

}

void ExcutionUnitClient::startup()
{
	startUDPClient();
	char buffer[128] = { 0 };
	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "%s:%d", getLocalUDPHost().c_str(), getLocalUDPPort());
	m_deviceDataStore.m_runTimeData.storeDeviceIPAddr(std::string(buffer));
	mainloop();
}

void ExcutionUnitClient::shutdown()
{

}

void ExcutionUnitClient::mainloop()
{
	while(m_exitFlag == false)
	{
		runFsmManager();
//		sleep(1);
	}
}

std::string ExcutionUnitClient::chiperDecrypt(std::string p_chiperText)
{
	std::string l_sessionKey = m_deviceDataStore.m_readOnlyData.getDeviceSn();
	std::string l_initVector = std::string("0000000000000000");
	char* l_plainText = new char[p_chiperText.size()];
	if(!l_plainText)
	{
		std::cout << "ExcutionUnitClient::chiperDecrypt new char failed\n";
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
	std::cout << "recv msg " << l_resultString << std::endl;
	return l_resultString;
}

std::string ExcutionUnitClient::plainEncrypt(std::string p_plainText)
{
	std::string l_sessionKey = m_deviceDataStore.m_readOnlyData.getDeviceSn();
	std::string l_initVector = std::string("0000000000000000");
	char* l_chiperText = new char[p_plainText.size() + 16];
	if(!l_chiperText)
	{
		std::cout << "ExcutionUnitClient::plainEncrypt new char failed\n";
		return std::string();
	}
	memset(l_chiperText, 0, p_plainText.size() + 16);

	std::string l_resultString;
	if(AES_SUCCESS == AES_CBC_Encrypt(
			p_plainText.c_str(), p_plainText.size(), l_chiperText, p_plainText.size() + 16,
			l_sessionKey.c_str(), l_initVector.c_str(), PKCS5Padding))
	{
		printf("l_chiperText length %d \n", strlen(l_chiperText));
		l_resultString = std::string(l_chiperText);
	}
	if(l_chiperText)
	{
		delete[] l_chiperText;
	}
	std::cout << "send msg " << p_plainText << std::endl;
	return l_resultString;
}
