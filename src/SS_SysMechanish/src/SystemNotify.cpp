/**************************************************************************
 * FileName    : SystemNotify.cpp
 * Author      : huxiaofeng
 * Create Data : 2018-07-29
 * Revision    :
 * Description : An object used to implement notification syncchronization between multiple
 *               threads. For details, refer to the linux condition variable.
 * CopyRight   :
 * OtherInfo   : Instruction/Scenario for use:
 *               1) When thread A needs to detect a certain condition, it call the wait function;
 *                  if the condition is satisfied, the wait function return immediately; otherwise
 *                  the wait function blocks until the condition is satisfied.
 *               2) The conditional state can be set in another thread, we assume that thread B;
 *               3) After thread B set condition state, B should call function notify/notifyAll to
 *                  notify all threads that care about this state;
 * ModifyLog   :
 ***************************************************************************/
#include "SystemNotify.hpp"

SystemNotify::SystemNotify()
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_cond, NULL);
}

SystemNotify::~SystemNotify()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
}

bool SystemNotify::wait(std::function<bool()> p_conditionCheckHandler)
{
	pthread_mutex_lock(&m_mutex);
	if(p_conditionCheckHandler() == true)
	{
		pthread_mutex_unlock(&m_mutex);
		return true;
	}
	else
	{
		pthread_cond_wait(&m_cond, &m_mutex);
		int l_checkResult = false;
		if(p_conditionCheckHandler() == true)
		{
			l_checkResult = true;
		}
		pthread_mutex_unlock(&m_mutex);
		return l_checkResult;
	}
}


bool SystemNotify::notify()
{
	pthread_cond_signal(&m_cond);
	return true;
}

bool SystemNotify::notifyAll()
{
	pthread_cond_broadcast(&m_cond);
	return true;
}
