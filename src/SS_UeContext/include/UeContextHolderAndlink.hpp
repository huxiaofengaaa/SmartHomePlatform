/*
 * UeContextHolderAndlink.hpp
 *
 *  Created on: 2018Äê7ÔÂ4ÈÕ
 *      Author: Administrator
 */

#pragma once

#include <random>
#include <iostream>
#include "UeContextHolder.hpp"
#include "UeContextAndlink.hpp"
#include "glog/logging.h"

class UeContextHolderAndlink: public UeContextHolder<std::shared_ptr<UeContextAndlink>>
{
public:
	UeContextHolderAndlink(){ }
	~UeContextHolderAndlink(){ }

	std::vector<std::string> getDeviceList();
	std::string DeviceRegister(std::string deviceMac, std::string deviceType, std::string productToken);

private:
	std::string generatorGwToken();
	std::string generatorDeviceID();
	std::string generatorDeviceToken();
	std::string generatorAndlinkToken();


	std::string generatorRandomCharString(int length);
	std::string generatorRandomNumberString(int length);
};
