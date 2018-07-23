#include "DeviceDataStore.hpp"
#include <stdio.h>

DeviceRunTimeData::DeviceRunTimeData()
{
	initDevRND();
}

void DeviceRunTimeData::initDevRND()
{
	RandomGenerator l_random;
	m_DevRND = l_random.generatorRandomNumberString(32);
}

std::string DeviceRunTimeData::getDevRND() const
{
	return m_DevRND;
}

std::string DeviceRunTimeData::getLastPluginKey() const
{
	return m_lastPluginKey;
}

std::string DeviceRunTimeData::getDeviceGwToken()
{
	return m_gwToken;
}

std::string DeviceRunTimeData::getDeviceID()
{
	return m_deviceID;
}

std::string DeviceRunTimeData::getDeviceToken()
{
	return m_deviceToken;
}

std::string DeviceRunTimeData::getDeviceAndlinkToken()
{
	return m_andlinkToken;
}

int DeviceRunTimeData::getHeartbeatInterval()
{
	return m_heartbeatInterval;
}

void DeviceRunTimeData::storePluginKey(std::string p_value)
{
	m_lastPluginKey = p_value;
}

void DeviceRunTimeData::storeDeviceGwToken(std::string p_value)
{
	m_gwToken = p_value;
}

void DeviceRunTimeData::storeDeviceID(std::string p_value)
{
	m_deviceID = p_value;
}

void DeviceRunTimeData::storeDeviceToken(std::string p_value)
{
	m_deviceToken = p_value;
}

void DeviceRunTimeData::storeDeviceAndlinkToken(std::string p_value)
{
	m_andlinkToken = p_value;
}

void DeviceRunTimeData::storeHeartbeatInterval(int p_value)
{
	m_heartbeatInterval = p_value;
}

void DeviceRunTimeData::storeEnctypt(int p_value)
{
	m_encrypt = p_value;
}

int DeviceRunTimeData::getEnctypt()
{
	return m_encrypt;
}

void DeviceRunTimeData::storeChallengeCode(std::string p_value)
{
	m_ChallengeCode = p_value;
}

std::string DeviceRunTimeData::getChallengeCode()
{
	return m_ChallengeCode;
}
