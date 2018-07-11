/*
 * ExcutionUnitTerminalCmdCallback.cpp
 *
 *  Created on: 2018��7��10��
 *      Author: Administrator
 */
#include "ExcutionUnitTerminal.hpp"

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

		std::string ipaddress = l_uecontext->peerUDPHost + ":" + std::to_string(l_uecontext->peerUDPPort);

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
		if(true == m_euAndlink->triggerPlugIn(l_uecontext->peerUDPHost, l_uecontext->peerUDPPort, l_deviceid))
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
		if(true == m_euAndlink->triggerDisconnect(l_uecontext->peerUDPHost, l_uecontext->peerUDPPort, l_deviceid))
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

std::string ExcutionUnitTerminal::terminalCmdCallback_query(std::string p_cmd)
{
	auto l_parameterlist = resolveParameter(p_cmd);
	if(l_parameterlist.size() != 3)
	{
		return "\n\tError, Usage:" + m_cmdList["query"]->help() + "\n\n";
	}
	std::string l_deviceid = l_parameterlist[1];
	std::string l_param = l_parameterlist[2];



	return std::string();
}

