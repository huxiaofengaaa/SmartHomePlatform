/*
 * ExcutionUnitAndlink.cpp
 *
 *  Created on: 2018��7��3��
 *      Author: Administrator
 */

#include "ExcutionUnitAndlink.hpp"

ExcutionUnitAndlink::ExcutionUnitAndlink(std::string p_host, int p_port,
		std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder):
	m_host(p_host), m_port(p_port),
	ExcutionUnitWithSignalQueue(
			"Andlink",
			5,
			std::bind(&ExcutionUnitAndlink::handleDataObject,
					this,
					std::placeholders::_1)),
	AsynUDPServerHandler(
			p_host,
			p_port,
			std::bind(&ExcutionUnitAndlink::asycUDPServerDataCallback,
					this,
					std::placeholders::_1)),
	m_ueContextHolder(p_ueContextHolder)
{
	LOG(INFO) << "construct ExcutionUnitAndlink";
}

ExcutionUnitAndlink::~ExcutionUnitAndlink()
{
	LOG(INFO) << "de-construct ExcutionUnitAndlink";
}

bool ExcutionUnitAndlink::start()
{
	LOG(INFO) << "ExcutionUnitAndlink start";
	runUDPServer();
	startExcutionUnit();
	return true;
}

void ExcutionUnitAndlink::shutdown()
{
	shutdownUDPServer();
	shutdownExcutionUnit();
	LOG(INFO) << "ExcutionUnitAndlink shutdown";
}

bool ExcutionUnitAndlink::triggerPlugIn(std::string p_host, int p_port, std::string p_deviceid)
{
	LOG(INFO) << "ExcutionUnitAndlink::triggerPlugIn " << p_deviceid;
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);

	auto l_eventObj = std::make_shared<EventTypeUDPClientDataObject>(
			p_host, p_port, -1, l_andlinkBuilder->buildPlugIuRequest(p_deviceid, m_host, m_port+1));

	if(writeUDPServerString(l_eventObj))
	{
		countSendPacket(l_eventObj->m_rawData.size());
		LOG(INFO) << l_eventObj;
	}
	else
	{
		LOG(INFO) << "writeUDPServerString failed";
	}
	return true;
}

bool ExcutionUnitAndlink::asycUDPServerDataCallback(std::shared_ptr<EventTypeUDPClientDataObject> p_obj)
{
	LOG(INFO) << p_obj;
	countRecvPacket(p_obj->m_rawData.size());
	return addDataObject(p_obj);
}

bool ExcutionUnitAndlink::handleDataObject(std::shared_ptr<EventTypeUDPClientDataObject> p_eventObj)
{
	if(!p_eventObj)
	{
		return false;
	}

	auto l_andlinkHandler = std::make_shared<AndlinkDeviceEventHandler>(m_ueContextHolder);
	std::string l_resp = l_andlinkHandler->run(p_eventObj);

	if(l_resp.empty() == false)
	{
		p_eventObj->m_rawData = l_resp;
		if(writeUDPServerString(p_eventObj))
		{
			countSendPacket(p_eventObj->m_rawData.size());
			LOG(INFO) << p_eventObj;
			return true;
		}
		else
		{
			LOG(INFO) << "writeUDPServerString failed";
			return false;
		}
	}
	return true;
}
