/*
 * ExcutionUnitAndlinkPlugIn.hpp
 *
 *  Created on: 2018��7��4��
 *      Author: Administrator
 */

#pragma once

#include "ExcutionUnit.hpp"
#include "glog/logging.h"
#include "UDPServerHandler.hpp"
#include "TCPListenerHandler.hpp"

class ExcutionUnitAndlinkPlugIn:
		public ExcutionUnit<std::shared_ptr<EventTypeUDPClientDataObject>>,
		public AsynTCPListenerHandler
{
public:
	ExcutionUnitAndlinkPlugIn();
	~ExcutionUnitAndlinkPlugIn();
	bool start();
	void shutdown();
private:
	bool excutionUnitHandleDataObject(std::shared_ptr<EventTypeUDPClientDataObject> p_eventObj);
	bool asycTcpConnectionHandler(std::shared_ptr<ClientConnectInfo> p_connectionInfo);
};
