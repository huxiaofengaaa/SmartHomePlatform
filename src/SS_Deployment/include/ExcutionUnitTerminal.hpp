#pragma once

#include <memory>
#include <thread>
#include <functional>
#include <map>
#include "EventTypeStruct.hpp"
#include "glog/logging.h"
#include "ExcutionUnit.hpp"
#include "TerminalHandler.hpp"
#include "TerminalCmd.hpp"

class ExcutionUnitTerminal:
		public ExcutionUnit<std::shared_ptr<EventTypeTerminalDataObject>>,
		public AsynTerminalHandler
{
public:
	ExcutionUnitTerminal();
	~ExcutionUnitTerminal();
	bool start();
	void shutdown();
	void registerCmd(std::string p_cmdName, std::shared_ptr<TerminalCmd> p_cmdObj);
	std::string getAllTerminalCmdUsage();
private:
	std::string runTerminalCmd(std::string p_cmd);
	bool terminalAsycDataCallback(std::string p_data);
	bool handleDataObject(std::shared_ptr<EventTypeTerminalDataObject> p_eventObj);

	std::map<std::string, std::shared_ptr<TerminalCmd>> m_cmdList;
};
