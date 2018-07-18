#include "DeviceDataStore.hpp"
#include "RandomGenerator.hpp"

std::string DeviceDataStore::getDeviceMAC()
{
	if(m_deviceMAC.empty() == true)
	{
		RandomGenerator l_random;
		m_deviceMAC = l_random.generatorRandomNumberString(12);
	}
	return m_deviceMAC;
}

std::string DeviceDataStore::getDeviceType()
{
	if(m_deviceType.empty() == true)
	{
		RandomGenerator l_random;
		m_deviceType = l_random.generatorRandomNumberString(5);
	}
	return m_deviceType;
}

std::string DeviceDataStore::getProductToken()
{
	if(m_productToken.empty() == true)
	{
		RandomGenerator l_random;
		m_productToken = l_random.generatorRandomNumberString(32);
	}
	return m_productToken;
}

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

std::string DeviceDataStore::getFirmWareVersion()
{
	return "fhhw1.0.0";
}

std::string DeviceDataStore::getSoftWareVersion()
{
	return "fhsw1.0.0";
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

std::string DeviceDataStore::getDeviceCheckSN()
{
	RandomGenerator l_random;
	return l_random.generatorRandomNumberString(32);
}

void DeviceDataStore::storeHeartbeatInterval(int p_value)
{
	m_heartbeatInterval = p_value;
}

int DeviceDataStore::getHeartbeatInterval()
{
	return m_heartbeatInterval;
}
