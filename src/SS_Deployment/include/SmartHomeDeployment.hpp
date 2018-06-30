#pragma once

#include <memory>
#include <vector>
#include "ExcutionUnitUDPNetwork.hpp"
#include "ExcutionUnitTerminal.hpp"

class SmartHomeDeployment
{
public:
	SmartHomeDeployment();
	~SmartHomeDeployment();
	void start();
	void shutdown();

	static void* NetworkMasterThreadTask(void* arg);

private:
	std::shared_ptr<MasterThreadNetworkUnit> m_udpNetworkUnit;
	std::shared_ptr<TerminalThreadUnit> m_terminalUnit;

};


