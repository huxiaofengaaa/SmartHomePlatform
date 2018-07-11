/*
 * ExcutionUnitAndlink.hpp
 *
 *  Created on: 2018��7��3��
 *      Author: Administrator
 */

#pragma once

#include <memory>
#include <functional>
#include "glog/logging.h"
#include "ExcutionUnit.hpp"
#include "UDPServerHandler.hpp"
#include "UeContextHolderAndlink.hpp"
#include "AndlinkDeviceEventHandler.hpp"
#include "AndlinkDeviceEventBuilder.hpp"
#include "PacketStatistician.hpp"

class ExcutionUnitAndlink:
		public ExcutionUnit<std::shared_ptr<EventTypeUDPClientDataObject>>,
		public AsynUDPServerHandler,
		public PacketStatistician
{
public:
	ExcutionUnitAndlink(std::string p_host, int p_port,
			std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder);
	~ExcutionUnitAndlink();
	bool start();
	void shutdown();

	bool triggerPlugIn(std::string p_host, int p_port, std::string p_deviceid);

private:
	bool asycUDPServerDataCallback(std::shared_ptr<EventTypeUDPClientDataObject> p_obj);
	bool handleDataObject(std::shared_ptr<EventTypeUDPClientDataObject> p_eventObj);

	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;

	std::string m_host;
	int m_port;
};
