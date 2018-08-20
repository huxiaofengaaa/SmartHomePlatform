#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <iterator>
#include "DataQueue.hpp"
#include "SystemNotify.hpp"
#include "glog/logging.h"

class ExcutionUnit
{
public:
	ExcutionUnit(std::string p_ExcutionUnitName, int p_threadNumber)
		: m_ExcutionUnitName(p_ExcutionUnitName),
		  m_threadNumber(p_threadNumber),
		  m_excutionUnitExitFlag(false)
	{
		LOG(INFO) << "construct ExcutionUnit " << m_ExcutionUnitName << " " << m_threadNumber;
	}
	virtual ~ExcutionUnit()
	{
		LOG(INFO) << "de-construct ExcutionUnit " << m_ExcutionUnitName << " " << m_threadNumber;
	}

	bool startExcutionUnit()
	{
		std::function<void()> l_excutionUnitTask = std::bind(&ExcutionUnit::excutionMainLoop, this);
		for(auto i = 0 ; i < m_threadNumber ; i++)
		{
			std::thread l_thread(l_excutionUnitTask);
			m_threadList.push_back(std::move(l_thread));
		}
		LOG(INFO) << "start ExcutionUnit " << m_ExcutionUnitName << " " << m_threadNumber;
		return true;
	}
	void shutdownExcutionUnit()
	{
		m_excutionUnitExitFlag = true;
		m_queueNotify.notifyAll();

		for(auto l_thread = m_threadList.begin() ; l_thread != m_threadList.end(); l_thread++)
		{
			(*l_thread).join();
		}
		m_threadList.clear();
		LOG(INFO) << "shutdown ExcutionUnit " << m_ExcutionUnitName << " " << m_threadNumber;
	}
	void notifyAll()
	{
		m_queueNotify.notifyAll();
	}

protected:
	bool ExcutionUnitWait(std::function<bool()> p_checker)
	{
		return m_queueNotify.wait(p_checker);
	}

	bool shouldExcutionUnitExit()
	{
		return m_excutionUnitExitFlag;
	}

private:
	virtual void excutionMainLoop() = 0;

	SystemNotify m_queueNotify;
	std::vector<std::thread> m_threadList;
	const int m_threadNumber;
	bool m_excutionUnitExitFlag;

	const std::string m_ExcutionUnitName;
};

