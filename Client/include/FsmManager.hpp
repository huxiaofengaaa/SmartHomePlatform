#ifndef CLIENT_INCLUDE_FSMMANAGER_HPP_
#define CLIENT_INCLUDE_FSMMANAGER_HPP_

#include <string>

enum class FsmManagerStates
{
	STATE_REGISTER,
	STATE_ONLINE,
	STATE_AUTH,
	STATE_HEARTBEAT,
	STATE_PLUGIN,
	STATE_PLUGIN_RESET,
	STATE_PLUGIN_ONLINE,
	STATE_PLUGIN_AUTH,
	STATE_PLUGIN_HEARTBEAT,
	STATE_DISCONNECT
};

class FsmManager
{
public:
	FsmManager();
	virtual ~FsmManager();
	void runFsmManager();
	void setHeartbeatIntervalValue(int p_interval);
	std::string getCurrentFsmState();

private:
	virtual bool deviceRegister() = 0;
	virtual bool deviceOnline() = 0;
	virtual bool deviceAuth() = 0;
	virtual bool deviceHeartbeat() = 0;
	virtual bool devicePlugin() = 0;
	virtual bool devicePluginOnline() = 0;
	virtual bool devicePluginAuth() = 0;
	virtual bool devicePluginHeartbeat() = 0;
	virtual bool deviceDisconnect() = 0;
	virtual int deviceUDPDownlinkAction() = 0;
	virtual bool deviceTCPDownlinkAction() = 0;

	FsmManagerStates m_currentState;
	int m_heartbeatInterval;
	long m_lastSuccessHeartbeat;

	int m_pluginRetryMax;
	int m_pluginRetry;
};

#endif /* CLIENT_INCLUDE_FSMMANAGER_HPP_ */
