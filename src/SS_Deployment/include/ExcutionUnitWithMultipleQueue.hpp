#pragma once
#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <iterator>
#include "ExcutionUnit.hpp"
#include "DataQueue.hpp"
#include "SystemNotify.hpp"
#include "glog/logging.h"

template<class KeyType, class DataType>
class ExcutionUnitWithMultipleQueue: public ExcutionUnit, MultipleDataQueue<KeyType, DataType>
{
public:
	ExcutionUnitWithMultipleQueue(std::string p_ExcutionUnitName, int p_threadNumber, std::function<bool(DataType)> p_callback)
		: ExcutionUnit(p_ExcutionUnitName, p_threadNumber), m_callback(p_callback)
	{

	}
	bool addDataObject(KeyType p_key, DataType p_obj)
	{
		bool l_status = MultipleDataQueue<KeyType, DataType>::insertDataObject(p_key, p_obj);
		if(l_status == true)
		{
			notifyAll();
		}
		return l_status;
	}

private:
	void excutionMainLoop() override
	{
		//LOG(INFO) << "ExcutionUnit " << m_ExcutionUnitName << " main loop start";
		std::function<bool()> l_checker = std::bind(&ExcutionUnitWithMultipleQueue::isDataNotEmpty, this);
		while(shouldExcutionUnitExit() == false)
		{
			if(true == ExcutionUnitWait(l_checker))
			{
				auto l_eventObj = MultipleDataQueue<KeyType, DataType>::detachDataObject();
				m_callback(std::get<1>(l_eventObj));
				MultipleDataQueue<KeyType, DataType>::setKeyDeActive(std::get<0>(l_eventObj));
			}
		}
		//LOG(INFO) << "ExcutionUnit " << m_ExcutionUnitName << " main loop exit";
	}

	bool isDataNotEmpty()
	{
		return MultipleDataQueue<KeyType, DataType>::isDataObjectNotEmpty();
	}

	std::function<bool(DataType)> m_callback;
};
