/*
 * AndlinkDeviceRequestPlugInRequest.cpp
 *
 *  Created on: 2018��7��3��
 *      Author: Administrator
 */
#include "AndlinkDeviceEvent.hpp"

std::string build_requestPlugIn_request_msg(struct Interface56_requestPlugIn_Req resp)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", resp.RPCMethod.c_str());
	cJSON_AddStringToObject(regJs, "Key", resp.Key.c_str());
	cJSON_AddStringToObject(regJs, "ServerAddr", resp.ServerAddr.c_str());
	cJSON_AddStringToObject(regJs, "ServerPort", resp.ServerPort.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	free(regch);
	cJSON_Delete(regJs);

	return l_result;
}



