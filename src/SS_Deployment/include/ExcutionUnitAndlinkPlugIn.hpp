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
#include "TCPServerHandler.hpp"

class ExcutionUnitAndlinkPlugIn:
		public ExcutionUnit<std::shared_ptr<EventTypeTCPClientDataObject>>,
		public AsynTCPListenerHandler,
		public AsynTCPServerHandler
{
public:
	ExcutionUnitAndlinkPlugIn();
	~ExcutionUnitAndlinkPlugIn();
	bool start();
	void shutdown();
private:
	bool excutionUnitHandleDataObject(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj);
	bool asycTCPServerReceiveDataHandler(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj);
	bool asycTcpConnectionHandler(std::shared_ptr<ClientConnectInfo> p_connectionInfo);
	bool asycTCPCloseHandler(std::shared_ptr<ClientConnectInfo> p_closeInfo);
};
