#include "ExcutionUnitTerminal.hpp"

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
	countRecvPacket(p_data.size());
	return addDataObject(p_data);
}

bool ExcutionUnitTerminal::handleDataObject(std::string p_str)
{
	if(p_str != std::string("\n"))
	{
		LOG(INFO) << "Terminal input, size:" << p_str.size() << ", msg:" << p_str;
		std::string l_result = runTerminalCmd(p_str);
		if(l_result.empty() == false)
		{
			countSendPacket(l_result.size());
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
		return std::string("Unknow Terminal Cmd ") + p_cmd + terminalCmdCallback_help(p_cmd);
	}
}

void ExcutionUnitTerminal::registerCmd(std::string p_cmdName, std::shared_ptr<TerminalCmd> p_cmdObj)
{
	m_cmdList.erase(p_cmdName);
	m_cmdList.insert({p_cmdName, p_cmdObj});
}

void ExcutionUnitTerminal::registerAllCmd()
{
	registerCmd("help", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_help,
						this,
						std::placeholders::_1),
				"show all terminal cmd"));

	registerCmd("list", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_list,
						this,
						std::placeholders::_1),
				"list device information cmd"));

	registerCmd("statistics", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_statistics,
						this,
						std::placeholders::_1),
				"show packet send and recv"));

	registerCmd("plugin", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_plugin,
						this,
						std::placeholders::_1),
				"trigger andlink device connect to TCP server, for example: plugin deviceID"));

	registerCmd("disconnect", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_disconnect,
						this,
						std::placeholders::_1),
				"trigger andlink device disconnect to TCP server, for example: disconnect deviceID"));
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

std::string ExcutionUnitTerminal::terminalCmdCallback_help(std::string p_cmd)
{
	char buffer[128] = { 0 };
	std::string l_result = "\nUsage:\n";
	for(auto l_cmd = m_cmdList.begin() ; l_cmd != m_cmdList.end() ; l_cmd++)
	{
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, sizeof(buffer), "\t%-16s    %s\n", l_cmd->first.c_str(), l_cmd->second->help().c_str());
		l_result += std::string(buffer);
	}
	l_result += "\n";
	return l_result;
}

std::string ExcutionUnitTerminal::terminalCmdCallback_list(std::string p_cmd)
{
	char buffer[512] = { 0 };

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\n\t%-24s | %-21s | %-17s | %-16s | %-16s | %-16s\n",
			"DeviceID", "IPaddress", "MAC", "firmwareVersion", "softwareVersion", "deviceVendor");

	std::string l_result = std::string(buffer);

	auto l_devicelist = m_ueContextHolder->getDeviceList();
	for(auto l_device: l_devicelist)
	{
		auto l_uecontext = m_ueContextHolder->getRef(l_device);
		memset(buffer, 0, sizeof(buffer));

		std::string ipaddress = l_uecontext->host + ":" + std::to_string(l_uecontext->port);

		snprintf(buffer, sizeof(buffer), "\t%-24s | %-21s | %-17s | %-16s | %-16s | %-16s\n",
				l_uecontext->deviceId.c_str(),
				ipaddress.c_str(),
				l_uecontext->deviceMac.c_str(),
				l_uecontext->firmwareVersion.c_str(),
				l_uecontext->softwareVersion.c_str(),
				l_uecontext->deviceVendor.c_str());
		l_result += std::string(buffer);
	}
	l_result += "\n";
	return l_result;
}

std::string ExcutionUnitTerminal::terminalCmdCallback_statistics(std::string p_cmd)
{
	std::string l_result = "\n";
	char buffer[512] = { 0 };
	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\tTerminal Send Packets: %d\n", getSendPacket());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         Recv Packets: %d\n", getRecvPacket());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         Send Bytes  : %d\n", getSendBytes());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         Recv Bytes  : %d\n", getRecvBytes());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\tAndlink  Send Packets: %d\n", m_euAndlink->getSendPacket());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         Recv Packets: %d\n", m_euAndlink->getRecvPacket());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         Send Bytes  : %d\n", m_euAndlink->getSendBytes());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         Recv Bytes  : %d\n", m_euAndlink->getRecvBytes());
	l_result += std::string(buffer);

	l_result += "\n";
	return l_result;
}

std::string ExcutionUnitTerminal::terminalCmdCallback_plugin(std::string p_cmd)
{
	auto l_parameterlist = resolveParameter(p_cmd);
	if(l_parameterlist.size() < 2)
	{
		return "\n\tError, Usage:" + m_cmdList["plugin"]->help() + "\n\n";
	}
	std::string l_deviceid = l_parameterlist[1];
	auto l_uecontext = m_ueContextHolder->getRef(l_deviceid);
	if(l_uecontext)
	{
		if(true == m_euAndlink->triggerPlugIn(l_uecontext->host, l_uecontext->port, l_deviceid))
		{
			return "\n\t" + l_deviceid + " trigger plugin success\n\n";
		}
		else
		{
			return "\n\t" + l_deviceid + " trigger plugin failed\n\n";
		}
	}
	else
	{
		return "\n\t" + l_deviceid + " Not Exist\n\n";
	}
}

std::string ExcutionUnitTerminal::terminalCmdCallback_disconnect(std::string p_cmd)
{
	auto l_parameterlist = resolveParameter(p_cmd);
	if(l_parameterlist.size() < 2)
	{
		return "\n\tError, Usage:" + m_cmdList["disconnect"]->help() + "\n\n";
	}
	std::string l_deviceid = l_parameterlist[1];
	auto l_uecontext = m_ueContextHolder->getRef(l_deviceid);
	if(l_uecontext)
	{
		if(true == m_euAndlink->triggerDisconnect(l_uecontext->host, l_uecontext->port, l_deviceid))
		{
			return "\n\t" + l_deviceid + " trigger disconnect success\n\n";
		}
		else
		{
			return "\n\t" + l_deviceid + " trigger disconnect failed\n\n";
		}
	}
	else
	{
		return "\n\t" + l_deviceid + std::string(" Not Exist\n\n");
	}
}
