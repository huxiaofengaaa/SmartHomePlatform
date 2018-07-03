/*
 * TerminalHandler.hpp
 *
 *  Created on: Jul 2, 2018
 *      Author: xiaofenh
 */

#pragma once

#include <string>
#include <functional>
#include <thread>

class SyncTerminalHandler
{
public:
	SyncTerminalHandler() = default;
	virtual ~SyncTerminalHandler() = default;
	int writeTerminalString(std::string p_str);
	std::string readTerminalString(int p_timeoutSec, int p_timeoutUsec);
private:

};

class AsynTerminalHandler
{
public:
	AsynTerminalHandler(std::function<bool(std::string)> p_callback);
	virtual ~AsynTerminalHandler();
	bool runTerminal();
	int writeTerminalString(std::string p_str);
	void shutdownTerminal();
private:
	void mainloop();
	std::thread m_thread;
	bool m_threadExitFlag;
	std::function<bool(std::string)> m_callback;
};

