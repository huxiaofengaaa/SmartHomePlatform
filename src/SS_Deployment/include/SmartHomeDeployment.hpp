#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <thread>
#include "ExcutionUnitTerminal.hpp"
#include "ExcutionUnitAndlink.hpp"
#include "ExcutionUnitAndlinkPlugIn.hpp"
#include "EventTypeStruct.hpp"
#include "SystemNotify.hpp"
#include "EventTerminal.hpp"

class SmartHomeDeployment
{
public:
	SmartHomeDeployment();
	~SmartHomeDeployment();
	void start();
	bool shutdown(int p_signum);

private:
	void registerAllTerminalCmd();

	std::string terminalCmdHelp();

	bool m_deploymentShouldExit;

	std::shared_ptr<ExcutionUnitTerminal> m_terminalExcutionUnit;
	std::shared_ptr<ExcutionUnitAndlink> m_andlinkExcutionUnit;
	std::shared_ptr<ExcutionUnitAndlinkPlugIn> m_andlinkPlugInExcutionUnit;
};


