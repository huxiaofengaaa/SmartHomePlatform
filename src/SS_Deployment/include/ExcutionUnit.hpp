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

template <class DataType>
class ExcutionUnit
{
public:
	ExcutionUnit(int p_threadNumber): m_threadNumber(p_threadNumber)
	{

	}
	virtual ~ExcutionUnit()
	{

	}

	bool start()
	{
		std::function<void()> l_excutionUnitTask = std::bind(&ExcutionUnit::mainloop, this);
		for(auto i = 0 ; i < m_threadNumber ; i++)
		{
			std::thread l_thread(l_excutionUnitTask);
			m_threadList.push_back(std::move(l_thread));
		}
		return true;
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

	void waitDataObject()
	{
		std::function<bool()> l_checker = std::bind(&ExcutionUnit::isDataObjectNotEmpty,this);
		m_queueNotify.wait(l_checker);
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

	virtual void mainloop() = 0;
private:
	std::vector<DataType> m_dataQueue;
	std::mutex m_queueMutex;
	SystemNotify m_queueNotify;
	std::vector<std::thread> m_threadList;
	const int m_threadNumber;
};
