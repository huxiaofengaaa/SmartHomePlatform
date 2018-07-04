#include <iostream>
#include <unistd.h>
#include "ExcutionUnitTerminal.hpp"
#include "EventTypeStruct.hpp"
#include "glog/logging.h"

#define PLATFORM_NAME	"SmartHomePlatform # "

ExcutionUnitTerminal::ExcutionUnitTerminal():
	ExcutionUnit("Terminal", 1, std::bind(&ExcutionUnitTerminal::handleDataObject, this, std::placeholders::_1)),
	AsynTerminalHandler(std::bind(&ExcutionUnitTerminal::terminalAsycDataCallback, this, std::placeholders::_1))
{
	LOG(INFO) << "construct ExcutionUnitTerminal";
}

ExcutionUnitTerminal::~ExcutionUnitTerminal()
{
	LOG(INFO) << "de-construct ExcutionUnitTerminal";
}

bool ExcutionUnitTerminal::start()
{
	LOG(INFO) << "ExcutionUnitTerminal start";
	runTerminal();
	startExcutionUnit();
	writeTerminalString(PLATFORM_NAME);
	return true;
}
void ExcutionUnitTerminal::shutdown()
{
	shutdownTerminal();
	shutdownExcutionUnit();
	LOG(INFO) << "ExcutionUnitTerminal shutdown";
}

bool ExcutionUnitTerminal::terminalAsycDataCallback(std::string p_data)
{
	return addDataObject(std::make_shared<EventTypeTerminalDataObject>(p_data));
}

bool ExcutionUnitTerminal::handleDataObject(std::shared_ptr<EventTypeTerminalDataObject> p_eventObj)
{
	if(p_eventObj->m_rawData != std::string("\n"))
	{
		LOG(INFO) << p_eventObj;
		std::string l_result = runTerminalCmd(p_eventObj->m_rawData);
		if(l_result.empty() == false)
		{
			writeTerminalString(l_result);
		}
	}
	writeTerminalString(PLATFORM_NAME);
	return true;
}

std::string ExcutionUnitTerminal::runTerminalCmd(std::string p_cmd)
{
	std::string l_cmdName;
	for(auto i = 0; i < p_cmd.size(); i++)
	{
		if(p_cmd[i] == ' ' || p_cmd[i] == '\n')
		{
			l_cmdName = p_cmd.substr(0, i);
			break;
		}
	}

	auto l_cmdObj = m_cmdList.find(l_cmdName);
	if(l_cmdObj != m_cmdList.end())
	{
		return l_cmdObj->second->run(p_cmd);
	}
	else
	{
		return std::string("Unknow Terminal Cmd ") + p_cmd + getAllTerminalCmdUsage();
	}
}

void ExcutionUnitTerminal::registerCmd(std::string p_cmdName, std::shared_ptr<TerminalCmd> p_cmdObj)
{
	m_cmdList.erase(p_cmdName);
	m_cmdList.insert({p_cmdName, p_cmdObj});
}

std::string ExcutionUnitTerminal::getAllTerminalCmdUsage()
{
	std::string l_result = "Usage:\n";
	for(auto l_cmd = m_cmdList.begin() ; l_cmd != m_cmdList.end() ; l_cmd++)
	{
		l_result += l_cmd->second->help();
	}
	return l_result;
}

