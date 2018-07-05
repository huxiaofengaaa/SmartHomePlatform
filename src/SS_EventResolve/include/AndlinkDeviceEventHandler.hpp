/*
 * AndlinkDeviceEventHandler.hpp
 *
 *  Created on: 2018��7��2��
 *      Author: Administrator
 */

#pragma once

#include <memory>
#include <string>
#include "AndlinkDeviceEvent.hpp"
#include "UeContextHolderAndlink.hpp"
#include "UDPServerHandler.hpp"

class AndlinkDeviceEventHandler
{
public:
	AndlinkDeviceEventHandler(std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder)
		: m_ueContextHolder(p_ueContextHolder)
	{

	}
	~AndlinkDeviceEventHandler() = default;
	std::string run(std::shared_ptr<EventTypeUDPClientDataObject> p_event);

	std::string buildPlugIuRequest(std::string p_deviceid);
	std::string buildDisconnectRequest(std::string p_deviceid);
private:
	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;
};
