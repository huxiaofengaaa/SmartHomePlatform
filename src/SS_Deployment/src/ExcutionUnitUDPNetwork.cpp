#include <iostream>
#include <unistd.h>
#include "ExcutionUnitUDPNetwork.hpp"
#include "ContainerNetwork.hpp"
#include "glog/logging.h"

MasterThreadNetworkUnit::MasterThreadNetworkUnit()
{
	LOG(INFO) << "Construct MasterThreadNetworkUnit";
}

MasterThreadNetworkUnit::~MasterThreadNetworkUnit()
{
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

bool MasterThreadNetworkUnit::masterThreadTask()
{
	if(false == m_udpContainer->create())
	{
		LOG(WARNING) << "MasterThreadNetworkUnit call UDPContainer::create failed";
		return false;
	}

	while(true)
	{
		std::string l_data = m_udpContainer->read(1, 0);
		if(l_data == std::string("selectError"))
		{
			LOG(WARNING) << "UDPContainer select error: " << l_data;
		}
		else if(l_data == std::string("timeout"))
		{

		}
		else if(l_data == std::string("readError"))
		{
			LOG(WARNING) << "UDPContainer read fd error: " << l_data;
		}
		else if(l_data == std::string("codeError"))
		{
			LOG(WARNING) << "UDPContainer code error: " << l_data;
		}
		else
		{
			if(l_data.empty() == false)
			{
				LOG(INFO) << "UDPContainer read data :" << l_data;
			}
		}
	}

	return true;
}
