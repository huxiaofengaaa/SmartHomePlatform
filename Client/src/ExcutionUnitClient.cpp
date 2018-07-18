#include "ExcutionUnitClient.hpp"
#include <unistd.h>
#include <string>

ExcutionUnitClient::ExcutionUnitClient():
	UDPClient(ANDLINK_SERVER_HOST, ANDLINK_SERVER_PORT),
	TCPClient(ANDLINK_SERVER_HOST, ANDLINK_SERVER_PORT+1),
	FsmManager(),
	m_exitFlag(false)
{

}

ExcutionUnitClient::~ExcutionUnitClient()
{

}

void ExcutionUnitClient::startup()
{
	startUDPClient();
	m_deviceDataStore.storeDeviceIPAddr(
			getLocalUDPHost() + ":" + std::to_string(getLocalUDPPort()));
	mainloop();
}

void ExcutionUnitClient::shutdown()
{

}

void ExcutionUnitClient::mainloop()
{
	while(m_exitFlag == false)
	{
		runFsmManager();
		sleep(1);
	}
}
