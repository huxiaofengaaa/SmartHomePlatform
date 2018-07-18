/*
 * ExcutionUnitAndlinkPlugIn.cpp
 *
 *  Created on: 2018��7��4��
 *      Author: Administrator
 */

#include "ExcutionUnitAndlinkPlugIn.hpp"

ExcutionUnitAndlinkPlugIn::ExcutionUnitAndlinkPlugIn(std::string p_host, int p_port,
		std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder):
	m_host(p_host),
	m_port(p_port),
	m_ueContextHolder(p_ueContextHolder),
	ExcutionUnit(
			"AndlinkPlugIn",
			5,
			std::bind(&ExcutionUnitAndlinkPlugIn::excutionUnitHandleDataObject,
					this,
					std::placeholders::_1)),
	AsynTCPListenerHandler(
			p_host,
			p_port,
			std::bind(&ExcutionUnitAndlinkPlugIn::asycTcpConnectionHandler,
					this,
					std::placeholders::_1)),
	AsynTCPServerHandler(
			std::bind(&ExcutionUnitAndlinkPlugIn::asycTCPServerReceiveDataHandler,
					this,
					std::placeholders::_1),
			std::bind(&ExcutionUnitAndlinkPlugIn::asycTCPCloseHandler,
					this,
					std::placeholders::_1))
{
	LOG(INFO) << "construct ExcutionUnitAndlinkPlugIn";
}

ExcutionUnitAndlinkPlugIn::~ExcutionUnitAndlinkPlugIn()
{
	LOG(INFO) << "de-construct ExcutionUnitAndlinkPlugIn";
}

bool ExcutionUnitAndlinkPlugIn::start()
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn start";
	startExcutionUnit();
	runTCPServer();
	runTCPListener();
	return true;
}

void ExcutionUnitAndlinkPlugIn::shutdown()
{
	shutdownTCPListener();
	shutdownTCPServer();
	shutdownExcutionUnit();
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn shutdown";
}

bool ExcutionUnitAndlinkPlugIn::excutionUnitHandleDataObject(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj)
{
	auto l_andlinkHandler = std::make_shared<AndlinkDeviceEventHandler>(m_ueContextHolder);
	std::string l_resp = l_andlinkHandler->run(p_eventObj);

	if(l_resp.empty() == false)
	{
		p_eventObj->m_rawData = l_resp;
		if(writeTCPServerString(p_eventObj))
		{
			LOG(INFO) << p_eventObj;
			countSendPacket(p_eventObj->m_rawData.size());
			return true;
		}
		else
		{
			LOG(INFO) << "writeTCPServerString failed";
			return false;
		}
	}
	return true;
}

bool ExcutionUnitAndlinkPlugIn::asycTCPServerReceiveDataHandler(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj)
{
	LOG(INFO) << p_eventObj;
	countRecvPacket(p_eventObj->m_rawData.size());
	return addDataObject(p_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::asycTcpConnectionHandler(std::shared_ptr<ClientConnectInfo> p_connectionInfo)
{
	LOG(INFO) << p_connectionInfo;
	addClient(p_connectionInfo);
	return true;
}

bool ExcutionUnitAndlinkPlugIn::asycTCPCloseHandler(std::shared_ptr<ClientConnectInfo> p_closeInfo)
{
	LOG(INFO) << "Close, " << p_closeInfo;
//	delClient(p_closeInfo->m_sockfd);
	return true;
}

bool ExcutionUnitAndlinkPlugIn::controlReqPreconditionCheck(std::string p_deviceid)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	if(!l_uecontext)
	{
		return false;
	}
	return true;
}

std::shared_ptr<EventTypeTCPClientDataObject> ExcutionUnitAndlinkPlugIn::buildTCPClientDataObject(
		std::string p_deviceID, std::string p_rawData)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceID);
	return std::make_shared<EventTypeTCPClientDataObject>(l_uecontext->TCPSocketfd,
			l_uecontext->peerTCPHost, l_uecontext->peerTCPPort, p_rawData);
}

bool ExcutionUnitAndlinkPlugIn::sendPlugInRequest(
		std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj)
{
	if(writeTCPServerString(p_eventObj))
	{
		LOG(INFO) << p_eventObj;
		countRecvPacket(p_eventObj->m_rawData.size());
		return true;
	}
	else
	{
		LOG(INFO) << "writeTCPServerString failed";
		return false;
	}
}

bool ExcutionUnitAndlinkPlugIn::triggerDisconnect(std::string p_deviceid)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerDisconnect " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_eventObj = buildTCPClientDataObject(
			p_deviceid, l_andlinkBuilder->buildDisconnectRequest(p_deviceid));
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerQuery(std::string p_deviceid, std::string p_param)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerQuery " << p_deviceid << " " << p_param;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	std::pair<std::string, std::string> l_queryReq;
	if(p_param == "apinfo")
	{
		l_queryReq = l_andlinkBuilder->buildApConfigInfoQueryRequest(p_deviceid);
	}
	else if(p_param == "stainfo")
	{
		l_queryReq = l_andlinkBuilder->buildSTAInfoQueryRequest(p_deviceid);
	}
	else if(p_param == "upsts")
	{
		l_queryReq = l_andlinkBuilder->buildUplinkStatusQueryRequest(p_deviceid);
	}
	else if(p_param == "wifists")
	{
		l_queryReq = l_andlinkBuilder->buildWiFiStatsQueryRequest(p_deviceid);
	}
	else
	{
		return false;
	}
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_queryReq.second);
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerReboot(std::string p_deviceid)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerReboot " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildRebootRequest(p_deviceid);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerLEDControl(std::string p_deviceid, bool p_turnOn)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerLEDControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildLEDControlRequest(p_deviceid, p_turnOn);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerPowerControl(std::string p_deviceid, int p_select, int p_percent)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerPowerControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildRadioPowerRequest(p_deviceid, p_select, p_percent);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerMACFilterControl(std::string p_deviceid, bool p_enable,
		int p_policy, std::string p_entry)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerMACFilterControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildMacFilterRequest(p_deviceid, p_enable, p_policy, p_entry);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerChannelReselectControl(std::string p_deviceid, int p_select)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerChannelReselectControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildChannelReselectRequest(p_deviceid, p_select);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerRoamingConfigControl(std::string p_deviceid,
		bool p_switch, int p_val1, int p_val2)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerRoamingConfigControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildRoamingConfigRequest(p_deviceid, p_switch, p_val1, p_val2);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerWiFiSwitchControl(std::string p_deviceid, bool p_turnOn, int p_select)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerWiFiSwitchControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildWiFiSwitchRequest(p_deviceid, p_turnOn, p_select);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerWPSControl(std::string p_deviceid, int p_select)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerWPSControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildWPSRequest(p_deviceid, p_select);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerWiFiParameterSyncControl(std::string p_deviceid)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerWiFiParameterSyncControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildWiFiParameterSyncRequest(p_deviceid);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(l_eventObj);
}
