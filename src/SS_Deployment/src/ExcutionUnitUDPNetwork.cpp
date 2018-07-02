#include <iostream>
#include <unistd.h>
#include "ExcutionUnitUDPNetwork.hpp"
#include "ContainerNetwork.hpp"
#include "glog/logging.h"

MasterThreadNetworkUnit::MasterThreadNetworkUnit(std::string p_host, int p_port,
		std::function<bool(std::shared_ptr<EventTypeNetworkDataObject>)> p_callback)
	: m_host(p_host), m_port(p_port), m_ThreadShouldExit(false), m_dataCallback(p_callback)
{
	LOG(INFO) << "Construct MasterThreadNetworkUnit - " << m_host << ":" << m_port;
}

MasterThreadNetworkUnit::~MasterThreadNetworkUnit()
{
	m_ThreadShouldExit = true;
	m_masterThread.join();
	LOG(INFO) << "de-construct MasterThreadNetworkUnit";
}

bool MasterThreadNetworkUnit::run()
{
	m_udpContainer = std::make_shared<UDPNetworkContainer>(m_host, m_port);

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
	LOG(INFO) << "MasterThreadNetworkUnit main loop start";
	if(false == m_udpContainer->create())
	{
		LOG(WARNING) << "MasterThreadNetworkUnit call UDPContainer::create failed";
		return false;
	}

	while(m_ThreadShouldExit == false)
	{
		std::shared_ptr<NetworkData> l_data = m_udpContainer->read(1, 0);
		if(true == isErrorOccurred(l_data->m_rawData))
		{
			LOG(WARNING) << "TerminalContainer read data failed: " << l_data;
		}
		else if(l_data->m_rawData == std::string("timeout"))
		{

		}
		else
		{
			if(l_data->m_rawData.empty() == false)
			{
				auto l_eventobj = std::make_shared<EventTypeNetworkDataObject>(
						l_data->m_clientAddr,
						l_data->m_fd,
						l_data->m_rawData);
				m_dataCallback(l_eventobj);
			}
		}
	}
	LOG(INFO) << "MasterThreadNetworkUnit main loop exit";
	return true;
}
