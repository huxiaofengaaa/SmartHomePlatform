#include "DeviceDataStore.hpp"
#include "DataBase.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

std::string DeviceReadOnlyData::initDeviceMAC() const
{
#ifdef CROSS_BUILD
	char l_deviceMAC[33] = { 0 };
	char rcvBuf[LINE_LEN] = { 0 };
    get_device_wanmac(rcvBuf);
    format_mac_from_colon(l_deviceMAC, rcvBuf);
    std::cout << "DeviceReadOnlyData - DeviceMAC : " << l_deviceMAC << std::endl;
    return std::string(l_deviceMAC);
#else
	RandomGenerator l_random;
	std::string l_result = l_random.generatorRandomMAC();
	std::cout << "DeviceReadOnlyData - DeviceMAC : " << l_result << std::endl;
	return l_result;
#endif
}

std::string DeviceReadOnlyData::initDeviceType() const
{
#ifdef CROSS_BUILD
	char deviceType[33] = { 0 };
	get_devicetype(deviceType);
	std::cout << "DeviceReadOnlyData - DeviceType : " << deviceType << std::endl;
	return std::string(deviceType);
#else
	RandomGenerator l_random;
	std::string l_result = l_random.generatorRandomNumberString(5);
	std::cout << "DeviceReadOnlyData - DeviceType : " << l_result << std::endl;
	return l_result;
#endif
}

std::string DeviceReadOnlyData::initProductToken() const
{
	RandomGenerator l_random;
	return l_random.generatorRandomNumberString(32);
}

std::string DeviceReadOnlyData::initDeviceSn() const
{
#ifdef CROSS_BUILD
	char deviceSN[33] = { 0 };
	get_devicesn(deviceSN);
    if (0 == strlen(deviceSN))
    {
    	char rcvBuf[LINE_LEN] = { 0 };
    	get_device_wanmac(rcvBuf);
        snprintf(deviceSN, sizeof(deviceSN), "%s-%s", "741E93-651D8", rcvBuf);
    }
	std::cout << "DeviceReadOnlyData - DeviceSn : " << deviceSN << std::endl;
	return std::string(deviceSN);
#else
	RandomGenerator l_random;
	std::string deviceSN = l_random.generatorRandomNumberString(16);
	std::cout << "DeviceReadOnlyData - DeviceSn : " << deviceSN << std::endl;
	return deviceSN;
#endif
}

std::string DeviceReadOnlyData::initFirmWareVersion() const
{
#ifdef CROSS_BUILD
	char hardwareversion[33] = { 0 };
	getcfgx(GW_VERSION, "hw_version", hardwareversion);
	std::cout << "DeviceReadOnlyData - FirmWareVersion : " << hardwareversion << std::endl;
	return std::string(hardwareversion);
#else
	std::string hardwareversion = "fhhw1.0.0";
	std::cout << "DeviceReadOnlyData - FirmWareVersion : " << hardwareversion << std::endl;
	return hardwareversion;
#endif
}

std::string DeviceReadOnlyData::initSoftWareVersion() const
{
#ifdef CROSS_BUILD
	char softwareversion[33] = { 0 };
	getcfgx(GW_VERSION, "sw_version", softwareversion);
	std::cout << "DeviceReadOnlyData - SoftwareVersion : " << softwareversion << std::endl;
	return std::string(softwareversion);
#else
	std::string softwareversion = "fhsw1.0.0";
	std::cout << "DeviceReadOnlyData - SoftwareVersion : " << softwareversion << std::endl;
	return softwareversion;
#endif
}

std::string DeviceReadOnlyData::initDeviceVendor() const
{
	return "fiberhome";
}

std::string DeviceReadOnlyData::initDeviceModel() const
{
#ifdef CROSS_BUILD
	char deviceModel[33] = { 0 };
	getcfgx(GW_VERSION, "model", deviceModel);
	std::cout << "DeviceReadOnlyData - DeviceModel : " << deviceModel << std::endl;
	return std::string(deviceModel);
#else
	std::string deviceModel = "unknown";
	std::cout << "DeviceReadOnlyData - DeviceModel : " << deviceModel << std::endl;
	return deviceModel;
#endif
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


