/*
 * ExcutionUnit.hpp
 *
 *  Created on: Jul 2, 2018
 *      Author: xiaofenh
 */

#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include "SystemNotify.hpp"
#include "glog/logging.h"

template<class DataType>
class ExcutionUnit
{
public:
	ExcutionUnit(std::string p_ExcutionUnitName, int p_threadNumber, std::function<bool(DataType)> p_callback)
		: m_ExcutionUnitName(p_ExcutionUnitName),
		  m_threadNumber(p_threadNumber),
		  m_excutionUnitExitFlag(false),
		  m_callback(p_callback)
	{
		LOG(INFO) << "construct ExcutionUnit " << m_ExcutionUnitName;
	}
	virtual ~ExcutionUnit()
	{
		LOG(INFO) << "de-construct ExcutionUnit " << m_ExcutionUnitName;
	}

	bool startExcutionUnit()
	{
		LOG(INFO) << "start ExcutionUnit " << m_ExcutionUnitName;
		std::function<void()> l_excutionUnitTask = std::bind(&ExcutionUnit::excutionMainLoop, this);
		for(auto i = 0 ; i < m_threadNumber ; i++)
		{
			std::thread l_thread(l_excutionUnitTask);
			m_threadList.push_back(std::move(l_thread));
		}
		return true;
	}

	void shutdownExcutionUnit()
	{
		LOG(INFO) << "shutdown ExcutionUnit " << m_ExcutionUnitName;
		m_excutionUnitExitFlag = true;
		m_queueNotify.notifyAll();

		for(auto l_thread = m_threadList.begin() ; l_thread != m_threadList.end(); l_thread++)
		{
			(*l_thread).join();
		}
		m_threadList.clear();
	}

	bool addDataObject(DataType p_obj)
	{
		m_queueMutex.lock();
		m_dataQueue.push_back(p_obj);
		m_queueMutex.unlock();
		m_queueNotify.notifyAll();
		return true;
	}

	DataType detachDataObject()
	{
		DataType l_obj;
		m_queueMutex.lock();
		if(m_dataQueue.empty() == false)
		{
			l_obj = m_dataQueue.front();
			m_dataQueue.erase(m_dataQueue.begin());
		}
		m_queueMutex.unlock();
		return l_obj;
	}

	int getDataObjectSize()
	{
		int l_size = 0;
		m_queueMutex.lock();
		l_size = m_dataQueue.size();
		m_queueMutex.unlock();
		return l_size;
	}

	bool isDataObjectNotEmpty()
	{
		return getDataObjectSize() > 0;
	}
private:
	void excutionMainLoop()
	{
		LOG(INFO) << "ExcutionUnit " << m_ExcutionUnitName << " main loop start";
		std::function<bool()> l_checker = std::bind(&ExcutionUnit::isDataObjectNotEmpty, this);
		while(m_excutionUnitExitFlag == false)
		{
			if(true == m_queueNotify.wait(l_checker))
			{
				DataType l_eventObj = detachDataObject();
				m_callback(l_eventObj);
			}
		}
		LOG(INFO) << "ExcutionUnit " << m_ExcutionUnitName << " main loop exit";
	}

	std::vector<DataType> m_dataQueue;
	std::mutex m_queueMutex;
	SystemNotify m_queueNotify;
	std::vector<std::thread> m_threadList;
	const int m_threadNumber;
	bool m_excutionUnitExitFlag;

	std::function<bool(DataType)> m_callback;
	const std::string m_ExcutionUnitName;
};
