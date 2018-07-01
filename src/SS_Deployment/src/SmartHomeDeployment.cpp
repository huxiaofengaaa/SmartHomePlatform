#include "SmartHomeDeployment.hpp"
#include "glog/logging.h"
#include <unistd.h>

SmartHomeDeployment::SmartHomeDeployment():
	m_udpNetworkUnit(std::make_shared<MasterThreadNetworkUnit>()),
	m_terminalUnit(std::make_shared<TerminalThreadUnit>())
{
	LOG(INFO) << "construct SmartHomeDeployment";
}

SmartHomeDeployment::~SmartHomeDeployment()
{
	LOG(INFO) << "de-constructor SmartHomeDeployment";
}

void SmartHomeDeployment::start()
{
	LOG(INFO) << "SmartHomeDeployment running...";
	m_udpNetworkUnit->run();
	m_terminalUnit->run();
	LOG(INFO) << "SmartHomeDeployment main loop";
	while(true)
	{
		sleep(1);
	}
	LOG(WARNING) << "SmartHomeDeployment is going to exit";
}

void SmartHomeDeployment::shutdown()
{
	LOG(INFO) << "SmartHomeDeployment is going to shutdown";
}


