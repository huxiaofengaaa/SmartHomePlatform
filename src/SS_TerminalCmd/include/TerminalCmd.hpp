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

class TerminalCmdNormal: public TerminalCmd
{
public:
	TerminalCmdNormal(std::function<std::string(std::string p_cmd)> p_action,
			std::string p_helpInfo):
		m_action(p_action), m_helpInfo(p_helpInfo)
	{

	}
	std::string run(std::string p_cmd) override
	{
		return m_action(p_cmd);
	}
	std::string help() override
	{
		return m_helpInfo;
	}
private:
	std::function<std::string(std::string p_cmd)> m_action;
	std::string m_helpInfo;
};
