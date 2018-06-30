#include "SmartHomeDeployment.hpp"
#include <unistd.h>

SmartHomeDeployment::SmartHomeDeployment():
	m_udpNetworkUnit(std::make_shared<MasterThreadNetworkUnit>()),
	m_terminalUnit(std::make_shared<TerminalThreadUnit>())
{

}

SmartHomeDeployment::~SmartHomeDeployment()
{

}

void SmartHomeDeployment::start()
{
	m_udpNetworkUnit->run();
	m_terminalUnit->run();
	while(true)
	{
		sleep(1);
	}
}

void SmartHomeDeployment::shutdown()
{

}


