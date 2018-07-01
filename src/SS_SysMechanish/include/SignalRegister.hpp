#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <utility>
#include <signal.h>

class SignalRegister
{
public:
	SignalRegister() = default;
	~SignalRegister() = default;
	bool registerSignal(int signum, std::function<bool(int)> p_signalCallback);
	bool deRegisterSignal(int signum);
	bool ignoreSignal(int signum);
};
