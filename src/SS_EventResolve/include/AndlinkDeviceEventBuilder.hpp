/*
 * AndlinkDeviceEventBuilder.hpp
 *
 *  Created on: 2018Äê7ÔÂ11ÈÕ
 *      Author: Administrator
 */
#pragma once
#include <string>
#include <utility>
#include "AndlinkDeviceEvent.hpp"
#include "AndlinkDeviceQueryEvent.hpp"
#include "AndlinkDeviceControlEvent.hpp"
#include "UeContextHolderAndlink.hpp"

class AndlinkDeviceEventBuilder
{
public:
	AndlinkDeviceEventBuilder(std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder)
		: m_ueContextHolder(p_ueContextHolder)
	{

	}

	// plugin and disconnect
	std::string buildPlugIuRequest(std::string p_deviceid, std::string p_host, int p_port);
	std::string buildDisconnectRequest(std::string p_deviceid);

	typedef std::pair<std::string, std::string> DownlinkRequest;
	// query
	DownlinkRequest buildApConfigInfoQueryRequest(std::string& p_deviceid);
	DownlinkRequest buildSTAInfoQueryRequest(std::string& p_deviceid);
	DownlinkRequest buildUplinkStatusQueryRequest(std::string& p_deviceid);
	DownlinkRequest buildWiFiStatsQueryRequest(std::string& p_deviceid);

	// control - led
	DownlinkRequest buildLEDControlRequest(std::string p_deviceid, bool p_turnOn);
	DownlinkRequest buildMacFilterRequest(std::string p_deviceid, bool p_enable, int p_policy, std::string p_entry);
	DownlinkRequest buildRadioPowerRequest(std::string p_deviceid, int p_select, int p_percent);
	DownlinkRequest buildRebootRequest(std::string p_deviceid);
	DownlinkRequest buildChannelReselectRequest(std::string p_deviceid, int p_select);
	DownlinkRequest buildRoamingConfigRequest(std::string p_deviceid, bool p_switch, int p_val1 = 0, int p_val2 = 0);
	DownlinkRequest buildUnbindRequest(std::string p_deviceid);
	DownlinkRequest buildUpgradeRequest(std::string p_deviceid, int p_type, std::string p_url, bool p_restart);
	DownlinkRequest buildWiFiParameterSyncRequest(std::string p_deviceid);
	DownlinkRequest buildWiFiSwitchRequest(std::string p_deviceid, bool p_turnOn, int p_select);
	DownlinkRequest buildWPSRequest(std::string p_deviceid, int p_select);

private:
	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;
};
