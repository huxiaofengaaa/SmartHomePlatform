#include <iostream>
#include <unistd.h>
#include "ExcutionUnitTerminal.hpp"
#include "ContainerTerminal.hpp"
#include "glog/logging.h"

#define PLATFORM_NAME	"SmartHomePlatform # "

TerminalThreadUnit::TerminalThreadUnit()
{
	LOG(INFO) << "construct TerminalThreadUnit";
}

TerminalThreadUnit::~TerminalThreadUnit()
{
	LOG(INFO) << "de-construct TerminalThreadUnit";
}

bool TerminalThreadUnit::run()
{
	m_terminalContainer = std::make_shared<TerminalContainer>();

	std::function<bool()> l_threadtask = std::bind(&TerminalThreadUnit::masterThreadTask, this);
	m_masterThread = std::thread(l_threadtask);
	LOG(INFO) << "create TerminalContainer thread successfully";
	return true;
}

bool TerminalThreadUnit::isErrorOccurred(std::string p_data)
{
	if(p_data == std::string("selectError") || p_data == std::string("readError") ||
			p_data == std::string("codeError"))
	{
		return true;
	}
	return false;
}

bool TerminalThreadUnit::masterThreadTask()
{
	m_terminalContainer->write(std::string(PLATFORM_NAME));
	do
	{
		std::string l_data = m_terminalContainer->read(1, 0);
		if(true == isErrorOccurred(l_data))
		{
			LOG(WARNING) << "TerminalContainer read data failed: " << l_data;
		}
		else if(l_data == std::string("timeout"))
		{

		}
		else if(l_data == std::string("\n"))
		{
			m_terminalContainer->write(std::string(PLATFORM_NAME));
		}
		else
		{
			if(l_data.empty() == false)
			{
				LOG(INFO) << "TerminalContainer read data :" << l_data;
			}
			m_terminalContainer->write(std::string(PLATFORM_NAME));
		}
	}
	while(true);

	return true;
}
