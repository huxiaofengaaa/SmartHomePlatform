#include <iostream>
#include <string>
#include <unistd.h>
#include "ExcutionUnitTerminal.hpp"
#include "EventTypeStruct.hpp"
#include "glog/logging.h"

#define PLATFORM_NAME	"SmartHomePlatform # "

ExcutionUnitTerminal::ExcutionUnitTerminal(std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder,
		std::shared_ptr<ExcutionUnitAndlink> p_euAndlink):
	ExcutionUnit("Terminal", 1, std::bind(&ExcutionUnitTerminal::handleDataObject, this, std::placeholders::_1)),
	AsynTerminalHandler(std::bind(&ExcutionUnitTerminal::terminalAsycDataCallback, this, std::placeholders::_1)),
	m_ueContextHolder(p_ueContextHolder), m_euAndlink(p_euAndlink)
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
	registerAllCmd();
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
		return std::string("Unknow Terminal Cmd ") + p_cmd + terminalCmdCallback_help();
	}
}

void ExcutionUnitTerminal::registerCmd(std::string p_cmdName, std::shared_ptr<TerminalCmd> p_cmdObj)
{
	m_cmdList.erase(p_cmdName);
	m_cmdList.insert({p_cmdName, p_cmdObj});
}

void ExcutionUnitTerminal::registerAllCmd()
{
	registerCmd("help", std::make_shared<TerminalCmdHelp>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_help, this)));

	registerCmd("list", std::make_shared<TerminalCmdList>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_list, this, std::placeholders::_1)));

	registerCmd("plugin", std::make_shared<TerminalCmdPlugIn>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_plugin, this, std::placeholders::_1)));
}

std::vector<std::string> ExcutionUnitTerminal::resolveParameter(std::string p_cmd)
{
	std::vector<std::string> l_result;
	int start = 0;
	while(start < p_cmd.size())
	{
		int position = p_cmd.find_first_of(' ', start);
		if(position == std::string::npos)
		{
			int sublen = p_cmd.size()-start;
			if(p_cmd.back() == '\n')
			{
				sublen -= 1;
			}
			if(sublen > 0)
			{
				std::string substr = p_cmd.substr(start, sublen);
				l_result.push_back(substr);
			}
			break;
		}
		else
		{
			int sublen = position - start;
			if(sublen > 0)
			{
				std::string substr = p_cmd.substr(start, sublen);
				l_result.push_back(substr);
			}
			start = position + 1;
		}
	}
	return l_result;
}

std::string ExcutionUnitTerminal::terminalCmdCallback_help()
{
	std::string l_result = "Usage:\n";
	for(auto l_cmd = m_cmdList.begin() ; l_cmd != m_cmdList.end() ; l_cmd++)
	{
		l_result += l_cmd->second->help();
	}
	return l_result;
}

std::string ExcutionUnitTerminal::terminalCmdCallback_list(std::string p_cmd)
{
	std::string l_result = "\n";
	auto l_devicelist = m_ueContextHolder->getDeviceList();
	for(auto l_device: l_devicelist)
	{
		auto l_uecontext = m_ueContextHolder->getRef(l_device);
		l_result += "\t" + l_uecontext->deviceId;
		l_result += "\t\t" + l_uecontext->host + ":" + std::to_string(l_uecontext->port);
		l_result += "\t\t" + l_uecontext->deviceMac;
		l_result += "\n";
	}

	return l_result;
}

std::string ExcutionUnitTerminal::terminalCmdCallback_plugin(std::string p_cmd)
{
	auto l_parameterlist = resolveParameter(p_cmd);
	if(l_parameterlist.size() < 2)
	{
		return "Error, Usage:\n" + m_cmdList["plugin"]->help();
	}
	std::string l_deviceid = l_parameterlist[1];
	if(false == m_ueContextHolder->isExist(l_deviceid))
	{
		return "Error, " + l_deviceid + " not exist\n";
	}
	auto l_uecontext = m_ueContextHolder->getRef(l_deviceid);
	m_euAndlink->triggerPlugIn(l_uecontext->host, l_uecontext->port, l_deviceid);
	return std::string();
}
