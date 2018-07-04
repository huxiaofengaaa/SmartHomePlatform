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
//#include "UeContextHolder.hpp"
//#include "UeContextAndlink.hpp"
#include "UeContextHolderAndlink.hpp"

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


