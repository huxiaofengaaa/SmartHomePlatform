#pragma once

#include <memory>
#include <unistd.h>
#include <functional>
#include "ExcutionUnitTerminal.hpp"
#include "ExcutionUnitAndlink.hpp"
#include "ExcutionUnitAndlinkPlugIn.hpp"
#include "UeContextHolderAndlink.hpp"
#include "SignalRegister.hpp"
#include "glog/logging.h"

#define MANAGER_SERVER_IP	"10.96.17.50"
#define MANAGER_SERVER_PORT	6887

class SmartHomeDeployment
{
public:
	SmartHomeDeployment();
	~SmartHomeDeployment();
	void start();
	bool shutdown(int p_signum);

private:
	bool m_deploymentShouldExit;

	std::shared_ptr<UeContextHolderAndlink> m_ueContextAndlinkHolder;
	std::shared_ptr<ExcutionUnitTerminal> m_terminalExcutionUnit;
	std::shared_ptr<ExcutionUnitAndlink> m_andlinkExcutionUnit;
	std::shared_ptr<ExcutionUnitAndlinkPlugIn> m_andlinkPlugInExcutionUnit;
};


