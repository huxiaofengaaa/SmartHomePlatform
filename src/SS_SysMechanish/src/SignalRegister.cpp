#include "SignalRegister.hpp"

std::map<int, std::function<bool(int)>> g_signalCallbackMap;
std::mutex g_signalMapMutex;

static void globalSignalHandler(int signum)
{
	for(auto l_handler = g_signalCallbackMap.begin() ; l_handler != g_signalCallbackMap.end() ; l_handler++)
	{
		if(l_handler->first == signum)
		{
			std::function<bool(int)> l_callback = l_handler->second;
			l_callback(signum);
			break;
		}
	}

}

bool SignalRegister::registerSignal(int p_signum, std::function<bool(int)> p_signalCallback)
{
	deRegisterSignal(p_signum);
	g_signalMapMutex.lock();
	signal(p_signum, globalSignalHandler);
	g_signalCallbackMap.insert(std::pair<int, std::function<bool(int)>>(p_signum, p_signalCallback));
	g_signalMapMutex.unlock();
	return true;
}

bool SignalRegister::deRegisterSignal(int signum)
{
	g_signalMapMutex.lock();
	for(auto l_handler = g_signalCallbackMap.begin() ; l_handler != g_signalCallbackMap.end() ; l_handler++)
	{
		if(l_handler->first == signum)
		{
			g_signalCallbackMap.erase(l_handler);
			break;
		}
	}
	signal(signum, SIG_DFL);
	g_signalMapMutex.unlock();
	return true;
}

bool SignalRegister::ignoreSignal(int p_signum)
{
	deRegisterSignal(p_signum);
	signal(p_signum, SIG_IGN);
	return true;
}
