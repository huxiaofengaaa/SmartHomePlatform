#include "ExcutionUnitClient.hpp"
#include "AndlinkDeviceControlEvent.hpp"
#include <stdio.h>

bool ExcutionUnitClient::deviceControlChecker(std::string p_req)
{
	struct Interface56_LEDControl_Req led_control_req;
	struct Interface56_MacFilter_Req mac_filter_req;
	struct Interface56_RadioConfig_Req radio_config_req;
	struct Interface56_Reboot_Req reboot_req;
	struct Interface56_RoamingConfig_Req roaming_req;
	struct Interface56_WiFiParameterSync_Req wifi_parameter_req;
	struct Interface56_WiFiSwitch_Req wifi_switch_req;
	struct Interface56_WPS_Req wps_req;

	if(true == resolveAndlinkDeviceLEDControlReq(p_req, &led_control_req))
	{
		Interface56_ControlCommon_Resp resp;
		resp.ID = led_control_req.ID;
		resp.respCode = 0;
		if(false == writeTCPString(buildAndlinkDeviceControlCommonResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceMacFilterReq(p_req, &mac_filter_req))
	{
		Interface56_ControlCommon_Resp resp;
		resp.ID = mac_filter_req.ID;
		resp.respCode = 0;
		if(false == writeTCPString(buildAndlinkDeviceControlCommonResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceRadioConfigReq(p_req, &radio_config_req))
	{
		Interface56_ControlCommon_Resp resp;
		resp.ID = radio_config_req.ID;
		resp.respCode = 0;
		if(false == writeTCPString(buildAndlinkDeviceControlCommonResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceRebootReq(p_req, &reboot_req))
	{
		Interface56_ControlCommon_Resp resp;
		resp.ID = reboot_req.ID;
		resp.respCode = 0;
		if(false == writeTCPString(buildAndlinkDeviceControlCommonResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceRoamingConfigReq(p_req, &roaming_req))
	{
		Interface56_ControlCommon_Resp resp;
		resp.ID = roaming_req.ID;
		resp.respCode = 0;
		if(false == writeTCPString(buildAndlinkDeviceControlCommonResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceWiFiParameterSyncReq(p_req, &wifi_parameter_req))
	{
		Interface56_ControlCommon_Resp resp;
		resp.ID = wifi_parameter_req.ID;
		resp.respCode = 0;
		if(false == writeTCPString(buildAndlinkDeviceControlCommonResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceWiFiSwitchReq(p_req, &wifi_switch_req))
	{
		Interface56_ControlCommon_Resp resp;
		resp.ID = wifi_switch_req.ID;
		resp.respCode = 0;
		if(false == writeTCPString(buildAndlinkDeviceControlCommonResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceWPSReq(p_req, &wps_req))
	{
		Interface56_ControlCommon_Resp resp;
		resp.ID = wps_req.ID;
		resp.respCode = 0;
		if(false == writeTCPString(buildAndlinkDeviceControlCommonResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	return false;
}


