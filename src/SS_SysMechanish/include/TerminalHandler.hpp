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
	~SyncTerminalHandler() = default;
	int writeString(std::string p_str);
	std::string readString(int p_timeoutSec, int p_timeoutUsec);
private:

};

class AsynTerminalHandler
{
public:
	AsynTerminalHandler(std::function<bool(std::string)> p_callback);
	~AsynTerminalHandler();
	bool run();
	int writeString(std::string p_str);
private:
	void mainloop();
	std::thread m_thread;
	bool m_threadExitFlag;
	std::function<bool(std::string)> m_callback;
};

