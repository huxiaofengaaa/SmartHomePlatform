/*
 * ExcutionUnitTerminalCmdCallback.cpp
 *
 *  Created on: 2018��7��10��
 *      Author: Administrator
 */
#include "ExcutionUnitTerminal.hpp"
#include <sstream>

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

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\tPlugIn   Send Packets: %d\n", m_euAndlinkPlugin->getSendPacket());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         Recv Packets: %d\n", m_euAndlinkPlugin->getRecvPacket());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         Send Bytes  : %d\n", m_euAndlinkPlugin->getSendBytes());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         Recv Bytes  : %d\n", m_euAndlinkPlugin->getRecvBytes());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\tClient   DeviceAll   : %d\n", m_ueContextHolder->size());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         DeviceOnline: %d\n", m_ueContextHolder->onlineSize());
	l_result += std::string(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "\t         DevicePlugin: %d\n", m_ueContextHolder->pluginSize());
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
	if(true == m_euAndlinkPlugin->triggerDisconnect(l_deviceid))
	{
		return "\n\t" + l_deviceid + " trigger disconnect success\n\n";
	}
	else
	{
		return "\n\t" + l_deviceid + " trigger disconnect failed\n\n";
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

	if(true == m_euAndlinkPlugin->triggerQuery(l_deviceid, l_param))
	{
		return "\n\t" + l_deviceid + " trigger query success\n\n";
	}
	else
	{
		return "\n\t" + l_deviceid + " trigger query failed\n\n";
	}

	return std::string();
}

std::string ExcutionUnitTerminal::terminalCmdCallback_control(std::string p_cmd)
{
	auto l_parameterlist = resolveParameter(p_cmd);
	bool l_controlResult = false;
	std::string l_cmd = l_parameterlist[0];
	if(l_cmd == "reboot")
	{
		if(l_parameterlist.size() != 2)
		{
			return "\n\tError, Usage:" + m_cmdList["reboot"]->help() + "\n\n";
		}
		std::string l_deviceID = l_parameterlist[1];
		l_controlResult = m_euAndlinkPlugin->triggerReboot(l_deviceID);
	}
	else if(l_cmd == "led")
	{
		if(l_parameterlist.size() != 3)
		{
			return "\n\tError, Usage:" + m_cmdList["led"]->help() + "\n\n";
		}
		std::string l_deviceID = l_parameterlist[1];
		std::string l_state = l_parameterlist[2];
		l_controlResult = m_euAndlinkPlugin->triggerLEDControl(
				l_deviceID, l_state == "on" ? true : false);
	}
	else if(l_cmd == "power")
	{
		if(l_parameterlist.size() != 4)
		{
			return "\n\tError, Usage:" + m_cmdList["power"]->help() + "\n\n";
		}
		std::string l_deviceID = l_parameterlist[1];
		int p_select = -1;
		int p_percent = -1;
		std::stringstream stream(l_parameterlist[2]);
		stream >> p_select;
		std::stringstream stream2(l_parameterlist[3]);
		stream2 >> p_percent;
		l_controlResult = m_euAndlinkPlugin->triggerPowerControl(l_deviceID, p_select, p_percent);
	}
	else if(l_cmd == "macfilter")
	{
		if(l_parameterlist.size() != 5)
		{
			return "\n\tError, Usage:" + m_cmdList["macfilter"]->help() + "\n\n";
		}
		std::string l_deviceID = l_parameterlist[1];

		int ll_enable;
		std::stringstream stream2(l_parameterlist[2]);
		stream2 >> ll_enable;
		bool l_enable = ll_enable == 0 ? false : true;

		int l_policy;
		std::stringstream stream(l_parameterlist[3]);
		stream >> l_policy;

		std::string p_entry = l_parameterlist[4];

		l_controlResult = m_euAndlinkPlugin->triggerMACFilterControl(
				l_deviceID, l_enable, l_policy, p_entry);
	}
	else if(l_cmd == "chReselct")
	{
		if(l_parameterlist.size() != 3)
		{
			return "\n\tError, Usage:" + m_cmdList["chReselct"]->help() + "\n\n";
		}
		std::string l_deviceID = l_parameterlist[1];

		int p_select = -1;
		std::stringstream stream(l_parameterlist[2]);
		stream >> p_select;
		l_controlResult = m_euAndlinkPlugin->triggerChannelReselectControl(l_deviceID, p_select);
	}
	else if(l_cmd == "roaming")
	{
		if(l_parameterlist.size() != 5)
		{
			return "\n\tError, Usage:" + m_cmdList["roaming"]->help() + "\n\n";
		}
		std::string l_deviceID = l_parameterlist[1];

		int l_enable = 0;
		std::stringstream stream2(l_parameterlist[2]);
		stream2 >> l_enable;

		int l_val1 = -1;
		std::stringstream stream3(l_parameterlist[3]);
		stream3 >> l_val1;

		int l_val2 = -1;
		std::stringstream stream4(l_parameterlist[4]);
		stream4 >> l_val2;

		l_controlResult = m_euAndlinkPlugin->triggerRoamingConfigControl(l_deviceID,
				l_enable == 0 ? false : true, l_val1, l_val2);
	}
	else if(l_cmd == "wifiswitch")
	{
		if(l_parameterlist.size() != 4)
		{
			return "\n\tError, Usage:" + m_cmdList["wifiswitch"]->help() + "\n\n";
		}
		std::string l_deviceID = l_parameterlist[1];

		bool l_enable = false;
		if(l_parameterlist[2] == "on")
		{
			l_enable = true;
		}

		int p_select = -1;
		std::stringstream stream3(l_parameterlist[3]);
		stream3 >> p_select;

		l_controlResult = m_euAndlinkPlugin->triggerWiFiSwitchControl(l_deviceID, l_enable, p_select);
	}
	else if(l_cmd == "wps")
	{
		if(l_parameterlist.size() != 3)
		{
			return "\n\tError, Usage:" + m_cmdList["wps"]->help() + "\n\n";
		}
		std::string l_deviceID = l_parameterlist[1];

		int p_select = -1;
		std::stringstream stream2(l_parameterlist[2]);
		stream2 >> p_select;

		l_controlResult = m_euAndlinkPlugin->triggerWPSControl(l_deviceID, p_select);
	}
	else if(l_cmd == "wifisync")
	{
		if(l_parameterlist.size() != 2)
		{
			return "\n\tError, Usage:" + m_cmdList["wifisync"]->help() + "\n\n";
		}
		std::string l_deviceID = l_parameterlist[1];

		l_controlResult = m_euAndlinkPlugin->triggerWiFiParameterSyncControl(l_deviceID);
	}
	if(true == l_controlResult)
	{
		return "\n\tTrigger " + l_cmd + " success\n\n";
	}
	else
	{
		return "\n\tTrigger " + l_cmd + " failed\n\n";
	}
}
