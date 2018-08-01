#ifndef CLIENT_INCLUDE_DEVICEDATASTORE_HPP_
#define CLIENT_INCLUDE_DEVICEDATASTORE_HPP_

#include <string>
#include <tuple>
#include <stdio.h>
#include "DataBase.h"
#include "RandomGenerator.hpp"

class BasicConfigInterface
{
public:
	virtual ~BasicConfigInterface(){ }
	virtual void init() = 0;
};

class DeviceBasicConfig: public BasicConfigInterface
{
public:
	void init()
	{
		m_workMode = initWorkMode();
	}

	int initWorkMode(){ return 0; }

	std::tuple<int, int, std::string> getMacFilterStatus();
	std::tuple<int, int, int> getRoamingStatus();
	int getLEDOnOff();

	int getWorkMode() const{return m_workMode;}
	bool setLEDOnOff(int p_value){ return true; }

	bool triggerSystemReboot(){return true;}
	bool setMacFilter(int p_enable, int p_policy, std::string p_entry){return true;}
	bool setRoaming(int p_switch, int p_lowRssi24G, int p_lowRssi5G){return true;}

private:
	int m_workMode;
};

class DeviceDownlinkInterface: public BasicConfigInterface
{
public:
	void init() {}
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

class DeviceUplinkInterface: public BasicConfigInterface
{
public:
	void init() {}
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

class DeviceHoldSTAInfo: public BasicConfigInterface
{
public:
	void init() {}
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

class DeviceRadioConfigurationList: public BasicConfigInterface
{
public:
	void init() {}
	int getRadioConfigurationNumber();

	/*
	 * The function getRadioConfigurationStatus is used to obtain the radio configuration
	 * information; in the returned result is:
	 * 		RadioName, Index, enable, SSID, securityMode, Pwd, MaxAssociateNum, SSIDAdvertisementEnabled
	 * In general, the Index value is p_index + 1;
	 */
	std::tuple<std::string, int, int, std::string, std::string, std::string, int, int>
		getRadioConfigurationStatus(int p_index);
};

class DeviceRadioConfig: public BasicConfigInterface
{
public:
	void init() {}
	int getRadioNumber();
	/*
	 * The function getRadioStatus obtains the status information of the specified
	 * radio frequency index. The first one of the returned results is the radio frequency,
	 * The result may be 2.4G or 5G. the second is the radio frequency enable state,
	 * the third is the radio frequency power, and the fourth is the radio frequency channel.
	 */
	std::tuple<std::string, int, std::string, int> getRadioStatus(int p_index);

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

class DeviceRunTimeData: public BasicConfigInterface
{
public:
	void init()
	{
		m_SyncCode = initSyncCode();
		m_startupTimestamps = time(NULL);
		initDevRND();
	}
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
	long m_startupTimestamps;
};

class DeviceReadOnlyData: public BasicConfigInterface
{
public:
	void init()
	{
		m_deviceMAC = initDeviceMAC();
		m_deviceType = initDeviceType();
		m_productToken = initProductToken();
		m_deviceSn = initDeviceSn();
		m_firmwareVersion = initFirmWareVersion();
		m_softwareVersion = initSoftWareVersion();
		m_deviceVendor = initDeviceVendor();
		m_deviceModel = initDeviceModel();
	}

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
	std::string m_deviceMAC;
	std::string m_deviceType;
	std::string m_productToken;
	std::string m_deviceSn;
	std::string m_firmwareVersion;
	std::string m_softwareVersion;
	std::string m_deviceVendor;
	std::string m_deviceModel;
};

#ifdef CROSS_BUILD
#include "DataBase.h"
#endif

class DeviceDataStore
{
public:
	DeviceDataStore()
	{
#ifdef CROSS_BUILD
		if(0 != Client_database_init())
		{
			printf("Client_database_init failed\n");
		}
		check_andlink_configuration_file();
#endif
		m_readOnlyData.init();
		m_runTimeData.init();
		m_basicConfig.init();
		m_radioConfig.init();
		m_radioConfigurationList.init();
		m_holdSTAinfo.init();
		m_uplinkInterface.init();
		m_downlinkInterface.init();
	}
	DeviceReadOnlyData m_readOnlyData;
	DeviceRunTimeData m_runTimeData;
	DeviceBasicConfig m_basicConfig;
	DeviceRadioConfig m_radioConfig;
	DeviceRadioConfigurationList m_radioConfigurationList;
	DeviceHoldSTAInfo m_holdSTAinfo;
	DeviceUplinkInterface m_uplinkInterface;
	DeviceDownlinkInterface m_downlinkInterface;
};

#endif /* CLIENT_INCLUDE_DEVICEDATASTORE_HPP_ */
