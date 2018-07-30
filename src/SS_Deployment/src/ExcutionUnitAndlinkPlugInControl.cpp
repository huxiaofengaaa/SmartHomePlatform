#include "ExcutionUnitAndlinkPlugIn.hpp"

bool ExcutionUnitAndlinkPlugIn::triggerReboot(std::string p_deviceid)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerReboot " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildRebootRequest(p_deviceid);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(p_deviceid, l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerLEDControl(std::string p_deviceid, bool p_turnOn)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerLEDControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildLEDControlRequest(p_deviceid, p_turnOn);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(p_deviceid, l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerPowerControl(std::string p_deviceid, int p_select, int p_percent)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerPowerControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildRadioPowerRequest(p_deviceid, p_select, p_percent);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(p_deviceid, l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerMACFilterControl(std::string p_deviceid, bool p_enable,
		int p_policy, std::string p_entry)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerMACFilterControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildMacFilterRequest(p_deviceid, p_enable, p_policy, p_entry);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(p_deviceid, l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerChannelReselectControl(std::string p_deviceid, int p_select)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerChannelReselectControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildChannelReselectRequest(p_deviceid, p_select);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(p_deviceid, l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerRoamingConfigControl(std::string p_deviceid,
		bool p_switch, int p_val1, int p_val2)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerRoamingConfigControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildRoamingConfigRequest(p_deviceid, p_switch, p_val1, p_val2);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(p_deviceid, l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerWiFiSwitchControl(std::string p_deviceid, bool p_turnOn, int p_select)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerWiFiSwitchControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildWiFiSwitchRequest(p_deviceid, p_turnOn, p_select);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(p_deviceid, l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerWPSControl(std::string p_deviceid, int p_select)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerWPSControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildWPSRequest(p_deviceid, p_select);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(p_deviceid, l_eventObj);
}

bool ExcutionUnitAndlinkPlugIn::triggerWiFiParameterSyncControl(std::string p_deviceid)
{
	LOG(INFO) << "ExcutionUnitAndlinkPlugIn::triggerWiFiParameterSyncControl " << p_deviceid;
	if(false == controlReqPreconditionCheck(p_deviceid))
	{
		return false;
	}
	auto l_andlinkBuilder = std::make_shared<AndlinkDeviceEventBuilder>(m_ueContextHolder);
	auto l_rawReq = l_andlinkBuilder->buildWiFiParameterSyncRequest(p_deviceid);
	auto l_eventObj = buildTCPClientDataObject(p_deviceid, l_rawReq.second);
	return sendPlugInRequest(p_deviceid, l_eventObj);
}
