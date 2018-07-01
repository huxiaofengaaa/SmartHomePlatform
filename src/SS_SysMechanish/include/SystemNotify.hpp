#pragma once

#include <pthread.h>
#include <functional>

class SystemNotify
{
public:
	SystemNotify();
	~SystemNotify();
	bool wait(std::function<bool()> p_conditionCheckHandler);
	bool notify();
	bool notifyAll();
private:

	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
};
