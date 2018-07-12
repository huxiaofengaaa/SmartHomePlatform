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

	// query
	std::string buildApConfigInfoQueryRequest(std::string p_deviceid);
	std::string buildSTAInfoQueryRequest(std::string p_deviceid);
	std::string buildUplinkStatusQueryRequest(std::string p_deviceid);
	std::string buildWiFiStatsQueryRequest(std::string p_deviceid);

	// control - led
	typedef std::pair<std::string, std::string> ControlRequest;
	ControlRequest buildLEDControlRequest(std::string p_deviceid, bool p_turnOn);
	ControlRequest buildMacFilterRequest(std::string p_deviceid, bool p_enable, int p_policy, std::string p_entry);
	ControlRequest buildRadioPowerRequest(std::string p_deviceid, int p_select, int p_percent);
	ControlRequest buildRebootRequest(std::string p_deviceid);
	ControlRequest buildChannelReselectRequest(std::string p_deviceid, int p_select);
	ControlRequest buildRoamingConfigRequest(std::string p_deviceid, bool p_switch, int p_val1 = 0, int p_val2 = 0);
	ControlRequest buildUnbindRequest(std::string p_deviceid);
	ControlRequest buildUpgradeRequest(std::string p_deviceid, int p_type, std::string p_url, bool p_restart);
	ControlRequest buildWiFiParameterSyncRequest(std::string p_deviceid);
	ControlRequest buildWiFiSwitchRequest(std::string p_deviceid, bool p_turnOn, int p_select);
	ControlRequest buildWPSRequest(std::string p_deviceid, int p_select);

private:
	std::shared_ptr<UeContextHolderAndlink> m_ueContextHolder;
};
