/*
 * TerminalCmd.hpp
 *
 *  Created on: Jul 3, 2018
 *      Author: xiaofenh
 */

#pragma once

#include <functional>
#include <string>

class TerminalCmd
{
public:
	virtual ~TerminalCmd() = default;
	virtual std::string run(std::string p_cmd) = 0;
	virtual std::string help() = 0;
};

class TerminalCmdHelp: public TerminalCmd
{
public:
	TerminalCmdHelp(std::function<std::string()> p_action): m_action(p_action){ }
	std::string run(std::string p_cmd) override
	{
		return m_action();
	}
	std::string help() override
	{
		return "show all terminal cmd";
	}
private:
	std::function<std::string()> m_action;
};

class TerminalCmdList: public TerminalCmd
{
public:
	TerminalCmdList(std::function<std::string(std::string)> p_action): m_action(p_action){ }
	std::string run(std::string p_cmd) override
	{
		return m_action(p_cmd);
	}
	std::string help() override
	{
		return "list device information cmd";
	}
private:
	std::function<std::string(std::string)> m_action;
};

class TerminalCmdStatistics: public TerminalCmd
{
public:
	TerminalCmdStatistics(std::function<std::string()> p_action): m_action(p_action){ }
	std::string run(std::string p_cmd) override
	{
		return m_action();
	}
	std::string help() override
	{
		return "show packet send and recv";
	}
private:
	std::function<std::string()> m_action;
};

class TerminalCmdPlugIn: public TerminalCmd
{
public:
	TerminalCmdPlugIn(std::function<std::string(std::string)> p_action): m_action(p_action){ }
	std::string run(std::string p_cmd) override
	{
		return m_action(p_cmd);
	}
	std::string help() override
	{
		return "trigger andlink device connect to TCP server, for example: plugin deviceID";
	}
private:
	std::function<std::string(std::string)> m_action;
};

class TerminalCmdDisconnect: public TerminalCmd
{
public:
	TerminalCmdDisconnect(std::function<std::string(std::string)> p_action): m_action(p_action){ }
	std::string run(std::string p_cmd) override
	{
		return m_action(p_cmd);
	}
	std::string help() override
	{
		return "trigger andlink device disconnect to TCP server, for example: disconnect deviceID";
	}
private:
	std::function<std::string(std::string)> m_action;
};
