#include "SmartHomeDeployment.hpp"
#include "SignalRegister.hpp"
#include "glog/logging.h"
#include <unistd.h>
#include <functional>

SmartHomeDeployment::SmartHomeDeployment()
{
	m_deploymentShouldExit = false;

	SignalRegister l_regster;
	std::function<bool(int)> l_sigintTask = std::bind(&SmartHomeDeployment::shutdown, this, std::placeholders::_1);
	l_regster.registerSignal(SIGINT, l_sigintTask);
	LOG(INFO) << "register user handler for linux signal " << SIGINT;

	m_terminalExcutionUnit = std::make_shared<ExcutionUnitTerminal>();
	m_andlinkExcutionUnit = std::make_shared<ExcutionUnitAndlink>();

	LOG(INFO) << "construct SmartHomeDeployment";
}

SmartHomeDeployment::~SmartHomeDeployment()
{
	LOG(INFO) << "Good Bye SmartHomePlatform...";
}

void SmartHomeDeployment::start()
{
	LOG(INFO) << "SmartHomeDeployment start";

	m_terminalExcutionUnit->start();
	registerAllTerminalCmd();
	m_andlinkExcutionUnit->start();

	LOG(INFO) << "SmartHomeDeployment main loop start";
	while(m_deploymentShouldExit == false)
	{
		sleep(1);
	}
	LOG(INFO) << "SmartHomeDeployment main loop exit";
}

bool SmartHomeDeployment::shutdown(int p_signum)
{
	LOG(INFO) << "call SmartHomeDeployment shutdown, signum = " <<  p_signum;
	SignalRegister l_regster;
	l_regster.ignoreSignal(SIGINT);
	LOG(INFO) << "set signal SIGINT handler to ignore";

	m_terminalExcutionUnit->shutdown();
	m_andlinkExcutionUnit->shutdown();
	m_deploymentShouldExit = true;
	return true;
}

void SmartHomeDeployment::registerAllTerminalCmd()
{
	m_terminalExcutionUnit->registerCmd("help", std::make_shared<TerminalCmdHelp>(
			std::bind(&SmartHomeDeployment::terminalCmdHelp, this)));
}


