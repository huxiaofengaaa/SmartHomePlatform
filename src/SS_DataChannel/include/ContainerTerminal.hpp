#pragma once

#include <iostream>
#include <string>
#include "SocketHandle.hpp"

class TerminalContainer
{
public:
	TerminalContainer()
	{

	}
	~TerminalContainer()
	{

	}
	std::string read(int p_timeoutSec, int p_timeoutUsec);
	ssize_t write(std::string p_data);
private:

};
