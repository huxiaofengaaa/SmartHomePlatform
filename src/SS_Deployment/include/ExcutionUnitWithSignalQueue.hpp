#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <iterator>
#include "DataQueue.hpp"
#include "ExcutionUnit.hpp"

template<class DataType>
class ExcutionUnitWithSignalQueue: public ExcutionUnit, public SignalDataQueue<DataType>
{
public:
	ExcutionUnitWithSignalQueue(std::string p_ExcutionUnitName, int p_threadNumber, std::function<bool(DataType)> p_callback)
		: ExcutionUnit(p_ExcutionUnitName, p_threadNumber), m_callback(p_callback)
	{

	}
	bool addDataObject(DataType p_obj)
	{
		bool l_status = SignalDataQueue<DataType>::insertDataObject(p_obj);
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
		std::function<bool()> l_checker = std::bind(&ExcutionUnitWithSignalQueue::isDataObjectNotEmpty, this);
		while(shouldExcutionUnitExit() == false)
		{
			if(true == ExcutionUnitWait(l_checker))
			{
				auto l_eventObj = SignalDataQueue<DataType>::detachDataObject();
				m_callback(std::get<1>(l_eventObj));
			}
		}
		//LOG(INFO) << "ExcutionUnit " << m_ExcutionUnitName << " main loop exit";
	}

	std::function<bool(DataType)> m_callback;
};
