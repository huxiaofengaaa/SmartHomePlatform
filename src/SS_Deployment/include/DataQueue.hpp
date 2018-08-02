#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <map>
#include <tuple>
#include <algorithm>
#include <iterator>

template<class KeyType, class DataType>
class DataQueueInterface
{
public:
	virtual ~DataQueueInterface(){}
	virtual bool insertDataObject(KeyType p_key, DataType p_obj) = 0;
	virtual std::tuple<KeyType, DataType> detachDataObject() = 0;
	virtual int getDataObjectSize() = 0;
	virtual bool isDataObjectNotEmpty() = 0;
	virtual void setKeyDeActive(KeyType p_key) = 0;
};

template<class KeyType, class DataType>
class _SignalDataQueue: public DataQueueInterface<int, DataType>
{
public:
	bool insertDataObject(KeyType p_key, DataType p_obj) override
	{
		return insertDataObject(p_obj);
	}

	bool insertDataObject(DataType p_obj)
	{
		m_queueMutex.lock();
		m_dataQueue.push_back(p_obj);
		m_queueMutex.unlock();
		return true;
	}

	std::tuple<KeyType, DataType> detachDataObject() override
	{
		DataType l_obj;
		m_queueMutex.lock();
		if(m_dataQueue.empty() == false)
		{
			l_obj = m_dataQueue.front();
			m_dataQueue.erase(m_dataQueue.begin());
		}
		m_queueMutex.unlock();
		return {1, l_obj};
	}

	int getDataObjectSize() override
	{
		int l_size = 0;
		m_queueMutex.lock();
		l_size = m_dataQueue.size();
		m_queueMutex.unlock();
		return l_size;
	}

	bool isDataObjectNotEmpty() override
	{
		return getDataObjectSize() > 0;
	}

	void setKeyDeActive(KeyType p_key) override
	{

	}
private:
	std::vector<DataType> m_dataQueue;
	std::mutex m_queueMutex;
};

template<class DataType>
class SignalDataQueue: public _SignalDataQueue<int, DataType>
{

};


template<class KeyType, class DataType>
class MultipleDataQueue: public DataQueueInterface<KeyType, DataType>
{
public:
	bool insertDataObject(KeyType p_key, DataType p_obj) override
	{
		m_queueMutex.lock();
		auto l_queueOfKey = m_multipleQueue.find(p_key);
		if(l_queueOfKey != m_multipleQueue.end())
		{
			l_queueOfKey->second.push_back(p_obj);
		}
		else
		{
			std::vector<DataType> l_queue;
			l_queue.push_back(p_obj);
			m_multipleQueue.emplace(p_key, l_queue);
		}
		m_queueMutex.unlock();
		return true;
	}

	std::tuple<KeyType, DataType> detachDataObject() override
	{
		KeyType l_key;
		DataType l_obj;
		m_queueMutex.lock();
		for(auto iter = m_multipleQueue.begin() ; iter != m_multipleQueue.end() ; iter++)
		{
			l_key = iter->first;
			if(false == isKeyActive(l_key) && false == iter->second.empty())
			{
				l_obj = iter->second.front();
				iter->second.erase(iter->second.begin());
				setKeyActive(l_key);
			}
		}
		m_queueMutex.unlock();
		return {l_key, l_obj};
	}

	int getDataObjectSize() override
	{
		int l_size = 0;
		m_queueMutex.lock();
		for(auto iter = m_multipleQueue.begin() ; iter != m_multipleQueue.end() ; iter++)
		{
			KeyType l_key = iter->first;
			if(false == isKeyActive(l_key) && false == iter->second.empty())
			{
				l_size += iter->second.size();
			}
		}
		m_queueMutex.unlock();
		return l_size;
	}

	bool isDataObjectNotEmpty() override
	{
		return getDataObjectSize() > 0;
	}

	void setKeyDeActive(KeyType p_key) override
	{
		m_queueMutex.lock();
		for(auto iter = m_activeKey.begin(); iter != m_activeKey.end(); iter++)
		{
			if(*iter == p_key)
			{
				m_activeKey.erase(iter);
				break;
			}
		}
		m_queueMutex.unlock();
	}

private:
	bool isKeyActive(KeyType p_key)
	{
		auto l_search = std::find(std::begin(m_activeKey), std::end(m_activeKey), p_key);
		if(l_search != std::end(m_activeKey))
		{
			return true;
		}
		return false;
	}
	void setKeyActive(KeyType p_key)
	{
		if(false == isKeyActive(p_key))
		{
			m_activeKey.push_back(p_key);
		}
	}

	std::vector<KeyType> m_activeKey;
	std::map<KeyType, std::vector<DataType>> m_multipleQueue;
	std::mutex m_queueMutex;
};
