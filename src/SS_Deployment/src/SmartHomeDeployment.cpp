#include "SmartHomeDeployment.hpp"
#include "SignalRegister.hpp"
#include "glog/logging.h"
#include <unistd.h>
#include <functional>

SmartHomeDeployment::SmartHomeDeployment()
{
	m_deploymentShouldExit = false;

	SignalRegister l_regster;
	std::function<bool(int)> l_sigintTask = std::bind(&SmartHomeDeployment::shutdown, this, std::placeholders::_1);
	l_regster.registerSignal(SIGINT, l_sigintTask);
	LOG(INFO) << "register user handler for linux signal " << SIGINT;

	std::function<bool(std::shared_ptr<EventTypeDataObject>)> l_UDPDataCallback =
			std::bind(&SmartHomeDeployment::UeContextRawDataCallback,
			this, std::placeholders::_1);
	m_udpNetworkUnit = std::make_shared<MasterThreadNetworkUnit>(l_UDPDataCallback);

	std::function<bool(std::shared_ptr<EventTypeDataObject>)> l_terminalDataCallback =
			std::bind(&SmartHomeDeployment::TerminalRawDataCallback,
			this, std::placeholders::_1);
	m_terminalUnit = std::make_shared<TerminalThreadUnit>(l_terminalDataCallback);

	LOG(INFO) << "construct SmartHomeDeployment";
}

SmartHomeDeployment::~SmartHomeDeployment()
{
	LOG(INFO) << "de-constructor SmartHomeDeployment";

	for(auto l_thread = m_UeContextThreadList.begin() ; l_thread != m_UeContextThreadList.end(); l_thread++)
	{
		l_thread->join();
	}
	m_UeContextThreadList.clear();
	m_UeContextEventQueue.clear();
	m_TerminalEventQueue.clear();

	LOG(INFO) << "Good Bye SmartHomePlatform...";
}

void SmartHomeDeployment::UeContextThreadTaskMainLoop()
{
	LOG(INFO) << "UeContextThreadTask main loop start";
	std::function<bool()> l_checker = std::bind(&SmartHomeDeployment::isUeContextEventObjectNotEmpty,this);
	while(m_deploymentShouldExit == false)
	{
		if(getUeContextEventObjectSize() <= 0)
		{
			m_UeContextEventQueueNotify.wait(l_checker);
		}
		else
		{
			auto l_event = detachUeContextEventObject();
			//TODO
		}
	}
	LOG(INFO) << "UeContextThreadTask main loop exit";

}

void SmartHomeDeployment::start()
{
	LOG(INFO) << "SmartHomeDeployment start";
	m_udpNetworkUnit->run();
	m_terminalUnit->run();

	LOG(INFO) << "SmartHomeDeployment main loop start";

	createUeContextThread(5);

	std::function<bool()> l_checker = std::bind(&SmartHomeDeployment::isTerminalEventObjectNotEmpty,this);
	while(m_deploymentShouldExit == false)
	{
		if(getTerminalEventObjectSize() <= 0)
		{
			m_terminalEventQueueNotify.wait(l_checker);
		}
		else
		{
			auto l_event = detachTerminalEventObject();
			// TODO
		}
	}
	LOG(INFO) << "SmartHomeDeployment main loop exit";
}

bool SmartHomeDeployment::shutdown(int p_signum)
{
	LOG(INFO) << "call SmartHomeDeployment shutdown, signum = " <<  p_signum;
	m_deploymentShouldExit = true;
	m_terminalEventQueueNotify.notifyAll();
	m_UeContextEventQueueNotify.notifyAll();
	return true;
}

void SmartHomeDeployment::addTerminalEventObject(std::shared_ptr<EventTypeDataObject> p_event)
{
	m_TerminalEventQueueMutex.lock();
	m_TerminalEventQueue.push_back(p_event);
	m_TerminalEventQueueMutex.unlock();
	m_terminalEventQueueNotify.notifyAll();
}

std::shared_ptr<EventTypeDataObject> SmartHomeDeployment::detachTerminalEventObject()
{
	std::shared_ptr<EventTypeDataObject> l_event;
	m_TerminalEventQueueMutex.lock();
	if(m_TerminalEventQueue.empty() == false)
	{
		l_event = m_TerminalEventQueue.front();
		m_TerminalEventQueue.erase(m_TerminalEventQueue.begin());
	}
	m_TerminalEventQueueMutex.unlock();
	return l_event;
}

int SmartHomeDeployment::getTerminalEventObjectSize()
{
	int l_queueSize = 0;
	m_TerminalEventQueueMutex.lock();
	l_queueSize = m_TerminalEventQueue.size();
	m_TerminalEventQueueMutex.unlock();
	return l_queueSize;
}

bool SmartHomeDeployment::isTerminalEventObjectNotEmpty()
{
	return getTerminalEventObjectSize() > 0;
}

bool SmartHomeDeployment::TerminalRawDataCallback(std::shared_ptr<EventTypeDataObject> p_event)
{
	addTerminalEventObject(p_event);
	LOG(INFO) << "add terminal raw event object to queue, current event number: " << getTerminalEventObjectSize();
	return true;
}

void SmartHomeDeployment::addUeContextEventObject(std::shared_ptr<EventTypeDataObject> p_event)
{
	m_UeContextEventQueueMutex.lock();
	m_UeContextEventQueue.push_back(p_event);
	m_UeContextEventQueueMutex.unlock();
	m_UeContextEventQueueNotify.notifyAll();
}

std::shared_ptr<EventTypeDataObject> SmartHomeDeployment::detachUeContextEventObject()
{
	std::shared_ptr<EventTypeDataObject> l_event;
	m_UeContextEventQueueMutex.lock();
	if(m_UeContextEventQueue.empty() == false)
	{
		l_event = m_UeContextEventQueue.front();
		m_UeContextEventQueue.erase(m_UeContextEventQueue.begin());
	}
	m_UeContextEventQueueMutex.unlock();
	return l_event;
}

int SmartHomeDeployment::getUeContextEventObjectSize()
{
	int l_queueSize = 0;
	m_UeContextEventQueueMutex.lock();
	l_queueSize = m_UeContextEventQueue.size();
	m_UeContextEventQueueMutex.unlock();
	return l_queueSize;
}

bool SmartHomeDeployment::isUeContextEventObjectNotEmpty()
{
	return getUeContextEventObjectSize() > 0;
}

bool SmartHomeDeployment::UeContextRawDataCallback(std::shared_ptr<EventTypeDataObject> p_event)
{
	addUeContextEventObject(p_event);
	LOG(INFO) << "add UDP raw event object to queue, current event number: " << getUeContextEventObjectSize();
	return true;
}

bool SmartHomeDeployment::createUeContextThread(int p_threadNumber)
{
	std::function<void()> l_UecontextThreadTaskObject =
			std::bind(&SmartHomeDeployment::UeContextThreadTaskMainLoop, this);
	for(auto i = 0 ; i < p_threadNumber ; i++)
	{
		std::thread l_thread(l_UecontextThreadTaskObject);
		m_UeContextThreadList.push_back(std::move(l_thread));
	}
	LOG(INFO) << "create UeContext thread " << p_threadNumber << " successfully";
	return true;
}


