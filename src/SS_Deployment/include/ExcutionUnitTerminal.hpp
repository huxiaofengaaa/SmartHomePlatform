#pragma once

#include <memory>
#include <thread>
#include <functional>
#include "ContainerTerminal.hpp"

class TerminalThreadUnit
{
public:
	TerminalThreadUnit();
	~TerminalThreadUnit();
	bool run();
private:
	bool masterThreadTask();
	bool dataCallback();
	bool isErrorOccurred(std::string p_data);

	std::shared_ptr<TerminalContainer> m_terminalContainer;
	std::thread m_masterThread;
};
