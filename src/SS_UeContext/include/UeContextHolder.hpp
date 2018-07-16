/*
 * UeContextHolder.hpp
 *
 *  Created on: 2018Äê7ÔÂ4ÈÕ
 *      Author: Administrator
 */

#pragma once

#include <memory>
#include <mutex>
#include <map>
#include <vector>

template<class ContextType>
class UeContextHolder
{
public:
	UeContextHolder()
	{

	}

	virtual ~UeContextHolder() = default;

	void add(std::string p_name, ContextType p_obj)
	{
		m_contextMapMutex.lock();
		m_contextMap.erase(p_name);
		m_contextMap.insert({p_name, p_obj});
		m_contextMapMutex.unlock();
	}

	void del(std::string p_name)
	{
		m_contextMapMutex.lock();
		m_contextMap.erase(p_name);
		m_contextMapMutex.unlock();
	}

	bool isExist(std::string p_name)
	{
		bool l_result = false;
		m_contextMapMutex.lock();
		for(auto l_uecontext = m_contextMap.begin(); l_uecontext != m_contextMap.end() ; l_uecontext++)
		{
			if(p_name == l_uecontext->first)
			{
				l_result = true;
				break;
			}
		}
		m_contextMapMutex.unlock();
		return l_result;
	}

	std::vector<std::string> getAllKey()
	{
		std::vector<std::string> l_result;
		m_contextMapMutex.lock();
		for(auto l_uecontext = m_contextMap.begin(); l_uecontext != m_contextMap.end() ; l_uecontext++)
		{
			l_result.push_back(l_uecontext->first);
		}
		m_contextMapMutex.unlock();
		return l_result;
	}

	ContextType getRef(std::string p_name)
	{
		m_contextMapMutex.lock();
		auto l_Obj = m_contextMap.find(p_name);
		if(l_Obj != m_contextMap.end())
		{
			m_contextMapMutex.unlock();
			return l_Obj->second;
		}
		else
		{
			m_contextMapMutex.unlock();
			return ContextType();
		}
	}

	bool shutdown()
	{
		m_contextMapMutex.lock();
		m_contextMap.clear();
		m_contextMapMutex.unlock();
		return true;
	}

	int size()
	{
		int l_size = 0;
		m_contextMapMutex.lock();
		l_size = m_contextMap.size();
		m_contextMapMutex.unlock();
		return l_size;
	}

private:
	std::map<std::string, ContextType> m_contextMap;
	std::mutex m_contextMapMutex;
};
