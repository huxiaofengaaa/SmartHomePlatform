#pragma once

#include <memory>
#include <thread>
#include <functional>
#include "EventTypeStruct.hpp"
#include "glog/logging.h"
#include "ExcutionUnit.hpp"
#include "TerminalHandler.hpp"

class ExcutionUnitTerminal: public ExcutionUnit, public AsynTerminalHandler
{
public:
	ExcutionUnitTerminal();
	~ExcutionUnitTerminal();
	bool start();
	void shutdown();

private:
	bool terminalAsycDataCallback(std::string p_data);
	bool handleDataObject(std::shared_ptr<EventTypeDataObjectBase> p_eventObj);
};
