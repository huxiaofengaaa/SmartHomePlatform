#ifndef CLIENT_INCLUDE_DEVICEDATASTORE_HPP_
#define CLIENT_INCLUDE_DEVICEDATASTORE_HPP_

#include <string>
#include "RandomGenerator.hpp"

class DeviceBasicConfig
{
public:
	DeviceBasicConfig():
		m_workMode(initWorkMode())
	{

	}

	int initWorkMode(){ return 0; }

	int getMacFilterEnable(){ return 0;}
	int getMacFilterPolicy(){ return 0;}
	int getLEDOnOff(){ return 0; }
	int getRoamingSwitch(){ return 0; }
	int getLowRSSI24G(){ return 0;}
	int getLowRSSI5G(){ return 0;}
	int getWorkMode() const{return m_workMode;}
	bool setLEDOnOff(int p_value){ return true; }
	std::string getMacFilterEntries() { return std::string();}

	bool triggerSystemReboot(){return true;}
	bool setMacFilter(int p_enable, int p_policy, std::string p_entry){return true;}
	bool setRoaming(int p_switch, int p_lowRssi24G, int p_lowRssi5G){return true;}

private:
	const int m_workMode;
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
	std::string getUplinkType(){ return std::string("Ethernet");}
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
	int get5GSupport(){ return 1;}

	bool set24GWps(){ return true;}
	bool set5GWps(){ return true;}
	bool set24GWiFiSwitch(int l_value){ return true;}
	bool set5GWiFiSwitch(int l_value){ return true;}
	bool set24GChannelReselect(){ return true;}
	bool set5GChannelReselect(){ return true;}
	bool set24GTransmitPower(std::string p_value){ return true;}
	bool set5GTransmitPower(std::string p_value){ return true;}
	bool sync24GWiFiParameter(
			std::string p_Radio, std::string p_Index, std::string p_Enable,
			std::string p_SSID, std::string p_SecurityMode, std::string p_Pwd,
			std::string p_MaxAssociateNum, std::string p_SSIDAdvertisementEnabled)
	{
		return true;
	}
	bool sync5GWiFiParameter(
			std::string p_Radio, std::string p_Index, std::string p_Enable,
			std::string p_SSID, std::string p_SecurityMode, std::string p_Pwd,
			std::string p_MaxAssociateNum, std::string p_SSIDAdvertisementEnabled)
	{
		return true;
	}
};

class DeviceRunTimeData
{
public:
	DeviceRunTimeData();

	void initDevRND();
	std::string initSyncCode(){ return "0";}

	std::string getDevRND() const;
	std::string getLastPluginKey() const;
	std::string getDeviceGwToken();
	std::string getDeviceID();
	std::string getDeviceToken();
	std::string getDeviceAndlinkToken();
	std::string getChallengeCode();
	std::string getSyncCode(){ return m_SyncCode;}
	std::string getUserKey();
	std::string getDeviceIPAddr();
	std::string getUpTime();
	long getTimestamps();
	int getHeartbeatInterval();
	int getEnctypt();

	void storePluginKey(std::string p_value);
	void storeDeviceGwToken(std::string p_value);
	void storeDeviceID(std::string p_value);
	void storeDeviceToken(std::string p_value);
	void storeDeviceAndlinkToken(std::string p_value);
	void storeHeartbeatInterval(int p_value);
	void storeEnctypt(int p_value);
	void storeChallengeCode(std::string p_value);
	bool storeSyncCode(std::string p_value){ return true;}
	void storeDeviceIPAddr(std::string p_value);

private:
	std::string m_gwToken;        // Used for upstream message encryption key
	std::string m_deviceID;
	std::string m_deviceToken;    // Used for downstream message encryption key
	std::string m_andlinkToken;
	std::string m_DevRND;
	std::string m_lastPluginKey;
	int m_heartbeatInterval;
	int m_encrypt;
	std::string m_ChallengeCode;
	std::string m_SyncCode;
	std::string m_UserKey;
	std::string m_deviceIPAddr;
	const long m_startupTimestamps;
};

class DeviceReadOnlyData
{
public:
	DeviceReadOnlyData();

	std::string initDeviceMAC() const;
	std::string initDeviceType() const;
	std::string initProductToken() const;
	std::string initDeviceSn() const;
	std::string initFirmWareVersion() const;
	std::string initSoftWareVersion() const;
	std::string initDeviceVendor() const;
	std::string initDeviceModel() const;

	std::string getDeviceMAC() const;
	std::string getDeviceType() const;
	std::string getProductToken() const;
	std::string getDeviceSn() const;
	std::string getFirmWareVersion() const;
	std::string getSoftWareVersion() const;
	std::string getDeviceVendor() const;
	std::string getDeviceModel() const;
private:
	const std::string m_deviceMAC;
	const std::string m_deviceType;
	const std::string m_productToken;
	const std::string m_deviceSn;
	const std::string m_firmwareVersion;
	const std::string m_softwareVersion;
	const std::string m_deviceVendor;
	const std::string m_deviceModel;
};

#ifdef CROSS_BUILD

#include "DataBase.h"

class DataBase
{
public:
	DataBase()
	{
		Client_database_init();
	}
	virtual ~DataBase()
	{

	}
};
#endif

class DeviceDataStore
{
public:
	DeviceReadOnlyData m_readOnlyData;
	DeviceRunTimeData m_runTimeData;
	DeviceBasicConfig m_basicConfig;
	DeviceRadioConfig m_radioConfig;
	DeviceRadioConfigurationList m_radioConfigurationList;
	DeviceHoldSTAInfo m_holdSTAinfo;
	DeviceUplinkInterface m_uplinkInterface;
	DeviceDownlinkInterface m_downlinkInterface;
#ifdef CROSS_BUILD
	DataBase m_database;
#endif
};

#endif /* CLIENT_INCLUDE_DEVICEDATASTORE_HPP_ */
