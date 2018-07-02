#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <thread>
#include "ExcutionUnitUDPNetwork.hpp"
#include "ExcutionUnitTerminal.hpp"
#include "EventTypeStruct.hpp"
#include "SystemNotify.hpp"
#include "EventTerminal.hpp"

class SmartHomeDeployment
{
public:
	SmartHomeDeployment();
	~SmartHomeDeployment();
	void start();
	bool shutdown(int p_signum);

	bool TerminalRawDataCallback(std::shared_ptr<EventTypeDataObject> p_event);
	bool UeContextRawDataCallback(std::shared_ptr<EventTypeNetworkDataObject> p_event);

private:
	bool createUeContextThread(int p_threadNumber);
	void UeContextThreadTaskMainLoop();

	bool m_deploymentShouldExit;
	std::vector<std::thread> m_UeContextThreadList;

	std::shared_ptr<TerminalCommnadHandler> m_cmdHandler;

	// terminal event
	std::shared_ptr<TerminalThreadUnit> m_terminalUnit;
	std::vector<std::shared_ptr<EventTypeDataObject>> m_TerminalEventQueue;
	std::mutex m_TerminalEventQueueMutex;
	SystemNotify m_terminalEventQueueNotify;
	void addTerminalEventObject(std::shared_ptr<EventTypeDataObject> p_event);
	std::shared_ptr<EventTypeDataObject> detachTerminalEventObject();
	int getTerminalEventObjectSize();
	bool isTerminalEventObjectNotEmpty();

	// Uecontext event
	std::shared_ptr<MasterThreadNetworkUnit> m_udpNetworkUnit;
	std::vector<std::shared_ptr<EventTypeNetworkDataObject>> m_UeContextEventQueue;
	std::mutex m_UeContextEventQueueMutex;
	SystemNotify m_UeContextEventQueueNotify;
	void addUeContextEventObject(std::shared_ptr<EventTypeNetworkDataObject> p_event);
	std::shared_ptr<EventTypeNetworkDataObject> detachUeContextEventObject();
	int getUeContextEventObjectSize();
	bool isUeContextEventObjectNotEmpty();
};


