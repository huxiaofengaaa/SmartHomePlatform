/*
 * UeContextAndlink.hpp
 *
 *  Created on: 2018Äê7ÔÂ4ÈÕ
 *      Author: Administrator
 */

#pragma once

#include "UeContext.hpp"

struct UeContextAndlink
{
	UeContextAndlink() = default;
	~UeContextAndlink() = default;

	std::string host;
	int port;

	std::string deviceMac;
	std::string deviceType;
	std::string productToken;
	std::string gwToken;
	std::string deviceId;
	std::string deviceToken;
	std::string andlinkToken;
};
