#include "DeviceDataStore.hpp"
#include <stdio.h>

long DeviceDataStore::getTimestamps()
{
	return time(NULL);
}

void DeviceDataStore::storeDeviceGwToken(std::string p_value)
{
	m_gwToken = p_value;
}

void DeviceDataStore::storeDeviceID(std::string p_value)
{
	m_deviceID = p_value;
}

void DeviceDataStore::storeDeviceToken(std::string p_value)
{
	m_deviceToken = p_value;
}

void DeviceDataStore::storeDeviceAndlinkToken(std::string p_value)
{
	m_andlinkToken = p_value;
}

std::string DeviceDataStore::getDeviceGwToken()
{
	return m_gwToken;
}

std::string DeviceDataStore::getDeviceID()
{
	return m_deviceID;
}

std::string DeviceDataStore::getDeviceToken()
{
	return m_deviceToken;
}

std::string DeviceDataStore::getDeviceAndlinkToken()
{
	return m_andlinkToken;
}

void DeviceDataStore::storeDeviceIPAddr(std::string p_value)
{
	m_deviceIPAddr = p_value;
}

std::string DeviceDataStore::getDeviceIPAddr()
{
	return m_deviceIPAddr;
}

void DeviceDataStore::storeEnctypt(int p_value)
{
	m_encrypt = p_value;
}

int DeviceDataStore::getEnctypt()
{
	return m_encrypt;
}

void DeviceDataStore::storeChallengeCode(std::string p_value)
{
	m_ChallengeCode = p_value;
}

std::string DeviceDataStore::getChallengeCode()
{
	return m_ChallengeCode;
}

void DeviceDataStore::storeHeartbeatInterval(int p_value)
{
	m_heartbeatInterval = p_value;
}

int DeviceDataStore::getHeartbeatInterval()
{
	return m_heartbeatInterval;
}

std::string DeviceBasicConfig::getUpTime()
{
	long l_upTime = time(NULL) - m_startupTimestamps;
	char tmp[128] = { 0 };
	snprintf(tmp, sizeof(tmp), "%d", l_upTime);
	return std::string(tmp);
}
