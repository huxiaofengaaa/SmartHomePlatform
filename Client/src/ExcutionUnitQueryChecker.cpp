#include "ExcutionUnitClient.hpp"
#include "AndlinkDeviceQueryEvent.hpp"
#include <stdio.h>

bool ExcutionUnitClient::deviceQueryChecker(std::string p_req)
{
	printf("deviceQueryChecker running....\n");
	struct Interface56_CommonQuery_Req l_queryReq;

	if(true == resolveAndlinkDeviceApConfigInfoQueryReq(p_req, &l_queryReq))
	{
		Interface56_ApConfigInfo_Resp resp;
		if(false == writeTCPString(buildAndlinkDeviceApConfigInfoQueryResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceSTAInfoQueryReq(p_req, &l_queryReq))
	{
		Interface56_STAInfo_Resp resp;
		if(false == writeTCPString(buildAndlinkDeviceSTAInfoQueryResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceUplinkStatusQueryReq(p_req, &l_queryReq))
	{
		Interface56_UplinkStatus_Resp resp;
		if(false == writeTCPString(buildAndlinkDeviceUplinkStatusQueryResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	else if(true == resolveAndlinkDeviceWiFiStatsQueryReq(p_req, &l_queryReq))
	{
		Interface56_WiFiStats_Resp resp;
		if(false == writeTCPString(buildAndlinkDeviceWiFiStatsQueryResp(resp)))
		{
			printf("send interface5_6 msg failed\n");
		}
		return true;
	}
	return false;
}


