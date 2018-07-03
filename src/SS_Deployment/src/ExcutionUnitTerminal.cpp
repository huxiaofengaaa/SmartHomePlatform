#include <iostream>
#include <unistd.h>
#include "ExcutionUnitTerminal.hpp"
#include "EventTypeStruct.hpp"
#include "glog/logging.h"

#define PLATFORM_NAME	"SmartHomePlatform # "

ExcutionUnitTerminal::ExcutionUnitTerminal():
	ExcutionUnit(1, std::bind(&ExcutionUnitTerminal::handleDataObject, this, std::placeholders::_1)),
	AsynTerminalHandler(std::bind(&ExcutionUnitTerminal::terminalAsycDataCallback, this, std::placeholders::_1))
{
	LOG(INFO) << "construct ExcutionUnitTerminal";
}

ExcutionUnitTerminal::~ExcutionUnitTerminal()
{
	LOG(INFO) << "de-construct ExcutionUnitTerminal";
}

bool ExcutionUnitTerminal::start()
{
	LOG(INFO) << "ExcutionUnitTerminal start";
	runTerminal();
	startExcutionUnit();
	writeTerminalString(PLATFORM_NAME);
	return true;
}
void ExcutionUnitTerminal::shutdown()
{
	shutdownTerminal();
	shutdownExcutionUnit();
	LOG(INFO) << "ExcutionUnitTerminal shutdown";
}

bool ExcutionUnitTerminal::terminalAsycDataCallback(std::string p_data)
{
	return addDataObject(std::make_shared<EventTypeTerminalDataObject>(p_data));
}

bool ExcutionUnitTerminal::handleDataObject(std::shared_ptr<EventTypeTerminalDataObject> p_eventObj)
{
	LOG(INFO) << p_eventObj;
	writeTerminalString(PLATFORM_NAME);
	return true;
}
