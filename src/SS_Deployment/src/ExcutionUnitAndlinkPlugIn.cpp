/*
 * ExcutionUnitAndlinkPlugIn.cpp
 *
 *  Created on: 2018��7��4��
 *      Author: Administrator
 */

#include "ExcutionUnitAndlinkPlugIn.hpp"

ExcutionUnitAndlinkPlugIn::ExcutionUnitAndlinkPlugIn(std::string p_host, int p_port):
	m_host(p_host),
	m_port(p_port),
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
	LOG(INFO) << p_eventObj;
	//TODO
	writeTCPServerString(p_eventObj);
	return true;
}

bool ExcutionUnitAndlinkPlugIn::asycTCPServerReceiveDataHandler(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj)
{
	LOG(INFO) << p_eventObj;
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
	return true;
}


