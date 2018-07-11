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
#include "TCPServerHandler.hpp"

class AndlinkDeviceEventHandler
{
public:
	AndlinkDeviceEventHandler(std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder)
		: m_ueContextHolder(p_ueContextHolder)
	{

	}
	~AndlinkDeviceEventHandler() = default;
	std::string run(std::shared_ptr<EventTypeUDPClientDataObject> p_event);
	std::string run(std::shared_ptr<EventTypeTCPClientDataObject> p_event);
private:
	std::string run(std::string& p_rawData,
			std::string& p_host, int p_port, int p_sockfd, bool isTCP = false);
	bool updateNetAddress(std::string p_deviceID, std::string p_host, int p_port, int p_sockfd, bool isTCP);
	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;
};

