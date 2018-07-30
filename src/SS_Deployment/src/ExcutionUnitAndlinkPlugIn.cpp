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
	auto l_deviceID = m_ueContextHolder->getDeviceIDByTCPAddress(
			p_eventObj->m_host, p_eventObj->m_port);

	auto l_andlinkHandler = std::make_shared<AndlinkDeviceEventHandler>(m_ueContextHolder);
	std::string l_resp = l_andlinkHandler->run(p_eventObj);

	if(l_resp.empty() == true)
	{
		return true;
	}
	p_eventObj->m_rawData = l_resp;
	return sendPlugInRequest(l_deviceID, p_eventObj, true);
}

bool ExcutionUnitAndlinkPlugIn::asycTCPServerReceiveDataHandler(std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj)
{
	if(p_eventObj->m_rawData.size() <= 0)
	{
		LOG(INFO) << "Error message recv, " << p_eventObj;
		return false;
	}
	std::string l_deviceID =
			m_ueContextHolder->getDeviceIDByTCPAddress(p_eventObj->m_host, p_eventObj->m_port);
	if(false == is_jSON_data(p_eventObj->m_rawData)
			&& true == m_ueContextHolder->shouldDeviceEncrypt(l_deviceID))
	{
		auto l_andlinkHandler = std::make_shared<AndlinkDeviceEventHandler>(m_ueContextHolder);
		p_eventObj->m_rawData = l_andlinkHandler->chiperDecrypt(
				p_eventObj->m_host, p_eventObj->m_port, p_eventObj->m_rawData);
	}

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
	return m_ueContextHolder->pluginDisconnect(
			p_closeInfo->m_sockfd,
			p_closeInfo->m_ip,
			p_closeInfo->m_port);
}

bool ExcutionUnitAndlinkPlugIn::controlReqPreconditionCheck(std::string p_deviceid)
{
	auto l_uecontext = m_ueContextHolder->getRef(p_deviceid);
	if(!l_uecontext)
	{
		LOG(INFO) << "control device " << p_deviceid << " failed, not exist.";
		return false;
	}
	if(l_uecontext->isPlugIn == false)
	{
		LOG(INFO) << "control device " << p_deviceid << " failed, not plugin.";
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
		std::string p_deviceid, std::shared_ptr<EventTypeTCPClientDataObject> p_eventObj,
		bool p_shouldEncrypt)
{
	LOG(INFO) << p_eventObj;
	if(p_shouldEncrypt == true && true == m_ueContextHolder->shouldDeviceEncrypt(p_deviceid))
	{
		/*
		 * Two parameters control whether the message needs to be encrypted; p_shouldEncrypt
		 * indicates whether the type of message needs to be encrypted, and the
		 * shouldDeviceEncrypt function detects whether it belongs to the encryption device;
		 */
		auto l_andlinkHandler = std::make_shared<AndlinkDeviceEventHandler>(m_ueContextHolder);
		p_eventObj->m_rawData = l_andlinkHandler->plainEncrypt(p_deviceid, p_eventObj->m_rawData);
	}

	if(writeTCPServerString(p_eventObj))
	{
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
	return sendPlugInRequest(p_deviceid, l_eventObj);
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
	return sendPlugInRequest(p_deviceid, l_eventObj);
}


