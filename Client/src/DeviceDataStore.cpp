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

std::string DeviceBasicConfig::getUpTime()
{
	long l_upTime = time(NULL) - m_startupTimestamps;
	char tmp[128] = { 0 };
	snprintf(tmp, sizeof(tmp), "%d", l_upTime);
	return std::string(tmp);
}
