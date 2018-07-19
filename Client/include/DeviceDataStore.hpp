#ifndef CLIENT_INCLUDE_DEVICEDATASTORE_HPP_
#define CLIENT_INCLUDE_DEVICEDATASTORE_HPP_

#include <string>
#include "RandomGenerator.hpp"

class DeviceBasicConfig
{
public:
	DeviceBasicConfig(): m_startupTimestamps(time(NULL))
	{

	}
	std::string getDeviceMAC()
	{
		if(m_deviceMAC.empty() == true)
		{
			RandomGenerator l_random;
			m_deviceMAC = l_random.generatorRandomNumberString(12);
		}
		return m_deviceMAC;
	}
	std::string getDeviceType()
	{
		if(m_deviceType.empty() == true)
		{
			RandomGenerator l_random;
			m_deviceType = l_random.generatorRandomNumberString(5);
		}
		return m_deviceType;
	}
	std::string getProductToken()
	{
		if(m_productToken.empty() == true)
		{
			RandomGenerator l_random;
			m_productToken = l_random.generatorRandomNumberString(32);
		}
		return m_productToken;
	}
	std::string getFirmWareVersion(){return "fhhw1.0.0";}
	std::string getSoftWareVersion(){return "fhsw1.0.0";}
	int getWorkMode(){ return 0;}
	std::string getUpTime();
	int getMacFilterEnable(){ return 0;}
	int getMacFilterPolicy(){ return 0;}
	std::string getMacFilterEntries() { return std::string();}
	int getLEDOnOff(){ return 0; }
	bool setLEDOnOff(int p_value){ return true; }
	int getRoamingSwitch(){ return 0; }
	int getLowRSSI24G(){ return 0;}
	int getLowRSSI5G(){ return 0;}

	bool triggerSystemReboot(){return true;}
	bool setMacFilter(int p_enable, int p_policy, std::string p_entry){return true;}
	bool setRoaming(int p_switch, int p_lowRssi24G, int p_lowRssi5G){return true;}
private:
	const long m_startupTimestamps;
	std::string m_deviceMAC;
	std::string m_deviceType;
	std::string m_productToken;
};

class DeviceDownlinkInterface
{
public:
	int getDownlinkInterfaceNumber(){ return 3;}
	std::string getDownlinkRadio(int index){ return std::string();}
	int getDownlinkIndex(int index){return 0;}
	std::string getDownlinkSSID(int index){ return std::string();}
	int getDownlinkTotalBytesSent(int index){return 0;}
	int getDownlinkTotalBytesReceived(int index){return 0;}
	int getDownlinkTotalPacketsSent(int index){return 0;}
	int getDownlinkTotalPacketsReceived(int index){return 0;}
	int getDownlinkErrorsSent(int index){return 0;}
	int getDownlinkErrorsReceived(int index){return 0;}
	int getDownlinkDiscardPacketsSent(int index){return 0;}
	int getDownlinkDiscardPacketsReceived(int index){return 0;}
};

class DeviceUplinkInterface
{
public:
	std::string getUplinkType(){ return std::string();}
	std::string getUplinkMacAddress(){ return std::string();}
	std::string getUplinkRadio(){ return std::string();}
	std::string getUplinkSSID(){ return std::string();}
	int getUplinkChannel(){ return 0;}
	std::string getUplinkNoise(){ return std::string();}
	std::string getUplinkSNR(){ return std::string();}
	std::string getUplinkRSSI(){ return std::string();}
	std::string getUplinkRxRate(){ return std::string();}
	std::string getUplinkTxRate(){ return std::string();}
};

class DeviceHoldSTAInfo
{
public:
	int getSTADeviceNumber(){ return 3;}
	std::string getSTADeviceMacAddress(int index){ return std::string();}
	std::string getSTADeviceVMacAddress(int index){ return std::string();}
	std::string getSTADeviceUpTime(int index){ return std::string();}
	std::string getSTADeviceRadio(int index){ return std::string();}
	std::string getSTADeviceSSID(int index){ return std::string();}
	std::string getSTADeviceRSSI(int index){ return std::string();}
	std::string getSTADeviceRxRate(int index){ return std::string();}
	std::string getSTADeviceTxRate(int index){ return std::string();}
};

class DeviceRadioConfigurationList
{
public:
	int getRadioConfigurationNumber(){ return 2;}
	std::string getRadioConfigName(int index){ return index == 0 ? "2.4G" : "5G";}
	int getRadioConfigIndex(int index){ return index;}
	int getRadioConfigEnable(int index){return 1;}
	std::string getRadioConfigSSID(int index){return std::string();}
	std::string getRadioConfigSecurityMode(int index){return std::string();}
	std::string getRadioConfigPwd(int index){return std::string();}
	int getRadioConfigMaxAssociateNum(int index){return 0;}
	int getRadioConfigSSIDAdvertisementEnabled(int index){return 0;}
};

class DeviceRadioConfig
{
public:
	int getRadioNumber(){ return 2;}
	std::string getRadioName(int index){ return index == 0 ? "2.4G" : "5G";}
	int getRadioEnable(int index) { return 0;}
	std::string getRadioTransmitPower(int index) { return std::string("100%");}
	int getRadioChannel(int index){ return 1;}

	bool set24GWps(){ return true;}
	bool set5GWps(){ return true;}
	bool set24GWiFiSwitch(int l_value){ return true;}
	bool set5GWiFiSwitch(int l_value){ return true;}
	bool set24GChannelReselect(){ return true;}
	bool set5GChannelReselect(){ return true;}
	bool set24GTransmitPower(std::string p_value){ return true;}
	bool set5GTransmitPower(std::string p_value){ return true;}
	bool sync24GWiFiParameter(std::string p_Radio, std::string p_Index, std::string p_Enable,
			std::string p_SSID, std::string p_SecurityMode, std::string p_Pwd,
			std::string p_MaxAssociateNum, std::string p_SSIDAdvertisementEnabled){ return true;}
	bool sync5GWiFiParameter(std::string p_Radio, std::string p_Index, std::string p_Enable,
			std::string p_SSID, std::string p_SecurityMode, std::string p_Pwd,
			std::string p_MaxAssociateNum, std::string p_SSIDAdvertisementEnabled){ return true;}
};

class DeviceDataStore
{
public:
	std::string getDeviceCheckSN()
	{
		RandomGenerator l_random;
		return l_random.generatorRandomNumberString(32);
	}
	int getSyncCode(){ return 0;}

	void storeDeviceGwToken(std::string p_value);
	void storeDeviceID(std::string p_value);
	void storeDeviceToken(std::string p_value);
	void storeDeviceAndlinkToken(std::string p_value);
	std::string getDeviceGwToken();
	std::string getDeviceID();
	std::string getDeviceToken();
	std::string getDeviceAndlinkToken();
	void storeDeviceIPAddr(std::string p_value);
	std::string getDeviceIPAddr();
	void storeEnctypt(int p_value);
	int getEnctypt();
	void storeChallengeCode(std::string p_value);
	std::string getChallengeCode();
	void storeHeartbeatInterval(int p_value);
	int getHeartbeatInterval();
	long getTimestamps();

	DeviceBasicConfig m_basicConfig;
	DeviceRadioConfig m_radioConfig;
	DeviceRadioConfigurationList m_radioConfigurationList;
	DeviceHoldSTAInfo m_holdSTAinfo;
	DeviceUplinkInterface m_uplinkInterface;
	DeviceDownlinkInterface m_downlinkInterface;

private:
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
