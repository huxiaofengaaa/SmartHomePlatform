#include <iostream>
#include <unistd.h>
#include "ExcutionUnitUDPNetwork.hpp"
#include "ContainerNetwork.hpp"
#include "glog/logging.h"

MasterThreadNetworkUnit::MasterThreadNetworkUnit(std::function<bool(std::shared_ptr<EventTypeDataObject>)> p_callback)
	: m_ThreadShouldExit(false), m_dataCallback(p_callback)
{
	LOG(INFO) << "Construct MasterThreadNetworkUnit";
}

MasterThreadNetworkUnit::~MasterThreadNetworkUnit()
{
	m_ThreadShouldExit = true;
	m_masterThread.join();
	LOG(INFO) << "de-construct MasterThreadNetworkUnit";
}

bool MasterThreadNetworkUnit::run()
{
	m_udpContainer = std::make_shared<UDPNetworkContainer>();

	std::function<bool()> l_threadtask = std::bind(&MasterThreadNetworkUnit::masterThreadTask, this);
	m_masterThread = std::thread(l_threadtask);
	LOG(INFO) << "create thread for MasterThreadNetworkUnit successfully";
	return true;
}

bool MasterThreadNetworkUnit::isErrorOccurred(std::string p_data)
{
	if(p_data == std::string("selectError") || p_data == std::string("readError") ||
			p_data == std::string("codeError"))
	{
		return true;
	}
	return false;
}

bool MasterThreadNetworkUnit::masterThreadTask()
{
	LOG(INFO) << "MasterThreadNetworkUnit::masterThreadTask start";
	if(false == m_udpContainer->create())
	{
		LOG(WARNING) << "MasterThreadNetworkUnit call UDPContainer::create failed";
		return false;
	}

	while(m_ThreadShouldExit == false)
	{
		std::string l_data = m_udpContainer->read(1, 0);
		if(true == isErrorOccurred(l_data))
		{
			LOG(WARNING) << "TerminalContainer read data failed: " << l_data;
		}
		else if(l_data == std::string("timeout"))
		{

		}
		else
		{
			if(l_data.empty() == false)
			{
				LOG(INFO) << "UDPContainer read data :" << l_data;
				auto l_eventobj = std::make_shared<EventTypeDataObject>(EventType::E_EVENT_TYPE_ANDLINK_DEVICE,
						l_data.c_str(), l_data.size());
				m_dataCallback(l_eventobj);
			}
		}
	}
	LOG(INFO) << "MasterThreadNetworkUnit::masterThreadTask start";
	return true;
}
