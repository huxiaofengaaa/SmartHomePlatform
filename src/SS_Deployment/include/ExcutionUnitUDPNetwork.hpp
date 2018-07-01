#pragma once

#include <memory>
#include <thread>
#include <functional>
#include "ContainerNetwork.hpp"

class MasterThreadNetworkUnit
{
public:
	MasterThreadNetworkUnit();
	~MasterThreadNetworkUnit();
	bool run();
private:
	bool masterThreadTask();
	bool networkDataCallback();

	std::shared_ptr<INetworkContainer> m_udpContainer;
	std::thread m_masterThread;

	bool m_ThreadShouldExit;
};
