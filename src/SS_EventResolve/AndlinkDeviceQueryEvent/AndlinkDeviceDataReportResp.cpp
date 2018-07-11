#include "AndlinkDeviceQueryEvent.hpp"

bool resolveAndlinkDeviceDataReportResp(std::string msg, struct Interface56_DataReport_Resp* resp)
{
	int i = 0;
	if(msg.empty() == true || resp == NULL)
	{
		return false;
	}

	cJSON* obj = cJSON_Parse(msg.c_str());
	if(obj == NULL)
	{
		return false;
	}
	cJSON* respCode = cJSON_GetObjectItem(obj, "respCode");

	if(respCode)
	{
		resp->respCode = respCode->valueint;
		cJSON_Delete(obj);
		return true;
	}
	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceDataReportResp(struct Interface56_DataReport_Resp resp)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON_AddNumberToObject(regJs, "respCode", resp.respCode);

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}

