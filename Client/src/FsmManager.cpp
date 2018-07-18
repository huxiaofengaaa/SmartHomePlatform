#include "FsmManager.hpp"
#include <time.h>
#include <iostream>
#include <unistd.h>

FsmManager::FsmManager(): m_currentState(FsmManagerStates::STATE_REGISTER)
{
	m_heartbeatInterval = 15;
	m_lastSuccessHeartbeat = 0;
}

FsmManager::~FsmManager()
{

}

std::string FsmManager::getCurrentFsmState()
{
	switch(m_currentState)
	{
	case FsmManagerStates::STATE_REGISTER:
		return "FsmManagerStates::STATE_REGISTER";

	case FsmManagerStates::STATE_ONLINE:
		return "FsmManagerStates::STATE_ONLINE";

	case FsmManagerStates::STATE_AUTH:
		return "FsmManagerStates::STATE_AUTH";

	case FsmManagerStates::STATE_HEARTBEAT:
		return "FsmManagerStates::STATE_HEARTBEAT";

	case FsmManagerStates::STATE_PLUGIN:
		return "FsmManagerStates::STATE_PLUGIN";

	case FsmManagerStates::STATE_PLUGIN_ONLINE:
		return "FsmManagerStates::STATE_PLUGIN_ONLINE";

	case FsmManagerStates::STATE_PLUGIN_AUTH:
		return "FsmManagerStates::STATE_PLUGIN_AUTH";

	case FsmManagerStates::STATE_PLUGIN_HEARTBEAT:
		return "FsmManagerStates::STATE_PLUGIN_HEARTBEAT";

	case FsmManagerStates::STATE_DISCONNECT:
		return "FsmManagerStates::STATE_DISCONNECT";

	default:
		return "FsmManagerStates::Unknown";
	}
	return "FsmManagerStates::CodeError";
}

void FsmManager::setHeartbeatIntervalValue(int p_interval)
{
	m_heartbeatInterval = p_interval;
}

bool FsmManager::runFsmManager()
{
	switch(m_currentState)
	{
	case FsmManagerStates::STATE_REGISTER:
		if(true == deviceRegister())
		{
			m_currentState = FsmManagerStates::STATE_ONLINE;
			return true;
		}
		return false;
	case FsmManagerStates::STATE_ONLINE:
		if(true == deviceOnline())
		{
			m_currentState = FsmManagerStates::STATE_AUTH;
			return true;
		}
		m_currentState = FsmManagerStates::STATE_REGISTER;
		return false;
	case FsmManagerStates::STATE_AUTH:
		if(true == deviceAuth())
		{
			m_currentState = FsmManagerStates::STATE_HEARTBEAT;
			return true;
		}
		m_currentState = FsmManagerStates::STATE_ONLINE;
		return false;
	case FsmManagerStates::STATE_HEARTBEAT:
		if(time(NULL) < (m_lastSuccessHeartbeat + m_heartbeatInterval))
		{
			long l_startTimeStamps = time(NULL);
			int l_downlinkResult = deviceUDPDownlinkAction();
			long l_endTimestamps = time(NULL);
			if(l_downlinkResult == false)
			{
				if(l_endTimestamps < (l_startTimeStamps + 2))
				{
					sleep(1);
				}
			}
			else
			{
				m_currentState = FsmManagerStates::STATE_PLUGIN;
			}
			return l_downlinkResult;
		}
		if(true == deviceHeartbeat())
		{
			m_lastSuccessHeartbeat = time(NULL);
			return true;
		}
		m_currentState = FsmManagerStates::STATE_ONLINE;
		return false;
	case FsmManagerStates::STATE_PLUGIN:
		if(true == devicePlugin())
		{
			m_currentState = FsmManagerStates::STATE_PLUGIN_ONLINE;
			return true;
		}
		m_currentState = FsmManagerStates::STATE_ONLINE;
		return false;
	case FsmManagerStates::STATE_PLUGIN_ONLINE:
		if(true == devicePluginOnline())
		{
			m_currentState = FsmManagerStates::STATE_PLUGIN_AUTH;
			return true;
		}
		m_currentState = FsmManagerStates::STATE_PLUGIN;
		return false;
	case FsmManagerStates::STATE_PLUGIN_AUTH:
		if(true == devicePluginAuth())
		{
			m_currentState = FsmManagerStates::STATE_PLUGIN_HEARTBEAT;
			return true;
		}
		m_currentState = FsmManagerStates::STATE_PLUGIN_ONLINE;
		return false;
	case FsmManagerStates::STATE_PLUGIN_HEARTBEAT:
		if(time(NULL) < (m_lastSuccessHeartbeat + m_heartbeatInterval))
		{
			long l_startTimeStamps = time(NULL);
			int l_downlinkResult = deviceTCPDownlinkAction();
			long l_endTimestamps = time(NULL);
			if(l_endTimestamps < (l_startTimeStamps + 2))
			{
				sleep(1);
			}
			return l_downlinkResult;
		}
		if(true == devicePluginHeartbeat())
		{
			m_lastSuccessHeartbeat = time(NULL);
			return true;
		}
		m_currentState = FsmManagerStates::STATE_PLUGIN_ONLINE;
		return false;
	case FsmManagerStates::STATE_DISCONNECT:
		if(true == deviceDisconnect())
		{
			m_currentState = FsmManagerStates::STATE_ONLINE;
			return true;
		}
		m_currentState = FsmManagerStates::STATE_ONLINE;
		return false;
	default:
		break;
	}
	return false;
}


