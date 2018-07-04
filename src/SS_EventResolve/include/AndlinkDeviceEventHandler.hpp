/*
 * AndlinkDeviceEventHandler.hpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */

#pragma once

#include <memory>
#include "EventTypeStruct.hpp"
#include "UeContextHolderAndlink.hpp"

class AndlinkDeviceEventHandler
{
public:
	AndlinkDeviceEventHandler(std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder)
		: m_ueContextHolder(p_ueContextHolder)
	{

	}
	~AndlinkDeviceEventHandler() = default;
	std::string run(std::shared_ptr<EventTypeUDPClientDataObject> p_event);
private:
	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;
};
