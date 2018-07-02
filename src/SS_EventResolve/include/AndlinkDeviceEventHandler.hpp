/*
 * AndlinkDeviceEventHandler.hpp
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Administrator
 */

#pragma once

#include <memory>
#include "EventTypeStruct.hpp"

class AndlinkDeviceEventHandler
{
public:
	std::string run(std::shared_ptr<EventTypeNetworkDataObject> p_event);
private:

};
