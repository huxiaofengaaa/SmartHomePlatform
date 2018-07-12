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
	delClient(p_closeInfo->m_sockfd);
	return true;
}

bool ExcutionUnitAndlinkPlugIn::triggerDisconnect(std::string p_deviceid)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerDisconnect " << p_deviceid;
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	if(!l_uecontext)
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_eventObj = std::make_shared<EventTypeTCPClientDataObject>(
			l_uecontext->TCPSocketfd,
			l_uecontext->peerTCPHost,
			l_uecontext->peerTCPPort,
			l_andlinkBuilder->buildDisconnectRequest(p_deviceid));

	if(writeTCPServerString(l_eventObj))
	{
		LOG(INFO) << l_eventObj;
		countRecvPacket(l_eventObj->m_rawData.size());
		return true;
	}
	else
	{
		LOG(INFO) << "writeTCPServerString failed";
		return false;
	}
}
