#pragma once

#include <memory>
#include <thread>
#include <functional>
#include "ContainerNetwork.hpp"
#include "EventTypeStruct.hpp"

class MasterThreadNetworkUnit
{
public:
	MasterThreadNetworkUnit(std::string p_host, int p_port,
			std::function<bool(std::shared_ptr<EventTypeNetworkDataObject>)> p_callback);
	~MasterThreadNetworkUnit();
	bool run();
private:
	bool masterThreadTask();
	bool networkDataCallback();
	bool isErrorOccurred(std::string p_data);

	std::shared_ptr<INetworkContainer> m_udpContainer;
	std::thread m_masterThread;
	std::function<bool(std::shared_ptr<EventTypeNetworkDataObject>)> m_dataCallback;

	bool m_ThreadShouldExit;

	std::string m_host;
	int m_port;
};
