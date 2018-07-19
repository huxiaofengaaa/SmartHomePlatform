#include "ExcutionUnitClient.hpp"
#include <unistd.h>
#include <string>
#include <string.h>
#include <stdio.h>

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
	char buffer[128] = { 0 };
	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "%s:%d", getLocalUDPHost().c_str(), getLocalUDPPort());
	m_deviceDataStore.storeDeviceIPAddr(std::string(buffer));
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
//		sleep(1);
	}
}
