#pragma once

#include <memory>
#include <thread>
#include <functional>
#include "ContainerTerminal.hpp"
#include "EventTypeStruct.hpp"

class TerminalThreadUnit
{
public:
	TerminalThreadUnit(std::function<bool(std::shared_ptr<EventTypeDataObject>)> p_callback);
	~TerminalThreadUnit();
	bool run();
private:
	bool masterThreadTask();
	bool dataCallback();
	bool isErrorOccurred(std::string p_data);

	std::shared_ptr<TerminalContainer> m_terminalContainer;
	std::thread m_masterThread;
	std::function<bool(std::shared_ptr<EventTypeDataObject>)> m_dataCallback;

	bool m_ThreadShouldExit;
};
