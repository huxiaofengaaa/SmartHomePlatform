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
#include "UeContextHolderAndlink.hpp"
#include "AndlinkDeviceEventHandler.hpp"
#include "AndlinkDeviceEventBuilder.hpp"
#include "PacketStatistician.hpp"

class ExcutionUnitAndlinkPlugIn:
		public ExcutionUnit<std::shared_ptr<EventTypeTCPClientDataObject>>,
		public AsynTCPListenerHandler,
		public AsynTCPServerHandler,
		public PacketStatistician
{
public:
	ExcutionUnitAndlinkPlugIn(std::string p_host, int p_port,
			std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder);
	~ExcutionUnitAndlinkPlugIn();
	bool start();
	void shutdown();
	bool triggerDisconnect(std::string p_deviceid);
	bool triggerQuery(std::string p_deviceid, std::string p_param);
	bool triggerReboot(std::string p_deviceid);
	bool triggerLEDControl(std::string p_deviceid, bool p_turnOn);

private:
	bool excutionUnitHandleDataObject(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj);
	bool asycTCPServerReceiveDataHandler(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj);
	bool asycTcpConnectionHandler(std::shared_ptr<ClientConnectInfo> p_connectionInfo);
	bool asycTCPCloseHandler(std::shared_ptr<ClientConnectInfo> p_closeInfo);

	bool controlReqPreconditionCheck(std::string p_deviceid);
	std::shared_ptr<EventTypeTCPClientDataObject> buildTCPClientDataObject(
			std::string p_deviceID, std::string p_rawData);
	bool sendPlugInRequest(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj);

	std::string m_host;
	int m_port;
	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;
};
