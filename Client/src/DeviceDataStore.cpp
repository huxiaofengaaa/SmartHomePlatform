#include "DeviceDataStore.hpp"
#include <stdio.h>

int DeviceBasicConfig::getWorkMode() const
{
	return m_workMode;
}

long DeviceDataStore::getTimestamps()
{
	return time(NULL);
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



std::string DeviceBasicConfig::getUpTime()
{
	long l_upTime = time(NULL) - m_startupTimestamps;
	char tmp[128] = { 0 };
	snprintf(tmp, sizeof(tmp), "%d", l_upTime);
	return std::string(tmp);
}
