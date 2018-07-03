/*
 * ExcutionUnitAndlink.cpp
 *
 *  Created on: 2018Äê7ÔÂ3ÈÕ
 *      Author: Administrator
 */

#include "ExcutionUnitAndlink.hpp"
#include "AndlinkDeviceEventHandler.hpp"

ExcutionUnitAndlink::ExcutionUnitAndlink():
	ExcutionUnit(5, std::bind(&ExcutionUnitAndlink::handleDataObject, this, std::placeholders::_1)),
	AsynUDPServerHandler("127.0.0.1", 6887,
			std::bind(&ExcutionUnitAndlink::asycUDPServerDataCallback, this, std::placeholders::_1))
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

bool ExcutionUnitAndlink::asycUDPServerDataCallback(std::shared_ptr<EventTypeUDPClientDataObject> p_obj)
{
	LOG(INFO) << p_obj;
	return addDataObject(p_obj);
}

bool ExcutionUnitAndlink::handleDataObject(std::shared_ptr<EventTypeUDPClientDataObject> p_eventObj)
{
	AndlinkDeviceEventHandler l_andlinkHandler;
	std::string l_resp = l_andlinkHandler.run(p_eventObj);

	p_eventObj->m_rawData = l_resp;
	if(writeUDPServerString(p_eventObj))
	{
		LOG(INFO) << p_eventObj;
	}

	return true;
}
