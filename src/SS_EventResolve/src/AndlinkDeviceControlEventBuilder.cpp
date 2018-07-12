/*
 * AndlinkDeviceControlEventBuilder.cpp
 *
 *  Created on: 2018Äê7ÔÂ12ÈÕ
 *      Author: Administrator
 */
#include "AndlinkDeviceEventBuilder.hpp"


std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildLEDControlRequest(
		std::string p_deviceid, bool p_turnOn)
{
	struct Interface56_LEDControl_Req l_LEDControlReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_LEDControlReq.ID = ID;
	l_LEDControlReq.deviceId = p_deviceid;
	l_LEDControlReq.LEDOnOff = p_turnOn == true ? 1: 0;
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceLEDControlReq(l_LEDControlReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildMacFilterRequest(
		std::string p_deviceid, bool p_enable, int p_policy, std::string p_entry)
{
	struct Interface56_MacFilter_Req l_macFilterReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_macFilterReq.ID = ID;
	l_macFilterReq.deviceId = p_deviceid;
	l_macFilterReq.MacFilterEnable = p_enable == true ? 1 : 0;
	l_macFilterReq.MacFilterPolicy = p_policy == 0 ? 0 : 1;
	l_macFilterReq.MacFilterEntries = p_entry;
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceMacFilterReq(l_macFilterReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildRadioPowerRequest(
		std::string p_deviceid, int p_select, int p_percent)
{
	struct Interface56_RadioConfig_Req l_radioConfigReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_radioConfigReq.ID = ID;
	l_radioConfigReq.deviceId = p_deviceid;
	switch(p_select)
	{
	case 0:
		l_radioConfigReq.Radio = "all"; break;
	case 1:
		l_radioConfigReq.Radio = "2.4G"; break;
	default:
		l_radioConfigReq.Radio = "5G"; break;
	}
	int l_power = p_percent;
	if(l_power > 100)
	{
		l_power = 100;
	}
	l_radioConfigReq.TransmitPower = std::to_string(l_power);
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceRadioConfigReq(l_radioConfigReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildRebootRequest(std::string p_deviceid)
{
	struct Interface56_Reboot_Req l_rebootReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_rebootReq.ID = ID;
	l_rebootReq.deviceId = p_deviceid;
	l_rebootReq.ControlType = "Reboot";
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceRebootReq(l_rebootReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildChannelReselectRequest(
		std::string p_deviceid, int p_select)
{
	struct Interface56_Reboot_Req l_rebootReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_rebootReq.ID = ID;
	l_rebootReq.deviceId = p_deviceid;
	switch(p_select)
	{
	case 0:
		l_rebootReq.ControlType = "2.4GChanReSelect"; break;
	default:
		l_rebootReq.ControlType = "5GChanReSelect"; break;
	}
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceRebootReq(l_rebootReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildRoamingConfigRequest(
		std::string p_deviceid, bool p_switch, int p_val1, int p_val2)
{
	struct Interface56_RoamingConfig_Req l_roamingConfigReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_roamingConfigReq.ID = ID;
	l_roamingConfigReq.deviceId = p_deviceid;
	l_roamingConfigReq.RoamingSwitch = p_switch == true ? 1 : 1;
	if(p_switch == true)
	{
		l_roamingConfigReq.LowRSSI24G = p_val1;
		l_roamingConfigReq.LowRSSI5G = p_val2;
	}
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceRoamingConfigReq(l_roamingConfigReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildUnbindRequest(std::string p_deviceid)
{
	struct Interface56_Unbind_Req l_unbindReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_unbindReq.ID = ID;
	l_unbindReq.deviceId = p_deviceid;
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceUnbindReq(l_unbindReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildUpgradeRequest(
		std::string p_deviceid, int p_type, std::string p_url, bool p_restart)
{
	struct Interface56_Upgrade_Req l_upgradeReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_upgradeReq.ID = ID;
	l_upgradeReq.deviceId = p_deviceid;
	l_upgradeReq.fileMode = "DOWN";
	switch(p_type)
	{
	case 0:
		l_upgradeReq.fileType = "FIRM";break;
	default:
		l_upgradeReq.fileType = "APP";break;
	}
	l_upgradeReq.fileUrl = p_url;
	l_upgradeReq.downMode = p_restart == true ? 1 : 0;
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceUpgradeReq(l_upgradeReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildWiFiParameterSyncRequest(
		std::string p_deviceid)
{
	struct Interface56_WiFiParameterSync_Req l_WiFiParamSyncReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_WiFiParamSyncReq.ID = ID;
	l_WiFiParamSyncReq.deviceId = p_deviceid;
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceWiFiParameterSyncReq(l_WiFiParamSyncReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildWiFiSwitchRequest(
		std::string p_deviceid, bool p_turnOn, int p_select)
{
	struct Interface56_WiFiSwitch_Req l_wifiSwitchReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_wifiSwitchReq.ID = ID;
	l_wifiSwitchReq.deviceId = p_deviceid;
	switch(p_select)
	{
	case 0:
		l_wifiSwitchReq.Radio = "all"; break;
	case 1:
		l_wifiSwitchReq.Radio = "2.4G"; break;
	default:
		l_wifiSwitchReq.Radio = "5G"; break;
	}
	l_wifiSwitchReq.Enable = p_turnOn == true ? 1: 0;
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceWiFiSwitchReq(l_wifiSwitchReq));
}

std::pair<std::string, std::string> AndlinkDeviceEventBuilder::buildWPSRequest(
		std::string p_deviceid, int p_select)
{
	struct Interface56_WPS_Req l_wpsReq;
	std::string ID = m_ueContextHolder->generatorRandomNumberString(16);
	l_wpsReq.ID = ID;
	l_wpsReq.deviceId = p_deviceid;
	switch(p_select)
	{
	case 0:
		l_wpsReq.Radio = "all"; break;
	case 1:
		l_wpsReq.Radio = "2.4G"; break;
	default:
		l_wpsReq.Radio = "5G"; break;
	}
	return std::pair<std::string, std::string>(ID, buildAndlinkDeviceWPSReq(l_wpsReq));
}



