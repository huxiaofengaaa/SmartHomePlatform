/**************************************************************************
 * FileName    : SignalRegister.cpp
 * Author      : huxiaofeng
 * Create Data : 2018-07-29
 * Revision    :
 * Description : An object class used to register the signal operation behavior of
 *               the Linux system. For details, refer to the linux signal;
 * CopyRight   :
 * OtherInfo   : Instruction for use:
 *               1) use SignalRegister class to create an new object,
 *                  auto l_object = SignalRegister();
 *               2) call interface to obtain information
 * ModifyLog   :
 ***************************************************************************/
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
