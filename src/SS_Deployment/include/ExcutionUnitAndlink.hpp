/*
 * ExcutionUnitAndlink.hpp
 *
 *  Created on: 2018Äê7ÔÂ3ÈÕ
 *      Author: Administrator
 */

#pragma once

#include <memory>
#include <thread>
#include <functional>
#include "EventTypeStruct.hpp"
#include "glog/logging.h"
#include "ExcutionUnit.hpp"
#include "UDPServerHandler.hpp"

class ExcutionUnitAndlink: public ExcutionUnit, public AsynUDPServerHandler
{
public:
	ExcutionUnitAndlink();
	~ExcutionUnitAndlink();
	bool start();
	void shutdown();

private:
	bool asycUDPServerDataCallback(std::shared_ptr<EventTypeUDPClientDataObject> p_obj);
	bool handleDataObject(std::shared_ptr<EventTypeDataObjectBase> p_eventObj);
};
