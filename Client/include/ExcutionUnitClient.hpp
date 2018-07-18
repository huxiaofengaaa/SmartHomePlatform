#ifndef CLIENT_INCLUDE_EXCUTIONUNITCLIENT_HPP_
#define CLIENT_INCLUDE_EXCUTIONUNITCLIENT_HPP_

#include "UDPClient.hpp"
#include "TCPClient.hpp"
#include "FsmManager.hpp"
#include "DeviceDataStore.hpp"

#define ANDLINK_SERVER_HOST	"10.96.17.50"
#define ANDLINK_SERVER_PORT	6887

class ExcutionUnitClient: public UDPClient, public TCPClient, public FsmManager
{
public:
	ExcutionUnitClient();
	~ExcutionUnitClient();
	void startup();
	void shutdown();
private:
	void mainloop();

	bool deviceRegister() override;
	bool deviceOnline() override;
	bool deviceAuth() override;
	bool deviceHeartbeat() override;
	bool devicePlugin() override;
	bool devicePluginOnline() override;
	bool devicePluginAuth() override;
	bool devicePluginHeartbeat() override;
	bool deviceDisconnect() override;

	bool m_exitFlag;
	DeviceDataStore m_deviceDataStore;
};

#endif /* CLIENT_INCLUDE_EXCUTIONUNITCLIENT_HPP_ */