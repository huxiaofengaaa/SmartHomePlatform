/*
 * ExcutionUnitAndlinkPlugIn.cpp
 *
 *  Created on: 2018Äê7ÔÂ4ÈÕ
 *      Author: Administrator
 */

#include "ExcutionUnitAndlinkPlugIn.hpp"


ExcutionUnitAndlinkPlugIn::ExcutionUnitAndlinkPlugIn()
	:ExcutionUnit("AndlinkPlugIn", 5,
			std::bind(&ExcutionUnitAndlinkPlugIn::excutionUnitHandleDataObject, this, std::placeholders::_1)),
	AsynTCPListenerHandler("127.0.0.1", 6888,
			std::bind(&ExcutionUnitAndlinkPlugIn::asycTcpConnectionHandler, this, std::placeholders::_1))
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
	runTCPListener();
	startExcutionUnit();
	return true;
}

void ExcutionUnitAndlinkPlugIn::shutdown()
{
	shutdownTCPListener();
	shutdownExcutionUnit();
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn shutdown";
}

bool ExcutionUnitAndlinkPlugIn::excutionUnitHandleDataObject(std::shared_ptr<EventTypeUDPClientDataObject> p_eventObj)
{

	return true;
}

bool ExcutionUnitAndlinkPlugIn::asycTcpConnectionHandler(std::shared_ptr<ClientConnectInfo> p_connectionInfo)
{
	LOG(INFO) << p_connectionInfo;
	close(p_connectionInfo->m_sockfd);
}
