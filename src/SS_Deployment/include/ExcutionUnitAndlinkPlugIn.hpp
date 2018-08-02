/*
 * ExcutionUnitAndlinkPlugIn.hpp
 *
 *  Created on: 2018��7��4��
 *      Author: Administrator
 */

#pragma once

#include "ExcutionUnit.hpp"
#include "ExcutionUnitWithSignalQueue.hpp"
#include "ExcutionUnitWithMultipleQueue.hpp"
#include "glog/logging.h"
#include "UDPServerHandler.hpp"
#include "TCPListenerHandler.hpp"
#include "TCPServerHandler.hpp"
#include "UeContextHolderAndlink.hpp"
#include "AndlinkDeviceEventHandler.hpp"
#include "AndlinkDeviceEventBuilder.hpp"
#include "PacketStatistician.hpp"

class ExcutionUnitAndlinkPlugIn:
		public ExcutionUnitWithMultipleQueue<std::string, std::shared_ptr<EventTypeTCPClientDataObject>>,
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
	bool triggerPowerControl(std::string p_deviceid, int p_select, int p_percent);
	bool triggerMACFilterControl(std::string p_deviceid, bool p_enable, int p_policy, std::string p_entry);
	bool triggerChannelReselectControl(std::string p_deviceid, int p_select);
	bool triggerRoamingConfigControl(std::string p_deviceid, bool p_switch, int p_val1, int p_val2);
	bool triggerWiFiSwitchControl(std::string p_deviceid, bool p_turnOn, int p_select);
	bool triggerWPSControl(std::string p_deviceid, int p_select);
	bool triggerWiFiParameterSyncControl(std::string p_deviceid);

private:
	bool excutionUnitHandleDataObject(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj);
	bool asycTCPServerReceiveDataHandler(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj);
	bool asycTcpConnectionHandler(std::shared_ptr<ClientConnectInfo> p_connectionInfo);
	bool asycTCPCloseHandler(std::shared_ptr<ClientConnectInfo> p_closeInfo);

	bool controlReqPreconditionCheck(std::string p_deviceid);
	std::shared_ptr<EventTypeTCPClientDataObject> buildTCPClientDataObject(
			std::string p_deviceID, std::string p_rawData);
	bool sendPlugInRequest(std::string p_deviceid,
			std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj, bool p_shouldEncrypt = true);

	std::string m_host;
	int m_port;
	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;
};
