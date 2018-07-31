#include "SmartHomeDeployment.hpp"

SmartHomeDeployment::SmartHomeDeployment(std::shared_ptr<ConfigParser> p_config):
	m_config(p_config)
{
	m_deploymentShouldExit = false;

	SignalRegister l_regster;
	std::function<bool(int)> l_sigintTask = std::bind(&SmartHomeDeployment::shutdown, this, std::placeholders::_1);
	l_regster.registerSignal(SIGINT, l_sigintTask);
	LOG(INFO) << "register user handler for linux signal " << SIGINT;

	std::string l_managerServerHost = m_config->getParamString("ManagerServerHost", "10.96.17.50");
	int l_managerServerPort = m_config->getParamInteger("ManagerServerPort", 6887);
	LOG(INFO) << "Manager server is " << l_managerServerHost << ":" << l_managerServerPort;

	std::string l_controlServerHost = m_config->getParamString("ControlServerHost", l_managerServerHost);
	int l_controlServerPort = m_config->getParamInteger("ControlServerPort", l_managerServerPort + 1);
	LOG(INFO) << "Control server is " << l_controlServerHost << ":" << l_controlServerPort;

	m_ueContextAndlinkHolder = std::make_shared<UeContextHolderAndlink>(
			l_managerServerHost,
			l_managerServerPort,
			m_config);

	m_andlinkExcutionUnit = std::make_shared<ExcutionUnitAndlink>(
			l_managerServerHost,
			l_managerServerPort,
			m_ueContextAndlinkHolder);

	m_andlinkPlugInExcutionUnit = std::make_shared<ExcutionUnitAndlinkPlugIn>(
			l_controlServerHost,
			l_controlServerPort,
			m_ueContextAndlinkHolder);

	m_terminalExcutionUnit = std::make_shared<ExcutionUnitTerminal>(
			m_ueContextAndlinkHolder,
			m_andlinkExcutionUnit,
			m_andlinkPlugInExcutionUnit);

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
	m_andlinkExcutionUnit->start();
	m_andlinkPlugInExcutionUnit->start();

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
	m_andlinkPlugInExcutionUnit->shutdown();
	m_deploymentShouldExit = true;
	return true;
}


