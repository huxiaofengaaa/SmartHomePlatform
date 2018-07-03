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
	virtual ~TerminalCmd(){ }
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
		return "\thelp\t\tshow all terminal cmd\n";
	}
private:
	std::function<std::string()> m_action;
};
