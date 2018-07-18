#ifndef CLIENT_INCLUDE_DEVICEDATASTORE_HPP_
#define CLIENT_INCLUDE_DEVICEDATASTORE_HPP_

#include <string>

class DeviceDataStore
{
public:
	std::string getDeviceMAC();
	std::string getDeviceType();
	std::string getProductToken();
	long getTimestamps();

	void storeDeviceGwToken(std::string p_value);
	void storeDeviceID(std::string p_value);
	void storeDeviceToken(std::string p_value);
	void storeDeviceAndlinkToken(std::string p_value);

	std::string getDeviceGwToken();
	std::string getDeviceID();
	std::string getDeviceToken();
	std::string getDeviceAndlinkToken();

	std::string getFirmWareVersion();
	std::string getSoftWareVersion();

	void storeDeviceIPAddr(std::string p_value);
	std::string getDeviceIPAddr();

	void storeEnctypt(int p_value);
	int getEnctypt();

	void storeChallengeCode(std::string p_value);
	std::string getChallengeCode();

	std::string getDeviceCheckSN();

	void storeHeartbeatInterval(int p_value);
	int getHeartbeatInterval();

private:
	std::string m_deviceMAC;
	std::string m_deviceType;
	std::string m_productToken;

	std::string m_gwToken;
	std::string m_deviceID;
	std::string m_deviceToken;
	std::string m_andlinkToken;

	std::string m_deviceIPAddr;

	int m_encrypt;
	std::string m_ChallengeCode;

	int m_heartbeatInterval;

};



#endif /* CLIENT_INCLUDE_DEVICEDATASTORE_HPP_ */
