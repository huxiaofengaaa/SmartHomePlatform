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
