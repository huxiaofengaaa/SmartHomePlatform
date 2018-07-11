/*
 * AndlinkDeviceEventBuilder.hpp
 *
 *  Created on: 2018Äê7ÔÂ11ÈÕ
 *      Author: Administrator
 */
#pragma once
#include <string>
#include "AndlinkDeviceEvent.hpp"
#include "UeContextHolderAndlink.hpp"

class AndlinkDeviceEventBuilder
{
public:
	AndlinkDeviceEventBuilder(std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder)
		: m_ueContextHolder(p_ueContextHolder)
	{

	}
	std::string buildPlugIuRequest(std::string p_deviceid, std::string p_host, int p_port);
	std::string buildDisconnectRequest(std::string p_deviceid);
private:
	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;
};
