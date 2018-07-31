#include "DeviceDataStore.hpp"
#include "DataBase.h"
#include <stdio.h>

DeviceReadOnlyData::DeviceReadOnlyData():
	m_deviceMAC(initDeviceMAC()),
	m_deviceType(initDeviceType()),
	m_productToken(initProductToken()),
	m_deviceSn(initDeviceSn()),
	m_firmwareVersion(initFirmWareVersion()),
	m_softwareVersion(initSoftWareVersion()),
	m_deviceVendor(initDeviceVendor()),
	m_deviceModel(initDeviceModel())
{

}

std::string DeviceReadOnlyData::initDeviceMAC() const
{
#ifdef CROSS_BUILD
	char l_deviceMAC[33] = { 0 };
	get_devicemac(l_deviceMAC);
	return std::string(l_deviceMAC);
#else
	RandomGenerator l_random;
	return l_random.generatorRandomMAC();
#endif
}

std::string DeviceReadOnlyData::initDeviceType() const
{
	RandomGenerator l_random;
	return l_random.generatorRandomNumberString(5);
}

std::string DeviceReadOnlyData::initProductToken() const
{
	RandomGenerator l_random;
	return l_random.generatorRandomNumberString(32);
}

std::string DeviceReadOnlyData::initDeviceSn() const
{
	RandomGenerator l_random;
	return l_random.generatorRandomNumberString(16);
}

std::string DeviceReadOnlyData::initFirmWareVersion() const
{
	return "fhhw1.0.0";
}

std::string DeviceReadOnlyData::initSoftWareVersion() const
{
	return "fhsw1.0.0";
}

std::string DeviceReadOnlyData::initDeviceVendor() const
{
	return "fiberhome";
}

std::string DeviceReadOnlyData::initDeviceModel() const
{
	return "unknown";
}

std::string DeviceReadOnlyData::getDeviceMAC() const
{
	return m_deviceMAC;
}

std::string DeviceReadOnlyData::getDeviceType() const
{
	return m_deviceType;
}

std::string DeviceReadOnlyData::getProductToken() const
{
	return m_productToken;
}

std::string DeviceReadOnlyData::getDeviceSn() const
{
	return m_deviceSn;
}

std::string DeviceReadOnlyData::getFirmWareVersion() const
{
	return m_firmwareVersion;
}

std::string DeviceReadOnlyData::getSoftWareVersion() const
{
	return m_softwareVersion;
}

std::string DeviceReadOnlyData::getDeviceVendor() const
{
	return m_deviceVendor;
}

std::string DeviceReadOnlyData::getDeviceModel() const
{
	return m_deviceModel;
}


