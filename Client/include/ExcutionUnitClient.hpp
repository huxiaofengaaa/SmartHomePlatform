#ifndef CLIENT_INCLUDE_EXCUTIONUNITCLIENT_HPP_
#define CLIENT_INCLUDE_EXCUTIONUNITCLIENT_HPP_

#include "UDPClient.hpp"
#include "TCPClient.hpp"
#include "FsmManager.hpp"
#include "DeviceDataStore.hpp"
#include <thread>

class ExcutionUnitClient: public UDPClient, public TCPClient, public FsmManager
{
public:
	ExcutionUnitClient(std::string p_host, int p_port);
	~ExcutionUnitClient();
	void startup(bool p_asDeamonThread = false);
	void shutdown();
private:
	void mainloop();

	std::string chiperDecrypt(std::string p_chiperText);
	std::string plainEncrypt(std::string p_plainText);

	std::string deviceControlChecker(std::string p_req);
	std::string deviceQueryChecker(std::string p_req);

	bool deviceRegister(); // override
	bool deviceOnline(); // override
	bool deviceAuth(); // override
	bool deviceHeartbeat(); // override
	int deviceUDPDownlinkAction(); // override

	bool devicePlugin(); // override
	bool devicePluginOnline(); // override
	bool devicePluginAuth(); // override
	bool devicePluginHeartbeat(); // override
	bool deviceTCPDownlinkAction(); // override

	bool deviceDisconnect(); // override

	std::string getRegisterReq();
	std::string getOnlineReq();
	std::string getAuthReq();
	std::string getHeartbeatReq();
	bool checkerRegisterResp(std::string p_resp);
	bool checkerOnlineResp(std::string p_resp);
	bool checkerAuthResp(std::string p_resp);
	bool checkerHeartbeatResp(std::string p_resp);

	bool m_exitFlag;
	DeviceDataStore m_deviceDataStore;
	const std::string m_host;
	const int m_port;

	bool m_runAsDeamonThread;
	std::thread m_deamonThread;
};

#endif /* CLIENT_INCLUDE_EXCUTIONUNITCLIENT_HPP_ */
