/*
 * UeContextAndlink.cpp
 *
 *  Created on: 2018��7��16��
 *      Author: Administrator
 */
#include "UeContextAndlink.hpp"
#include "RandomGenerator.hpp"

UeContextAndlink::UeContextAndlink(std::string p_deviceID): deviceId(p_deviceID)
{

}

std::string UeContextAndlink::getDeviceMAC() const
{
	return deviceMac;
}

std::string UeContextAndlink::getDeviceID() const
{
	return deviceId;
}
