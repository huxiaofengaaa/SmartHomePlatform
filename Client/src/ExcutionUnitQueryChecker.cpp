#include "ExcutionUnitClient.hpp"
#include "AndlinkDeviceQueryEvent.hpp"
#include <stdio.h>

std::string ExcutionUnitClient::deviceQueryChecker(std::string l_plainReq)
{
	printf("deviceQueryChecker running....\n");
	struct Interface56_CommonQuery_Req l_queryReq;

	std::string l_plainResp;
	if(true == resolveAndlinkDeviceApConfigInfoQueryReq(l_plainReq, &l_queryReq))
	{
		Interface56_ApConfigInfo_Resp resp;
		resp.ID = l_queryReq.ID;
		resp.deviceId = m_deviceDataStore.m_runTimeData.getDeviceID();
		resp.SyncCode = m_deviceDataStore.m_runTimeData.getSyncCode();
		resp.hardwareVersion = m_deviceDataStore.m_readOnlyData.getFirmWareVersion();
		resp.softwareVersion = m_deviceDataStore.m_readOnlyData.getSoftWareVersion();
		resp.WorkingMode = m_deviceDataStore.m_basicConfig.getWorkMode();
		resp.UpTime = m_deviceDataStore.m_runTimeData.getUpTime();
		resp.MacFilterEnable = m_deviceDataStore.m_basicConfig.getMacFilterEnable();
		resp.MacFilterPolicy = m_deviceDataStore.m_basicConfig.getMacFilterPolicy();
		resp.MacFilterEntries = m_deviceDataStore.m_basicConfig.getMacFilterEntries();
		resp.LEDOnOff = m_deviceDataStore.m_basicConfig.getLEDOnOff();
		resp.RoamingSwitch = m_deviceDataStore.m_basicConfig.getRoamingSwitch();
		resp.LowRSSI24G = m_deviceDataStore.m_basicConfig.getLowRSSI24G();
		resp.LowRSSI5G = m_deviceDataStore.m_basicConfig.getLowRSSI5G();

		int l_radioNumber = m_deviceDataStore.m_radioConfig.getRadioNumber();
		RadioInformation* l_radio = new RadioInformation[l_radioNumber];
		if(l_radio)
		{
			resp.Radios = l_radio;
			resp.RadioNumber = l_radioNumber;
			for(int i = 0 ; i < l_radioNumber ; i++)
			{
				l_radio[i].Radio = m_deviceDataStore.m_radioConfig.getRadioName(i);
				l_radio[i].Enable = m_deviceDataStore.m_radioConfig.getRadioEnable(i);
				l_radio[i].Channel = m_deviceDataStore.m_radioConfig.getRadioChannel(i);
				l_radio[i].TransmitPower = m_deviceDataStore.m_radioConfig.getRadioTransmitPower(i);
			}
		}
		else
		{
			resp.Radios = NULL;
			resp.RadioNumber = 0;
		}

		int l_radioConfigurationNumber =
				m_deviceDataStore.m_radioConfigurationList.getRadioConfigurationNumber();
		RadioConfigurations* l_radioConfiguration = new RadioConfigurations[l_radioConfigurationNumber];
		if(l_radioConfiguration)
		{
			resp.Configurations = l_radioConfiguration;
			resp.ConfigurationNumber = l_radioConfigurationNumber;
			for(int i = 0 ; i < l_radioConfigurationNumber ; i++)
			{
				l_radioConfiguration[i].Radio =
						m_deviceDataStore.m_radioConfigurationList.getRadioConfigName(i);
				l_radioConfiguration[i].Index =
						m_deviceDataStore.m_radioConfigurationList.getRadioConfigIndex(i);
				l_radioConfiguration[i].Enable =
						m_deviceDataStore.m_radioConfigurationList.getRadioConfigEnable(i);
				l_radioConfiguration[i].SSID =
						m_deviceDataStore.m_radioConfigurationList.getRadioConfigSSID(i);
				l_radioConfiguration[i].SecurityMode =
						m_deviceDataStore.m_radioConfigurationList.getRadioConfigSecurityMode(i);
				l_radioConfiguration[i].Pwd =
						m_deviceDataStore.m_radioConfigurationList.getRadioConfigPwd(i);
				l_radioConfiguration[i].MaxAssociateNum =
						m_deviceDataStore.m_radioConfigurationList.getRadioConfigMaxAssociateNum(i);
				l_radioConfiguration[i].SSIDAdvertisementEnabled =
						m_deviceDataStore.m_radioConfigurationList.getRadioConfigSSIDAdvertisementEnabled(i);
			}
		}
		else
		{
			resp.Configurations = NULL;
			resp.ConfigurationNumber = 0;
		}
		l_plainResp = buildAndlinkDeviceApConfigInfoQueryResp(resp);
		if(l_radio)
		{
			delete[] l_radio;
		}
		if(l_radioConfiguration)
		{
			delete[] l_radioConfiguration;
		}
	}
	else if(true == resolveAndlinkDeviceSTAInfoQueryReq(l_plainReq, &l_queryReq))
	{
		Interface56_STAInfo_Resp resp;
		resp.ID = l_queryReq.ID;
		resp.deviceId = m_deviceDataStore.m_runTimeData.getDeviceID();

		int l_staDeviceNumber = m_deviceDataStore.m_holdSTAinfo.getSTADeviceNumber();
		DeviceStatus* l_staDevice = new DeviceStatus[l_staDeviceNumber];
		if(l_staDevice)
		{
			resp.DevicesNumber = l_staDeviceNumber;
			resp.Devices = l_staDevice;
			for(int i = 0 ; i < l_staDeviceNumber ; i++)
			{
				l_staDevice[i].MacAddress = m_deviceDataStore.m_holdSTAinfo.getSTADeviceMacAddress(i);
				l_staDevice[i].VMacAddress = m_deviceDataStore.m_holdSTAinfo.getSTADeviceVMacAddress(i);
				l_staDevice[i].UpTime = m_deviceDataStore.m_holdSTAinfo.getSTADeviceUpTime(i);
				l_staDevice[i].Radio = m_deviceDataStore.m_holdSTAinfo.getSTADeviceRadio(i);
				l_staDevice[i].SSID = m_deviceDataStore.m_holdSTAinfo.getSTADeviceSSID(i);
				l_staDevice[i].RSSI = m_deviceDataStore.m_holdSTAinfo.getSTADeviceRSSI(i);
				l_staDevice[i].RxRate = m_deviceDataStore.m_holdSTAinfo.getSTADeviceRxRate(i);
				l_staDevice[i].TxRate = m_deviceDataStore.m_holdSTAinfo.getSTADeviceTxRate(i);
			}
		}
		else
		{
			resp.DevicesNumber = 0;
			resp.Devices = NULL;
		}
		l_plainResp = buildAndlinkDeviceSTAInfoQueryResp(resp);
		if(l_staDevice)
		{
			delete[] l_staDevice;
		}
	}
	else if(true == resolveAndlinkDeviceUplinkStatusQueryReq(l_plainReq, &l_queryReq))
	{
		Interface56_UplinkStatus_Resp resp;
		resp.ID = l_queryReq.ID;
		resp.deviceId = m_deviceDataStore.m_runTimeData.getDeviceID();
		resp.UplinkType = m_deviceDataStore.m_uplinkInterface.getUplinkType();
		resp.MacAddress = m_deviceDataStore.m_uplinkInterface.getUplinkMacAddress();
		resp.Radio = m_deviceDataStore.m_uplinkInterface.getUplinkRadio();
		resp.SSID = m_deviceDataStore.m_uplinkInterface.getUplinkSSID();
		resp.Channel = m_deviceDataStore.m_uplinkInterface.getUplinkChannel();
		resp.Noise = m_deviceDataStore.m_uplinkInterface.getUplinkNoise();
		resp.SNR = m_deviceDataStore.m_uplinkInterface.getUplinkSNR();
		resp.RSSI = m_deviceDataStore.m_uplinkInterface.getUplinkRSSI();
		resp.RxRate = m_deviceDataStore.m_uplinkInterface.getUplinkRxRate();
		resp.TxRate = m_deviceDataStore.m_uplinkInterface.getUplinkTxRate();

		l_plainResp = buildAndlinkDeviceUplinkStatusQueryResp(resp);
	}
	else if(true == resolveAndlinkDeviceWiFiStatsQueryReq(l_plainReq, &l_queryReq))
	{
		Interface56_WiFiStats_Resp resp;
		resp.ID = l_queryReq.ID;
		resp.deviceId = m_deviceDataStore.m_runTimeData.getDeviceID();

		int l_downlinkInterfaceNumber = m_deviceDataStore.m_downlinkInterface.getDownlinkInterfaceNumber();
		WiFiStats* l_downlinkInterface = new WiFiStats[l_downlinkInterfaceNumber];
		if(l_downlinkInterface)
		{
			resp.Status = l_downlinkInterface;
			resp.StatusNumber = l_downlinkInterfaceNumber;
			for(int i = 0 ; i < l_downlinkInterfaceNumber ; i++)
			{
				l_downlinkInterface[i].Radio =
						m_deviceDataStore.m_downlinkInterface.getDownlinkRadio(i);
				l_downlinkInterface[i].Index =
						m_deviceDataStore.m_downlinkInterface.getDownlinkIndex(i);
				l_downlinkInterface[i].SSID =
						m_deviceDataStore.m_downlinkInterface.getDownlinkSSID(i);
				l_downlinkInterface[i].TotalBytesSent =
						m_deviceDataStore.m_downlinkInterface.getDownlinkTotalBytesSent(i);
				l_downlinkInterface[i].TotalBytesReceived =
						m_deviceDataStore.m_downlinkInterface.getDownlinkTotalBytesReceived(i);
				l_downlinkInterface[i].TotalPacketsSent =
						m_deviceDataStore.m_downlinkInterface.getDownlinkTotalPacketsSent(i);
				l_downlinkInterface[i].TotalPacketsReceived =
						m_deviceDataStore.m_downlinkInterface.getDownlinkTotalPacketsReceived(i);
				l_downlinkInterface[i].ErrorsSent =
						m_deviceDataStore.m_downlinkInterface.getDownlinkErrorsSent(i);
				l_downlinkInterface[i].ErrorsReceived =
						m_deviceDataStore.m_downlinkInterface.getDownlinkErrorsReceived(i);
				l_downlinkInterface[i].DiscardPacketsSent =
						m_deviceDataStore.m_downlinkInterface.getDownlinkDiscardPacketsSent(i);
				l_downlinkInterface[i].DiscardPacketsReceived =
						m_deviceDataStore.m_downlinkInterface.getDownlinkDiscardPacketsReceived(i);
			}
		}
		else
		{
			resp.Status = NULL;
			resp.StatusNumber = 0;
		}
		l_plainResp = buildAndlinkDeviceWiFiStatsQueryResp(resp);
		if(l_downlinkInterface)
		{
			delete[] l_downlinkInterface;
		}
	}

	return l_plainResp;
}

