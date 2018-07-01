#include <iostream>
#include <unistd.h>
#include "ExcutionUnitTerminal.hpp"
#include "ContainerTerminal.hpp"
#include "EventTypeStruct.hpp"
#include "glog/logging.h"

#define PLATFORM_NAME	"SmartHomePlatform # "

TerminalThreadUnit::TerminalThreadUnit(std::function<bool(std::shared_ptr<EventTypeDataObject>)> p_callback)
	: m_dataCallback(p_callback), m_ThreadShouldExit(false)
{
	LOG(INFO) << "construct TerminalThreadUnit";
}

TerminalThreadUnit::~TerminalThreadUnit()
{
	m_ThreadShouldExit = true;
	m_masterThread.join();
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
	LOG(INFO) << "TerminalThreadUnit::masterThreadTask start";
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
				auto l_eventobj = std::make_shared<EventTypeDataObject>(EventType::E_EVENT_TYPE_TERMINAL_INPUT,
						l_data.c_str(), l_data.size());
				m_dataCallback(l_eventobj);
			}
			m_terminalContainer->write(std::string(PLATFORM_NAME));
		}
	}
	while(m_ThreadShouldExit == false);
	LOG(INFO) << "TerminalThreadUnit::masterThreadTask exit";
	return true;
}
