#include "FsmManager.hpp"
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

FsmManager::FsmManager(): m_currentState(FsmManagerStates::STATE_REGISTER)
{
	m_heartbeatInterval = 15;
	m_lastSuccessHeartbeat = 0;

	m_pluginRetryMax = 5;
	m_pluginRetry = 0;
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

	case FsmManagerStates::STATE_PLUGIN_RESET:
		return "FsmManagerStates::STATE_PLUGIN_RESET";

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

void FsmManager::runFsmManager()
{
	printf("%s\n", getCurrentFsmState().c_str());
	switch(m_currentState)
	{
	case FsmManagerStates::STATE_REGISTER:
		if(true == deviceRegister())
		{
			m_currentState = FsmManagerStates::STATE_ONLINE;
		}
		else
		{
			printf("register failed\n");
			sleep(1);
		}
		break;
	case FsmManagerStates::STATE_ONLINE:
		if(true == deviceOnline())
		{
			m_currentState = FsmManagerStates::STATE_AUTH;
		}
		else
		{
			m_currentState = FsmManagerStates::STATE_REGISTER;
			printf("online failed\n");
		}
		break;
	case FsmManagerStates::STATE_AUTH:
		if(true == deviceAuth())
		{
			m_currentState = FsmManagerStates::STATE_HEARTBEAT;
		}
		else
		{
			m_currentState = FsmManagerStates::STATE_ONLINE;
			printf("auth failed\n");
		}
		break;
	case FsmManagerStates::STATE_HEARTBEAT:
		if(time(NULL) < (m_lastSuccessHeartbeat + m_heartbeatInterval))
		{
			const int l_resultNoAction = 0;
			const int l_resultReturnToBoot = -1;
			const int l_resultShouldPlugin = 1;
			long l_startTimeStamps = time(NULL);
			int l_downlinkResult = deviceUDPDownlinkAction();
			long l_endTimestamps = time(NULL);
			if(l_downlinkResult == l_resultNoAction)
			{
				if(l_endTimestamps < (l_startTimeStamps + 2))
				{
					sleep(1);
				}
			}
			else if(l_downlinkResult == l_resultShouldPlugin)
			{
				m_currentState = FsmManagerStates::STATE_PLUGIN;
			}
			else if(l_downlinkResult == l_resultReturnToBoot)
			{
				m_currentState = FsmManagerStates::STATE_ONLINE;
			}
		}
		else
		{
			if(true == deviceHeartbeat())
			{
				m_lastSuccessHeartbeat = time(NULL);
			}
			else
			{
				m_currentState = FsmManagerStates::STATE_ONLINE;
				printf("heartbeat failed\n");
			}
		}
		break;
	case FsmManagerStates::STATE_PLUGIN:
		if(true == devicePlugin())
		{
			m_currentState = FsmManagerStates::STATE_PLUGIN_ONLINE;
			m_pluginRetry = 0;
		}
		else
		{
			m_pluginRetry++;
			if(m_pluginRetry > m_pluginRetryMax)
			{
				m_pluginRetry = 0;
				m_currentState = FsmManagerStates::STATE_ONLINE;
				printf("plugin failed\n");
			}
			sleep(1);
		}
		break;
	case FsmManagerStates::STATE_PLUGIN_RESET:
		if(false == deviceDisconnect())
		{
			printf("disconnect failed\n");
		}
		m_currentState = FsmManagerStates::STATE_PLUGIN;
		break;
	case FsmManagerStates::STATE_PLUGIN_ONLINE:
		static int g_pluginOnlineRetry = 0;
		if(true == devicePluginOnline())
		{
			m_currentState = FsmManagerStates::STATE_PLUGIN_AUTH;
			g_pluginOnlineRetry = 0;
		}
		else
		{
			g_pluginOnlineRetry++;
			if(g_pluginOnlineRetry <= 10)
			{
				m_currentState = FsmManagerStates::STATE_PLUGIN_RESET;
			}
			else
			{
				m_currentState = FsmManagerStates::STATE_DISCONNECT;
				g_pluginOnlineRetry = 0;
			}
			printf("plugin online failed, %d\n", g_pluginOnlineRetry);

		}
		break;
	case FsmManagerStates::STATE_PLUGIN_AUTH:
		if(true == devicePluginAuth())
		{
			m_currentState = FsmManagerStates::STATE_PLUGIN_HEARTBEAT;
		}
		else
		{
			m_currentState = FsmManagerStates::STATE_PLUGIN_RESET;
			printf("plugin auth failed\n");
		}
		break;
	case FsmManagerStates::STATE_PLUGIN_HEARTBEAT:
		if(time(NULL) < (m_lastSuccessHeartbeat + m_heartbeatInterval))
		{
			long l_startTimeStamps = time(NULL);
			int l_downlinkResult = deviceTCPDownlinkAction();
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
				m_currentState = FsmManagerStates::STATE_DISCONNECT;
			}
		}
		else
		{
			if(true == devicePluginHeartbeat())
			{
				m_lastSuccessHeartbeat = time(NULL);
			}
			else
			{
				m_currentState = FsmManagerStates::STATE_PLUGIN_RESET;
				printf("plugin heartbeat failed\n");
			}
		}
		break;
	case FsmManagerStates::STATE_DISCONNECT:
		if(false == deviceDisconnect())
		{
			printf("disconnect failed\n");
		}
		m_currentState = FsmManagerStates::STATE_ONLINE;
		break;
	default:
		break;
	}
}


