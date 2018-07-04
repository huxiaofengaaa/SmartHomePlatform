/*
 * AndlinkDeviceDisconnectRequest.cpp
 *
 *  Created on: 2018��7��3��
 *      Author: Administrator
 */

#include "AndlinkDeviceEvent.hpp"

std::string build_disconnect_request_msg(struct Interface56_disconnect_req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddStringToObject(regJs, "RPCMethod", req.RPCMethod.c_str());
	cJSON_AddStringToObject(regJs, "ID", req.ID.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	free(regch);
	cJSON_Delete(regJs);

	return l_result;
}


