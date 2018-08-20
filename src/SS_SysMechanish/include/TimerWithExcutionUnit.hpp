#ifndef SRC_SS_SYSMECHANISH_INCLUDE_TIMERWITHEXCUTIONUNIT_HPP_
#define SRC_SS_SYSMECHANISH_INCLUDE_TIMERWITHEXCUTIONUNIT_HPP_

#include "ExcutionUnit.hpp"
#include <sys/time.h>
#include <vector>
#include <memory>
#include <mutex>

typedef struct
{
	std::string m_host;
	int m_port;
	time_t m_ses;
	suseconds_t m_usec;
	int absoluteTime;
}TimeNode;

//bool operator<(std::shared_ptr<TimeNode> p_first, std::shared_ptr<TimeNode> p_second)
//{
//
//	return false;
//}

class TimerWithExcutionUnit: public ExcutionUnit
{
public:
	TimerWithExcutionUnit(): ExcutionUnit("TimerWithExcutionUnit", 1)
	{

	}

private:
	void excutionMainLoop() override
	{

	}

	void insertNode(std::shared_ptr<TimeNode> m_newNode)
	{
		m_listMutex.lock();
		for(auto p_iter = m_timeNodeList.begin() ; p_iter != m_timeNodeList.end(); p_iter++)
		{

		}
		m_listMutex.unlock();
	}

	void delNode()
	{
		m_listMutex.lock();

		m_listMutex.unlock();
	}

	std::mutex m_listMutex;
	std::vector<std::shared_ptr<TimeNode>> m_timeNodeList;
};

#endif /* SRC_SS_SYSMECHANISH_INCLUDE_TIMERWITHEXCUTIONUNIT_HPP_ */
