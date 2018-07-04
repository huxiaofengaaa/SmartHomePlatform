#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <memory>
#include <functional>
#include <map>
#include "glog/logging.h"
#include "ExcutionUnit.hpp"
#include "ExcutionUnitAndlink.hpp"
#include "TerminalHandler.hpp"
#include "TerminalCmd.hpp"
#include "UeContextHolderAndlink.hpp"

class ExcutionUnitTerminal:
		public ExcutionUnit<std::string>,
		public AsynTerminalHandler
{
public:
	ExcutionUnitTerminal(std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder,
			std::shared_ptr<ExcutionUnitAndlink> p_euAndlink);
	~ExcutionUnitTerminal();
	bool start();
	void shutdown();

private:
	std::string runTerminalCmd(std::string p_cmd);
	void registerCmd(std::string p_cmdName, std::shared_ptr<TerminalCmd> p_cmdObj);
	void registerAllCmd();

	std::vector<std::string> resolveParameter(std::string p_cmd);

	std::string terminalCmdCallback_help();
	std::string terminalCmdCallback_list(std::string p_cmd);
	std::string terminalCmdCallback_plugin(std::string p_cmd);

	bool terminalAsycDataCallback(std::string p_data);
	bool handleDataObject(std::string p_eventObj);

	std::map<std::string, std::shared_ptr<TerminalCmd>> m_cmdList;
	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;
	std::shared_ptr<ExcutionUnitAndlink> m_euAndlink;
};
