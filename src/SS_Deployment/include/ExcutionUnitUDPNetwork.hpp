#pragma once

#include <memory>
#include <thread>
#include <functional>
#include "ContainerNetwork.hpp"
#include "EventTypeStruct.hpp"

class MasterThreadNetworkUnit
{
public:
	MasterThreadNetworkUnit(std::function<bool(std::shared_ptr<EventTypeDataObject>)> p_callback);
	~MasterThreadNetworkUnit();
	bool run();
private:
	bool masterThreadTask();
	bool networkDataCallback();
	bool isErrorOccurred(std::string p_data);

	std::shared_ptr<INetworkContainer> m_udpContainer;
	std::thread m_masterThread;
	std::function<bool(std::shared_ptr<EventTypeDataObject>)> m_dataCallback;

	bool m_ThreadShouldExit;
};
