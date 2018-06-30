#include <iostream>
#include <unistd.h>
#include "ExcutionUnitUDPNetwork.hpp"
#include "ContainerNetwork.hpp"

MasterThreadNetworkUnit::MasterThreadNetworkUnit()
{

}

MasterThreadNetworkUnit::~MasterThreadNetworkUnit()
{

}

bool MasterThreadNetworkUnit::run()
{
	m_udpContainer = std::make_shared<UDPNetworkContainer>();

	std::function<bool()> l_threadtask = std::bind(&MasterThreadNetworkUnit::masterThreadTask, this);
	m_masterThread = std::thread(l_threadtask);
	return true;
}

bool MasterThreadNetworkUnit::masterThreadTask()
{
	if(false == m_udpContainer->create())
	{
		return false;
	}

	while(true)
	{
		std::string l_data = m_udpContainer->read(1, 0);
		if(l_data == std::string("selectError"))
		{
			std::cout << l_data << std::endl;
		}
		else if(l_data == std::string("timeout"))
		{
//			std::cout << l_data << std::endl;
		}
		else if(l_data == std::string("readError"))
		{
			std::cout << l_data << std::endl;
		}
		else if(l_data == std::string("codeError"))
		{
			std::cout << l_data << std::endl;
		}
		else
		{
			if(l_data.empty() == false)
			{
				std::cout << l_data << std::endl;
			}
		}
	}

	return true;
}
