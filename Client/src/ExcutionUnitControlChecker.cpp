#include "ExcutionUnitClient.hpp"
#include "AndlinkDeviceControlEvent.hpp"
#include <stdio.h>
#include "AES.h"

std::string ExcutionUnitClient::deviceControlChecker(std::string l_plainReq)
{
	struct Interface56_LEDControl_Req led_control_req;
	struct Interface56_MacFilter_Req mac_filter_req;
	struct Interface56_RadioConfig_Req radio_config_req;
	struct Interface56_Reboot_Req reboot_req;
	struct Interface56_RoamingConfig_Req roaming_req;
	struct Interface56_WiFiParameterSync_Req wifi_parameter_req;
	struct Interface56_WiFiSwitch_Req wifi_switch_req;
	struct Interface56_WPS_Req wps_req;

	Interface56_ControlCommon_Resp resp;
	if(true == resolveAndlinkDeviceLEDControlReq(l_plainReq, &led_control_req))
	{
		resp.ID = led_control_req.ID;
		int l_LEDOnOff = led_control_req.LEDOnOff;
		if(true == m_deviceDataStore.m_basicConfig.setLEDOnOff(l_LEDOnOff))
		{
			resp.respCode = 1; // Success
		}
		else
		{
			resp.respCode = 0; // Received Success
		}
	}
	else if(true == resolveAndlinkDeviceMacFilterReq(l_plainReq, &mac_filter_req))
	{
		int l_macFilterEnable = mac_filter_req.MacFilterEnable;
		int l_macFilterPolicy = mac_filter_req.MacFilterPolicy;
		std::string l_macFilterEntry = mac_filter_req.MacFilterEntries;

		resp.ID = mac_filter_req.ID;
		if(true == m_deviceDataStore.m_basicConfig.setMacFilter(l_macFilterEnable,
				l_macFilterPolicy, l_macFilterEntry))
		{
			resp.respCode = 1;
		}
		else
		{
			resp.respCode = 0;
		}
	}
	else if(true == resolveAndlinkDeviceRadioConfigReq(l_plainReq, &radio_config_req))
	{
		std::string l_radio = radio_config_req.Radio;
		std::string l_TransmitPower = radio_config_req.TransmitPower;
		resp.ID = radio_config_req.ID;
		bool l_result = false;
		if(l_radio == "2.4G")
		{
			l_result = m_deviceDataStore.m_radioConfig.set24GTransmitPower(l_TransmitPower);
			resp.respCode = l_result == true ? 1: 0;
		}
		else if(l_radio == "5G")
		{
			l_result = m_deviceDataStore.m_radioConfig.set5GTransmitPower(l_TransmitPower);
			resp.respCode = l_result == true ? 1: 0;
		}
		else if(l_radio == "all")
		{
			l_result = m_deviceDataStore.m_radioConfig.set24GTransmitPower(l_TransmitPower) &&
					m_deviceDataStore.m_radioConfig.set5GTransmitPower(l_TransmitPower);
			resp.respCode = l_result == true ? 1: 0;
		}
		else
		{
			resp.respCode = 1003;
		}
	}
	else if(true == resolveAndlinkDeviceRebootReq(l_plainReq, &reboot_req))
	{
		std::string l_ControlType = reboot_req.ControlType;

		resp.ID = reboot_req.ID;
		bool l_result = false;
		if(l_ControlType == "Reboot")
		{
			l_result = m_deviceDataStore.m_basicConfig.triggerSystemReboot();
			resp.respCode = l_result == true ? 1: 0;
		}
		else if(l_ControlType == "2.4GChanReSelect")
		{
			l_result = m_deviceDataStore.m_radioConfig.set24GChannelReselect();
			resp.respCode = l_result == true ? 1: 0;
		}
		else if(l_ControlType == "5GChanReSelect")
		{
			l_result = m_deviceDataStore.m_radioConfig.set5GChannelReselect();
			resp.respCode = l_result == true ? 1: 0;
		}
		else
		{
			resp.respCode = 1003;
		}
	}
	else if(true == resolveAndlinkDeviceRoamingConfigReq(l_plainReq, &roaming_req))
	{
		int l_roamingSwitch = roaming_req.RoamingSwitch;
		int l_lowRSSI24G = roaming_req.LowRSSI24G;
		int l_lowRSSI5G = roaming_req.LowRSSI5G;

		resp.ID = roaming_req.ID;
		if(true == m_deviceDataStore.m_basicConfig.setRoaming(l_roamingSwitch,
				l_lowRSSI24G, l_lowRSSI5G))
		{
			resp.respCode = 1;
		}
		else
		{
			resp.respCode = 0;
		}
	}
	else if(true == resolveAndlinkDeviceWiFiParameterSyncReq(l_plainReq, &wifi_parameter_req))
	{
		resp.ID = wifi_parameter_req.ID;

		bool l_result = false;
		int l_configSize = wifi_parameter_req.configSize;
		if(l_configSize > 0 && wifi_parameter_req.Configurations)
		{
			for(int i = 0 ; i < l_configSize ; i++)
			{
				std::string l_Radio = wifi_parameter_req.Configurations[i].Radio;
				std::string l_Index = wifi_parameter_req.Configurations[i].Index;
				std::string l_Enable = wifi_parameter_req.Configurations[i].Enable;
				std::string l_SSID = wifi_parameter_req.Configurations[i].SSID;
				std::string l_SecurityMode = wifi_parameter_req.Configurations[i].SecurityMode;
				std::string l_Pwd = wifi_parameter_req.Configurations[i].Pwd;
				std::string l_MaxAssociateNum = wifi_parameter_req.Configurations[i].MaxAssociateNum;
				std::string l_SSIDAdvertisementEnabled =
						wifi_parameter_req.Configurations[i].SSIDAdvertisementEnabled;

				if(l_Radio == "2.4G")
				{
					l_result = m_deviceDataStore.m_radioConfig.sync24GWiFiParameter(l_Radio, l_Index,
							l_Enable, l_SSID, l_SecurityMode, l_Pwd, l_MaxAssociateNum,
							l_SSIDAdvertisementEnabled);
					if(l_result == false)
					{
						resp.respCode = 0;
						break;
					}
					resp.respCode = 1;
				}
				else if(l_Radio == "5G")
				{
					l_result = m_deviceDataStore.m_radioConfig.sync5GWiFiParameter(l_Radio, l_Index,
							l_Enable, l_SSID, l_SecurityMode, l_Pwd, l_MaxAssociateNum,
							l_SSIDAdvertisementEnabled);
					if(l_result == false)
					{
						resp.respCode = 0;
						break;
					}
					resp.respCode = 1;
				}
				else
				{
					resp.respCode = 1003;
					break;
				}
			}
		}
		else
		{
			resp.respCode = 0;
		}
	}
	else if(true == resolveAndlinkDeviceWiFiSwitchReq(l_plainReq, &wifi_switch_req))
	{
		std::string l_radio = wifi_switch_req.Radio;
		int l_enable = wifi_switch_req.Enable;

		resp.ID = wifi_switch_req.ID;
		bool l_result = false;
		if(l_radio == "2.4G")
		{
			l_result = m_deviceDataStore.m_radioConfig.set24GWiFiSwitch(l_enable);
			resp.respCode = l_result == true ? 1: 0;
		}
		else if(l_radio == "5G")
		{
			l_result = m_deviceDataStore.m_radioConfig.set5GWiFiSwitch(l_enable);
			resp.respCode = l_result == true ? 1: 0;
		}
		else if(l_radio == "all")
		{
			l_result = m_deviceDataStore.m_radioConfig.set24GWiFiSwitch(l_enable) &&
					m_deviceDataStore.m_radioConfig.set5GWiFiSwitch(l_enable);
			resp.respCode = l_result == true ? 1: 0;
		}
		else
		{
			resp.respCode = 1003;
		}
	}
	else if(true == resolveAndlinkDeviceWPSReq(l_plainReq, &wps_req))
	{
		std::string l_radio = wps_req.Radio;

		resp.ID = wps_req.ID;
		bool l_result = false;
		if(l_radio == "2.4G")
		{
			l_result = m_deviceDataStore.m_radioConfig.set24GWps();
			resp.respCode = l_result == true ? 1: 0;
		}
		else if(l_radio == "5G")
		{
			l_result = m_deviceDataStore.m_radioConfig.set5GWps();
			resp.respCode = l_result == true ? 1: 0;
		}
		else if(l_radio == "all")
		{
			l_result = m_deviceDataStore.m_radioConfig.set24GWps() &&
					m_deviceDataStore.m_radioConfig.set5GWps();
			resp.respCode = l_result == true ? 1: 0;
		}
		else
		{
			resp.respCode = 1003;
		}
	}
	else
	{
		return std::string();
	}

	std::string l_plainResp = buildAndlinkDeviceControlCommonResp(resp);
	if(l_plainResp.empty() == true)
	{
		return std::string();
	}
	return l_plainResp;
}


