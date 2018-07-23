#include "DeviceDataStore.hpp"
#include <stdio.h>

DeviceRunTimeData::DeviceRunTimeData():
	m_SyncCode(initSyncCode()),
	m_startupTimestamps(time(NULL))
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

void DeviceRunTimeData::storeDeviceIPAddr(std::string p_value)
{
	m_deviceIPAddr = p_value;
}

std::string DeviceRunTimeData::getDeviceIPAddr()
{
	return m_deviceIPAddr;
}

long DeviceRunTimeData::getTimestamps()
{
	return time(NULL);
}

std::string DeviceRunTimeData::getUpTime()
{
	long l_upTime = time(NULL) - m_startupTimestamps;
	char tmp[128] = { 0 };
	snprintf(tmp, sizeof(tmp), "%d", l_upTime);
	return std::string(tmp);
}

std::string DeviceRunTimeData::getUserKey()
{
	RandomGenerator l_random;
	if(m_UserKey.empty() == true)
	{
		m_UserKey = l_random.generatorRandomNumberString(32);
	}
	return m_UserKey;
}
